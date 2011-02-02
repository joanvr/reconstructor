#ifndef _DANTZIG_SELECTOR_
#define _DANTZIG_SELECTOR_

#include <string>
#include "Matrix.h"
#include "gurobi_c++.h"


class DantzigSelector {

public:
	
	DantzigSelector();
	~DantzigSelector();

	void init (int n);
	void setCoeffs(const Matrix& A, int nrows);
	bool solve(const Matrix& A, const double* b, double siglam, double* res);

	static GRBEnv* env;


private:

//	GRBEnv*		 env;
	GRBModel*	 model;

	std::string* namesU;
	std::string* namesV;
	double*      coefs;
	char*		 types;
	GRBVar*		 u;
	GRBVar*		 v;
	GRBConstr*	 constrLeft;
	GRBConstr*   constrRight;
	float*      expcfs;

	GRBConstr* 	vecConstrs;
	GRBVar* 	vecVars;
	double*      vecVals;


	int			 n;
	int			 k;

	bool		 initialized;

};

#endif
