

#include "../include/stdmath.h"


 double m_abs(double x) {
    return x < 0 ? -x : x;
}

double m_normalize_angle(double x) {
    while (x > M_PI)  x -= M_TWO_PI;
    while (x < -M_PI) x += M_TWO_PI;
    return x;
}


 double m_sin(double x) {
    x = m_normalize_angle(x);
    double x2 = x * x;


    double term = x;
    double result = term;

    term *= -x2 / (2.0 * 3.0);
    result += term;
    term *= -x2 / (4.0 * 5.0);
    result += term;
    term *= -x2 / (6.0 * 7.0);
    result += term;
    term *= -x2 / (8.0 * 9.0);
    result += term;
    term *= -x2 / (10.0 * 11.0);
    result += term;

    return result;
}


 double m_cos(double x) {
    x = m_normalize_angle(x);
    double x2 = x * x;

    double term = 1.0;
    double result = term;

    term *= -x2 / (1.0 * 2.0);
    result += term;
    term *= -x2 / (3.0 * 4.0);
    result += term;
    term *= -x2 / (5.0 * 6.0);
    result += term;

    return result;
}

double m_sqrt(double n) {
    if (n < 0) return -1; 
    if (n == 0) return 0;

    double x = n;
    double last = 0;


    for (int i = 0; i < 20 && x != last; i++) {
        last = x;
        x = 0.5 * (x + n / x);
    }

    return x;
}


double m_tan(double x) {
    double c = m_cos(x);
    if (m_abs(c) < 1e-12) {
     
        return (x > 0) ? 1e12 : -1e12; 
    }
    return m_sin(x) / c;
}
