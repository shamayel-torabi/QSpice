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

    void operator()(double ids, double iqs, double id, double iq, double vd, double vq, double t){
         double edk = ids - id;
         double eqk = iqs - iq;
        
         double vdk = pid_d(edk, t);         
         double vqk = pid_q(eqk, t);

        Vd = vdk + vd - WL * iq;
        Vq = vqk + vq + WL * id;
    }

    double Vd;
    double Vq;

private:
    PIController pid_d;
    PIController pid_q;
    double WL;
};


#endif  //DQCONTROLLER_H