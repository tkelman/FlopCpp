// ******************** flopc++ **********************************************
// File: MP_constraint.cpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
//****************************************************************************

#include <iostream>
#include <sstream>
using std::cout;
using std::endl;

#include "MP_constraint.hpp"
#include "MP_expression.hpp"
#include "MP_model.hpp"
#include "MP_constant.hpp"
#include "MP_data.hpp"

using namespace flopc;

std::string MP_constraint::toString() const {
	std::stringstream ss;
	ss<<" S.T.: "<<getName();
	for (size_t i=offset; i<offset+size(); i++) 
	{
		if(M->bl!=NULL)
		{
			ss<<"@"<<i<<" lb/ub:"<<M->bl[i]<<"/"<<M->bu[i];
			if(M->rowActivity)
				ss<<(M->rowActivity[i]!=0)?" active:":" inactive";
			else
				ss<<" No row info: solve not run";
			if(M->rowPrice)
				ss<<"  RowPrice:"<<M->rowPrice[i];
			ss<<std::ends;
		}
		else
		{
			ss<<" Model not yet generated";
		}
		ss<<"-->"<<left->toString()<<" "<<right->toString()<<"<--";
	}
	return ss.str();
}

void MP_constraint::operator=(const Constraint &v) {
   left = v.left;
   right = v.right;
   sense = v.sense;
}

int MP_constraint::row_number() const { 
    return offset + f(I1->evaluate(),I2->evaluate(),I3->evaluate(),
		      I4->evaluate(),I5->evaluate()); 
}

double MP_constraint::price(int i1, int i2, int i3, int i4, int i5) const {
    return  M->rowPrice[offset + f(i1,i2,i3,i4,i5)];
}
 
MP_constraint::MP_constraint(
    const MP_set_base &s1, 
    const MP_set_base &s2, 
    const MP_set_base &s3,
    const MP_set_base &s4, 
    const MP_set_base &s5) :
    RowMajor(s1.size(),s2.size(),s3.size(),s4.size(),s5.size()),
//     pprice(MP_data(d1->getSet(),d2->getSet(),d3->getSet(),
// 		   d4->getSet(),d5->getSet())),
    S1(s1),S2(s2),S3(s3),S4(s4),S5(s5),
    I1(0),I2(0),I3(0),I4(0),I5(0) {
    MP_model::current_model->add(*this);
}

void MP_constraint::coefficients(GenerateFunctor& f) {
    f.setConstraint(this);

    vector<Constant> v;

        if (I1.root!=0) {
	left->generate(S1(I1)*S2(I2)*S3(I3)*S4(I4)*S5(I5).such_that(B),v,f,1.0);
 	right->generate(S1(I1)*S2(I2)*S3(I3)*S4(I4)*S5(I5).such_that(B),v,f,-1.0);
	 } else {
		cout<<"FLOPC++ Warning: Constraint declared but not defined."<<endl;
	}
}

void MP_constraint::insertVariables(set<MP_variable*>& v) {
    if (left.root!=0) {
	left->insertVariables(v);
    }
    if (right.root!=0) {
	right->insertVariables(v);
    }
}

void MP_constraint::display(string s) const {
    cout<<s<<endl;
    for (int i=offset; i<offset+size(); i++) {
	cout<<i<<"  "<<M->bl[i]<<"  "<<M->rowActivity[i]<<"  "<<M->bu[i]<<"  "<<M->rowPrice[i]<<endl;
    }
}
