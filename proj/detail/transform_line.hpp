// Andrew Naplavkov

#ifndef BRIG_PROJ_DETAIL_TRANSFORM_LINE_HPP
#define BRIG_PROJ_DETAIL_TRANSFORM_LINE_HPP

#include <brig/detail/ogc.hpp>
#include <brig/proj/detail/lib.hpp>
#include <brig/proj/detail/transform_point.hpp>
#include <brig/proj/transform.hpp>
#include <cstdint>
#include <cstring>

namespace brig { namespace proj { namespace detail {

template <typename InputIterator, typename OutputIterator>
void transform_line(uint8_t byte_order, InputIterator& in_itr, OutputIterator& out_itr, projPJ in_pj, projPJ out_pj)
{
  using namespace brig::detail::ogc;
  const uint32_t count(read<uint32_t>(byte_order, in_itr)); write<uint32_t>(out_itr, count);
  for (uint32_t i(0); i < count; ++i)
    transform_point(byte_order, in_itr, out_itr, in_pj, out_pj);
}

template <typename InputType, typename OutputType>
void transform_line(uint8_t byte_order, InputType*& in_ptr, OutputType*& out_ptr, projPJ in_pj, projPJ out_pj) // optimization
{
  using namespace brig::detail::ogc;
  static_assert(sizeof(InputType) == sizeof(uint8_t), "size error");
  static_assert(sizeof(OutputType) == sizeof(uint8_t), "size error");
  const uint32_t point_xy_count(read<uint32_t>(byte_order, in_ptr)); write<uint32_t>(out_ptr, point_xy_count);
  double* point_xy_begin((double*)out_ptr);
  if (HostEndian == byte_order)
  {
    const size_t bytes(point_xy_count * 2 * sizeof(double));
    if (in_ptr != out_ptr) memcpy(out_ptr, in_ptr, bytes);
    in_ptr += bytes;
    out_ptr += bytes;
  }
  else
    for (uint32_t i(0); i < point_xy_count; ++i)
    {
      write<double>(out_ptr, read<double>(byte_order, in_ptr));
      write<double>(out_ptr, read<double>(byte_order, in_ptr));
    }
  transform(point_xy_begin, long(point_xy_count), in_pj, out_pj);
}

} } } // brig::proj::detail

#endif // BRIG_PROJ_DETAIL_TRANSFORM_LINE_HPP
