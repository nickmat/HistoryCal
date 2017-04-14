/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/cal/calastro.cpp
 * Project:     Cal: Programmable Historical Calendar library.
 * Purpose:     Functions for astrological calculations.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
 * Created:     19th December 2014
 * Copyright:   Copyright (c) 2014 ~ 2017, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  The Cal library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The Cal library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the Cal library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#include "calastro.h"

#include "calmath.h"
#include "calbase.h"
#include "calgregorian.h"

#include <algorithm>
#include <cassert>
#include <cmath>

using namespace Cal;

// CC3 p187
const double Cal::mean_tropical_year = 365.242189; // days

// CC3 p193
const double Cal::mean_synodic_month = 29.530588853; // days

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
inline double universal_from_dynamical( double moment )
{
    return moment - ephemeris_correction( moment );
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
double Cal::solar_longitude( double moment )
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

// Based on CC p21
static double solar_longitude_inverted( double angle, double a, double b )
{
    assert( a < b );
    const double epsilon = 1e-5;
    const int maxSearch = 1000;
    for( int i = 0 ; i < maxSearch ; i++ ) {
        double x = ( a + b ) / 2;
        double y = solar_longitude( x );
        double e = fmod_r( y - angle, 360 );
        if( std::abs( e ) < epsilon ) {
            return x;
        }
        if( e < 0 ) {
            a = x;
        } else {
            b = x;
        }
    }
    assert( false ); // Should not be here. Increase maxSearch?
    return ( a + b ) / 2;
}

// CC3 p190
double Cal::solar_longitude_after( double season, double moment )
{
    double rate = mean_tropical_year / 360;
    double tau = moment + rate * cal_mod( season - solar_longitude( moment ), 360 );
    double a = std::max( moment, tau - 5 );
    double b = tau + 5;
    return solar_longitude_inverted( season, a, b );
}

// CC3 p193
double Cal::estimate_prior_solar_longitude( double lambda, double moment )
{
    double rate = mean_tropical_year / 360;
    double tau = 
        moment - rate * cal_mod( solar_longitude( moment ) - lambda, 360 );
    double delta = cal_mod( solar_longitude( tau ) - lambda + 180, 360 ) - 180;
    return std::min( moment, tau - rate * delta );
}

// CC3 p195 nth-new-moon
// Centered on g# 1 Jan 2000,
// nth_new_moon was based on new moon of g:dmy# 11 Jan 1.
static
double nth_new_moon_since_j2000( Field n )
{
    static const struct Arg1s { // Table 13.3
        double v; int w; double x, y, z;
    } a1[] = {
        { -0.40720, 0,  0, 1,  0 },
        {  0.17241, 1,  1, 0,  0 },
        {  0.01608, 0,  0, 2,  0 },
        {  0.01039, 0,  0, 0,  2 },
        {  0.00739, 1, -1, 1,  0 },
        { -0.00514, 1,  1, 1,  0 },
        {  0.00208, 2,  2, 0,  0 },
        { -0.00111, 0,  0, 1, -2 },
        { -0.00057, 0,  0, 1,  2 },
        {  0.00056, 1,  1, 2,  0 },
        { -0.00042, 0,  0, 3,  0 },
        {  0.00042, 1,  1, 0,  2 },
        {  0.00038, 1,  1, 0, -2 },
        { -0.00024, 1, -1, 2,  0 },
        { -0.00007, 0,  2, 1,  0 },
        {  0.00004, 0,  0, 2, -2 },
        {  0.00004, 0,  3, 0,  0 },
        {  0.00003, 0,  1, 1, -2 },
        {  0.00003, 0,  0, 2,  2 },
        { -0.00003, 0,  1, 1,  2 },
        {  0.00003, 0, -1, 1,  2 },
        { -0.00002, 0, -1, 1, -2 },
        { -0.00002, 0,  1, 3,  0 },
        {  0.00002, 0,  0, 4,  0 }
    };
    static const size_t size1 = sizeof( a1 ) / sizeof( Arg1s );

    static const struct Arg2s { // Table 13.4
        double h, j, l; // Yes, that really is a lowercase L
    } a2[] = {
        { 251.88,  0.016321, 0.000165 },
        { 251.83, 26.641886, 0.000164 },
        { 349.42, 36.412478, 0.000126 },
        {  84.66, 18.206239, 0.000110 },
        { 141.74, 53.303771, 0.000062 },
        { 207.14,  2.453732, 0.000060 },
        { 154.84,  7.306860, 0.000056 },
        {  34.52, 27.261239, 0.000047 },
        { 207.19,  0.121824, 0.000042 },
        { 291.34,  1.844379, 0.000040 },
        { 161.72, 24.198154, 0.000037 },
        { 239.56, 25.513099, 0.000035 },
        { 331.55,  3.592518, 0.000023 }
    };
    static const size_t size2 = sizeof( a2 ) / sizeof( Arg2s );

    double c = double( n ) / 1236.85;
    double c2 = c * c;
    double c3 = c2 * c;
    double c4 = c3 * c;
    double approx = j2000 + 5.09765 + mean_synodic_month * 1236.85 * c
        + 0.0001337 * c2 - 0.00000015 * c3 + 0.00000000073 * c4;

    double E = 1.0 - 0.002516 * c - 0.0000074 * c2;
    double solar_a = 2.5534 + 1236.85 * 29.10535669 * c
        - 0.0000218 * c2 - 0.00000011 * c3;
    double lunar_a = 201.5643 + 385.81693528 * 1236.85 * c
        + 0.0107438 * c2 + 0.00001239 * c3 - 0.000000058 * c4;
    double moon_arg = 160.7108 + 390.67050274 * 1236.85 * c
        - 0.0016341 * c2 - 0.00000227 * c3 + 0.000000011 * c4;
    double omega = 124.7746 - 1.56375580 * 1236.85 * c
        + 0.0020691 * c2 + 0.00000215 * c3;

    double sum1 = 0.0;
    for( size_t i = 0 ; i < size1 ; i++ ) {
        double vEw = a1[i].v;
        switch( a1[i].w )
        {
        case 2:
            vEw *= E; // fall thru
        case 1:
            vEw *= E;
        }
        sum1 += vEw * sin_d(
            a1[i].x * solar_a + a1[i].y * lunar_a + a1[i].z * moon_arg );
    }
    double correction = -0.00017 * sin_d( omega ) + sum1;

    double extra = 0.000325 * 
        sin_d( 299.77 + 132.8475848 * c - 0.009173 * c2 );

    double additional = 0.0;
    for( size_t i = 0 ; i < size2 ; i++ ) {
        additional =+ a2[i].l * sin_d( a2[i].h + a2[i].j * n );
    }

    return universal_from_dynamical(
        approx + correction + extra + additional );
}

// CC3 p198 required for new_moon_before
static
bool new_moon_before_max_func( Field jdn, const void* data )
{
    double constant = *static_cast<const double*>(data);
    return nth_new_moon_since_j2000( jdn ) < constant;
}

static double lunar_phase( double moment );

// CC3 p198
double Cal::new_moon_before( double moment )
{
    double t0 = nth_new_moon_since_j2000( 0 ); // TODO: make constant
    double phi = lunar_phase( moment );
    Field n = Field( ( ( moment - t0 ) / mean_synodic_month ) - ( phi / 360 ) + 0.5 );

    return nth_new_moon_since_j2000( max_search( n - 1, new_moon_before_max_func, &moment ) );
}

// CC3 p198 required for new_moon_before
static
bool new_moon_at_or_after_min_func( Field jdn, const void* data )
{
    double constant = *static_cast<const double*>(data);
    return nth_new_moon_since_j2000( jdn ) >= constant;
}

// CC3 p198
double Cal::new_moon_at_or_after( double moment )
{
    double t0 = nth_new_moon_since_j2000( 0 ); // TODO: make constant
    double phi = lunar_phase( moment );
    Field n = Field( ( ( moment - t0 ) / mean_synodic_month ) - ( phi / 360 ) + 0.5 );

    return nth_new_moon_since_j2000(
        min_search( n, new_moon_at_or_after_min_func, &moment )
    );
}

static double mean_lunar_longitude( double c );
static double lunar_elongation( double c );
static double solar_anomaly( double c );
static double lunar_anomaly( double c );
static double moon_node( double c );

// CC3 p199
static
double lunar_longitude( double moment )
{
    static const struct Args {
        double v, w; int x; double y, z;
    } a[] = { // Table 13.5
        { 6288774, 0,  0,  1,  0 },
        { 1274027, 2,  0, -1,  0 },
        {  658314, 2,  0,  0,  0 },
        {  213618, 0,  0,  2,  0 },
        { -185116, 0,  1,  0,  0 },
        { -114332, 0,  0,  0,  2 },
        {   58793, 2,  0, -2,  0 },
        {   57066, 2, -1, -1,  0 },
        {   53322, 2,  0,  1,  0 },
        {   45758, 2, -1,  0,  0 },
        {  -40923, 0,  1, -1,  0 },
        {  -34720, 1,  0,  0,  0 },
        {  -30383, 0,  1,  1,  0 },
        {   15327, 2,  0,  0, -2 },
        {  -12528, 0,  0,  1,  2 },
        {   10980, 0,  0,  1, -2 },
        {   10675, 4,  0, -1,  0 },
        {   10034, 0,  0,  3,  0 },
        {    8548, 4,  0, -2,  0 },
        {   -7888, 2,  1, -1,  0 },
        {   -6766, 2,  1,  0,  0 },
        {   -5163, 1,  0, -1,  0 },
        {    4987, 1,  1,  0,  0 },
        {    4036, 2, -1,  1,  0 },
        {    3994, 2,  0,  2,  0 },
        {    3861, 4,  0,  0,  0 },
        {    3665, 2,  0, -3,  0 },
        {   -2689, 0,  1, -2,  0 },
        {   -2602, 2,  0, -1,  2 },
        {    2390, 2, -1, -2,  0 },
        {   -2348, 1,  0,  1,  0 },
        {    2236, 2, -2,  0,  0 },
        {   -2120, 0,  1,  2,  0 },
        {   -2069, 0,  2,  0,  0 },
        {    2048, 2, -2, -1,  0 },
        {   -1773, 2,  0,  1, -2 },
        {   -1595, 2,  0,  0,  2 },
        {    1215, 4, -1, -1,  0 },
        {   -1110, 0,  0,  2,  2 },
        {    -892, 3,  0, -1,  0 },
        {    -810, 2,  1,  1,  0 },
        {     759, 4, -1, -2,  0 },
        {    -713, 0,  2, -1,  0 },
        {    -700, 2,  2, -1,  0 },
        {     691, 2,  1, -2,  0 },
        {     596, 2, -1,  0, -2 },
        {     549, 4,  0,  1,  0 },
        {     537, 0,  0,  4,  0 },
        {     520, 4, -1,  0,  0 },
        {    -487, 1,  0, -2,  0 },
        {    -399, 2,  1,  0, -2 },
        {    -381, 0,  0,  2, -2 },
        {     351, 1,  1,  1,  0 },
        {    -340, 3,  0, -2,  0 },
        {     330, 4,  0, -3,  0 },
        {     327, 2, -1,  2,  0 },
        {    -323, 0,  2,  1,  0 },
        {     299, 1,  1, -1,  0 },
        {     294, 2,  0,  3,  0 }
    };
    static const size_t size = sizeof( a ) / sizeof( Args );

    double c = julian_centries( moment );
    double Ld = mean_lunar_longitude( c );
    double D = lunar_elongation( c );
    double M = solar_anomaly( c );
    double Md = lunar_anomaly( c );
    double F = moon_node( c );
    double E = 1.0 - 0.002516 * c - 0.0000074 * c * c;

    double sum = 0.0;
    for( size_t i = 0 ; i < size ; i++ ) {
        double vEx = a[i].v;
        switch( a[i].x )
        {
        case 2: case -2:
            vEx *= E; // fall thru
        case 1: case -1:
            vEx *= E;
        }
        sum += vEx * sin_d(
            a[i].w * D + double( a[i].x ) * M + a[i].y * Md + a[i].z * F );

    }
    double correction = 0.000001 * sum;

    double venus = 0.003958 * sin_d( 119.75 + c * 131.849 );
    double jupiter = 0.000318 * sin_d( 53.09 + c * 479264.29 );
    double flat_earth = 0.001962 * sin_d( Ld - F );
    double n = nutation_from_jc( c );

    return cal_mod( Ld + correction + venus + jupiter + flat_earth + n, 360 );
}

// CC3 p200
double mean_lunar_longitude( double c )
{
    double c2 = c * c;
    double c3 = c2 * c;
    double c4 = c3 * c;
    return 218.3164477 + c * 481267.88123421 - c2 * 0.0015786
        + c3 / 538841 - c4 / 65194000;
}

// CC3 p201
double lunar_elongation( double c )
{
    double c2 = c * c;
    double c3 = c2 * c;
    double c4 = c3 * c;
    return 297.8501921 + c * 445267.1114034 - c2 * 0.0018819
        + c3 / 545868 - c4 / 113065000;
}

// CC3 p201
double solar_anomaly( double c )
{
    double c2 = c * c;
    double c3 = c2 * c;
    return 357.5291092 + c * 35999.0502909 - c2 * 0.0001536
        + c3 / 24490000;
}

// CC3 p201
double lunar_anomaly( double c )
{
    double c2 = c * c;
    double c3 = c2 * c;
    double c4 = c3 * c;
    return 134.9633964 + c * 477198.8675055 + c2 * 0.0087414
        + c3 / 69699 - c4 / 14712000;
}

// CC3 p201
double moon_node( double c )
{
    double c2 = c * c;
    double c3 = c2 * c;
    double c4 = c3 * c;
    return 93.2720950 + c * 483202.0175233 - c2 * 0.0036539
        - c3 / 3526000 + c4 / 863310000;
}

// CC3 p201
double lunar_phase( double moment )
{
    double phi = cal_mod(
        lunar_longitude( moment ) - solar_longitude( moment ), 360 );
    double t0 = nth_new_moon_since_j2000( 0 ); // TODO: make constant
    Field n = Field( ( moment - t0 ) / mean_synodic_month + 0.5 );
    double phi_dash = 360 *
        cal_mod( 
            ( moment - nth_new_moon_since_j2000( n ) ) / mean_synodic_month, 1
        )
    ;

    if( abs( phi - phi_dash ) > 180 ) {
        return phi_dash;
    }
    return phi;
}

// End of src/cal/calAstro.cpp
