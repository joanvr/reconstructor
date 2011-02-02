#ifndef MATRIX_H
#define MATRIX_H

#include<iostream>

/* Classe matriu general */

/* Resize: canvia la mida. Si es menor, perdem elements. Si es major posa zeros */
/* Producte: multiplica la matriu (M x N) per m2 (M2 x N2). 
 *           Resultat es una matriu (M x N2)  si N = M2
 *           Si N != M2 retorna una matriu de 0x0
 */
/* Transposta: retorna una matriu intercanviant files per columnes */


class Matrix {
	
public:
		Matrix();
		Matrix(const Matrix &m);
		Matrix(int rows, int cols);
		Matrix(float **m, int rows, int cols);
		~Matrix();
		
		int      getRows() const 					 { return nRows; };
		int      getCols() const 					 { return nCols; };
		float    get(int row, int col) const 		 { return mat[row][col]; };
		void     set(int row, int col, float value) { mat[row][col] = value; };
		void     setRow(int row, float* v);	
		float**  getData() const;	
        void     writeIn(std::ostream& os) const;
		
		Matrix   getSubMatrix(int row, int col, int nrows, int ncols) const;
		Matrix   multMatrix(const Matrix& m2) const;
        Matrix	 transposed() const;

        /* Operadors */
        void       operator=(const Matrix& m2);
		Matrix     operator*(const Matrix& m2) const;
		Matrix	   operator+(const Matrix& m2) const;
		Matrix	   operator-(const Matrix& m2) const;
		float*       operator[](int i) { return mat[i]; };
		

protected:

		void deleteData();

		float **mat;
		int nRows;
		int nCols;

};

#endif
