#include <iostream>
#include "Reconstructor.h"
#include "gurobi_c++.h"
#include "Transforms.h"
#include "Dantzig.h"
using namespace std;

#define LIMIT 4

Reconstructor::Reconstructor() {
    m_transform = DCT;
    blockSize = 16;
	blockMask = 15;
	blockBits = 4;
    sigmaLambda = 0.01;
    yuv = false;
    m_nthreads = 4;
	listener = NULL;
}

Reconstructor::~Reconstructor() {

}

void* calcBlocks(void* v) {
    Reconstructor* r = (Reconstructor*)v;

    int block;
    int blockSize = r->blockSize;
    int blockMask = r->blockMask;
    int blockBits = r->blockBits;
    double sigmaLambda = r->sigmaLambda;
    Matrix YR(blockSize, blockSize);
    Matrix YG(blockSize, blockSize);
    Matrix YB(blockSize, blockSize);
    std::vector<int> S;

    int n = blockSize*blockSize;
    double* RecR = new double[n];
    double* RecG = new double[n];
    double* RecB = new double[n];
    double* bR = new double[n]; // k
    double* bG = new double[n]; // k
    double* bB = new double[n]; // k
    
    Matrix A(n, n);
    cerr << "Initializing model" << endl;
    DantzigSelector dantzig;
	dantzig.init(n);
    cerr << "Init completed" << endl;
	
	while (r->running && (block = r->Y.getNextBlock(YR, YG, YB, S)) >= 0) {
        cerr << "*** Starting BLOCK " << block << endl;
		int k = S.size();

        // Imatge samplejada
        for (int i = 0; i < k; i++) {
			int j = S[i];
            bR[i] = YR[j >> blockBits][j & blockMask];
            bG[i] = YG[j >> blockBits][j & blockMask];
            bB[i] = YB[j >> blockBits][j & blockMask];
        }

        // Matriu per a la resolucio del CS
		int ii;
        for (ii = 0; ii < k; ii++) {
			float* tpi = r->Psinv[S[ii]];
			for (int j = 0; j < n; j++) {
				A[ii][j] = tpi[j];
			}
		}
		// Finish with 0
		for (; ii < n; ii++) {
			for (int j = 0; j < n; j++) {
				A[ii][j] = 0;
			}
		}

        // Reconstrucio del CS amb Dantzig Selector
    	cerr << "Setting coefficients" << endl;
		dantzig.setCoeffs(A, k);
    	cerr << "Setting coefficients completed" << endl;

        bool sol = false;
        int iter = 0;
        while (!sol && iter < LIMIT) {
            if (iter != 0)
                cerr << "*** Retrying BLOCK " << block
                            << " component R (" << iter << ")" << endl;
            sol = dantzig.solve(A, bR, sigmaLambda, RecR);
            iter++;
        }

        sol = false;
        iter = 0;
        while (!sol && iter < LIMIT) {
            if (iter != 0)
                cerr << "*** Retrying BLOCK " << block
                            << " component G (" << iter << ")" << endl;
            sol = dantzig.solve(A, bG, sigmaLambda, RecG);
            iter++;
        }

        sol = false;
        iter = 0;
        while (!sol && iter < LIMIT) {
            if (iter != 0)
                cerr << "*** Retrying BLOCK " << block
                            << " component B (" << iter << ")" << endl;
            sol = dantzig.solve(A, bB, sigmaLambda, RecB);
            iter++;
        }

        // Put Reconstruction back to Image Block
        for (int i = 0; i < blockSize; i++) {
            for (int j = 0; j < blockSize; j++) {
                YR[i][j] = RecR[(i << blockBits) + j];
                YG[i][j] = RecG[(i << blockBits) + j];
                YB[i][j] = RecB[(i << blockBits) + j];
            }
        }
        r->T.setBlock(block, YR, YG, YB);

        for (int i = 0; i < n; i++) {
            int row = i >> blockBits;
            int col = i &  blockMask;
            YR[row][col] = 0.0;
            YG[row][col] = 0.0;
            YB[row][col] = 0.0;
            for (int j = 0; j < n; j++) {
                YR[row][col] += r->Psinv[i][j]*RecR[j];
                YG[row][col] += r->Psinv[i][j]*RecG[j];
                YB[row][col] += r->Psinv[i][j]*RecB[j];
            }
            YR[row][col] = int(YR[row][col] + 0.5);
            YG[row][col] = int(YG[row][col] + 0.5);
            YB[row][col] = int(YB[row][col] + 0.5);
        }
        r->R.setBlock(block, YR, YG, YB);


        cerr << "*** BLOCK " << block << " DONE!" << endl;
        if (r->listener != NULL)
			r->listener->blockCompleted(block, YR, YG, YB);

    }
    
	// Clean up
    delete[] bR;
    delete[] bG;
    delete[] bB;
    delete[] RecR;
    delete[] RecG;
    delete[] RecB;
    
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

		GRBEnv* grbenv = new GRBEnv();
		DantzigSelector::env = grbenv;
		
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

		delete grbenv;

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


