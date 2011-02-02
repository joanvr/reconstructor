#ifndef _IMAGE_
#define _IMAGE_

#include <string>
#include <vector>
#include "Matrix.h"
#include "io.h"

#define IMAGE_NOT_FOUND   -1
#define SAMPLES_NOT_FOUND -2

class Image {

public:
	Image();
    Image(std::string name, bool samples, bool yuv, int blockSize);
    Image(int width, int height, int maxintensity, int blockSize);
	~Image();

	int getNextBlock(Matrix& R, Matrix& G, Matrix& B, std::vector<int>& S);
	void setBlock(int block, const Matrix& R, const Matrix& G, const Matrix& B);
	void resetNextBlock();

	int getWidth();
	int getHeight();
	int getMaxIntensity();

	void setPixel(int x, int y, double r, double g, double b);

	bool save(std::string name, bool yuv);

    void setBlockSize(int size);

private:
	Matrix m_R;
	Matrix m_G;
	Matrix m_B;

	Matrix m_S;

	int m_nextBlock;
	pthread_mutex_t m_mutex;

	int m_maxBlocks;
	int m_colsBlocks;

	int m_maxIntensity;

        int m_blockSize;
};

inline int Image::getWidth()  { 
	return m_R.getCols(); 
}

inline int Image::getHeight() {
	return m_R.getRows();
}

inline void Image::resetNextBlock() {
	m_nextBlock = 0;
}

inline int Image::getMaxIntensity() {
	return m_maxIntensity;
}

inline void Image::setPixel(int x, int y, double r, double g, double b) {
	m_R[y][x] = r;
	m_G[y][x] = g;
	m_B[y][x] = b;
}

#endif
