//sogi.h
#ifndef SOGI_H
#define SOGI_H

#ifdef __cplusplus
extern "C" {
#endif

#define K  1.4142135623731

class SOGI {
public:
    SOGI(){
        t_prev = 0.0;

        va_n_1 = 0.0;
        va_n_2 = 0.0;

        vb_n_1 = 0.0;
        vb_n_2 = 0.0;

        vac_n_1 = 0.0;
        vac_n_2 = 0.0;
    };

    void operator()(double vac_n, double w, double t, double *va, double *vb){
        double Ts = (t - t_prev);
        double tw = Ts * w;

        double den = (4.0 + 2.0 * K * w * Ts + tw * tw);
        double b0  = 2.0 * K * w * Ts / den;
        double b1  = K * w * w * Ts * Ts / den;
        double a1  = (8.0 - 2.0 * tw * tw) / den;
        double a2  = (2.0 * K * w * Ts  - 4.0 - tw * tw) / den;

        double va_n = b0 * (vac_n_1 - vac_n_2) + a1 * va_n_1 + a2 * va_n_2; 
        double vb_n = b1 * (vac_n + 2.0 * vac_n_1 + vac_n_2)  + a1 * vb_n_1 + a2 * vb_n_2;          

        t_prev = t;
        
        vac_n_2 = vac_n_1;
        vac_n_1 = vac_n;

        va_n_2 = va_n_1;
        va_n_1 = va_n;

        vb_n_2 = vb_n_1;
        vb_n_1 = vb_n;

        *va = va_n;
        *vb = vb_n;
    };


protected:
    double t_prev;

    double va_n_1;
    double va_n_2;

    double vb_n_1;
    double vb_n_2;

    double vac_n_1;
    double vac_n_2;
};

#ifdef __cplusplus
}
#endif
#endif  //SOGI_H