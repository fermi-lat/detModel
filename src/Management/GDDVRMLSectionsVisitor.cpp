#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "detModel/Management/GDDVRMLSectionsVisitor.h"
#include "detModel/Management/GDDmanager.h"
#include "detModel/Sections/GDDsection.h"
#include "detModel/GDD.h"
#include "detModel/Sections/GDDbox.h"
#include "detModel/Sections/GDDcomposition.h"
#include "detModel/Sections/GDDposXYZ.h"
#include "detModel/Sections/GDDstack.h"
#include "detModel/Sections/GDDaxisPos.h"
#include "detModel/Sections/GDDaxisMPos.h"
#include "detModel/Sections/GDDidField.h"
#include "detModel/Sections/GDDanyPosition.h"

GDDVRMLSectionsVisitor::GDDVRMLSectionsVisitor(std::string nvol)
{
  unsigned int i;
  typedef std::map<std::string,float>M1;
  typedef std::map<std::string,int>M2;
  typedef std::map<std::string,GDDvolume*>M3;
  M3::const_iterator j;

  setRecursive(0);
  setType(sectionsVisitor);
  actualVolume = nvol;
  depth = 0;

  out.open("sections.wrl");

  GDDmanager* manager = GDDmanager::getPointer();
  GDD* g = manager->getGDD();
  
  /// We initialize the opacity map
  for(i=0;i<g->getMaterialNames().size();i++)
    opacityMap.insert(M1::value_type(g->getMaterialNames()[i],0.0));  

  /// We initialize the depth map
  M3 m = g->getVolumesMap();
  for(j=m.begin();j!=m.end();j++)
    {
      depthMap.insert(M2::value_type(j->first,20));        
    }    
};

GDDVRMLSectionsVisitor::~GDDVRMLSectionsVisitor()
{
  out.close();
}

void GDDVRMLSectionsVisitor::visitGDD(GDD* gdd)
{
  unsigned int i;

  out << "#VRML V2.0 utf8 " << std::endl;
  out << "#Generated by detModel " << std::endl;
  makeColor();

  for(i=0; i<gdd->getSections()->size();i++){
    ((* gdd->getSections() )[i])->AcceptNotRec(this);
  }
}
  
