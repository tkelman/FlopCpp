// ******************** FlopCpp **********************************************
// File: MP_variable.cpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
//****************************************************************************

#include <iostream>
#include <sstream>
using std::cout;
using std::endl;

#include <OsiSolverInterface.hpp>
#include "MP_variable.hpp"
#include "MP_domain.hpp" 
#include "MP_constant.hpp" 
#include "MP_model.hpp"
using namespace flopc;

 
MP_variable::MP_variable(const MP_set_base &s1, 
			 const MP_set_base &s2, 
			 const MP_set_base &s3,
			 const MP_set_base &s4, 
			 const MP_set_base &s5) :
  RowMajor(s1.size(),s2.size(),s3.size(),s4.size(),s5.size()),
  upperLimit(MP_data(s1,s2,s3,s4,s5)),
  lowerLimit(MP_data(s1,s2,s3,s4,s5)),
  S1(&s1),S2(&s2),S3(&s3),S4(&s4),S5(&s5),
  offset(-1)
{
  lowerLimit.initialize(0.0);
  upperLimit.initialize(MP_model::getDefaultModel().getInfinity());
  type = continuous;
}    

double MP_variable::level(int i1, int i2, int i3, int i4, int i5) {
  return M->Solver->getColSolution()[offset +  f(i1,i2,i3,i4,i5)];
}

void MP_variable::operator()() const {
  if (S1!=&MP_set::getEmpty()) cout << i1.evaluate() << " ";
  if (S2!=&MP_set::getEmpty()) cout << i2.evaluate() << " ";
  if (S3!=&MP_set::getEmpty()) cout << i3.evaluate() << " ";
  if (S4!=&MP_set::getEmpty()) cout << i4.evaluate() << " ";
  if (S5!=&MP_set::getEmpty()) cout << i5.evaluate() << " ";
  cout<<"  "<< M->Solver->getColSolution()[offset +
                                           f(i1.evaluate(),
                                             i2.evaluate(),
                                             i3.evaluate(),
                                             i4.evaluate(),
                                             i5.evaluate())]<<endl;
}

void MP_variable::display(const std::string &s) {
  cout<<s<<endl;
  if (offset >= 0) {
    ((*S1)(i1)*(*S2)(i2)*(*S3)(i3)*(*S4)(i4)*(*S5)(i5)).forall(this);
  } else {
    cout<<"No solution available!"<<endl;
  }
}