#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

#include "Reconstructor.h"

int main(int argc, char* argv[]) {

    if (argc < 5) {
        cout << "Usage: ./CS inputFile blockSize transform [rgb/yuv] [nthreads]" << endl;
        return 0;
    }
    string file = string(argv[1]);
    int bs = atoi(argv[2]);
    int transform = atoi(argv[3]);
    bool yuv = false;
    if (argc >= 5) yuv = string(argv[4]) == "yuv";
    int nthreads = 4;
    if (argc >= 6) nthreads = atoi(argv[5]);

    Reconstructor r;
    r.setFile(file);
    r.setBlockSize(bs);
    r.setTransform(transform);
    r.setColorSpace(yuv);
    r.setNThreads(nthreads);

    r.reconstruct();
}
