#include "datatypes.h"

int input_matrix (const char* filename, Matrix &R, Matrix &G, Matrix &B, bool yuv);
bool input_samples(const char* filename, Matrix &S);
bool output_matrix(const char* filename, const Matrix &R, const Matrix &G, const Matrix &B, int maxintensity, bool yuv);
bool output_samples(const char* filename, const Matrix &S);


