
#ifndef CAL_CALASTRO_H
#define CAL_CALASTRO_H

namespace Cal {

    extern const double mean_tropical_year;

    extern const double spring; // 0 degrees
    extern const double summer; // 90 degrees
    extern const double autumn; // 180 degrees
    extern const double winter; // 270 degrees

    extern const double j2000;  // 1 Jan 2000 (G) plus 12 hours

    double equation_of_time( double moment );

    double solar_longtitude( double moment );

    double estimate_prior_solar_longitude( double season, double moment );

}

#endif // CAL_CALASTRO_H