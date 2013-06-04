// Andrew Naplavkov

#ifndef BRIG_BOOST_DETAIL_READ_POLYGON_HPP
#define BRIG_BOOST_DETAIL_READ_POLYGON_HPP

#include <brig/detail/ogc.hpp>
#include <brig/boost/detail/read_line.hpp>
#include <brig/boost/geometry.hpp>
#include <cstdint>

namespace brig { namespace boost { namespace detail {

template <typename InputIterator>
void read_polygon(uint8_t byte_order, InputIterator& itr, polygon& poly)
{
  const size_t count(brig::detail::ogc::read<uint32_t>(byte_order, itr));
  if (count == 0) return;
  read_line(byte_order, itr, poly.outer());
  poly.inners().resize(count - 1);
  for (size_t i(0); i < poly.inners().size(); ++i)
    read_line(byte_order, itr, poly.inners()[i]);
}

} } } // brig::boost::detail

#endif // BRIG_BOOST_DETAIL_READ_POLYGON_HPP
