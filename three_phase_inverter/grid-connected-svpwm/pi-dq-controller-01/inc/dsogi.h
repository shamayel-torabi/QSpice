//dsogi.h
#ifndef DSOGI_PLL_H
#define DSOGI_PLL_H

#include <math.h>
#include "sogi.h"
#include "integrator.h"
#include "pi_controller.h"

#define PI 3.1415926535897932384626

#ifdef __cplusplus
extern "C" {
#endif

class DSOGI_PLL {
public:
    DSOGI_PLL();

    void init(double Kp, double Ki, double F){
        Freq = F;
        omega = 2 * PI * F;
        theta = 0.0;
        pi_controller.init(Kp, Ki);
    }

    double operator()(double Valpha, double Vbeta, double t){
        double v_a , v_a_q;
        double v_b , v_b_q;

        sogi_a(Valpha, omega, t, &v_a , &v_a_q);
        sogi_b(Vbeta,  omega, t, &v_b , &v_b_q);

        Va = (v_b_q - v_a) / 2.0;
        Vb = (v_b + v_a_q) / 2.0;

        // Park Transformation
        double sinValue = sin(theta);
        double cosValue = cos(theta);

        Vd =  Va * cosValue + Vb * sinValue;
        Vq = -Va * sinValue + Vb * cosValue;

        Vm = sqrt(Vd * Vd + Vq * Vq);
        double v = Vq / max(Vm, 1e-4);
        
        omega_err = pi_controller(v, t);
        omega = omega_err + 2 * PI * Freq;
        theta = integrator(omega, t);

        return theta;
    };

    double theta;
    double omega;
    double omega_err;
    double Vm;
    double Vd;
    double Vq;
    double Va;
    double Vb;

protected:
    double Freq;

    SOGI sogi_a;
    SOGI sogi_b;
    Integrator integrator;
    PIController pi_controller;

private: 
    double max(double a, double b){
    if(a > b)
        return a;
    else
        return b;
    };
};

#ifdef __cplusplus
}
#endif
#endif  //DSOGI_PLL_H