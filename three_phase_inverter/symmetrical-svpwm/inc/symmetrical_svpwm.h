//symmetrical-svpwm.h
#ifndef SYMMERTIC_SVPWM_H
#define SYMMERTIC_SVPWM_H

#ifdef __cplusplus
extern "C" {
#endif

//#include <math.h>
//#define PI 3.1415926535897932384626

class SymmetricalSVPWM {
public:
    SymmetricalSVPWM();

    void init(double vdc, double fsw, double mcuclk){
        Vdc = vdc;
        mcu_clk = mcuclk;     
        xpeak = mcuclk / (2 * fsw);
    }

    void operator()(double va, double vb, double vc, double theta){
        double Va = va * 2 / Vdc;
        double Vb = vb * 2 / Vdc;
        double Vc = vc * 2 / Vdc;

        double vMax = max(Va, Vb, Vc);
        double vMin = min(Va, Vb, Vc);
        double vMean = (vMax + vMin) / 2.0;

        ma = Va - vMean;
        mb = Vb - vMean;
        mc = Vc - vMean;
        
        if(ma < 0.0)
            ma = -ma;
        if(mb < 0.0)
            mb = -mb;
        if(mc < 0.0)
            mc = -mc;

        if(ma > 1.0)
            ma = 1.0;
        if(mb > 1.0)
            mb = 1.0;
        if(mc > 1.0)
            mc = 1.0;

        switchtime_a = xpeak * (1.0 - ma);
        switchtime_b = xpeak * (1.0 - mb);
        switchtime_c = xpeak * (1.0 - mc);
    }

    double switchtime_a;
    double switchtime_b;
    double switchtime_c;

    double ma;
    double mb;
    double mc;
private:
    double Vdc;
    //double t_half;
    double mcu_clk;
    double xpeak;

    double min(double Va, double Vb, double Vc) {
        double Vret = Va;

        if(Vb < Vret)
            Vret = Vb;

        if(Vc < Vret)
            Vret = Vc;

        return Vret;
    }

    double max(double Va, double Vb, double Vc) {
        double Vret = Va;

        if(Vb > Vret)
            Vret = Vb;

        if(Vc > Vret)
            Vret = Vc;

        return Vret;
    }   
};


#ifdef __cplusplus
}
#endif
#endif  //SYMMERTIC_SVPWM_H