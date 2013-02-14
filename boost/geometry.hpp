// Andrew Naplavkov

#ifndef BRIG_BOOST_GEOMETRY_HPP
#define BRIG_BOOST_GEOMETRY_HPP

#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/multi/geometries/multi_linestring.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/variant.hpp>
#include <vector>

namespace brig { namespace boost {

typedef ::boost::geometry::model::d2::point_xy<double> point;
typedef ::boost::geometry::model::box<point> box;
typedef ::boost::geometry::model::ring<point, false, true> linearring;
typedef ::boost::geometry::model::linestring<point> linestring;
typedef ::boost::geometry::model::polygon<point, false, true> polygon;
typedef ::boost::geometry::model::multi_point<point> multi_point;
typedef ::boost::geometry::model::multi_linestring<linestring> multi_linestring;
typedef ::boost::geometry::model::multi_polygon<polygon> multi_polygon;
struct geometry_collection;
typedef ::boost::variant<
  point,
  linestring,
  polygon,
  multi_point,
  multi_linestring,
  multi_polygon,
  ::boost::recursive_wrapper<geometry_collection>
> geometry;
struct geometry_collection : std::vector<geometry>  {};

} } // brig::boost

#endif // BRIG_BOOST_GEOMETRY_HPP
