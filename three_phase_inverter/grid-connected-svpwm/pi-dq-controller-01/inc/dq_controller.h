//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "pi_controller.h"

typedef struct  {
  double Ids;
  double Iqs;

  double Id;
  double Iq;
  double Vd;
  double Vq;
} dq_current_t;

class DQController {
public:
    DQController();

    void init(double kp, double ki, double wl, double limit = NOT_SET, double ramp = NOT_SET){
        pid_d.init(kp, ki, limit, ramp);
        pid_q.init(kp, ki, limit, ramp);
        WL = wl;
    }

    void operator()(dq_current_t *inst, double t){
         double edk = inst->Ids - inst->Id;
         double eqk = inst->Iqs - inst->Iq;
        
         double vdk = pid_d(edk, t);         
         double vqk = pid_q(eqk, t);

        Vd = vdk + inst->Vd - WL * inst->Iq;
        Vq = vqk + inst->Vq + WL * inst->Id;
    }
    
    double Vd;
    double Vq;

private:
    PIController pid_d;
    PIController pid_q;
    double WL;
};


#endif  //DQCONTROLLER_H