#ifndef __FISTA_H
#define __FISTA_H

#include "datatypes.h"
#include <math.h>

class EllOneSolver {

 public:
    
    EllOneSolver(const Matrix& _A, int _max_iter, number_t _eps, number_t _tol) :
        t_k        (1.0),
        t_km1      (1.0),
        L          (1.0),
        lambda     (0),
        lambda_bar (0),
        eta        (0.6),
        beta       (1.5),
        eps        (_eps),
        tolerance  (_tol),
        A          (_A),
        c          (Vector(_A.size2())),
        xk         (Vector(_A.size2())),
        max_iter   (_max_iter),
        nz_x       (std::vector<bool>(_A.size2()))
        {}
    
    bool optimize(const Vector& b, Vector& sol) {
        c.assign(prod(trans(A), b));
        lambda = 0.99 * norm_inf(c);
        lambda_bar = 0.0001 * lambda;
        // finished initialization

        bool keep_going(true);
        Vector xkm1(xk);
        int n_iter(0);
        
        while(keep_going && n_iter++ < max_iter) {
            const Vector 
                yk(xk + ((t_km1-1.0)/t_k) * (xk - xkm1)),
                A_times_yk(prod(A, yk)),
                gradient(prod(trans(A), A_times_yk) - c);
            Vector xkp1(xk.size());
            bool stop_backtrack(false);
            while(!stop_backtrack) {
                const Vector gk(yk - (1.0/L) * gradient);
                soft(gk, lambda/L, xkp1);

                number_t 
                    temp1 = norm_2( b - prod(A, xkp1) ),
                    temp2 = norm_2( b - A_times_yk ),
                    temp3 = norm_2( xkp1 - yk );
                temp1 *= 0.5 * temp1; //  temp1 = 0.5*norm(b-A*xkp1)^2 ;
                temp2 *= 0.5 * temp2;
                temp3 *= 0.5 * L * temp3;
                temp2 += inner_prod(xkp1 - yk, gradient);
                temp2 += temp3;  //  temp2 = 0.5*norm(b-A*yk)^2 + (xkp1-yk)'*temp + (L/2)*norm(xkp1-yk)^2
                
                if (temp1 <= temp2) {
                    stop_backtrack = true;
                } else {
                    L *= beta;
                }
            }

            const std::vector<bool> nz_x_prev (nz_x);
            Vector::const_iterator it_xkp1 = xkp1.begin(), it_xkp1_end = xkp1.end();
            std::vector<bool>::iterator it_nz_x = nz_x.begin();
            std::vector<bool>::const_iterator it_nz_x_prev = nz_x_prev.begin();
            int num_changes_active(0), num_nz_x(0);
            for (; it_xkp1 != it_xkp1_end; ++it_xkp1, ++it_nz_x, ++it_nz_x_prev) {
                *it_nz_x = (fabs(*it_xkp1) > eps * 10.0);
                if (*it_nz_x != *it_nz_x_prev) {
                    ++num_changes_active;
                }
                if (*it_nz_x) {
                    ++num_nz_x;
                }
            }
            if (num_nz_x) {
                const number_t criterionActiveSet = num_changes_active / (number_t) num_nz_x;
                keep_going = (criterionActiveSet > tolerance);
            }
            
            lambda *= eta;
            if (lambda < lambda_bar) {
                lambda = lambda_bar;
            }  //  lambda = max(eta*lambda,lambda_bar)

            const number_t t_kp1 = 0.5 * (1.0 + sqrt(1.0 + 4 * t_k * t_k));
            t_km1 = t_k;
            t_k = t_kp1;
            xkm1 = xk;
            xk = xkp1;
        }
        sol = xk;
        return (n_iter < max_iter + 1);
    }

 private:
    
    void soft (const Vector& x, const number_t t, Vector& y) {
        if (t == 0) {
            y = x;
        } else {
            Vector::const_iterator xit = x.begin(), xend = x.end();
            Vector::iterator yit = y.begin();
            for (; xit != xend; ++xit, ++yit) {
                number_t tmp = fabs(*xit) - t;
                if (tmp < 0)
                    tmp = 0;
                if (*xit < 0) {
                    *yit = -tmp;
                } else if (*xit == 0) {
                    *yit = 0;
                } else {
                    *yit = tmp;
                }
            }
        }
    }

    number_t t_k, t_km1, L, lambda, lambda_bar, eta, beta, eps, tolerance;
    Matrix A;
    Vector c, xk;
    int max_iter;
    std::vector<bool> nz_x;
};

#endif

// Local Variables: 
// mode:c++
// End:             
