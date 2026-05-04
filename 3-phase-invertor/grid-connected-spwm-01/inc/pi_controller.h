#ifndef PIController_H
#define PIController_H

/**
 *  PI controller class
 */

#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define _isset(a) ( (a) != (NOT_SET) )
#define NOT_SET -12345.0

#define PI 3.1415926535897932384626

class PIController
{
public:
    PIController();

    void init(double kp, double ki, double oramp = NOT_SET, double olimit = NOT_SET){
        Kp = kp;
        Ki = ki;
        output_ramp = oramp;
        limit = olimit;
        reset();
    }

    double operator() (double error, double t){
        double dt = (t - t_prev);
        double proportional = Kp * error;
        double integral = integral_prev + Ki * dt * 0.5 *(error + error_prev);

        // antiwindup - limit the output
        if(_isset(limit)) integral = _constrain(integral, -limit, limit);

        double output = proportional + integral;
        
        // antiwindup - limit the output variable
        if(_isset(limit)) output = _constrain(output, -limit, limit);

        // if output ramp defined
        if(_isset(output_ramp) && output_ramp > 0.0){
            // limit the acceleration by ramping the output
            double output_rate = (output - output_prev)/dt;

            if (output_rate > output_ramp)
                output = output_prev + output_ramp*dt;
            else if (output_rate < -output_ramp)
                output = output_prev - output_ramp*dt;
        }
        
        // saving for the next pass
        integral_prev = integral;
        output_prev = output;
        error_prev = error;
        t_prev = t;

        return output;
    }

    void reset(){
        integral_prev = 0.0;
        output_prev = 0.0;
        error_prev = 0.0;
    }
protected:
    double Kp;          //!< Proportional gain 
    double Ki;          //!< Integral gain 
    double output_ramp; //!< Maximum speed of change of the output value
    double limit;       //!< Maximum output value

    double error_prev; //!< last tracking error value
    double output_prev;  //!< last pid output value
    double integral_prev; //!< last integral component value
    double t_prev;
};

#endif // PIController_H