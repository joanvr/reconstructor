#ifndef RECONSTRUCTOR_H
#define RECONSTRUCTOR_H

#include <string>
#include "Image.h"
#include "Matrix.h"
#include "ReconstructorListener.h"

enum Transform {RANDOM1, RANDOMSQRT3, DCT, DAUBECHIES8, CDF};
#define INVALID_TRANSFORM -3

class Reconstructor {

public:
    Reconstructor();
    ~Reconstructor();

    void setFile(std::string file);

    void setTransform(int transform);
    void setBlockSize(int blockSize);
    void setSigmaLambda(double sigmaLambda);
    void setColorSpace(bool yuv);
    void setNThreads(int nthreads);

    void setReconstructorListener(ReconstructorListener* l);

    void reconstruct();
    void stop();

    int blockSize;
	int blockMask;
	int blockBits;
    double sigmaLambda;
    bool running;
    bool yuv;
    Image Y;
    Image T;
    Image R;

    Matrix Psinv;

    ReconstructorListener* listener;


private:
    std::string m_file;
    int m_transform;
    int m_nthreads;

};

inline void Reconstructor::setFile(std::string file) {
    m_file = file;
}

inline void Reconstructor::setTransform(int transform) {
    m_transform = transform;
}

inline void Reconstructor::setBlockSize(int bs) {
    blockSize = bs;
	blockMask = bs-1;
	blockBits = 0;
	bs = bs >> 1;
	while (bs) {
		blockBits++;
		bs = bs >> 1;
	}
}

inline void Reconstructor::setSigmaLambda(double sl) {
    sigmaLambda = sl;
}

inline void Reconstructor::setColorSpace(bool b) {
    yuv = b;
}

inline void Reconstructor::setNThreads(int nthreads) {
    m_nthreads = nthreads;
}

inline void Reconstructor::setReconstructorListener(ReconstructorListener *l) {
    listener = l;
}

#endif // RECONSTRUCTOR_H
