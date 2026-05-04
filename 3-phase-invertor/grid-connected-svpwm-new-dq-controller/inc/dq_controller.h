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

    void operator()(double ids, double iqs, double id, double iq, double vd, double vq, double t){
        double dt = (t - t_prev);
        double ed = ids - id;
        double eq = iqs - iq;
        
        double vdk = vd_1 + Kp * ed + Ki * dt * 0.5 * (ed + ed_1);
        double vqk = vq_1 + Kp * eq + Ki * dt * 0.5 * (eq + eq_1);

        Vd = vdk - vd - WL * iq;
        Vq = vqk - vq + WL * id;

        ed_1 = ed;
        eq_1 = eq;
        vd_1 = vdk;
        vq_1 = vqk;
        t_prev = t;
    }

    double Vd;
    double Vq;

private:
    double Ki;
    double Kp;
    double WL;
    double Vmax;

    double vd_1;
    double vq_1;

    double ed_1;
    double eq_1;
    
    double t_prev;
};


#endif  //DQCONTROLLER_H