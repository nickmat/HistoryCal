
#include "calastro.h"

#include "calmath.h"
#include "calbase.h"
#include "calgregorian.h"

#include <cmath>
#include <algorithm>

using namespace Cal;

// CC3 p187
const double Cal::mean_tropical_year = 365.242189; // days

// CC3 p191
const double Cal::spring = 0.0; // degrees
const double Cal::summer = 90.0; 
const double Cal::autumn = 180.0; 
const double Cal::winter = 270.0;

//
double const Cal::j2000 = 2451545.5; // 1 Jan 2000 (G) plus 12 hours

namespace {

inline long date_difference( long jdn1, long jdn2 ) { return jdn2 - jdn1; } 

// NOTE: 
// 1 Jan 2000 (G) = 2451545 (JDN)
// 1 Jan 1900 (G) = 2415021 (JDN)
// 1 Jan 1810 (G) = 2382149 (JDN)

// CC3 p177
double ephemeris_correction( double moment )
{
    const double daypersec = 1.0 / 86400.0;
    Field jdn = (Field) moment;
    Field year = Gregorian::year_from_jdn( jdn );

    if( year <= 2019 ) {
        if( year >= 1988 ) {
            return daypersec * (double) ( year - 1933 );
        }
        if( year >= 1800 ) {
            Field jdn2 = Gregorian::to_jdn( year, 7, 1 );
            double c = 
                ( 1.0 / 36525.0 ) * (double) date_difference( 2415021L, jdn2 )
            ;
            double c2 = c * c;
            double c3 = c2 * c;
            double c4 = c3 * c;
            double c5 = c4 * c;
            double c6 = c5 * c;
            double c7 = c6 * c;
            if( year >= 1900 ) {
                return 
                    - 0.00002 + 0.000297 * c + 0.025184 * c2
                    - 0.181133 * c3 + 0.553040 * c4 - 0.861938 * c5
                    + 0.677066 * c6 - 0.212591 * c7
                ;
            }
            double c8 = c7 * c;
            double c9 = c8 * c;
            double c10 = c9 * c;
            return
                - 0.000009 + 0.003844 * c + 0.083563 * c2 + 0.865736 * c3
                + 4.867575 * c4 + 15.845535 * c5 + 31.332267 * c6
                + 38.291999 * c7 + 28.316289 * c8 + 11.636204 * c9
                + 2.0437994 * c10
            ;
        }
        if( year >= 1700 ) {
            double ya = (double) ( year - 1700 );
            double ya2 = ya * ya;
            double ya3 = ya2 * ya;
            return 
                daypersec * ( 8.118780842 - 0.005092142 * ya
                + 0.003336121 * ya2 - 0.0000266484 * ya3 )
            ;
        }
        if( year >= 1620 ) {
            double yb = (double) ( year - 1600 );
            double yb2 = yb * yb;
            return daypersec * ( 196.58333 - 4.0675 * yb + 0.0219167 * yb2 );
        }
    }
    Field jdn3 = Gregorian::to_jdn( year, 1, 1 );
    double x = 0.5 + (double) date_difference( 2382149L, jdn3 );
    return daypersec * ( ( x * x ) / 41048480.0 - 15.0 );
}

// CC3 p179
double julian_centries( double moment )
{
    double dynamictime = moment + ephemeris_correction( moment );
    return ( 1.0 / 36525.0 ) * ( dynamictime - j2000 );
}

// CC3 p186
double obliquity_from_jc( double c )
{
    double c2 = c * c;
    double c3 = c2 * c;
    return 23.4392911 - 0.01300417 * c - 0.000000164 * c2 + 0.00000050361 * c3;
}

// CC3 p189
double aberration_from_jc( double c )
{
    return 0.000974 * cos( ( 177.63 + 35999.01848 * c ) * cal_pi / 180 ) - 0.005575;
}

// CC3 p189
double nutation_from_jc( double c )
{
    double c2 = c * c;
    double A = 124.9 - 1934.134 * c + 0.002063 * c2;
    double B = 201.11 + 72001.5377 * c + 0.00057 * c2;
    return - 0.004778 * sin( A * cal_pi / 180 ) - 0.0003667 * sin( B * cal_pi / 180 );
}

} // namespace

