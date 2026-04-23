#ifndef PID_H
#define PID_H

/**
 *  PI controller class
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

    double operator() (double e, double t){
        double Ts = (t - tk_1);
        double a0 = Ki * Ts / 2.0 + Kp;
        double a1 = Ki * Ts / 2.0 - Kp;

        double y = yk_1 + a0 * e + a1 * ek_1;        
        yk_1 = y;
        ek_1 = e;
        tk_1 = t;
        return y;
    }

    double Kp; //!< Proportional gain 
    double Ki; //!< Integral gain 

protected:
    double ek_1; //!< last tracking error value
    double yk_1; //!< last integral component value
    double tk_1; //!< Last execution timestamp
};

#endif // PID_H