// Boost.Geometry

// Copyright (c) 2018, Oracle and/or its affiliates.

// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_GEOMETRY_STRATEGIES_GEOGRAPHIC_SEGMENT_INTERPOLATE_POINT_HPP
#define BOOST_GEOMETRY_STRATEGIES_GEOGRAPHIC_SEGMENT_INTERPOLATE_POINT_HPP

#include <boost/geometry/core/assert.hpp>
#include <boost/geometry/core/coordinate_dimension.hpp>
#include <boost/geometry/core/coordinate_type.hpp>
#include <boost/geometry/core/radian_access.hpp>
#include <boost/geometry/srs/spheroid.hpp>
#include <boost/geometry/strategies/segment_interpolate_point.hpp>
#include <boost/geometry/strategies/geographic/parameters.hpp>
#include <boost/geometry/util/select_most_precise.hpp>


namespace boost { namespace geometry
{

namespace strategy { namespace segment_interpolate_point
{


/*!
\brief Interpolate point on a geographic segment.
\ingroup strategies
\tparam FormulaPolicy The geodesic formulas used internally.
\tparam Spheroid The spheroid model.
\tparam CalculationType \tparam_calculation

\qbk{
[heading See also]
\* [link geometry.reference.srs.srs_spheroid srs::spheroid]
}
 */
template
<
    typename FormulaPolicy = strategy::andoyer,
    typename Spheroid = srs::spheroid<double>,
    typename CalculationType = void
>
class geographic
{
public:
    geographic()
        : m_spheroid()
    {}

    explicit geographic(Spheroid const& spheroid)
        : m_spheroid(spheroid)
    {}

    // point-point strategy getters
    struct distance_pp_strategy
    {
        typedef distance::geographic<FormulaPolicy, Spheroid, CalculationType> type;
    };

    inline typename distance_pp_strategy::type get_distance_pp_strategy() const
    {
        typedef typename distance_pp_strategy::type distance_type;
        return distance_type(m_spheroid);
    }

    //result type
    template <typename Point>
    struct result_type
    {
        typedef typename select_most_precise
            <
                typename coordinate_type<Point>::type,
                CalculationType
            >::type calc_t;

        result_type() :
            distance(0),
            azimuth(0)
        {}

        result_type(calc_t d, calc_t a) :
            distance(d),
            azimuth(a)
        {}

        calc_t distance;
        calc_t azimuth;
    };

    template <typename Point>
    inline result_type<Point> compute(Point const& p0,
                                      Point const& p1) const
    {
        typedef typename result_type<Point>::calc_t calc_t;

        typedef typename FormulaPolicy::template inverse
                <calc_t, true, true, false, false, false> inverse_t;

        typename inverse_t::result_type
            inv_r = inverse_t::apply(get_as_radian<0>(p0), get_as_radian<1>(p0),
                                     get_as_radian<0>(p1), get_as_radian<1>(p1),
                                     m_spheroid);

        return result_type<Point>(inv_r.distance, inv_r.azimuth);
    }

    template <typename Point, typename T1, typename T2>
    inline void apply(Point const& p0,
                      Point const& p1,
                      T1 const& fraction,
                      Point & p,
                      T2 const& inv_r) const
    {
        typedef typename select_most_precise
            <
                typename coordinate_type<Point>::type,
                CalculationType
            >::type calc_t;

        typedef typename FormulaPolicy::template direct
                <calc_t, true, false, false, false> direct_t;
        //typedef typename FormulaPolicy::template inverse
        //        <calc_t, true, true, false, false, false> inverse_t;

        //typename inverse_t::result_type
        //    inv_r = inverse_t::apply(get_as_radian<0>(p0), get_as_radian<1>(p0),
        //                             get_as_radian<0>(p1), get_as_radian<1>(p1),
        //                             m_spheroid);
        typename direct_t::result_type
        dir_r = direct_t::apply(get_as_radian<0>(p0), get_as_radian<1>(p0),
                                inv_r.distance * fraction, inv_r.azimuth,
                                m_spheroid);

        set_from_radian<0>(p, dir_r.lon2);
        set_from_radian<1>(p, dir_r.lat2);
    }

private:
    Spheroid m_spheroid;
};


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS
namespace services
{

template <>
struct default_strategy<geographic_tag>
{
    typedef strategy::segment_interpolate_point::geographic<> type;
};


} // namespace services
#endif // DOXYGEN_NO_STRATEGY_SPECIALIZATIONS


}} // namespace strategy::segment_interpolate_point


}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_STRATEGIES_GEOGRAPHIC_SEGMENT_INTERPOLATE_POINT_HPP
