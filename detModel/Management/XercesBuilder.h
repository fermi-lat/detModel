#ifndef XERCESBUILDER_H
#define XERCESBUILDER_H
#include "detModel/Management/Builder.h"
#include "xml/XmlParser.h"

namespace detModel{

  class Element;
  class Composite;
  class Section;
  class Box;
  class Composition;
  class SinglePos;
  class StackedPos;
  class Stack;
  class Choice;
  class Const;
  class DMDocClient;
  /**
   * This class is a concrete Builder that use the Xerces parser and the
   * DOM functionalities to parse the xml file and build the generic model
   * hierarchy
   * @author D.Favretto & R.Giannitrapani
   */
  class XercesBuilder : public Builder {
  public:

    /**
     * This is the constructor. It defines the dom tree (domfile), uses
     * the class Substitute to evaluate all the derived constants and substitute
     * them in the sections. It accepts a string for the name of the file  to open
     */
    XercesBuilder();


    virtual ~XercesBuilder();
  
    /** This method initialize the parser 
     */
    virtual void parseFile(std::string nameFile);
    /**
     * This method implement the virutal method of Builder and builds the
     * sections part of the hierarchy, putting it in the private pointer currentSections
     */
    virtual void buildSections();
    /**
     * This method implement the virtual method of Builder and buil the constant part of
     * the hierarchy.
     */
    virtual void  buildConstants();
    /**
     * This method implement the virtual method of Builder and buil the constant part of
     * the hierarchy.
     */
    virtual void  buildMaterials();



  private:
    /**
     * This method build the Section object and return a pointer to it
     */
    Section* buildSection(DOM_Node* e);
    /**
     * This method build the Const object and return a pointer to it
     */
    Const* buildConst(DOM_Node e);
    /// This method builds the Element object and return a pointer to it
    Element* buildElement(DOM_Node* e);
    /// This method builds the Composite object and return a pointer to it
    Composite* buildComposite(DOM_Node* e);
    /**
     * This method build a Choice object and return a pointer to it
     */
    Choice* buildChoice(DOM_Node* e);
    /**
     * This method build a Composition object and return a pointer to it
     */
    Composition* buildComposition(DOM_Node* e);
    /**
     * This method build a Stack object and return a pointer to it
     */
    Stack* buildStack(DOM_Node* e);
    /**
     * This method build a Box object and return a pointer to it
     */
    Box* buildBox(DOM_Node* e);
    /**
     * This method build a Position object and return a pointer to it
     */
    SinglePos* buildPosition(DOM_Node* e);
    StackedPos* buildRelativePosition(DOM_Node* e);

    void setAttributePosition(SinglePos* pos, DOM_Node* e);
    void setAttributeRelativePosition(StackedPos* pos, DOM_Node* e);

  private:
    DMDocClient* m_docClient;
    /**  used to verify if a DOM_Node is a comment */
    enum{Comment=8}; 
  };
}
#endif //XERCESBUILDER_H







