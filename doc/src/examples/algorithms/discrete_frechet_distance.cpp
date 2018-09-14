// Boost.Geometry
// QuickBook Example
// Copyright (c) 2018, Oracle and/or its affiliates
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[discrete_frechet_distance
//` Calculate Similarity between two geometries as the discrete frechet distance between them.
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/discrete_frechet_distance.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/linestring.hpp>
int main()
{
    typedef boost::geometry::model::d2::point_xy<double> point_type;
    typedef boost::geometry::model::linestring<point_type> linestring_type;
    linestring_type linestring1,linestring2;
    boost::geometry::read_wkt("LINESTRING(0 0,1 1,1 2,2 1,2 2)", linestring1);
    boost::geometry::read_wkt("LINESTRING(1 0,0 1,1 1,2 1,3 1)", linestring2);
 	double res;
    res = boost::geometry::discrete_frechet_distance(linestring1,linestring2);
    std::cout << "Discrete Frechet Distance: " << res << std::endl;
    return 0;
}
//]
 //[discrete_frechet_distance_output
/*`
Output:
[pre
Discrete Frechet Distance:  1.41421
]
*/
//]