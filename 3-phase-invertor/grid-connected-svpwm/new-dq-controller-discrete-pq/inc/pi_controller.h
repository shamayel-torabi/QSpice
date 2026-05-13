#ifndef PIController_H
#define PIController_H

/**
 *  PI controller class
 */

#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define _isset(a) ( (a) != (NOT_SET) )
#define NOT_SET -12345.0


class PIController
{
public:
    PIController();

    void init(double kp, double ki,double ts, double limit = NOT_SET, double ramp = NOT_SET){
        Kp = kp;
        Ki = ki;
        Ts = ts;
        output_ramp  = ramp;
        output_limit = limit;
        reset();
    }

    double operator() (double error){
        double proportional = Kp * error;
        double integral = integral_prev + Ki * Ts * 0.5 *(error + error_prev);

        // antiwindup - limit the output
        if(_isset(output_limit)) integral = _constrain(integral, -output_limit, output_limit);

        double output = proportional + integral;
        
        // antiwindup - limit the output variable
        if(_isset(output_limit)) output = _constrain(output, -output_limit, output_limit);

        // if output ramp defined
        if(_isset(output_ramp) && output_ramp > 0.0){
            // limit the acceleration by ramping the output
            double output_rate = (output - output_prev) / Ts;

            if (output_rate > output_ramp)
                output = output_prev + output_ramp * Ts;
            else if (output_rate < -output_ramp)
                output = output_prev - output_ramp * Ts;
        }
        
        // saving for the next pass
        integral_prev = integral;
        output_prev = output;
        error_prev = error;

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
    double Ts;
    double output_ramp; //!< Maximum speed of change of the output value
    double output_limit;       //!< Maximum output value

    double error_prev; //!< last tracking error value
    double output_prev;  //!< last pid output value
    double integral_prev; //!< last integral component value
};

#endif // PIController_H