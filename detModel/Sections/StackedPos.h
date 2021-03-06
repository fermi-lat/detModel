#ifndef STACKEDPOS_H
#define STACKEDPOS_H
#include "detModel/Sections/Stack.h"
#include "detModel/Sections/Position.h"

namespace detModel{

  /**
   * This abstract class represent a stacked position
   * 
   * @author R.Giannitrapani
   */
  class StackedPos :public Position {

  public:
    /** 
     *  This constuctor needs the direction of the stacked position,
     *  that is an anumerative type actually in GDDstack (to be changed) 
     */
    StackedPos(Stack::axisDir dir):
      Position(),aDir(dir),
      dx(0),dy(0),dz(0),
      rotation(0),shift(0),gap(0){;}

    /// This constructor does not need anything; the default direction is Z
    StackedPos():
      Position(),aDir(Stack::zDir),
      dx(0),dy(0),dz(0),
      rotation(0),shift(0),gap(0){;}

    /// Methods to get and set the direction of the stacked position
    Stack::axisDir getAxisDir(){return aDir;};
    void setAxisDir(Stack::axisDir dir){aDir = dir;};

    /// Methods to set and get displacement on x y and z direction
    void setDx(double pdx){dx=pdx;}
    void setDy(double pdy){dy=pdy;}
    void setDz(double pdz){dz=pdz;}
    double getDx(){return dx;}
    double getDy(){return dy;}
    double getDz(){return dz;}

    /// Methods to set and get the rotation around the stacked direction
    void setRotation(double protation){rotation=protation;}
    double getRotation(){return rotation;}

    /// Methods to set and get shift or gap for the stacked pos
    void setShift(double pshift){shift=pshift;}
    void setGap(double pgap){gap=pgap;}
    double getShift()const{return shift;}
    double getGap()const{return gap;}


  private:
    /// The direction of the stacked positioning
    Stack::axisDir aDir;

    double dx; 
    double dy; 
    double dz; 
    double rotation; 
    double shift; 
    double gap;  
  };
}
#endif //STACKEDPOS_H
