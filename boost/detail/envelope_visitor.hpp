// Andrew Naplavkov

#ifndef BRIG_BOOST_DETAIL_ENVELOPE_VISITOR_HPP
#define BRIG_BOOST_DETAIL_ENVELOPE_VISITOR_HPP

#include <boost/geometry/geometry.hpp>
#include <brig/boost/geometry.hpp>
#include <stdexcept>

namespace brig { namespace boost { namespace detail {

class envelope_visitor : public ::boost::static_visitor<box> {
  template <typename T> box envelope_single(const T& r) const;
  template <typename T> box envelope_multi(const T& r) const;
public:
  box operator()(const point& r) const  { return envelope_single(r); }
  box operator()(const linestring& r) const  { return envelope_single(r); }
  box operator()(const polygon& r) const  { return envelope_single(r); }
  box operator()(const multi_point& r) const  { return envelope_multi(r); }
  box operator()(const multi_linestring& r) const  { return envelope_multi(r); }
  box operator()(const multi_polygon& r) const  { return envelope_multi(r); }
  box operator()(const ::boost::recursive_wrapper<geometry_collection>& r) const;
}; // envelope_visitor

template <typename T>
box envelope_visitor::envelope_single(const T& r) const
{
  box res;
  ::boost::geometry::envelope(r, res);
  return res;
}

template <typename T>
box envelope_visitor::envelope_multi(const T& r) const
{
  box res;
  auto itr(std::begin(r)), end(std::end(r));
  if (itr == end) throw std::runtime_error("envelope error");
  ::boost::geometry::envelope(*itr, res); ++itr;
  for (; itr != end; ++itr)
  {
    box env;
    ::boost::geometry::envelope(*itr, env);
    ::boost::geometry::expand(res, env);
  }
  return res;
}

inline box envelope_visitor::operator()(const ::boost::recursive_wrapper<geometry_collection>& r) const
{
  auto itr(std::begin(r.get())), end(std::end(r.get()));
  if (itr == end) throw std::runtime_error("envelope error");
  envelope_visitor visitor;
  box res(::boost::apply_visitor(visitor, *itr)); ++itr;
  for (; itr != end; ++itr)
    ::boost::geometry::expand(res, ::boost::apply_visitor(visitor, *itr));
  return res;
} // envelope_visitor::

} } } // brig::boost::detail

#endif // BRIG_BOOST_DETAIL_ENVELOPE_VISITOR_HPP
