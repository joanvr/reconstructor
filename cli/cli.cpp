#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

#include "Reconstructor.h"

int main(int argc, char* argv[]) {

    if (argc < 5) {
        cout << "Usage: ./CS inputFile sigmaLambda blockSize transform [rgb/yuv] [nthreads]" << endl;
        return 0;
    }
    string file = string(argv[1]);
    double sl = atof(argv[2]);
    int bs = atoi(argv[3]);
    int transform = atoi(argv[4]);
    bool yuv = false;
    if (argc >= 6) yuv = string(argv[5]) == "yuv";
    int nthreads = 4;
    if (argc >=7) nthreads = atoi(argv[6]);

    Reconstructor r;
    r.setFile(file);
    r.setSigmaLambda(sl);
    r.setBlockSize(bs);
    r.setTransform(transform);
    r.setColorSpace(yuv);
    r.setNThreads(nthreads);

    r.reconstruct();
}
