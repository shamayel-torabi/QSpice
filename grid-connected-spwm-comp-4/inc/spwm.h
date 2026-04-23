//spwm.h
#ifndef SPWM_H
#define SPWM_H

#include <math.h>

class SPWM {
public:
    SPWM();

    void init(double vdc, double peak, double mcu_clk){
        Vdc = vdc;
        xpeak = peak;
        Ts = 2 * xpeak / mcu_clk;
    }

    void operator()(double valpha, double vbeta){
        double va =  valpha;
        double vb =  (sqrt(3.0) * vbeta -  valpha) / 2.0;
        double vc = -(va + vb);

        ma = va * 2 / Vdc;
        mb = vb * 2 / Vdc;
        mc = vc * 2 / Vdc;

        if(ma < -1.0)
            ma = -1.0;
        if(mb < -1.0)
            mb = -1.0;
        if(mc < -1.0)
            mc = -1.0;

        if(ma > 1.0)
            ma = 1.0;
        if(mb > 1.0)
            mb = 1.0;
        if(mc > 1.0)
            mc = 1.0;

        switchtime_a = xpeak * (ma + 1.0) / 2.0;
        switchtime_b = xpeak * (mb + 1.0) / 2.0;
        switchtime_c = xpeak * (mc + 1.0) / 2.0;
    }

    double switchtime_a;
    double switchtime_b;
    double switchtime_c;

    double ma;
    double mb;
    double mc;

private:
    double Vdc;
    double xpeak;
    double Ts;
};

#endif  //SPWM_H