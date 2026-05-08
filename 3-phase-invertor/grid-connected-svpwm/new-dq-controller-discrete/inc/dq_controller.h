//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "lowpass_filter.h"

class DQController {
public:
    DQController();

    void init(double kp, double ki, double W, double L, double Ts, double Tf=0.05){
        Kp = kp;
        Ki = ki;
        wl = W * L;       
        sin_wt = sin(W * Ts);
        cos_wt = cos(W * Ts);

        vdc_lp.init(Tf, Ts);
        reset();
    }

    void operator()(double ids, double iqs, double id, double iq, double vod, double voq, double vdc){
        ieLd = ids - id;
        ieLq = iqs - iq;

        double vcd = D_CC(ieLd, id, vod, voq);
        double vcq = Q_CC(ieLq, iq, vod, voq);

        vcd -= wl * iLq;
        vcq += wl * iLd;

        Vd = (1.0 + cos_wt) * vcd / 2.0 - sin_wt * vcq / 2.0;
        Vq = (1.0 + cos_wt) * vcq / 2.0 + sin_wt * vcd / 2.0;

        Vdcf = max(vdc_lp(vdc / 2.0), 1.0);

        Vd /= Vdcf;
        Vq /= Vdcf;
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

    double D_CC(double ied, double ild_1, double vod_1, double voq_1){
        double ierrLd = Ki * ied + ierrLd_1;
        iLd = ild_1 * cos_wt + (vcd_1 - vod_1) * sin_wt / wl - voq_1 * (1.0 - cos_wt) / wl;
        double vcd = ierrLd - Kp * iLd; 

        ierrLd_1 = ierrLd;
        iLd_1 = iLd;
        vcd_1 = vcd;

        return vcd;
    }
    
    double Q_CC(double ieq, double ilq_1, double vod_1, double voq_1){
        double ierrLq = Ki * ieq + ierrLq_1;
        iLq = ilq_1 * cos_wt + (vcq_1 - voq_1) * sin_wt / wl - vod_1 * (-1.0 + cos_wt) / wl;
        double vcq = ierrLq - Kp * iLq;
        
        ierrLq_1 = ierrLq;
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