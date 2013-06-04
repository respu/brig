// Andrew Naplavkov

#ifndef BRIG_GDAL_DETAIL_DATASET_ALLOCATOR_HPP
#define BRIG_GDAL_DETAIL_DATASET_ALLOCATOR_HPP

#include <brig/gdal/detail/dataset.hpp>
#include <string>

namespace brig { namespace gdal { namespace detail {

class dataset_allocator {
  std::string m_ds;
public:
  dataset_allocator(const std::string& ds) : m_ds(ds)  {}
  dataset allocate()  { return dataset(m_ds); }
}; // dataset_allocator

} } } // brig::gdal::detail

#endif // BRIG_GDAL_DETAIL_DATASET_ALLOCATOR_HPP
