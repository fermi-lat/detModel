#ifndef COUNTMATERIAL_H
#define COUNTMATERIAL_H
#include "detModel/Management/SectionsVisitor.h"
#include "detModel/Sections/Box.h"
#include "detModel/Sections/Tube.h"
#include <map>

namespace detModel{

  /**
   *  This visitor counts the number of volumes that has
   *  a particular material assigned and fill a map
   *  @author R.Giannitrapani
   */
  class CountMaterial : public SectionsVisitor {

  public:
    CountMaterial(std::string);
    virtual ~CountMaterial(){};

    virtual void visitGdd(Gdd*);
    virtual void visitSection(Section*);
    virtual void visitEnsemble(Ensemble*);
    virtual void visitBox(Box*);
    virtual void visitTube(Tube*);
    virtual void visitPosXYZ(PosXYZ*);
    virtual void visitAxisMPos(AxisMPos*);
    virtual void visitIdField(IdField*){};
    virtual void visitSeg(Seg*){};

    /// This method return the number of volumes given a material name
    double getNumber(std::string name){return m_matNumber[name];}

    /// This method dumps to std output the matNumber table
    void printTable();

  private:
    /** 
       This is the map of double indicized by the name of material:
       it is double and not int since in future it will contain the volume
       in mm3 of material.
    */
    std::map<std::string, double> m_matNumber;

    /// The name of the topVolume
    std::string m_actualVolume;    
  };

}
#endif //COUNTMATERIAL_H






