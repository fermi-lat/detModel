#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "detModel/Management/VrmlSectionsVisitor.h"
#include "detModel/Management/Manager.h"
#include "detModel/Sections/Section.h"
#include "detModel/Sections/Volume.h"
#include "detModel/Gdd.h"
#include "detModel/Sections/Box.h"
#include "detModel/Sections/Tube.h"
#include "detModel/Sections/Trap.h"
#include "detModel/Sections/Sphere.h"
#include "detModel/Sections/Composition.h"
#include "detModel/Sections/Ensemble.h"
#include "detModel/Sections/PosXYZ.h"
#include "detModel/Sections/Stack.h"
#include "detModel/Sections/AxisMPos.h"
#include "detModel/Sections/IdField.h"
#include "detModel/Sections/Position.h"
#include "detModel/Utilities/ColorCreator.h"
#include "detModel/Utilities/Color.h"
#include "detModel/Utilities/Global.h"
#include "detModel/Materials/MatCollection.h"


namespace detModel{

  /// This is the main constructor
  VrmlSectionsVisitor::VrmlSectionsVisitor(std::string nvol)
  {

    /// This visitor is not recursive
    setRecursive(0);
    actualVolume = nvol;
    depth = 0;

    /// This is the output file \todo The user should choose the name
    out.open("sections.wrl");
  

    numVol = 0;
  };

  VrmlSectionsVisitor::~VrmlSectionsVisitor()
  {
    out.close();
  }

  void VrmlSectionsVisitor::visitGdd(Gdd* gdd)
  {
    // Some variables
    typedef std::map<std::string,int>M2;
    typedef std::map<std::string,Volume*>M3;
    M3::const_iterator j;

    /// We use this visit to set up the header of the VRML file
    typedef std::vector<Section*>sec;
    std::vector <Section*>::iterator i;

    m_gdd = gdd;

    /// We retrive the manager and the Gdd
    //    Manager* manager = Manager::getPointer();
    //    Gdd* g = manager->getGdd();

    /// We set the colorMaps from the Gdd
    colorsMap=m_gdd->getMaterials()->getMaterialColors();
    
    /// We initialize the depth map
    M3 m = m_gdd->getVolumesMap();
    for(j=m.begin();j!=m.end();j++)
      depthMap.insert(M2::value_type(j->first,20));        


    out << "#VRML V2.0 utf8 " << std::endl;
    out << "#Generated by detModel " << std::endl;
    /// This method generate materials color in the VRML file
    makeColor();

    /// We iterate on all the sections (actually there should be only 1)
    sec s = gdd->getSections();
    for(i=s.begin(); i!=s.end();i++)
      (*i)->AcceptNotRec(this);
  }


  void  VrmlSectionsVisitor::visitSection(Section* section)
  {
    float dimX, dimY, dimZ;
    Volume* vol=0;
    std::vector <Volume*>::iterator v;

    /// If no volume has been specified in the constructor we search for the TopVolume
    if (actualVolume == "")
      {
	std::vector<Volume*> volumes = section->getVolumes();
      
	for(v=volumes.begin(); v!=volumes.end(); v++){
	  if(Ensemble* ens = dynamic_cast<Ensemble*>(*v))
	    {
	      if (ens == section->getTopVolume())
		vol = ens;
	    }
	}
      }
    else /// otherwise we use the volume speicified in the constructor
      {
        //	Manager* manager = Manager::getPointer();
        //	if (manager->getGdd()->getVolumeByName(actualVolume))
        //	  vol = manager->getGdd()->getVolumeByName(actualVolume);
	if (m_gdd->getVolumeByName(actualVolume))
	  vol = m_gdd->getVolumeByName(actualVolume);
	else /// we abort if such a volume does not exists
	  detAbort("No such a volume!!");
      }
  
    /// We calulate the dimensions for the projected views
    dimX = 2*vol->getBBox()->getXDim()/0.005;
    dimY = 2*vol->getBBox()->getYDim()/0.005;
    dimZ = 2*vol->getBBox()->getZDim()/0.005;

    /** 
     * We setup three standard view on the three coordinates planes;
     * since in VRML97 there is no orthographic camera, we simulate it
     * with a far away camera and a small field of view. \todo To be 
     * corrected; some views are not properly performed
     */
  
    out << "Viewpoint {" << std::endl;
    out << "position 0 0 " << dimX << std::endl;
    out << "orientation 0 0 1 0" << std::endl;
    out << "fieldOfView 0.005" << std::endl;
    out << "description \"Plane XY\"" << std::endl;
    out << "}" << std::endl;

    out << "Viewpoint {" << std::endl;
    out << "position 0 " << -dimY << " 0" << std::endl;
    out << "orientation 1 0 0 1.5707963" << std::endl;
    out << "fieldOfView 0.005" << std::endl;
    out << "description \"Plane XZ\"" << std::endl;
    out << "}" << std::endl;

    out << "Viewpoint {" << std::endl;
    out << "position " << -dimZ << " 0 0" << std::endl; 
    out << "orientation 0 1 0 -1.5707963" << std::endl;
    out << "fieldOfView 0.005" << std::endl;
    out << "description \"Plane ZY\"" << std::endl;
    out << "}" << std::endl;

    /// Here we start the visit of the hierarchy.
    vol->AcceptNotRec(this);
  }


