#ifndef CALCULUS_H_INCLUDED
#define CALCULUS_H_INCLUDED

#include "PlotFunction.h"
#include <valfunction.h>

int partialfraction(const val::valfunction& f, val::rational &cont, val::pol<val::rational> &fp,val::d_array<val::pol<val::rational>> &numpol,
                    val::d_array<val::pol<val::rational>> &denompol, val::d_array<int> &denumexpo, int comment = 0);


void analize_rationalfunction(val::valfunction& f,const double& eps=1e-9,int decimal=4);

void analyzefunction(const plotobject &f,std::string input);

void intersection(const plotobject &f, const plotobject &g, std::string input);

val::valfunction integral(const val::valfunction &f, int k = 1);

void computeintegral(const plotobject& f,std::string x1,std::string x2,double delta,int n,int dez,int arclength=0);



#endif // CALCULUS_H_INCLUDED
