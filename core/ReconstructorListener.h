#ifndef RECONSTRUCTORLISTENER_H
#define RECONSTRUCTORLISTENER_H

#include "datatypes.h"

class ReconstructorListener {
public:
    virtual void blockCompleted(int block, const Matrix& R, const Matrix& G, const Matrix& B) = 0;
    virtual void finished() = 0;
};

#endif // RECONSTRUCTORLISTENER_H
