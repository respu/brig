// Andrew Naplavkov

#ifndef BRIG_BOOST_SCAN_HPP
#define BRIG_BOOST_SCAN_HPP

#include <brig/blob_t.hpp>
#include <brig/boost/as_binary.hpp>
#include <brig/boost/geom_from_text.hpp>

namespace brig { namespace boost {

template <typename Text>
blob_t scan(Text wkt)
{
  return as_binary(geom_from_text(wkt));
}

} } // brig::boost

#endif // BRIG_BOOST_SCAN_HPP
