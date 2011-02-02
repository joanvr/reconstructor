#include "Dantzig.h"
#include "Matrix.h"
#include "gurobi_c++.h"
#include <sstream>


GRBEnv* DantzigSelector::env = NULL;

DantzigSelector::DantzigSelector() {
	namesU = NULL;
	namesV = NULL;
	coefs  = NULL;
	types  = NULL;
	model  = NULL;
	u      = NULL;
	v      = NULL;
	constrLeft  = NULL;
	constrRight = NULL;

	initialized = false;
}


DantzigSelector::~DantzigSelector() {
	if (namesU) 	 delete[] namesU;
	if (namesV) 	 delete[] namesV;
	if (coefs)  	 delete[] coefs;
	if (types)  	 delete[] types;
	if (constrLeft)  delete[] constrLeft;
	if (constrRight) delete[] constrRight;
	if (expcfs)  	 delete[] expcfs;
	if (model)		 delete model;
}


void DantzigSelector::init(int size) {
	// dimensions
	n = size;

	// variables
    namesU = new std::string[n];
    namesV = new std::string[n];
	coefs  = new double[n];
	types  = new char[n];
	for (int i = 0; i < n; i++) {
		std::ostringstream oss;
		oss << i;
		namesU[i] = "u" + oss.str();
		namesV[i] = "v" + oss.str();
		coefs [i] = 1.0;
		types [i] = GRB_CONTINUOUS;
	}

	// creem el model
	std::cout << "  Creating Gurobi model..." << std::endl;
	model = new GRBModel(*DantzigSelector::env);

	// posem les variables
	std::cout << "  Adding variables to model..." << std::endl;
	u = model->addVars(NULL, NULL, coefs, types, namesU, n);
	v = model->addVars(NULL, NULL, coefs, types, namesV, n); 
	model->update();
	
	// indiquem les restriccions (DANTZIG SELECTOR)
	// el terme independent l'afegirem despres
	std::cout << "  Adding constraints to model..." << std::endl;
	constrLeft  = new GRBConstr[n]; 
	constrRight = new GRBConstr[n]; 
	for (int i = 0; i < n; i++) {
		// creem la expressio lineal
		GRBLinExpr expr = v[0] + u[0];
		for (int c = 1; c < n; c++) {
			expr += v[c] + u[c];
		}
		// posem les restriccions al gurobi
		constrLeft[i]  = model->addConstr(expr, GRB_GREATER_EQUAL, 0);
		constrRight[i] = model->addConstr(expr, GRB_LESS_EQUAL,    0);
	}

	model->update();

	vecConstrs = new GRBConstr[4*n*n];
	vecVars    = new GRBVar   [4*n*n];
	vecVals    = new double   [4*n*n];
	expcfs     = new float[n];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			vecConstrs[i*4*n+j*4+0] = constrLeft[i];
			vecVars   [i*4*n+j*4+0] = u[j];

			vecConstrs[i*4*n+j*4+1] = constrLeft[i];
			vecVars   [i*4*n+j*4+1] = v[j];

			vecConstrs[i*4*n+j*4+2] = constrRight[i];
			vecVars   [i*4*n+j*4+2] = u[j];
			
			vecConstrs[i*4*n+j*4+3] = constrRight[i];
			vecVars   [i*4*n+j*4+3] = v[j];
		}
	}
	
	initialized = true;
}


void DantzigSelector::setCoeffs(const Matrix& A, int nrows) {
	k = nrows;
	// indiquem les restriccions (DANTZIG SELECTOR)
	// el terme independent l'afegirem despres
	for (int i = 0; i < n; i++) {
		// calculem la restriccio i de:  -sl <= A'(b - Au + Av) <= sl
		for (int c = 0; c < n; c++)
			expcfs[c] = 0.0;	
		// fem la fila j de: A' * (b - Au + Av)
		for (int j = 0; j < k; j++) {
			for (int l = 0; l < n; l++) {
				expcfs[l] += A.get(j,i) * A.get(j, l);
			}
		}
		// creem la expressio lineal
		for (int c = 0; c < n; c++) {
			vecVals[i*4*n+c*4+0] = -expcfs[c];
			vecVals[i*4*n+c*4+1] =  expcfs[c];
			vecVals[i*4*n+c*4+2] = -expcfs[c];
			vecVals[i*4*n+c*4+3] =  expcfs[c];
//			model->chgCoeff(constrLeft[i],  u[c], -expcfs[c]);
//			model->chgCoeff(constrLeft[i],  v[c],  expcfs[c]);
//			model->chgCoeff(constrRight[i], u[c], -expcfs[c]);
//			model->chgCoeff(constrRight[i], v[c],  expcfs[c]);
		}

	}
	model->chgCoeffs(vecConstrs, vecVars, vecVals, 4*n*n);

	model->update();

}


bool DantzigSelector::solve(const Matrix& A, const double* b, double siglam, double* res) {
	
	if (!initialized) return false;
	
	// indiquem el terme independent
	std::cout << "Setting up sampled values to constraints..." << std::endl;
	
	for (int i = 0; i < n; i++) {	
		float indep = 0.0;	
		for (int j = 0; j < k; j++) {
			indep += A.get(j, i) * b[j];
		}	
		constrLeft[i].set(GRB_DoubleAttr_RHS, -siglam -indep);
		constrRight[i].set(GRB_DoubleAttr_RHS, siglam -indep);
	}

	// busquem la solucio
	std::cout << "Optimizing..." << std::endl;
	try {		
		model->optimize();
	}
	catch (GRBException grbe) {
		std::cout << grbe.getMessage() << std::endl;
		return false;
	}

	// recollim resultats
	for (int i = 0; i < n; i++) {
		float uval = u[i].get(GRB_DoubleAttr_X);
		float vval = v[i].get(GRB_DoubleAttr_X);
		res[i] = uval - vval;
	}
	
	return true;

}

