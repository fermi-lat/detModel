#ifndef GDDSTACK_H
#define GDDSTACK_H
#include <vector>

#include "detModel/Sections/GDDvolume.h"
#include "detModel/Sections/GDDanyRelativePosition.h"


class GDDsectionsVisitor;

/**
 * @author D.Favretto & R.Giannitrapani
 */

class GDDstack :public GDDvolume {
 public:
  GDDstack(stacktype pStype):
    GDDvolume(stack),Stype(pStype),origin(atStart){}
  virtual ~GDDstack();
  /**
   *
   */ 
  virtual void Accept(GDDsectionsVisitor*);
  /**
   *
   */ 
  virtual void AcceptNotRec(GDDsectionsVisitor* v){v->visitStack(this);};
  /**
   *
   */ 
  void addPosition(GDDanyRelativePosition *pPosition){anyRelativePosition.push_back(pPosition);}
  /**
   *
   */ 
  virtual double getBBX();
  /**
   *
   */ 
  virtual double getBBY();
  /**
   *
   */ 
  virtual double getBBZ();
  
  /**
   *
   */ 
  void setOrigin(stackorigin porigin){origin=porigin;}
  /**
   *
   */ 
  stackorigin getOrigin()const{return origin;}
  /**
   *
   */ 
  stacktype getStackType()const{return Stype;}
  /**
   *
   */ 
  vector <GDDanyRelativePosition*> getPositions(){return anyRelativePosition;};
 private:
  stacktype Stype;
  stackorigin origin; ///default atStart
  /** @link aggregation
   *  @supplierCardinality 1..n */
  vector < GDDanyRelativePosition * > anyRelativePosition;
};
#endif //GDDSTACK_H



