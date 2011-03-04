#include <iostream>
#include "datatypes.h"
#include "Reconstructor.h"
#include "Transforms.h"
#include "Fista.h"

using namespace std;

#define LIMIT 4
#define SOLVER_ITER_LIMIT 500
#define SOLVER_EPS .001
#define SOLVER_TOL .001

Reconstructor::Reconstructor() {
    m_transform = DCT;
    blockSize = 16;
	blockMask = 15;
	blockBits = 4;
    yuv = false;
    m_nthreads = 4;
	listener = NULL;
}

Reconstructor::~Reconstructor() {}

void solve(EllOneSolver& ellonesolver, const Vector& bC, char cC, int block, Vector& RecC) {
    bool sol = false;
    int iter = 0;
    while (!sol && iter < LIMIT) {
        if (iter != 0)
            cerr << "*** Retrying BLOCK " << block
                 << " component " << cC << " (" << iter << ")" << endl;
        sol = ellonesolver.optimize(bC, RecC);
        iter++;
    }
}

void make_sampled_image (int k, int blockBits, int blockMask, Vector& bR, Vector& bG, Vector& bB, const Matrix& YR, const Matrix& YG, const Matrix& YB, const std::vector<int>& S) {
    for (int i = 0; i < k; i++) {
        const int j = S[i];
        bR.insert_element(i, YR(j >> blockBits, j & blockMask));
        bG.insert_element(i, YG(j >> blockBits, j & blockMask));
        bB.insert_element(i, YB(j >> blockBits, j & blockMask));
    }
}

void put_back(int blockSize, int blockBits, const Vector& RecR, const Vector& RecG, const Vector& RecB, Matrix& YR, Matrix& YG, Matrix& YB) {
    for (int i = 0; i < blockSize; i++) {
        for (int j = 0; j < blockSize; j++) {
            YR.insert_element(i, j, RecR[(i << blockBits) + j]);
            YG.insert_element(i, j, RecG[(i << blockBits) + j]);
            YB.insert_element(i, j, RecB[(i << blockBits) + j]);
        }
    }
}

void make_A (int n, int k, const std::vector<int>& S, Matrix& A, const Matrix& Psinv) {
    int ii;
    for (ii = 0; ii < k; ii++) {
        const int row = S[ii];
        for (int j = 0; j < n; j++) {
            A.insert_element(ii, j, Psinv(row, j));
        }
    }
    // Finish with 0
    for (; ii < n; ii++) {
        for (int j = 0; j < n; j++) {
            A.insert_element(ii, j, 0);
        }
    }
}

void make_Ys (int n, int blockBits, int blockMask, const Matrix& Psinv, const Vector& RecR, const Vector& RecG, const Vector& RecB, Matrix& YR, Matrix& YG, Matrix& YB ) {
    for (int i = 0; i < n; i++) {
        int row = i >> blockBits;
        int col = i &  blockMask;
        number_t cr(0.0), cg(0.0), cb(0.0);
        for (int j = 0; j < n; j++) {
            cr += Psinv(i,j)*RecR[j];
            cg += Psinv(i,j)*RecG[j];
            cb += Psinv(i,j)*RecB[j];
        }
        YR.insert_element(row, col, floor(cr));
        YG.insert_element(row, col, floor(cg));
        YB.insert_element(row, col, floor(cb));
    }
}

