#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "detModel/Gdd.h"
#include "detModel/Constants/Constants.h"
#include "detModel/Constants/ConstCategory.h"
#include "detModel/Constants/IntConst.h"
#include "detModel/Constants/FloatConst.h"
#include "detModel/Constants/StringConst.h"
#include "detModel/Constants/DoubleConst.h"
#include "detModel/Management/HtmlConstantsVisitor.h"

namespace detModel{

  HtmlConstantsVisitor::HtmlConstantsVisitor()
  {
    setRecursive(0);
    out.open("constants.html");
  }

  HtmlConstantsVisitor::~HtmlConstantsVisitor()
  {
    out.close();
  }


  void HtmlConstantsVisitor::visitGdd(Gdd* gdd)
  {

    out << "<html>" << std::endl;
    out << "<head>" << std::endl;
    out << "<title>Detector Constants</title>" << std::endl;
    out << "</head>" << std::endl;
    out << "<body>" << std::endl;

    gdd->getConstants()->AcceptNotRec(this);

    out << "Generated by detModel from " << gdd->getCVSid() << std::endl;  
    out << "</body>" << std::endl;  
    out << "</html>" << std::endl;  
  }
  
  void  HtmlConstantsVisitor::visitConstants(Constants* constants)
  {
    unsigned int i;

    out << "<H1 align=\"center\"> Detector Constants </H1>" <<std::endl;

    out << "<H3> Primary Constants </H3>" <<std::endl;
    out << "<ul>" << std::endl;
    for(i=0; i<constants->getCategories().size();i++){
      if(constants->getCategories()[i]->getPrimary())
        {
          std::string name = constants->getCategories()[i]->getName();
          out << "<li>" << std::endl;
          out << "<a href=\"#" << name << "\">" << name << "</a>" << std::endl;
          out << "</li>" << std::endl;        
        }
    }
    out << "</ul>" << std::endl;

    out << "<H3> Derived Constants </H3>" <<std::endl;
    out << "<ul>" << std::endl;
    for(i=0; i<constants->getCategories().size();i++){
      if(!constants->getCategories()[i]->getPrimary())
        {
          std::string name = constants->getCategories()[i]->getName();
          out << "<li>" << std::endl;
          out << "<a href=\"#" << name << "\">" << name << "</a>" << std::endl;
          out << "</li>" << std::endl;        
        }
    }
    out << "</ul>" << std::endl;

    for(i=0; i<constants->getCategories().size();i++){
      ((constants->getCategories() )[i])->AcceptNotRec(this);}    
  }

  void  HtmlConstantsVisitor::visitConstCategory(ConstCategory* category)
  {
    unsigned int i;

    if (category->getPrimary())
      out << "<h2>Primary Category <font color=\"#f08000\">" << 
        "<a name=\"" << category->getName() << "\">" <<
        category->getName() << "</font> </h2>" << std::endl;
    else
      out << "<h2>Derived Category <font color=\"#f08000\">" << 
        "<a name=\"" << category->getName() << "\">" <<
	category->getName() << "</font> </h2>" << std::endl;

    out << "<strong><font color=\"#208020\">Overview:</font></strong> &nbsp;" 
	<< category->getOverview() << "<br> &nbsp;<br>" << std::endl;

    out << " <table cellpadding=\"3\" border=\"1\">" << std::endl;
    if (category->getPrimary()) 
      out << "<tr bgcolor=\"#c0ffc0\" align=\"left\">" << std::endl;
    else
      out << "<tr bgcolor=\"#c0coff\" align=\"left\">" << std::endl;
    out << "<th>Name</th><th>Value</th><th>Description</th> </tr>" << std::endl;

    for(i=0; i<category->getConsts().size();i++){
      Const* c = (category->getConsts() )[i];

      out << "<tr>" << std::endl;
      out << "<td><b>" << c->getName() << "</b></td>" << std::endl;    
      c->AcceptNotRec(this);
    
      if(c->getConstMeaning() == Const::length)
	out <<  " " << c->getUnitLength();

      out << "</td>" << std::endl;
      out << "<td>" << c->getNote() << "</td>" << std::endl;     
      out << "</tr>" << std::endl;
    }
  
    out << "</table> <br> &nbsp;<br>" << std::endl;
  

  }

  void  HtmlConstantsVisitor::visitIntConst(IntConst* c)
  {
    out << "<td>" << c->getValue();
  }

  void  HtmlConstantsVisitor::visitFloatConst(FloatConst* c)
  {
    out << "<td>" << c->getValue();
  }

  void  HtmlConstantsVisitor::visitDoubleConst(DoubleConst* c)
  {
    out << "<td>" << c->getValue();
  }

  void  HtmlConstantsVisitor::visitStringConst(StringConst* c)
  {
    out << "<td>" << c->getValue();
  }

}
