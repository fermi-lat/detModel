#include "detModel/Management/DMDocClient.h"
#include "detModel/Utilities/Global.h"

#include "dom/DOM_Element.hpp"
#include "dom/DOMString.hpp"
#include "xml/Dom.h"

#include <string>
#include <iostream>

namespace detModel{

  void DMDocClient::handleChild(DOM_Node node) {
    
    DOMString eltName = node.getNodeName();
    std::string name(xml::Dom::transToChar(eltName));

    if(name == "constants")
      {
	m_constants = node;
	std::cout << "detModel Doc Client: registered the constants!" << std::endl; 
      }
    else if (name == "section")
      {
	m_sections = node;
	std::cout << "detModel Doc Client: registered the sections!" << std::endl; 
      }
    else if (name == "materials")
      {
	m_materials = node;
	std::cout << "detModel Doc Client: registered the materials!" << std::endl; 
      }
    else
      {
	detAbort ("detModel Doc Client asked for an unknown DOM element!");
      }
  }
}

