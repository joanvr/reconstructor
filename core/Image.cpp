#include "Image.h"
#include <string>

#include "datatypes.h"
#include "io.h"

Image::Image() {
	m_nextBlock = 0;
	pthread_mutex_init(&m_mutex, NULL);
	
	m_colsBlocks = 0;
	m_maxBlocks = 0;

	m_maxIntensity = 0;
}

Image::Image(std::string name, bool samples, bool yuv, int blockSize) {

    std::string imgfile = name;
    std::string smpfile = name.substr(0, name.length()-4) + ".txt";
	
	if ((m_maxIntensity = input_matrix(imgfile.c_str(), m_R, m_G, m_B, yuv)) == 0)
		throw IMAGE_NOT_FOUND;
	
	if (samples && !input_samples(smpfile.c_str(), m_S))
		throw SAMPLES_NOT_FOUND;

	m_nextBlock = 0;
	pthread_mutex_init(&m_mutex, NULL);

    setBlockSize(blockSize);
}

Image::Image(int width, int height, int maxintensity, int blockSize) {
	m_R = Matrix(height, width);
	m_G = Matrix(height, width);
	m_B = Matrix(height, width);
	
	m_nextBlock = 0; // Probably not used if created that way
	pthread_mutex_init(&m_mutex, NULL);
	
    setBlockSize(blockSize);

	m_maxIntensity = maxintensity;
}

Image::~Image() {

}

void Image::setBlockSize(int size) {
    m_blockSize = size;
    m_colsBlocks = m_R.size2() / m_blockSize;
    m_maxBlocks = m_colsBlocks * (m_R.size1() / m_blockSize);
}

bool Image::save(std::string name, bool yuv) {
    std::string imgname = name + ".ppm";
	return output_matrix(imgname.c_str(), m_R, m_G, m_B, m_maxIntensity, yuv);
}

int Image::getNextBlock(Matrix& R, Matrix& G, Matrix& B, std::vector<int>& S) {
	pthread_mutex_lock(&m_mutex);
	int n = m_nextBlock++;
	pthread_mutex_unlock(&m_mutex);
	if (n >= m_maxBlocks) return -1;

    int r = (n / m_colsBlocks) * m_blockSize;
    int c = (n % m_colsBlocks) * m_blockSize;
	int k = 0;
    for (int i = 0; i < m_blockSize; i++) {
        for (int j = 0; j < m_blockSize; j++) {
            if (m_S(r+i, c+j)) {
                R.insert_element(i, j, m_R(r+i, c+j));
                G.insert_element(i, j, m_G(r+i, c+j));
                B.insert_element(i, j, m_B(r+i, c+j));
                k++;
            }
        }
    }
    
    S.clear();
    S = std::vector<int>(k); 
    k = 0;
    for (int i = 0; i < m_blockSize; i++) {
        for (int j = 0; j < m_blockSize; j++) {
            if (m_S(r+i, c+j)) {
                S[k] = i*m_blockSize + j;
                k++;
            }
        }
    }
    return n;
}

void Image::setBlock(int block, const Matrix& R, 
								const Matrix& G, 
								const Matrix& B) 
{
    int r = (block / m_colsBlocks) * m_blockSize;
    int c = (block % m_colsBlocks) * m_blockSize;
    
    for (int i = 0; i < m_blockSize; i++) {
        for (int j = 0; j < m_blockSize; j++) {
            m_R.insert_element(r+i, c+j, R(i, j));
            m_G.insert_element(r+i, c+j, G(i, j));
            m_B.insert_element(r+i, c+j, B(i, j));
        }
    }
    
}