void* calcBlocks(void* v) {
    Reconstructor* r = (Reconstructor*)v;

    int block;
    int blockSize = r->blockSize;
    int blockMask = r->blockMask;
    int blockBits = r->blockBits;
    Matrix YR(blockSize, blockSize);
    Matrix YG(blockSize, blockSize);
    Matrix YB(blockSize, blockSize);
    std::vector<int> S;

    const int n = blockSize*blockSize;
    Vector 
        RecR(n), 
        RecG(n),
        RecB(n),
        bR(n),
        bG(n),
        bB(n);    
    Matrix A(n, n);
    
    while (r->running && (block = r->Y.getNextBlock(YR, YG, YB, S)) >= 0) {
        cerr << "*** Starting BLOCK " << block << endl;
        const int k = S.size();
        
        // Imatge samplejada
        make_sampled_image(k, blockBits, blockMask, bR, bG, bB, YR, YG, YB, S);

        // Matriu per a la resolucio del CS
        make_A(n, k, S, A, r->Psinv);

        EllOneSolver ellonesolver(A, SOLVER_ITER_LIMIT, SOLVER_EPS, SOLVER_TOL);
        solve(ellonesolver, bR, 'R', block, RecR);
        solve(ellonesolver, bG, 'G', block, RecG);
        solve(ellonesolver, bB, 'B', block, RecB);

        // Put Reconstruction back to Image Block
        put_back(blockSize, blockBits, RecR, RecG, RecB, YR, YG, YB);

        r->T.setBlock(block, YR, YG, YB);

        make_Ys(n, blockBits, blockMask, r->Psinv, RecR, RecG, RecB, YR, YG, YB);
        r->R.setBlock(block, YR, YG, YB);


        cerr << "*** BLOCK " << block << " DONE!" << endl;
        if (r->listener != NULL)
            r->listener->blockCompleted(block, YR, YG, YB);

    }
    
    pthread_exit(NULL);
}

void Reconstructor::reconstruct() {
    try {
        cerr << m_file << endl;
        Y = Image(m_file, true, yuv, blockSize);
        T = Image(Y.getWidth(), Y.getHeight(), Y.getMaxIntensity(), blockSize);
        R = Image(Y.getWidth(), Y.getHeight(), Y.getMaxIntensity(), blockSize);

        Y.save("0-original", yuv);

        int nrows = Y.getHeight();
        int ncols = Y.getWidth();
        int n = nrows*ncols;

        cerr << "  " << n << " pixels from " << nrows << "x" << ncols << " image" << endl;

        // transformacio a fer servir
        cout << "Computing transform..." << endl;
        int ws = blockSize;
        int wavesize = ws*ws;
        Psinv = Matrix(wavesize, wavesize);
        switch(m_transform) {
            case RANDOM1:
                getRandomOnes(Psinv, 0);
            break;
            case RANDOMSQRT3:
                getRandomZeroOnes(Psinv, 0);
            break;
            case DCT:
                getTransformFromFile(Psinv,"dct", blockSize);
            break;
            case DAUBECHIES8:
                getTransformFromFile(Psinv,"db8", blockSize);
            break;
            case CDF:
                getTransformFromFile(Psinv,"cdf", blockSize);
            break;
            default:
                throw INVALID_TRANSFORM;
            break;
        }

		
        cout << "Starting " << m_nthreads << " threads..." << endl;
        running = true;
        pthread_t* threads = new pthread_t[m_nthreads];
        for (int i = 0; i < m_nthreads; i++) {
            pthread_create(&threads[i], NULL, calcBlocks, (void*)this);
        }

        cout << "Waiting for all threads to complete" << endl;
        for (int i = 0; i < m_nthreads; i++) {
            pthread_join(threads[i], NULL);
        }
        delete[] threads;


        T.save("1-transformed", yuv);
        R.save("2-reconstructed", yuv);

		if (listener != NULL)
			listener->finished();

    } catch (int e) {
        switch(e) {
            case IMAGE_NOT_FOUND:
                cerr << "Error: IMAGE NOT FOUND" << endl;
            break;
            case SAMPLES_NOT_FOUND:
                cerr << "Error: SAMPLES NOT FOUND" << endl;
            break;
            case INVALID_TRANSFORM:
                cerr << "Error: INVALID TRANSFORM" << endl;
            break;
            default:
                cerr << "Error: UNKNOWN ERROR" << endl;
            break;
        }
    }
}

void Reconstructor::stop() {
    running = false;
}


