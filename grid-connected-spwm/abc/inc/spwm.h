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

    void operator()(double ma, double mb, double mc){
        switchtime_a = xPeak * (1.0 + ma) / 2.0;
        switchtime_b = xPeak * (1.0 + mb) / 2.0;
        switchtime_c = xPeak * (1.0 + mc) / 2.0;
    }

    double switchtime_a;
    double switchtime_b;
    double switchtime_c;
private:
    double xPeak;
};

#endif  //SPWM_H