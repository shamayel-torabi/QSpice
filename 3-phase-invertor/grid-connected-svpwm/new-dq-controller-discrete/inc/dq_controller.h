//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "lowpass_filter.h"

class DQController {
public:
    DQController();

    void init(double kp, double ki, double W, double L, double Ts){
        Kp = kp;
        Ki = ki;
        wl = W * L;       
        sin_wt = sin(W * Ts);
        cos_wt = cos(W * Ts);

        vdc_lp.init(0.05, Ts);

        reset();
    }

    void operator()(double ids, double iqs, double iLd, double iLq, double vod, double voq, double vdc){
        double ierrLd = ids - iLd;
        double ierrLq = iqs - iLq;

        double vcd = D_CC(ierrLd, iLd, vod, voq);
        double vcq = Q_CC(ierrLq, iLq, vod, voq);

        vcd -= wl * iLq;
        vcq += wl * iLd;

        Vd = (1.0 + cos_wt) * vcd / 2.0 - sin_wt * vcq / 2.0;
        Vq = (1.0 + cos_wt) * vcq / 2.0 + sin_wt * vcd / 2.0;

        Vdcf = max(vdc_lp(vdc), 1.0);

        Vd /= Vdcf;
        Vq /= Vdcf;
    }

    void reset() {
        ierrLd_prev = 0.0;
        ierrLq_prev = 0.0;
        
        iLd = 0.0;
        iLq = 0.0;
        
        iLd_prev = 0.0;
        iLq_prev = 0.0;
        vcd_prev = 0.0;
        vcq_prev = 0.0;
    }

    double Vd;
    double Vq;
    double Vdcf;

private:
    double D_CC(double ierrLd, double iLd, double vod, double voq){
        double errLd = Ki * ierrLd + ierrLd_prev;
        ierrLd_prev = errLd;

        iLd = iLd_prev * cos_wt + (vcd_prev - vod) * sin_wt / wl - voq * (1.0 - cos_wt) / wl;
        iLd_prev = iLd;

        double vcd = errLd - Kp * iLd;
        vcd_prev = vcd;

        return vcd;
    }
    
    double Q_CC(double ierrLq, double iLq, double vod, double voq){
        double errLq = Ki * ierrLq + ierrLq_prev;
        ierrLq_prev = errLq;

        iLq = iLq_prev * cos_wt + (vcq_prev - voq) * sin_wt / wl - vod * (-1.0 + cos_wt) / wl;
        iLq_prev = iLq;

        double vcq = errLq - Kp * iLq;
        vcq_prev = vcq;

        return vcq;
    }

    double max(double a, double b){
        if(a > b)
            return a;
        else
            return b;
    }

    double Ki;
    double Kp;

    double sin_wt;
    double cos_wt;
    double wl;

    double ierrLd_prev;
    double ierrLq_prev;

    double iLd;
    double iLq;

    double iLd_prev;
    double iLq_prev;

    double vcd_prev;
    double vcq_prev;

    LowPassFilter vdc_lp;
};


#endif  //DQCONTROLLER_H