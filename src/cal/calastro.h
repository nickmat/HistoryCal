
#ifndef CAL_CALASTRO_H
#define CAL_CALASTRO_H

namespace Cal {

    extern const double mean_tropical_year;

    extern const double spring; // 0 degrees
    extern const double summer; // 90 degrees
    extern const double autumn; // 180 degrees
    extern const double winter; // 270 degrees

    extern const double j2000;  // 1 Jan 2000 (G) plus 12 hours

    // CC3 p175
    inline double zone_from_longitude( double longitude )
    {
        return longitude / 360;
    }

    // CC3 p175
    inline double universal_from_local( double moment, double longitude )
    {
        return moment - zone_from_longitude( longitude );
    }

    // CC3 p175
    inline double local_from_universal( double moment, double longitude )
    {
        return moment + zone_from_longitude( longitude );
    }

    // CC3 p177
    inline double standard_from_universal( double moment, double zone )
    {
        return moment + zone;
    }

    // CC3 p177
    inline double universal_from_standard( double moment, double zone )
    {
        return moment - zone;
    }

    double equation_of_time( double moment );

    double solar_longitude( double moment );
    double solar_longitude_after( double season, double moment );

    double estimate_prior_solar_longitude( double season, double moment );

}

#endif // CAL_CALASTRO_H