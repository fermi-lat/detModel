#ifndef MANAGER_H
#define MANAGER_H
#include <map>

#include "detModel/Management/Manager.h"
#include "detModel/Management/Visitor.h"
#include "detModel/Gdd.h"

namespace detModel{

  class Builder;

  /**
   * This class implement the manager for the sections part of the XML file. 
   * It is a singleton.
   * @author D.Favretto & R.Giannitrapani
   */
  class Manager {
  public:
    enum buildType{all,constants,sections,materials};

  public:
  
    /** The destructor must be invoched by the client before leaving the
	application. It will destroy explicitely the GDD object and this
	will start the deallocation of all the objects hierarchy */
    ~Manager();

    /** This method is used to access the pointer to the singleton */
    static Manager* getPointer();
  
    /** This method is used to specify a builder to be used by the manager. */
    void setBuilder(Builder * b);

    /** This method start the parse of the xml file by the builder.
     */
    void build(buildType);

    /// This method gives back the actual mode for choice elements
    std::string getMode(){return manMode;};

    /// This method sets the actual mode for choice elements
    void setMode(std::string pmode){manMode = pmode;};

    /// This method starts the visit of a hierarchy with a specified visitor
    void startVisitor(Visitor*);
  
    void cleanGdd();

    /** This method gives back the pointer to the GDD object. It coule be
	used by expert clients to access information without the need of
	a visitor */
    Gdd* getGdd(){return manGdd;};

    MatCollection* getMatCollection(){return manGdd->getMaterials();};

    void setNameFile(char* pname){nameFile = pname;};
    char* getNameFile(){return nameFile;};

  protected:
    /** The constructor is protected; in such a way it is forbidden to *
	build directely a manager. Here the new GDD object is explicitely
	built */
    Manager():manBuilder(0),manMode(""){manGdd = new Gdd;};

  private:

    /** 
	This is a private pointer to the GDDsectionsContainer that
	provide the entry point to all the generic model hierarchy of
	sections */
    Gdd * manGdd;  
    /**
       This is the pointer to the only manager that can be istanciated */
    static Manager* pointer;
    /** This is a private pointer to the builder used by the manager to
	build the hierarchy from the XML file */
    Builder * manBuilder;
    /// This string represent the mode for choices elements
    std::string manMode;

    char* nameFile;
  };

}
#endif //MANAGER_H
