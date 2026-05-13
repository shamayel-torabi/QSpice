//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "lowpass_filter.h"

typedef struct {
    double Vdc;
    double P;
    double Q;
    double ILd;
    double ILq;
    double Vod;
    double Voq;
    double Iod;
    double Ioq;
} dq_type;

class DQController {
public:
    DQController();

    void init(double kp, double ki, double W, double L, double ts, double Tf=0.002){
        Kp = kp;
        Ki = ki;
        Ts = ts;
        wl = W * L;       
        sin_wt = sin(W * ts);
        cos_wt = cos(W * ts);

        vdc_lp.init(Tf, ts);

        p_pi_controller.init(0.5, 242.3, ts, 15.0);
        q_pi_controller.init(0.5, 242.3, ts, 15.0);

        reset();
    }

    void operator()(dq_type* in){
        IerrLd = p_pi_controller(in->P - in->Vod);
        IerrLq = q_pi_controller(in->Q - in->Voq);

        double vcd = D_CC(in->ILd, in->Vod, in->Voq);
        double vcq = Q_CC(in->ILq, in->Vod, in->Voq);

        vcd -= wl * ILq;
        vcq += wl * ILd;

        Vid = (1.0 + cos_wt) * vcd / 2.0 - sin_wt * vcq / 2.0;
        Viq = (1.0 + cos_wt) * vcq / 2.0 + sin_wt * vcd / 2.0;

        Vdcf = max(vdc_lp(in->Vdc / 2.0), 1.0);

        double U_ref = hypotf(Vid, Viq);
        double theta = atan2(Viq, Vid);

        if (U_ref > Vdcf) {
            U_ref = Vdcf;
        }
        
        Vid = U_ref * cos(theta);
        Viq = U_ref * sin(theta);
    }

    void reset() {
        //iLd_1 = 0.0;
        //iLq_1 = 0.0;
        
        vcd_1 = 0.0;
        vcq_1 = 0.0;

        ierr_d_1 = 0.0;
        ierr_q_1 = 0.0;

        err_output_d_1 = 0.0;
        err_output_q_1 = 0.0;
    }

    double Vid;
    double Viq;
    double Vdcf;
    
    double ILd;
    double ILq;

    double IerrLd;
    double IerrLq;
private:
    double Ki;
    double Kp;
    double Ts;

    double sin_wt;
    double cos_wt;
    double wl;

    //double iLd_1;
    //double iLq_1;

    double vcd_1;
    double vcq_1;

    double ierr_d_1;
    double ierr_q_1;

    double err_output_d_1;
    double err_output_q_1;
    
    LowPassFilter vdc_lp;
    PIController p_pi_controller;
    PIController q_pi_controller;

    double D_CC(double ild_1, double vod_1, double voq_1){
        //double err_output_d = err_output_d_1 + Ki * IerrLd;
        double err_output_d = err_output_d_1 + Ki * Ts * 0.5 * (IerrLd + ierr_d_1);        
        
        ILd = ild_1 * cos_wt + (vcd_1 - vod_1) * sin_wt / wl - voq_1 * (1.0 - cos_wt) / wl;
        double vcd = err_output_d - Kp * ILd; 

        err_output_d_1 = err_output_d;
        ierr_d_1 = IerrLd;
        //iLd_1 = ILd;
        vcd_1 = vcd;

        return vcd;
    }
    
    double Q_CC(double ilq_1, double vod_1, double voq_1){
        //double err_output_q = err_output_q_1 + Ki * IerrLq;
        double err_output_q = err_output_q_1 + Ki * Ts * 0.5 * (IerrLq + ierr_d_1);
        
        ILq = ilq_1 * cos_wt + (vcq_1 - voq_1) * sin_wt / wl - vod_1 * (-1.0 + cos_wt) / wl;
        double vcq = err_output_q - Kp * ILq;
        
        err_output_q_1 = err_output_q;
        ierr_q_1 = IerrLq;
        //iLq_1 = ILq;
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