/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file Method.ccp
 * @author Frank Dellaert
 **/

#include <iostream>
#include <fstream>

#include <boost/foreach.hpp>

#include "Method.h"
#include "utilities.h"

using namespace std;

/* ************************************************************************* */
void Method::matlab_mfile(const string& classPath) {

  // open destination m-file
  string wrapperFile = classPath + "/" + name_ + ".m";
  ofstream ofs(wrapperFile.c_str());
  if(!ofs) throw CantOpenFile(wrapperFile);
  if(verbose_) cerr << "generating " << wrapperFile << endl;

  // generate code
  string returnType = returnVal_.matlab_returnType();
  ofs << "function " << returnType << " = " << name_ << "(obj";
  if (args_.size()) ofs << "," << args_.names();
  ofs << ")" << endl;
  ofs << "% usage: obj." << name_ << "(" << args_.names() << ")" << endl;
  ofs << "  error('need to compile " << name_ << ".cpp');" << endl;
  ofs << "end" << endl;

  // close file
  ofs.close();
}

/* ************************************************************************* */
void Method::matlab_wrapper(const string& classPath, 
			    const string& className,
			    const string& nameSpace) 
{
  // open destination wrapperFile
  string wrapperFile = classPath + "/" + name_ + ".cpp";
  ofstream ofs(wrapperFile.c_str());
  if(!ofs) throw CantOpenFile(wrapperFile);
  if(verbose_) cerr << "generating " << wrapperFile << endl;

  // generate code

  // header
  emit_header_comment(ofs, "//");
  ofs << "#include <wrap/matlab.h>\n";
  ofs << "#include <" << className << ".h>\n";
  if (!nameSpace.empty()) ofs << "using namespace " << nameSpace << ";" << endl;

  // call
  ofs << "void mexFunction(int nargout, mxArray *out[], int nargin, const mxArray *in[])\n";
  // start
  ofs << "{\n";

  // check arguments
  // extra argument obj -> nargin-1 is passed !
  // example: checkArguments("equals",nargout,nargin-1,2);
  ofs << "  checkArguments(\"" << name_ << "\",nargout,nargin-1," << args_.size() << ");\n";

  // get class pointer
  // example: shared_ptr<Test> = unwrap_shared_ptr< Test >(in[0], "Test");
  ofs << "  shared_ptr<" << ((is_const_) ? "const " : "") << className << "> self = unwrap_shared_ptr< " << className
      << " >(in[0],\"" << className << "\");" << endl;

  // unwrap arguments, see Argument.cpp
  args_.matlab_unwrap(ofs,1);

  // call method
  // example: bool result = self->return_field(t);
  ofs << "  ";
  if (returnVal_.returns_!="void")
    ofs << returnVal_.return_type(true,ReturnValue::pair) << " result = ";
  ofs << "self->" << name_ << "(" << args_.names() << ");\n";

  // wrap result
  // example: out[0]=wrap<bool>(result);
  returnVal_.wrap_result(ofs);

  // finish
  ofs << "}\n";

  // close file
  ofs.close();
}

/* ************************************************************************* */
