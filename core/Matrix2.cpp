#include "Matrix.h"


/* Constructores */
Matrix::Matrix() {
    nRows = 0;
    nCols = 0;
    mat = 0;
}

Matrix::Matrix(const Matrix &m) {
    nRows = m.nRows;
    nCols = m.nCols;
    mat = new double*[nRows];
    for (int i = 0; i < nRows; i++) {
        mat[i] = new double[nCols];
        for (int j = 0; j < nCols; j++) {
            mat[i][j] = m.mat[i][j];       
        }
    }
}

Matrix::Matrix(int rows, int cols) {
	nRows = rows;
    nCols = cols;
    mat = new double*[nRows];
    for (int i = 0; i < nRows; i++) {
        mat[i] = new double[nCols];
        for (int j = 0; j < nCols; j++) {
            mat[i][j] = 0;       
        }
    }
}

Matrix::Matrix(double **m, int rows, int cols) {
    nRows = rows;
    nCols = cols;
    mat = new double*[rows];
    for (int i = 0; i < rows; i++) {
        mat[i] = new double[cols];
        for (int j = 0; j < cols; j++) {
            mat[i][j] = m[i][j];       
        }
    }
}

/* Destructora */
Matrix::~Matrix() {
    deleteData();
}


void Matrix::deleteData() {
	if (mat != 0 && nRows > 0) {
		for (int i = 0; i < nRows; i++) {
   	    	if (mat[i] != 0) delete mat[i];
    	}
    	delete[] mat;
	}
}






/* Consultores */
double** Matrix::getData() const {
    double **res = new double*[nRows];
    for (int i = 0; i < nRows; i++) {
        res[i] = new double[nCols];
        for (int j = 0; j < nCols; j++) {
            res[i][j] = mat[i][j];       
        }
    }
    return res;
}

Matrix Matrix::getSubMatrix(int row, int col, int nr, int nc) const {
    if (row + nr >= nRows) nr = nRows - row;
    if (col + nc >= nCols) nc = nCols - col;
    if (nr == 0 || nc == 0) return Matrix(0, 0);
    Matrix res = Matrix(nr, nc);
    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            res[i][j] = mat[row+i][col+j];
        }   
    }
    return res;
}



void  Matrix::setRow(int row, double* v) {
	if (mat[row]) delete[] mat[row];
	mat[row] = v;
}



/* Operacions */
Matrix  Matrix::transposed() const {
    Matrix res = Matrix(nCols, nRows); 
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            res[j][i] = mat[i][j];       
        }
    }   
    return res;
}

Matrix  Matrix::multMatrix(const Matrix& m2) const {
    if (m2.nRows != nCols) return Matrix(0, 0);
    
	Matrix res = Matrix(nRows, m2.nCols); 
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < m2.nCols; j++) {
			res[i][j] = 0.0;
            for (int k = 0; k < nCols; k++) {
                res[i][j] += mat[i][k]*m2.mat[k][j];
            } 
        }
    }   
    return res;
}


Matrix Matrix::operator*(const Matrix& m2) const {
	if (m2.nRows != nCols) return Matrix(0, 0);

    Matrix res = Matrix(nRows, m2.nCols); 
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < m2.nCols; j++) {
			res[i][j] = 0.0;
            for (int k = 0; k < nCols; k++) {
                res[i][j] += mat[i][k]*m2.mat[k][j];
            } 
        }
    } 
    return res;
}


Matrix Matrix::operator+(const Matrix& m2) const {
	if (m2.nRows != nRows || m2.nCols != nCols) 
		return Matrix(0, 0);

    Matrix res = Matrix(nRows, m2.nCols); 
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < m2.nCols; j++) {
			res[i][j] = mat[i][j] + m2.mat[i][j];
        }
    } 
    return res;
}


Matrix Matrix::operator-(const Matrix& m2) const {
	if (m2.nRows != nRows || m2.nCols != nCols) 
		return Matrix(0, 0);

    Matrix res = Matrix(nRows, m2.nCols); 
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < m2.nCols; j++) {
			res[i][j] = mat[i][j] - m2.mat[i][j];
        }
    } 
    return res;
}


void Matrix::operator=(const Matrix& m2) {
	deleteData();
    nRows = m2.nRows;
    nCols = m2.nCols;
    mat = new double*[nRows];
    for (int i = 0; i < nRows; i++) {
        mat[i] = new double[nCols];
        for (int j = 0; j < nCols; j++) {
            mat[i][j] = m2.get(i, j);       
        }
    }
}


void Matrix::writeIn(std::ostream& os) const {
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            os << " " << mat[i][j];       
        }
        os << std::endl;
    }  
}

