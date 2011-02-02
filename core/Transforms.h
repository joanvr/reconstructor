#ifndef _INVERSE_TRANSFORMS_H_
#define _INVERSE_TRANSFORMS_H_

#include "Matrix.h"

void getRandomOnes     (Matrix& Psiinv, int seed);
void getRandomZeroOnes (Matrix& Psiinv, int seed);
void getTransformFromFile(Matrix& Psiinv, const char* filename, int blockSize);

#endif

