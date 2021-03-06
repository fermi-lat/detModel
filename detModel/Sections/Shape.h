#ifndef SHAPE_H
#define SHAPE_H
#include "detModel/Sections/Volume.h"
#include "detModel/Sections/Units.h"

namespace detModel{

  /**
   * This class contains all the common properties and 
   * functionalities of shapes. Concrete shapes must 
   * sublcass this one.
   * @author D.Favretto & R.Giannitrapani 
   */
  class Shape : public Volume {
  public:
    /** Some constructors; since this is an abstract class, these
     * constructors are useful only in its concrete subclasses
     */
    Shape(std::string pname):Volume(pname),m_detectorType(0),m_sensitive(0){m_units = new Units;}
    Shape():Volume(),m_detectorType(0),m_sensitive(0){m_units = new Units;}

    virtual ~Shape(){delete m_units;};

    /**
     * With this method it is possible to set and get the shape
     * sensitive for simulations purpouses
     */
    void setSensitive(unsigned int psensitive){m_sensitive = psensitive;};
    unsigned int getSensitive()const{return m_sensitive;};

    /**
     * With this method it is possible to set and get the 
     * detector type attribute (see the gdd.dtd for meaning)
     */
    void setDetectorType(unsigned int pdt){m_detectorType = pdt;};
    unsigned int getDetectorType()const{return m_detectorType;};

    /**
     * This method set the material (a string) of the shape
     */  
    void setMaterial(std::string pmaterial){m_material = pmaterial;};

    /**
     * This method gives back the material (a string) of the shape
     */  
    std::string getMaterial()const{return m_material;};

    Units* getUnits()const{return m_units;};

  private:
    unsigned int m_detectorType;
    std::string m_material;
    /// 0=non sensitive, 1 = posHit, 2 = intHit
    unsigned int m_sensitive;          
    Units* m_units;
  };
}
#endif //SHAPE_H
