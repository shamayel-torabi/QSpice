//symmetrical-svpwm.h
#ifndef DPWM3_H
#define DPWM3_H

//#include <math.h>
//#define PI 3.1415926535897932384626

class DPWM1 {
public:
    DPWM1();

    void init(double vdc, double fsw, double mcuclk){
        Vdc = vdc;
        mcu_clk = mcuclk;     
        xpeak = mcuclk / (2 * fsw);
    }

    void operator()(double va, double vb, double vc, double theta){
        ma = va * 2 / Vdc;
        mb = vb * 2 / Vdc;
        mc = vc * 2 / Vdc;
        
        double vMax = max(ma, mb, mc);
        double vMin = min(ma, mb, mc);
        double vm;

        if(aabs(vMax) > aabs(vMin))
            vm = vMax - 1.0;
        else
            vm = vMin + 1.0;

        ma -= vm;
        mb -= vm;
        mc -= vm;
        
        // if(ma < -1.0)
        //     ma = -1.0;
        // if(mb < -1.0)
        //     mb = -1.0;
        // if(mc < -1.0)
        //     mc = -1.0;

        // if(ma > 1.0)
        //     ma = 1.0;
        // if(mb > 1.0)
        //     mb = 1.0;
        // if(mc > 1.0)
        //     mc = 1.0;
        
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
    
    double aabs(double in){
        if(in < 0.0)
            return -in;
        else
            return in;
    }
};

#endif  //SYMMERTIC_SVPWM_H