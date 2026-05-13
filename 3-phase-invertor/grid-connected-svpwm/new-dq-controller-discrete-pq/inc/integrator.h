//integrator.h
#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#ifndef PI
#define PI 3.1415926535897932384626
#endif

class Integrator {
public:
    Integrator(){
        error_prev = 0.0;
        output_prev = 0.0;
    };

    void init(double ts){
        Ts = ts;
        reset();
    }

    double operator()(double error){
        double output = output_prev + Ts * 0.5 * (error + error_prev);
        
        if(output >= 2 * PI)
         output = 0.0;

        output_prev = output;
        error_prev = error;
        
        return output;
    };

    void reset(){
        error_prev = 0.0;
        output_prev = 0.0;
    }

protected:
    double Ts;
    double error_prev; 
    double output_prev;
    
};

#endif  //INTEGRATOR_H