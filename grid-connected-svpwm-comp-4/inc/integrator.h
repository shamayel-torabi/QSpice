//integrator.h
#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#define PI 3.1415926535897932384626

class Integrator {
public:
    Integrator(){
        error_prev = 0.0;
        output_prev = 0.0;
    };

    double operator()(double error, double t){
        double Ts = (t - t_prev);
        double output = output_prev + Ts*0.5*(error + error_prev);
        
        if(output >= 2 * PI)
         output = 0.0;

        output_prev = output;
        error_prev = error;
        t_prev = t;
        return output;
    };

protected:
    double error_prev; //!< last tracking error value
    double output_prev; //!< last integral component value
    double t_prev; //!< Last execution timestamp
    
};

#endif  INTEGRATOR_H