  void  VrmlSectionsVisitor::visitEnsemble(Ensemble* ensemble)
  {
    std::vector <Position*>::iterator i;
    typedef std::vector<Position*> pos;
  
    typedef std::map<std::string, int> M;
    M::const_iterator j; 

    j = depthMap.find(ensemble->getName());
  
    /// If we have not reached the depth limit we draw the full ensamble
    if(j->second > depth) 
      {
	depth++;
      
	out << "# " << ensemble->getName() << std::endl;
      
	/// Here the positioned volumes are visited
	pos p = ensemble->getPositions();
	for(i=p.begin(); i!=p.end();i++)
	  (*i)->AcceptNotRec(this);
      
	/// Here the envelope is visited if the ensamble is a composition
	if (Composition* comp = dynamic_cast<Composition*>(ensemble))
	  comp->getEnvelope()->AcceptNotRec(this);
      
	/// The depth level is decreased
	depth--;
      }
    else /// Else we draw only the bounding box
      {
	out << "Shape {   #" << ensemble->getName() << std::endl;
	out << "  geometry Box { " << std::endl;
	out << "                     size " 
	    << ensemble->getBBox()->getXDim() << " "
	    << ensemble->getBBox()->getXDim() << " "
	    << ensemble->getBBox()->getXDim() << std::endl;
	out << "                    }" << std::endl;  
	out << "   }" << std::endl;
      }
  }

  void  VrmlSectionsVisitor::visitBox(Box* box)
  {
    out << "Shape {   #" << box->getName() << std::endl;
    out << "appearance USE " << box->getMaterial() << std::endl;
  
    out << "  geometry Box { " << std::endl;
    out << "                     size " 
	<< box->getX() << " " << box->getY() << " " << box->getZ() << std::endl; 
    out << "                    }" << std::endl;  
    out << "   }" << std::endl;
    numVol++;
  }

  // Approximate with a box; it's *so* much easier
  void  VrmlSectionsVisitor::visitTrap(Trap* trap)
  {
    double x = 0.5 * (trap->getX1() + trap->getX2());

    out << "Shape {   #" << trap->getName() << std::endl;
    out << "appearance USE " << trap->getMaterial() << std::endl;
    out << "  geometry Box { " << std::endl;
    out << "                     size " 
	<< x << " " << trap->getY() << " " << trap->getZ() << std::endl; 
    out << "   }" << std::endl;
    out << "] " << std::endl; 
    out << "} " << std::endl;
    numVol++;
  }


  void  VrmlSectionsVisitor::visitTube(Tube* tube)
  {
    out << "Transform { " << std::endl;
    out << "rotation " << " 1 0 0 " <<  3.141927/2 << std::endl;  
    out << "children [ " << std::endl;
    out << "Shape {   #" << tube->getName() << std::endl;
    out << "appearance USE " << tube->getMaterial() << std::endl;
    out << "  geometry Cylinder { " << std::endl;
    out << "                     height " << tube->getZ() << std::endl; 
    out << "                     radius " << tube->getRout() << std::endl; 
    out << "                    }" << std::endl;  
    out << "   }" << std::endl;
    out << "] " << std::endl; 
    out << "} " << std::endl;
    numVol++;
  }

