//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "pi_controller.h"

class DQController {
public:
    DQController();

    void init(double kp, double ki, double L, double f, double oramp = NOT_SET, double olimit = NOT_SET ){
        pid_d.init(kp, ki, oramp, olimit);
        pid_q.init(kp, ki, oramp, olimit);
        WL = 2.0 * PI * f * L;
    }

    void operator()(double Ids, double Iqs, double Id, double Iq, double Vd, double Vq, double t){
         double edk = Ids - Id;
         double eqk = Iqs - Iq;
        
         double vdk = pid_d(edk, t);         
         double vqk = pid_q(eqk, t);

        Vds = vdk + Vd - WL * Iq;
        Vqs = vqk + Vq + WL * Id;
    }

    double Vds;
    double Vqs;

private:
    PIController pid_d;
    PIController pid_q;
    double WL;
};


#endif  //DQCONTROLLER_H