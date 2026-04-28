//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "pi_controller.h"

class DQController {
public:
    DQController();

    void init(double kp, double ki, double L, double f, double olimit = NOT_SET, double ramp = NOT_SET){
        pid_d.init(kp, ki, olimit, ramp);
        pid_q.init(kp, ki, olimit, ramp);
        Vmax = olimit;
        WL = 2.0 * PI * f * L;
    }

    void operator()(double Ids, double Iqs, double Id, double Iq, double Vd, double Vq, double t){
         double edk = Ids - Id;
         double eqk = Iqs - Iq;
        
         double vdk = pid_d(edk, t);         
         double vqk = pid_q(eqk, t);

        Vds = vdk + Vd - WL * Iq;
        Vqs = vqk + Vq + WL * Id;

        //Vds = _constrain(Vds, -Vmax , Vmax);
        //Vqs = _constrain(Vqs, -Vmax , Vmax);
        
        // double V_mag_nosat = hypotf(Vds, Vqs);
        // double V_mag = V_mag_nosat / Vmax;
        
        // if(V_mag > 0.995){
        //     V_mag = 0.995;
        // }

        // Vds = Vds * Vmax * V_mag / V_mag_nosat;
        // Vqs = Vqs * Vmax * V_mag / V_mag_nosat;
    }

    double Vds;
    double Vqs;

private:
    PIController pid_d;
    PIController pid_q;
    double Vmax;
    double WL;
};


#endif  //DQCONTROLLER_H