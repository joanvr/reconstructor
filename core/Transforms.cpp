#include "Transforms.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <sstream>

void getRandomOnes(Matrix& M, int seed) {
	
	srand(seed);

	int n = M.getCols();	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int r = rand()%2;
			r = r*2-1;
			M[i][j] = r/sqrt(n);
		}
	}

}


void getRandomZeroOnes(Matrix& M, int seed) {

	srand(seed);

	int n = M.getCols();	
	for (int i = 0; i < n; i++) {
		double t = 0.0;
		for (int j = 0; j < n; j++) {
			int r = rand()%6;
			if (r == 0)      M[i][j] =  sqrt(3);
			else if (r == 1) M[i][j] = -sqrt(3);
			else 			 M[i][j] = 0;
			t += M[i][j]*M[i][j];
		}
		if (t > 0) {
			t = sqrt(t);
			for (int j = 0; j < n; j++) {
				M[i][j] /= t;
			}
		}
	}

}


void getTransformFromFile(Matrix& M, const char* name, int blockSize) {
    std::ostringstream oss;
    oss << "./wavs/" << name << "_" << blockSize << ".txt";
    std::string filename = oss.str();
    std::cerr << filename << std::endl;

    std::ifstream f(filename.c_str(), std::ifstream::in);
    if (!f.good()) {
        return;
	}

	int n = M.getCols();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			double d;
			f >> d;
			M[i][j] = d;
		}
	}

	f.close();
}


