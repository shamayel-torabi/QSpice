//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "lowpass_filter.h"

class DQController {
public:
    DQController();

    void init(double kp, double ki, double W, double L, double ts, double Tf=0.001){
        Kp = kp;
        Ki = ki;
        Ts = ts;
        wl = W * L;       
        sin_wt = sin(W * ts);
        cos_wt = cos(W * ts);

        vdc_lp.init(Tf, ts);
        reset();
    }

    void operator()(double ids, double iqs, double id, double iq, double vod, double voq, double vdc){
        ierrLd = ids - id;
        ierrLq = iqs - iq;

        double vcd = D_CC(ierrLd, id, vod, voq);
        double vcq = Q_CC(ierrLq, iq, vod, voq);

        vcd -= wl * iLq;
        vcq += wl * iLd;

        Vd = (1.0 + cos_wt) * vcd / 2.0 - sin_wt * vcq / 2.0;
        Vq = (1.0 + cos_wt) * vcq / 2.0 + sin_wt * vcd / 2.0;

        Vdcf = max(vdc_lp(vdc / 2.0), 1.0);

        // Vd /= Vdcf;
        // Vq /= Vdcf;

        double U_ref = hypotf(Vd, Vq);
        double theta = atan2(Vq, Vd);

        if (U_ref > Vdcf) {
            U_ref = Vdcf;            
        }
        
        Vd = U_ref * cos(theta);
        Vq = U_ref * sin(theta);
    }

    void reset() {
        iLd_1 = 0.0;
        iLq_1 = 0.0;
        
        vcd_1 = 0.0;
        vcq_1 = 0.0;

        ierr_d_1 = 0.0;
        ierr_q_1 = 0.0;

        err_output_d_1 = 0.0;
        err_output_q_1 = 0.0;
    }

    double Vd;
    double Vq;
    double Vdcf;
    
    double iLd;
    double iLq;

    double ierrLd;
    double ierrLq;
private:
    double Ki;
    double Kp;
    double Ts;

    double sin_wt;
    double cos_wt;
    double wl;

    double iLd_1;
    double iLq_1;

    double vcd_1;
    double vcq_1;

    double ierr_d_1;
    double ierr_q_1;

    double err_output_d_1;
    double err_output_q_1;
    
    LowPassFilter vdc_lp;

    double D_CC(double ierr_d, double ild_1, double vod_1, double voq_1){
        //double err_output_d = err_output_d_1 + Ki * ierr_d;
        double err_output_d = err_output_d_1 + Ki * Ts * 0.5 * (ierr_d + ierr_d_1);

        iLd = ild_1 * cos_wt + (vcd_1 - vod_1) * sin_wt / wl - voq_1 * (1.0 - cos_wt) / wl;
        double vcd = err_output_d - Kp * iLd; 

        err_output_d_1 = err_output_d;
        ierr_d_1 = ierr_d;
        iLd_1 = iLd;
        vcd_1 = vcd;

        return vcd;
    }
    
    double Q_CC(double ierr_q, double ilq_1, double vod_1, double voq_1){
        //double err_output_q = err_output_q_1 + Ki * ierr_q;
        double err_output_q = err_output_q_1 + Ki * Ts * 0.5 * (ierr_q + ierr_d_1);

        iLq = ilq_1 * cos_wt + (vcq_1 - voq_1) * sin_wt / wl - vod_1 * (-1.0 + cos_wt) / wl;
        double vcq = err_output_q - Kp * iLq;
        
        err_output_q_1 = err_output_q;
        ierr_q_1 = ierr_q;
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