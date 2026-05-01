//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "pi_controller.h"

class DQController {
public:
    DQController();

    void init(double kp, double ki, double wl, double limit = NOT_SET, double ramp = NOT_SET){
        pid_d.init(kp, ki, limit, ramp);
        pid_q.init(kp, ki, limit, ramp);
        WL = wl;
    }

    void operator()(double Ids, double Iqs, double Id, double Iq, double Vd, double Vq, double t){
         double edk = Ids - Id;
         double eqk = Iqs - Iq;
        
         double vdk = pid_d(edk, t);         
         double vqk = pid_q(eqk, t);

        Vd = vdk + Vd - WL * Iq;
        Vq = vqk + Vq + WL * Id;
    }

    double Vd;
    double Vq;

private:
    PIController pid_d;
    PIController pid_q;
    double WL;
};


#endif  //DQCONTROLLER_H