  void  VrmlSectionsVisitor::visitSphere(Sphere* sphere)
  {
    out << "Transform { " << std::endl;
    out << "rotation " << " 1 0 0 " <<  3.141927/2 << std::endl;  
    out << "children [ " << std::endl;
    out << "Shape {   #" << sphere->getName() << std::endl;
    out << "appearance USE " << sphere->getMaterial() << std::endl;
    out << "  geometry Sphere { " << std::endl;
    out << "                     radius " << sphere->getRout() << std::endl; 
    out << "                    }" << std::endl;  
    out << "   }" << std::endl;
    out << "] " << std::endl; 
    out << "} " << std::endl;
    numVol++;
  }
  
  void  VrmlSectionsVisitor::visitPosXYZ(PosXYZ* pos)
  {
    out << "Transform { " << std::endl;
  
    out << "translation " << pos->getX() << " " << pos->getY() << " " << pos->getZ() << std::endl;
    out << "rotation " << " 1 0 0 " <<  pos->getXRot()*3.141927/180 << std::endl;  
    out << "rotation " << " 0 1 0 " <<  pos->getYRot()*3.141927/180 << std::endl;  
    out << "rotation " << " 0 0 1 " <<  pos->getZRot()*3.141927/180 << std::endl;  
    out << "children [ " << std::endl;
    pos->getVolume()->AcceptNotRec(this);
    out << "] " << std::endl; 
    out << "} " << std::endl;
  }


  void  VrmlSectionsVisitor::visitAxisMPos(AxisMPos* pos)
  {
    int i;
    int n;

    n = pos->getNcopy();

    for(i=0;i<n;i++){
      out << "Transform { " << std::endl;
      switch(pos->getAxisDir()){
      case (Stack::xDir):
	out << "rotation " << " 1 0 0 " <<  pos->getRotation()*3.141927/180 << std::endl;  
	out << "translation " << 
	  pos->getDx()+pos->getDisp(i) << " " << pos->getDy() << " " << pos->getDz() << std::endl;
	break;
      case (Stack::yDir):
	out << "rotation " << " 0 1 0 " <<  pos->getRotation()*3.141927/180 << std::endl;  
	out << "translation " << 
	  pos->getDx() << " " << pos->getDy()+pos->getDisp(i) << " " << pos->getDz() << std::endl;
	break;
      case (Stack::zDir):
	out << "rotation " << " 0 0 1 " <<  pos->getRotation()*3.141927/180 << std::endl;  
	out << "translation " << 
	  pos->getDx() << " " << pos->getDy() << " " << pos->getDz()+pos->getDisp(i) << std::endl;
	break;
      }
      out << "children [ " << std::endl;
      pos->getVolume()->AcceptNotRec(this);
      out << "] " << std::endl; 
      out << "} " << std::endl;
    }


  }

  void  VrmlSectionsVisitor::visitIdField(IdField*)
  {
    /// We don't need identifiers in the VRML visitor
  }


  void  VrmlSectionsVisitor::visitSeg(Seg*)
  {
  }

  void VrmlSectionsVisitor::setDepth(std::string name, int d)
  {
    typedef std::map<std::string, int> M;
    M::const_iterator j; 
  
    j = depthMap.find(name);
    if (j == depthMap.end()) return;
    else 
      {
	depthMap.erase(name);
	depthMap.insert(M::value_type(name,d));
      }  
  }

  void VrmlSectionsVisitor::makeColor()
  {
    typedef std::map<std::string, Color*> M;
    M::const_iterator j; 
  
    for(j=colorsMap.begin();j!=colorsMap.end();j++)
      {
	out << " DEF " << j->first << std::endl;
	out << " Appearance { " <<  std::endl;
	out << " material Material { " <<  std::endl;

	out << "       diffuseColor  " 
	    << j->second->getRed() << " " 
	    << j->second->getGreen() << " " 
	    << j->second->getBlue() 
	    << std::endl;
	out << "     transparency    " << j->second->getTra() << std::endl;          

	out << "     }" <<  std::endl;
	out << " }" <<  std::endl;
      }
  }

}



