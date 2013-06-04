// Andrew Naplavkov

#ifndef BRIG_PROVIDER_HPP
#define BRIG_PROVIDER_HPP

#include <boost/utility.hpp>
#include <brig/boost/geometry.hpp>
#include <brig/identifier.hpp>
#include <brig/insert_iterator.hpp>
#include <brig/inserter.hpp>
#include <brig/pyramid_def.hpp>
#include <brig/rowset.hpp>
#include <brig/rowset_iterator.hpp>
#include <brig/table_def.hpp>
#include <memory>
#include <string>
#include <vector>

namespace brig {

struct provider : ::boost::noncopyable
{
  virtual ~provider()  {}

  virtual std::vector<identifier> get_tables() = 0;
  virtual std::vector<identifier> get_geometry_layers() = 0;
  virtual std::vector<pyramid_def> get_raster_layers() = 0;
  virtual table_def get_table_def(const identifier& tbl) = 0;
  virtual boost::box get_extent(const table_def& tbl) = 0;
  virtual std::shared_ptr<rowset> select(const table_def& tbl) = 0;

  virtual bool is_readonly() = 0;
  /*!
  *  AFTER CALL:
  *  * for database::provider there is a chance to get an extra field in the tail - the primary key with automatic generation
  *  * define bounding box with brig::boost::as_binary() in result column_def.query_value if result column_def.is_extent_requested()
  */
  virtual table_def fit_to_create(const table_def& tbl) = 0;
  virtual void create(const table_def& tbl) = 0;
  virtual void drop(const table_def& tbl) = 0;
  virtual pyramid_def fit_to_reg(const pyramid_def& raster) = 0;
  virtual void reg(const pyramid_def& raster) = 0;
  virtual void unreg(const pyramid_def& raster) = 0;
  virtual std::shared_ptr<inserter> get_inserter(const table_def& tbl) = 0;
}; // provider

} // brig

#endif // BRIG_PROVIDER_HPP
