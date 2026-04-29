//integrator.h
#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#define PI        3.14159265358979

class Integrator {
public:
    Integrator(){
        error_prev = 0.0;
        output_prev = 0.0;
    };

    double operator()(double error, double t){
        double Ts = (t - timestamp_prev);
        double output = output_prev + Ts*0.5*(error + error_prev);
        
        if(output >= 2 * PI)
         output = 0.0;

        output_prev = output;
        error_prev = error;
        timestamp_prev = t;
        return output;
    };

protected:
    double error_prev; //!< last tracking error value
    double output_prev; //!< last integral component value
    double timestamp_prev; //!< Last execution timestamp
    
};

#ifdef __cplusplus
}
#endif
#endif  INTEGRATOR_H