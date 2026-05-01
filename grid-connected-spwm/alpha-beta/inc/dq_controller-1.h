//dq_controller.h
#ifndef DQCONTROLLER_H
#define DQCONTROLLER_H

#include <math.h>

class DQController {
public:
    DQController();

    void init(double kp, double ki, double vdc, double wl){
        Kp = kp;
        Ki = ki;
        Vmax = vdc / sqrt(3.0);
        WL = wl;
    }

    void operator()(double Ids, double Iqs, double Id, double Iq, double Vd, double Vq, double t){
        double dt = (t - t_prev);
        double e_d = Ids - Id;
        double e_q = Iqs - Iq;
        
        v_d = v_d + dt * e_d - 0.1 * dt * bkt_d;
        v_q = v_q + dt * e_q - 0.1 * dt * bkt_d;

        Vd = Kp * e_d + Ki * v_d - WL * Iq;
        Vq = Kp * e_q + Ki * v_q + WL * Id;
        
        double V_mag_nosat = hypotf(Vd, Vq);
        double V_mag = V_mag_nosat / Vmax;
        
        if(V_mag > 0.995){
            V_mag = 0.995;
        }

        bkt_d = Vd - Vd * Vmax * V_mag / V_mag_nosat;
        bkt_q = Vq - Vq * Vmax * V_mag / V_mag_nosat;
        t_prev = t;
    }

    double Vd;
    double Vq;

private:
    double Ki;
    double Kp;
    double WL;
    double Vmax;

    double v_d;
    double v_q;
    
    double bkt_d;
    double bkt_q;
    double t_prev;
};


#endif  //DQCONTROLLER_H