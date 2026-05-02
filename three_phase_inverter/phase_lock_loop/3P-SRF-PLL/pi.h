#ifndef PID_H
#define PID_H

/**
 *  PID controller class
 */
class PIController
{
public:
    /**
     *  
     * @param P - Proportional gain 
     * @param I - Integral gain
     */
    PIController();

    void init(double P, double I){
        Kp = P;
        Ki = I;
    }

    double operator() (double error, double t){
        double Ts = (t - timestamp_prev);

        double proportional = Kp * error;
        // Tustin transform of the integral part
        // u_ik = u_ik_1  + I*Ts/2*(ek + ek_1)
        double integral = integral_prev + Ki*Ts*0.5f*(error + error_prev);

        // sum all the components
        double output = proportional + integral;
        
        integral_prev = integral;
        output_prev = output;
        error_prev = error;
        timestamp_prev = t;
        return output;
    }

    double Kp; //!< Proportional gain 
    double Ki; //!< Integral gain 

protected:
    double error_prev; //!< last tracking error value
    double output_prev;  //!< last pid output value
    double integral_prev; //!< last integral component value
    double timestamp_prev; //!< Last execution timestamp
};

#endif // PID_H