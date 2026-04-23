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

    void init(double K_p, double K_i, double F){
        Freq = F;
        omega = 2 * PI * Freq;
        theta = 0.0;
        pi_controller.init(K_p, K_i);
    }

    double operator()(double Valpha, double Vbeta, double t){
        double v_a , v_a_q;
        double v_b , v_b_q;

        sogi_a(Valpha, omega, t, &v_a , &v_a_q);
        sogi_b(Vbeta, omega, t, &v_b , &v_b_q);

        double V_a = v_a - v_b_q;
        double V_b = v_b + v_a_q;

        // Park Transformation
        double sinValue = sin(theta);
        double cosValue = cos(theta);

        double Vd =  V_a * cosValue + V_b * sinValue;
        double Vq = -V_a * sinValue + V_b * cosValue;

        vm = sqrt(Vd * Vd + Vq * Vq);
        double Vin = Vd / max(vm, 1e-4);
        
        omega = pi_controller(Vin, t);
        omega += 2 * PI * Freq;
        theta = integrator(omega, t);

        return theta;
    };

    double theta;
    double omega;
    double vm;

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