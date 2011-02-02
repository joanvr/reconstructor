#include "io.h"

#include <fstream>
#include <iostream>
#include "Matrix.h"
using namespace std;


int input_matrix (const char* filename, Matrix &R, Matrix &G, Matrix &B, bool yuv)
{
    char line[80];
    int xres, yres, maxintensity;

    ifstream fp(filename, ifstream::in);
    if (!fp.good()) {
        cerr << "Couldn't open file " << filename << endl;
        return 0;
    }

    fp.getline(line,80);
    if ((line[0]=='P') && (line[1]=='6')) {
        while (fp.peek()=='#') fp.getline(line,80);
        fp >> yres >> xres >> maxintensity;

        R = Matrix(xres, yres);
		G = Matrix(xres, yres);
		B = Matrix(xres, yres);
        fp.getline(line,80);  // read past eol
        for (int i=0; i<xres; ++i) {
            for (int j=0; j<yres; ++j) {
            	const int r = fp.get();
	            const int g = fp.get();
	            const int b = fp.get();

				if (yuv) {
	            	double Y  = (0.257 * r) + (0.504 * g) + (0.098 * b) + 16;
					double V =  (0.439 * r) - (0.368 * g) - (0.071 * b) + 128;
					double U = -(0.148 * r) - (0.291 * g) + (0.439 * b) + 128;
					R[i][j] = Y;//r;
					G[i][j] = V;//g;
					B[i][j] = U;//b;
				}
				else {
					R[i][j] = r;
					G[i][j] = g;
					B[i][j] = b;
				}
            }
		}
    } else {
        cerr << "Can't read file format \"" << line << "\"; aborting." << endl;
		fp.close();
        return 0;
    }
	fp.close();
    return maxintensity;
}


bool input_samples(const char* filename, Matrix& S) {
    ifstream fp(filename, ifstream::in);
    if (!fp.good()) {
        cerr << "Couldn't open file " << filename << endl;
        return false;
    }
	int rows, cols;
	fp >> rows >> cols;
	S = Matrix(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			fp >> S[i][j];
		}
	}
	fp.close();
	return true;
}

// output the data to a .ppm file
bool output_matrix(const char* filename, const Matrix &R, const Matrix &G, const Matrix &B, int maxintensity, bool yuv)
{
	ofstream fp(filename, ofstream::out);
	if (!fp.good()) {
		cerr << "Couldn't open file " << filename << endl;
		return false;
	}
	fp << "P6" << endl;
	fp << R.getCols() << " " << R.getRows() << endl;

	// Searching for the max intensity value
	fp << maxintensity << endl;

	for (int i = 0; i < R.getRows(); ++i) {
		for (int j = 0; j < R.getCols(); ++j) {

			if (yuv) {
				double Y = R.get(i,j);
				double V = G.get(i,j);
				double U = B.get(i,j);
				
				int b = 1.164*(Y - 16) + 2.018*(U - 128);
				int g = 1.164*(Y - 16) - 0.813*(V - 128) - 0.391*(U - 128);
				int r = 1.164*(Y - 16) + 1.596*(V - 128);

				fp.put(r);
				fp.put(g);
				fp.put(b);
			}
			else {
				int r = R.get(i,j);
				int g = G.get(i,j);
				int b = B.get(i,j);

				fp.put(r);
				fp.put(g);
				fp.put(b);
			}
		}
	}
	fp.flush();
	fp.close();

	return true;
}


bool output_samples(const char* filename, const Matrix& S) {
	ofstream fp(filename, ofstream::out);
    if (!fp.good()) {
        cerr << "Couldn't open file " << filename << endl;
        return false;
    }
	int rows = S.getRows();
	int cols = S.getCols();
	fp << rows << " " << cols << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			fp << S.get(i, j) << " ";
		}
		fp << endl;
	}
	fp.flush();
	fp.close();
	return true;
}

// output the data to a .ppm file