// CC3 p183  moment: Moment in Universal Time.  Returns fraction of day
double Cal::equation_of_time( double moment )
{
    double c = julian_centries( moment );
    double c2 = c * c;
    double c3 = c2 * c;
    double lambda = 280.46645 + 36000.76983 * c + 0.0003032 * c2;
    double anomaly = 357.52910 + 35999.050303 * c - 0.0001559 * c2 - 0.00000048 * c3;
    double eccentricity = 0.016708617 - 0.000042037 * c - 0.0000001236 * c2;
    double epsilon = obliquity_from_jc( c );
    double y = tan( deg_to_rad( epsilon / 2 ) );
    y *= y;

    double equation = 
        ( 1.0 / ( 2 * cal_pi ) ) * (
        y * sin( 2 * lambda * cal_pi / 180 ) 
        - 2 * eccentricity * sin( deg_to_rad( anomaly ) )
        + 4 * eccentricity * y * sin( deg_to_rad( anomaly ) ) * cos( 2 * deg_to_rad( lambda ) )
        - 0.5 * y * y * sin( 4 * lambda * cal_pi / 180 )
        - 1.25 * eccentricity * eccentricity * sin( 2 * deg_to_rad( anomaly ) ) )
    ;
    double t1 = y * sin( 2 * lambda * cal_pi / 180 );
    double t2 = - 2 * eccentricity * sin( anomaly * cal_pi / 180 );
    double t3 = 4 * eccentricity * y * sin( anomaly * cal_pi / 180 ) * cos( 2 * lambda * cal_pi / 180 );
    double t4 = - 0.5 * y * y * sin( 4 * lambda * cal_pi / 180 );
    double t5 = - 1.25 * eccentricity * eccentricity * sin( 2 * anomaly * cal_pi / 180 );
    double equation2 = ( 1.0 / ( 2 * cal_pi ) ) * ( t1 + t2 + t3 + t4 + t5 );
    return cal_signum( equation ) * std::min( (double) abs( equation ), 0.5 );
}

// CC3 p189
double Cal::solar_longtitude( double moment )
{
    struct Args {
        double x; double y; double z;
    } a[] = {
        { 403406, 270.54861, 0.9287892 },
        { 195207, 340.19128, 35999.1376958 },
        { 119433, 63.91854, 35999.4089666 },
        { 112392, 331.26220, 35998.7287385 },
        { 3891, 317.843, 71998.20261 },
        { 2819, 86.631, 71998.4403 },
        { 1721, 240.052, 36000.35726 },
        { 660, 310.26, 71997.4812 },
        { 350, 247.23, 32964.4678 },
        { 334, 260.87, -19.441 },
        { 314, 297.82, 445267.1117 },
        { 268, 343.14, 45036.884 },
        { 242, 166.79, 3.1008 },
        { 234, 81.53, 22518.4434 },
        { 158, 3.5, -19.9739 },
        { 132, 132.75, 65928.9345 },
        { 129, 182.95, 9038.0293 },
        { 114, 162.03, 3034.7684 },
        { 99, 29.8, 33718.148 },
        { 93, 266.4, 3034.448 },
        { 86, 249.2, -2280.773 },
        { 78, 157.6, 29929.992 },
        { 72, 257.8, 31556.493 },
        { 68, 185.1, 149.588 },
        { 64, 69.9, 9037.75 },
        { 46, 8, 107997.405 },
        { 38, 197.1, -4444.176 },
        { 37, 250.4, 151.771 },
        { 32, 65.3, 67555.316 },
        { 29, 162.7, 31556.08 },
        { 28, 341.5, -4561.54 },
        { 27, 291.6, 107996.706 },
        { 27, 98.5, 1221.655 },
        { 25, 146.7, 62894.167 },
        { 24, 110, 31437.369 },
        { 21, 5.2, 14578.298 },
        { 21, 342.6, -31931.757 },
        { 20, 230.9, 34777.243 },
        { 18, 256.1, 1221.999 },
        { 17, 45.3, 62894.511 },
        { 14, 242.9, -4442.039 },
        { 13, 115.2, 107997.909 },
        { 13, 151.8, 119.066 },
        { 13, 285.3, 16859.071 },
        { 12, 53.3, -4.578 },
        { 10, 126.6, 26895.292 },
        { 10, 205.7, -39.127 },
        { 10, 85.9, 12297.536 },
        { 10, 146.1, 90073.778 }
    };
    size_t size = sizeof( a ) / sizeof( Args );
    double c = julian_centries( moment );
    double sum = 0.0;
    for( size_t i = 0 ; i < size ; i++ ) {
        sum += a[i].x * sin( deg_to_rad( a[i].y + a[i].z * c ) );
    }
    double lambda = 282.7771834 + 36000.76953744 * c + 0.000005729577951308232 * sum;

    return cal_mod( lambda + aberration_from_jc( c ) + nutation_from_jc( c ), 360 );
}

// CC3 p193
double Cal::estimate_prior_solar_longitude( double lambda, double moment )
{
    double rate = mean_tropical_year / 360;
    double tau = 
        moment - rate * cal_mod( solar_longtitude( moment ) - lambda, 360 );
    double delta = cal_mod( solar_longtitude( tau ) - lambda + 180, 360 ) - 180;
    return std::min( moment, tau - rate * delta );
}

// End of src/cal/calAstro.cpp
