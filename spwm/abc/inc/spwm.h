//spwm.h
#ifndef SPWM_H
#define SPWM_H

#include <math.h>

class SPWM {
public:
    SPWM();

    void init(double vdc, double xpeak){
        Vdc = vdc;
        xPeak = xpeak;
    }

    void operator()(double va, double vb, double vc){
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

        switchtime_a = xPeak * (ma + 1.0) / 2.0;
        switchtime_b = xPeak * (mb + 1.0) / 2.0;
        switchtime_c = xPeak * (mc + 1.0) / 2.0;
    }

    double switchtime_a;
    double switchtime_b;
    double switchtime_c;

    double ma;
    double mb;
    double mc;

private:
    double Vdc;
    double xPeak;
};

#endif  //SPWM_H