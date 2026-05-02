//spwm.h
#ifndef SPWM_H
#define SPWM_H

#include <math.h>

class SPWM {
public:
    SPWM();

    void init(double xpeak){
        xPeak = xpeak;
    }

    void operator()(double valpha, double vbeta){        
        double Vref = hypotf(valpha, vbeta);
        
        double Valpha = valpha * 0.95 / Vref;
        double Vbeta  = vbeta  * 0.95 / Vref;

        ma =  Valpha;
        mb =  (sqrt(3.0) * Vbeta -  Valpha) / 2.0;
        mc = -(ma + mb);

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
    double xPeak;
};

#endif  //SPWM_H