void  GDDVRMLSectionsVisitor::visitSection(GDDsection* section)
{
  float dimX, dimY, dimZ;
  unsigned int i;
  GDDvolume* vol;

  if (actualVolume == "")
    {
      for(i=0; i<section->getVolumes().size();i++){
	if(section->getVolumes()[i]->getVolumeType() == composition)
	  {
	    GDDcomposition* comp = static_cast<GDDcomposition*>(section->getVolumes()[i]);
	    if (comp == section->getTopVolume())
	      {
		vol = section->getVolumes()[i];
	      }
	  }
      }
    }
  else
    {
      GDDmanager* manager = GDDmanager::getPointer();
      
      if (manager->getGDD()->getVolumeByName(actualVolume))
	{
	  vol = manager->getGDD()->getVolumeByName(actualVolume);
	}
    }

  /// We calulate the dimensions for the projected views
  dimX = 2*vol->getBBX()/0.005;
  dimY = 2*vol->getBBY()/0.005;
  dimZ = 2*vol->getBBZ()/0.005;

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


void  GDDVRMLSectionsVisitor::visitComposition(GDDcomposition* composition)
{
  unsigned int i;
  typedef std::map<std::string, int> M;
  M::const_iterator j; 

  j = depthMap.find(composition->getName());

  if(j->second > depth) 
    {
      depth++;

      out << "# " << composition->getName() << std::endl;

      for(i=0; i<composition->getPositions().size();i++){
	GDDanyPosition* pos = composition->getPositions()[i];
	pos->AcceptNotRec(this);
      }
      (composition->getEnvelope())->AcceptNotRec(this);
      depth--;
    }
  else
    {
      out << "Shape {   #" << composition->getName() << std::endl;
      out << "  geometry Box { " << std::endl;
      out << "                     size " 
	  << composition->getBBX() << " " 
	  << composition->getBBY() << " " 
	  << composition->getBBZ() << std::endl; 
      out << "                    }" << std::endl;  
      out << "   }" << std::endl;
    }
}

/** \todo The visitStack mechanism is undergoing a big redesign .. 
 *too much duplicated code in different visitors
 */
void  GDDVRMLSectionsVisitor::visitStack(GDDstack* st)
{
  unsigned int j;
  double deltap, delta;
  GDDanyRelativePosition* apos;
  typedef std::map<std::string, int> M;
  M::const_iterator k; 

  k = depthMap.find(st->getName());

  deltap = 0;
  if(k->second > depth) 
    {
      depth++;
      out << "Transform {  # " << st->getName() << std::endl;
      switch(st->getStackType()){
      case sx:
	out << "translation " << -(st->getBBX())*0.5 << " 0 " << " 0 " << std::endl;
	break;	
      case sy:
	out << "translation " << " 0 " << -(st->getBBY())*0.5 << " 0 " << std::endl;
	break;	
      case sz:
	out << "translation " << " 0 " << " 0 " << -(st->getBBZ())*0.5 << std::endl;
	break;	
      }
      out << "children [ " << std::endl;
      for(j=0;j<st->getPositions().size();j++)
	{
	  apos = st->getPositions()[j];
	  out << "Transform {  #" << apos->getVolume()->getName() << std::endl;
	  switch(st->getStackType()){
	  case sx:
	    delta = (apos->getBBX()/2+apos->getGap()) + deltap;
	    out << "translation " << delta << " 0 " << " 0 " << std::endl;
	    deltap += apos->getBBX()+apos->getGap();;
	    break;	
	  case sy:
	    delta = (apos->getBBY()/2+apos->getGap()) + deltap;
	    out << "translation " << " 0 " << delta << " 0 " << std::endl;
	    deltap += apos->getBBY()+apos->getGap();
	    break;	
	  case sz:
	    delta = (apos->getBBZ()/2+apos->getGap()) + deltap;
	    out << "translation " << " 0 " << " 0 " << delta << std::endl;
	    deltap += apos->getBBZ()+apos->getGap();
	    break;	
	  }
	  out << "children [ " << std::endl;
	  apos->AcceptNotRec(this);
	  out << "] " << std::endl; 
	  out << "} " << std::endl;
	}  
      
      out << "] " << std::endl; 
      out << "} " << std::endl;
      depth --;
    }
  else
    {
      out << "Shape {   #" << st->getName() << std::endl;
      out << "  geometry Box { " << std::endl;
      out << "                     size " 
	  << st->getBBX() << " " 
	  << st->getBBY() << " " 
	  << st->getBBZ() << std::endl; 
      out << "                    }" << std::endl;  
      out << "   }" << std::endl;
    }
}



void  GDDVRMLSectionsVisitor::visitBox(GDDbox* box)
{
  out << "Shape {   #" << box->getName() << std::endl;
  out << "appearance USE " << box->getMaterial() << std::endl;

  out << "  geometry Box { " << std::endl;
  out << "                     size " 
	    << box->getX() << " " << box->getY() << " " << box->getZ() << std::endl; 
  out << "                    }" << std::endl;  
  out << "   }" << std::endl;
}

void  GDDVRMLSectionsVisitor::visitPosXYZ(GDDposXYZ* pos)
{
  out << "Transform { " << std::endl;

  out << "translation " << pos->getX() << " " << pos->getY() << " " << pos->getZ() << std::endl;
  out << "rotation " << " 1 0 0 " <<  pos->getXrot()*3.141927/180 << std::endl;  
  out << "rotation " << " 0 1 0 " <<  pos->getYrot()*3.141927/180 << std::endl;  
  out << "rotation " << " 0 0 1 " <<  pos->getZrot()*3.141927/180 << std::endl;  
  out << "children [ " << std::endl;
  pos->getVolume()->AcceptNotRec(this);
  out << "] " << std::endl; 
  out << "} " << std::endl;
}


/** \todo The visitAxisPos mechanism is undergoing a big redesign .. 
 *too much duplicated code in different visitors
 */
void  GDDVRMLSectionsVisitor::visitAxisPos(GDDaxisPos* pos)
{
  out << "Transform { " << std::endl;
  switch(pos->getPosDir()){
  case sx:
    out << "rotation " << " 1 0 0 " <<  pos->getRotation()*3.141927/180 << std::endl;  
    break;
  case sy:
    out << "rotation " << " 0 1 0 " <<  pos->getRotation()*3.141927/180 << std::endl;  
    break;
  case sz:
    out << "rotation " << " 0 0 1 " <<  pos->getRotation()*3.141927/180  << std::endl;  
    break;
  }
  out << "translation " << pos->getDx() << " " << pos->getDy() << " " << pos->getDz() << std::endl;
  out << "children [ " << std::endl;
  pos->getVolume()->AcceptNotRec(this);
  out << "] " << std::endl; 
  out << "} " << std::endl;

}

/** \todo The visitAxisMPos mechanism is undergoing a big redesign .. 
 *too much duplicated code in different visitors
 */
void  GDDVRMLSectionsVisitor::visitAxisMPos(GDDaxisMPos* pos)
{
  int i;
  int n;
  float g, w;


  n = pos->getNcopy();
  g = pos->getGap();
	  
  for(i=0;i<n;i++){
    out << "Transform { " << std::endl;
    switch(pos->getPosDir()){
    case sx:
      w = pos->getVolume()->getBBX();	
      out << "translation " << i*(w+g)-(n-1)*(w+g)*0.5  << " 0 " << " 0 " << std::endl;  
      break;
    case sy:
      w = pos->getVolume()->getBBY();
      out << "translation " << " 0 " << i*(w+g)-(n-1)*(w+g)*0.5  << " 0 " << std::endl;  
      break;
    case sz:
      w = pos->getVolume()->getBBZ();
      out << "translation " << " 0 " << " 0 " << i*(w+g)-(n-1)*(w+g)*0.5  << std::endl;  
      break;
    }
    out << "children [ " << std::endl;
    pos->getVolume()->AcceptNotRec(this);
    out << "] " << std::endl; 
    out << "} " << std::endl;
  }


}

void  GDDVRMLSectionsVisitor::visitIdField(GDDidField*)
{
  /// We don't need identifiers in the VRML visitor
}

void  GDDVRMLSectionsVisitor::visitSeg(GDDseg*)
{
  /// \todo Still not implemented the subdivision in seg of a box
}

void GDDVRMLSectionsVisitor::setDepth(std::string name, int d)
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

void GDDVRMLSectionsVisitor::setOpacity(std::string name, float op)
{
  typedef std::map<std::string, float> M;
  M::const_iterator j; 
  
  j = opacityMap.find(name);
  if (j == opacityMap.end()) return;
  else 
    {
      opacityMap.erase(name);
      opacityMap.insert(M::value_type(name,op));
    }  
}

/** This function convert from the HSV to the RGB colours spaces \todo
    To bring outside the visitor and inside detModel */
void HSVtoRGB(double *r, double *g, double *b, double h, double s, double v)
{
  double f,p,q,t;
  int i;
  
  if (h==360.0)
    h = 0.0;
  
  h /= 60.0;
  i = (int)h;
  f = h - i;
  p = v*(1.0-s);
  q = v*(1.0 - (s*f));
  t = v*(1.0 - (s*(1.0-f)));

  switch(i){
  case 0: *r = v; *g = t; *b = p; break;
  case 1: *r = q; *g = v; *b = p; break;
  case 2: *r = p; *g = v; *b = t; break;
  case 3: *r = p; *g = q; *b = v; break;
  case 4: *r = t; *g = p; *b = v; break;
  case 5: *r = v; *g = p; *b = q; break;
  }
}

/** This function generates enought colors for the GDD partitioning
    the HSV space uniformly.  \todo To bring outside the visitor and
    inside detModel */
void GDDVRMLSectionsVisitor::makeColor()
{
  unsigned int i;
  unsigned int ncol;
  double r, g, b;

  typedef std::map<std::string, float> M;
  M::const_iterator j; 
  GDDmanager* manager = GDDmanager::getPointer();
  GDD* gdd = manager->getGDD();

  ncol = gdd->getMaterialNames().size();

  for(i=0;i<ncol;i++)
    {
      j = opacityMap.find(gdd->getMaterialNames()[i]);
      out << " DEF " << gdd->getMaterialNames()[i] << std::endl;
      out << " Appearance { " <<  std::endl;
      out << " material Material { " <<  std::endl;

      /// Automatic coloring scheme \todo To be done better
      HSVtoRGB(&r, &g, &b, 360.0*(ncol-i)/ncol, 1.0, 1.0);
      out << "       diffuseColor  " 
	  << r << " " 
	  << g << " "
	  << b <<  std::endl;

      if (j == opacityMap.end())
	out << "     transparency    0.0" << std::endl;          
      else
	out << "     transparency    " << j->second << std::endl;          
      out << "     }" <<  std::endl;
      out << " }" <<  std::endl;
    }
}




