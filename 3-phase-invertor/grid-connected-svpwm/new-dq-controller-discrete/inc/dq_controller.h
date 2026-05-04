//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>

class DQController {
public:
    DQController();

    void init(double kp, double ki, double vdc, double W, double L, double Ts){
        Kp = kp;
        Ki = ki;
        Vdc = vdc / 2.0;
        wl = W * L;       
        sin_wt = sin(W * Ts);
        cos_wt = cos(W * Ts);

        err_Ld_1 = 0.0;
        err_Lq_1 = 0.0;
        
        iLd = 0.0;
        iLq = 0.0;
        
        iLd_1 = 0.0;
        iLq_1 = 0.0;
        vcd_1 = 0.0;
        vcq_1 = 0.0;
    }

    void operator()(double ids, double iqs, double iLd_1, double iLq_1, double vod_1, double voq_1){
        double ierrLd_1 = ids - iLd_1;
        double ierrLq_1 = iqs - iLq_1;

        double vcd = D_CC(ierrLd_1, iLd_1, vod_1, voq_1);
        double vcq = Q_CC(ierrLq_1, iLq_1, vod_1, voq_1);

        vcd -= wl * iLq;
        vcq += wl * iLd;

        double vid = (1.0 + cos_wt) * vcd / 2.0 - sin_wt * vcq / 2.0;
        double viq = (1.0 + cos_wt) * vcq / 2.0 + sin_wt * vcd / 2.0;

        Vd = vid / Vdc;
        Vq = viq / Vdc;
    }

    double Vd;
    double Vq;

private:
    double D_CC(double ierrLd_1, double iLd_1, double vod_1, double voq_1){
        double errLd = Ki * ierrLd_1 + err_Ld_1;
        err_Ld_1 = errLd;
        iLd = iLd_1 * cos_wt + (vcd_1 - vod_1) * sin_wt / wl - voq_1 * (1.0 - cos_wt) / wl;
        iLd_1 = iLd;
        double vcd = errLd - Kp * iLd;
        vcd_1 = vcd;
        return vcd;
    }
    
    double Q_CC(double ierrLq_1, double iLq_1, double vod_1, double voq_1){
        double errLq = Ki * ierrLq_1 + err_Lq_1;
        err_Lq_1 = errLq;
        iLq = iLq_1 * cos_wt + (vcq_1 - voq_1) * sin_wt / wl - vod_1 * (1.0 - cos_wt) / wl;
        iLq_1 = iLq;
        double vcq = errLq - Kp * iLq;
        vcq_1 = vcq;
        return vcq;
    }


    double Ki;
    double Kp;
    double Vdc;

    double sin_wt;
    double cos_wt;
    double wl;

    double err_Ld_1;
    double err_Lq_1;

    double iLd;
    double iLq;

    double iLd_1;
    double iLq_1;

    double vcd_1;
    double vcq_1;
};


#endif  //DQCONTROLLER_H