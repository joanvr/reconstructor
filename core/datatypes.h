#ifndef __DATATYPES_H_
#define __DATATYPES_H_

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/foreach.hpp>

//#define foreach BOOST_FOREACH

typedef float number_t;
typedef boost::numeric::ublas::vector<number_t> Vector;
//typedef boost::numeric::ublas::zero_vector<number_t> Zero_vector;
typedef boost::numeric::ublas::matrix<number_t> Matrix;


#endif


// Local Variables: 
// mode:c++
// End:             

