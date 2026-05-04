//dsogi.h
#ifndef DSOGI_PLL_H
#define DSOGI_PLL_H

#include <math.h>
#include "sogi.h"
#include "integrator.h"
#include "pi_controller.h"

#define PI 3.1415926535897932384626

class DSOGI_PLL {
public:
    DSOGI_PLL();

    void init(double Kp, double Ki, double F){
        Freq = F;
        omega = 2 * PI * Freq;
        theta = 0.0;
        
        //limit omega error to 0.1 Hz
        pi_controller.init(Kp, Ki, 2 * PI * 0.1);
    }

    double operator()(double Valpha, double Vbeta, double t){
        double v_a , v_a_q;
        double v_b , v_b_q;

        sogi_a(Valpha, omega, t, &v_a , &v_a_q);
        sogi_b(Vbeta, omega, t, &v_b , &v_b_q);

        double V_a = v_a - v_b_q;
        double V_b = v_b + v_a_q;
        Va = V_a;
        Vb = V_b;

        // Park Transformation
        double sinValue = sin(theta);
        double cosValue = cos(theta);

        Vd =  V_a * cosValue + V_b * sinValue;
        Vq = -V_a * sinValue + V_b * cosValue;

        vm = sqrt(Vd * Vd + Vq * Vq);
        double Vin = Vq / max(vm, 1e-4);
        
        omega_err = pi_controller(Vin, t);
        omega = omega_err + 2 * PI * Freq;
        theta = integrator(omega, t);

        return theta;
    };

    double theta;
    double omega;
    double vm;
    double Va;
    double Vb;
    double Vd;
    double Vq;
    double omega_err;

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

#endif  //DSOGI_PLL_H