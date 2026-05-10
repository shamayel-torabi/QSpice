//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "lowpass_filter.h"

class DQController {
public:
    DQController();

    void init(double kp, double ki, double W, double L, double Ts, double Tf=0.002){
        Kp = kp;
        Ki = ki;
        wl = W * L;       
        sin_wt = sin(W * Ts);
        cos_wt = cos(W * Ts);

        vdc_lp.init(Tf, Ts);
        d_pi.init(kp, ki, Ts);
        q_pi.init(kp, ki, Ts);
        reset();
    }

    void operator()(double ids, double iqs, double id, double iq, double vod, double voq, double vdc){
        double vcd = D_CC(ids, id, vod, voq);
        double vcq = Q_CC(iqs, iq, vod, voq);

        vcd -= wl * iLq;
        vcq += wl * iLd;

        Vd = (1.0 + cos_wt) * vcd / 2.0 - sin_wt * vcq / 2.0;
        Vq = (1.0 + cos_wt) * vcq / 2.0 + sin_wt * vcd / 2.0;

        Vdcf = max(vdc_lp(vdc / 2.0), 1.0);

        double U_ref = hypotf(Vd, Vq);
        double theta = atan2(Vq, Vd);

        if (U_ref > Vdcf) {
            U_ref = Vdcf;
        }
        
        Vd = U_ref * cos(theta);
        Vq = U_ref * sin(theta);
    }

    void reset() {
        ierrLd_1 = 0.0;
        ierrLq_1 = 0.0;
        
        iLd_1 = 0.0;
        iLq_1 = 0.0;
        
        vcd_1 = 0.0;
        vcq_1 = 0.0;
    }

    double Vd;
    double Vq;
    double Vdcf;
    
    double iLd;
    double iLq;

    double ieLd;
    double ieLq;
private:
    double Ki;
    double Kp;

    double sin_wt;
    double cos_wt;
    double wl;

    double ierrLd_1;
    double ierrLq_1;

    double iLd_1;
    double iLq_1;

    double vcd_1;
    double vcq_1;

    LowPassFilter vdc_lp;
    PIController  d_pi;
    PIController  q_pi;

    double D_CC(double ids, double ild_1, double vod_1, double voq_1){
        //estimate iLd
        iLd = iLd_1 * cos_wt + (vcd_1 - vod_1) * sin_wt / wl - voq_1 * (1.0 - cos_wt) / wl;

        //calculate error
        double ieLd = ids - ild_1;

        //calculate pi-controller output
        double vcd = d_pi(ieLd);

        //store value
        ierrLd_1 = ieLd;
        iLd_1 = iLd;
        vcd_1 = vcd;

        return vcd;
    }
    
    double Q_CC(double iqs, double ilq_1, double vod_1, double voq_1){
        //estimate iLq
        iLq = iLq_1 * cos_wt + (vcq_1 - voq_1) * sin_wt / wl - vod_1 * (-1.0 + cos_wt) / wl;

        //calculate error
        double ieLq = iqs - ilq_1;

        //calculate pi-controller output
        double vcq = q_pi(ieLq);
        
        //store value
        ierrLq_1 = ieLq;
        iLq_1 = iLq;
        vcq_1 = vcq;

        return vcq;
    }

    double max(double a, double b){
        if(a > b)
            return a;
        else
            return b;
    }
};


#endif  //DQCONTROLLER_H