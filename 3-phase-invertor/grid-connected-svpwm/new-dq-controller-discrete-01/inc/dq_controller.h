//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>
#include "lowpass_filter.h"

typedef struct {
    double Vdc;
    double P;
    double Q;
    double Vod;
    double Voq;
    double ILd;
    double ILq;
    double Iod;
    double Ioq;
} dq_type;

class DQController {
public:
    DQController();

    void init(double kp, double ki, double W, double L, double ts, double Tf=0.02){
        Kp = kp;
        Ki = ki;
        Ts = ts;
        wl = W * L;       
        sin_wt = sin(W * ts);
        cos_wt = cos(W * ts);

        vdc_lp.init(Tf, ts);
        ids_lp.init(10 * Tf, ts);
        iqs_lp.init(10 * Tf, ts);

        reset();
    }

    void operator()(dq_type* in){
        double vm = in->Vod * in->Vod + in->Voq * in->Voq;
        double Iods = (in->P * in->Vod - in->Q * in->Voq) / vm;
        double Ioqs = (in->P * in->Voq + in->Q * in->Vod) / vm;

        double IerrLds = Iods + ILd - in->Iod - in->ILd;
        double IerrLqs = Ioqs + ILq - in->Ioq - in->ILq;

        double vcd = D_CC(IerrLd, in->ILd, in->Vod, in->Voq);
        double vcq = Q_CC(IerrLq, in->ILq, in->Vod, in->Voq);

        vcd -= wl * ILq;
        vcq += wl * ILd;

        Vid = (1.0 + cos_wt) * vcd / 2.0 - sin_wt * vcq / 2.0;
        Viq = (1.0 + cos_wt) * vcq / 2.0 + sin_wt * vcd / 2.0;

        Vdcf = max(vdc_lp(in->Vdc / 2.0), 1.0);

        Vid /= Vdcf;
        Viq /= Vdcf;

        // double U_ref = hypotf(Vd, Vq);
        // double theta = atan2(Vq, Vd);

        // if (U_ref > Vdcf) {
        //     U_ref = Vdcf;
        // }
        
        // Vd = U_ref * cos(theta);
        // Vq = U_ref * sin(theta);
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

    double iLd_1;
    double iLq_1;

    double vcd_1;
    double vcq_1;

    double ierr_d_1;
    double ierr_q_1;

    double err_output_d_1;
    double err_output_q_1;
    
    LowPassFilter vdc_lp;
    LowPassFilter ids_lp;
    LowPassFilter iqs_lp;

    double D_CC(double ierr_d, double ild_1, double vod_1, double voq_1){
        //double err_output_d = err_output_d_1 + Ki * ierr_d;
        double err_output_d = err_output_d_1 + Ki * Ts * 0.5 * (ierr_d + ierr_d_1);

        ILd = ild_1 * cos_wt + (vcd_1 - vod_1) * sin_wt / wl - voq_1 * (1.0 - cos_wt) / wl;
        double vcd = err_output_d - Kp * ILd; 

        err_output_d_1 = err_output_d;
        ierr_d_1 = ierr_d;
        iLd_1 = ILd;
        vcd_1 = vcd;

        return vcd;
    }
    
    double Q_CC(double ierr_q, double ilq_1, double vod_1, double voq_1){
        //double err_output_q = err_output_q_1 + Ki * ierr_q;
        double err_output_q = err_output_q_1 + Ki * Ts * 0.5 * (ierr_q + ierr_q_1);

        ILq = ilq_1 * cos_wt + (vcq_1 - voq_1) * sin_wt / wl - vod_1 * (-1.0 + cos_wt) / wl;
        double vcq = err_output_q - Kp * ILq;
        
        err_output_q_1 = err_output_q;
        ierr_q_1 = ierr_q;
        iLq_1 = ILq;
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