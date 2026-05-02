//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "pi_controller.h"

struct DQ_Type {
  double Ids;
  double Iqs;

  double Id;
  double Iq;
  double Vd;
  double Vq;
};

class DQController {
public:
    DQController();

    void init(double kp, double ki, double L, double f, double limit = NOT_SET, double ramp = NOT_SET){
        pid_d.init(kp, ki, limit, ramp);
        pid_q.init(kp, ki, limit, ramp);
        WL = 2.0 * PI * f * L;
    }

    void operator()(DQ_Type *inst, double t){
         double edk = inst->Ids - inst->Id;
         double eqk = inst->Iqs - inst->Iq;
        
         double vdk = pid_d(edk, t);         
         double vqk = pid_q(eqk, t);

        Vdr = vdk + inst->Vd - WL * inst->Iq;
        Vqr = vqk + inst->Vq + WL * inst->Id;
    }
    
    double Vdr;
    double Vqr;

private:
    PIController pid_d;
    PIController pid_q;
    double WL;
};


#endif  //DQCONTROLLER_H