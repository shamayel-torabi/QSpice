//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#define PI 3.1415926535897932384626

class DQController {
public:
    DQController();

    void init(double kp, double ki, double L, double f){
        Kp = kp;
        Ki = ki;
        WL = 2.0 * PI * f * L;
    }

    void operator()(double Ids, double Iqs, double Id, double Iq, double Vd, double Vq, double t){
        double Ts = (t - tk_1);
        double a0 = Ki * Ts / 2.0 + Kp;
        double a1 = Ki * Ts / 2.0 - Kp;

        double edk = Ids - Id;
        double eqk = Iqs - Iq;
        
        double vdk = vdk_1 + a0 * edk + a1 * edk_1;        
        double vqk = vqk_1 + a0 * eqk + a1 * eqk_1; 
        
        Vds = vdk + Vd - WL * Iq;
        Vqs = vqk + Vq + WL * Id;

        edk_1 = edk;
        vdk_1 = vdk;

        eqk_1 = eqk;
        vqk_1 = vqk;
        tk_1 = t;
    }

    double Vds;
    double Vqs;

private:
    double Kp;
    double Ki;
    double WL;
    
    //d axis
    double edk_1; 
    double vdk_1;
    //q axis
    double eqk_1; 
    double vqk_1;

    double tk_1;
};


#endif  //DQCONTROLLER_H