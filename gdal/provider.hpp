// Andrew Naplavkov

#ifndef BRIG_GDAL_PROVIDER_HPP
#define BRIG_GDAL_PROVIDER_HPP

#include <brig/boost/envelope.hpp>
#include <brig/boost/geometry.hpp>
#include <brig/gdal/detail/dataset_allocator.hpp>
#include <brig/gdal/detail/lib.hpp>
#include <brig/gdal/detail/rowset.hpp>
#include <brig/gdal/detail/transform.hpp>
#include <brig/global.hpp>
#include <brig/provider.hpp>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string>

namespace brig { namespace gdal {

class provider : public brig::provider {
  detail::dataset_allocator m_allocator;

public:
  provider(const std::string& ds) : m_allocator(ds)  {}

  std::vector<identifier> get_tables() override;
  std::vector<identifier> get_geometry_layers() override;
  std::vector<pyramid_def> get_raster_layers() override;
  table_def get_table_def(const identifier& tbl) override;

  brig::boost::box get_mbr(const table_def& tbl, const std::string& col) override;
  std::shared_ptr<rowset> select(const table_def& tbl) override;

  table_def fit_to_create(const table_def&) override  { throw std::runtime_error("GDAL error"); }
  void create(const table_def&) override  { throw std::runtime_error("GDAL error"); }
  void drop(const table_def&) override  { throw std::runtime_error("GDAL error"); }
  
  pyramid_def fit_to_reg(const pyramid_def&) override  { throw std::runtime_error("GDAL error"); }
  void reg(const pyramid_def&) override  { throw std::runtime_error("GDAL error"); }
  void unreg(const pyramid_def&) override  { throw std::runtime_error("GDAL error"); }

  std::shared_ptr<inserter> get_inserter(const table_def&) override  { throw std::runtime_error("GDAL error"); }
}; // provider

inline std::vector<identifier> provider::get_tables()
{
  using namespace std;
  using namespace detail;

  unique_ptr<dataset> ds(m_allocator.allocate());
  vector<identifier> res;
  identifier id;
  id.name = TBL();
  res.push_back(id);
  return res;
}

inline std::vector<identifier> provider::get_geometry_layers()
{
  using namespace std;
  using namespace detail;

  unique_ptr<dataset> ds(m_allocator.allocate());
  vector<identifier> res;
  identifier id;
  id.name = TBL();
  id.qualifier = WKB();
  res.push_back(id);
  return res;
}

inline std::vector<pyramid_def> provider::get_raster_layers()
{
  using namespace std;
  using namespace brig::boost;
  using namespace detail;

  unique_ptr<dataset> ds(m_allocator.allocate());
  transform tr;
  lib::check(lib::singleton().p_GDALGetGeoTransform(*ds, tr.coef));
  const box env(envelope(tr.pixel_to_proj(box(point(0, 0), point(1, 1)))));
  
  vector<pyramid_def> res;
  pyramid_def pyr;
  tiling_def lvl;
  lvl.resolution_x = env.max_corner().get<0>() - env.min_corner().get<0>();
  lvl.resolution_y = env.max_corner().get<1>() - env.min_corner().get<1>();
  lvl.geometry.name = pyr.id.name = TBL();
  lvl.geometry.qualifier = WKB();
  lvl.raster.name = pyr.id.qualifier = PNG();
  lvl.raster.type = Blob;
  pyr.levels.push_back(lvl);
  res.push_back(pyr);
  return res;
}

inline table_def provider::get_table_def(const identifier&)
{
  using namespace std;
  using namespace detail;

  unique_ptr<dataset> ds(m_allocator.allocate());
  table_def res;
  res.id.name = TBL();

  {
    column_def col;
    col.name = WKB();
    col.type = Geometry;
    auto del = [](void* ptr) { lib::singleton().p_OSRDestroySpatialReference(OGRSpatialReferenceH(ptr)); };
    unique_ptr<void, decltype(del)> srs(lib::singleton().p_OSRNewSpatialReference(lib::singleton().p_GDALGetProjectionRef(*ds)), del);
    if (!srs.get()) throw runtime_error("GDAL error");
    lib::singleton().p_OSRAutoIdentifyEPSG(srs.get());
    const char* name(lib::singleton().p_OSRGetAuthorityName(srs.get(), 0));
    const char* code(lib::singleton().p_OSRGetAuthorityCode(srs.get(), 0));
    if (name && code && string(name).compare("EPSG") == 0)
      col.epsg = atoi(code);
    else
    {
      char* proj(0);
      if (OGRERR_NONE == lib::singleton().p_OSRExportToProj4(srs.get(), &proj) && proj)
      {
        col.proj = proj;
        lib::singleton().p_OGRFree(proj);
      }
    }
    res.columns.push_back(col);
  }

  {
    column_def col;
    col.name = PNG();
    col.type = Blob;
    res.columns.push_back(col);
  }

  return res;
}

inline brig::boost::box provider::get_mbr(const table_def&, const std::string&)
{
  using namespace std;
  using namespace brig::boost;
  using namespace detail;
  unique_ptr<dataset> ds(m_allocator.allocate());
  transform tr;
  lib::check(lib::singleton().p_GDALGetGeoTransform(*ds, tr.coef));
  return envelope(tr.pixel_to_proj(box(point(0, 0), point(lib::singleton().p_GDALGetRasterXSize(*ds), lib::singleton().p_GDALGetRasterYSize(*ds)))));
}

inline std::shared_ptr<rowset> provider::select(const table_def& tbl)
{
  using namespace std;
  if (typeid(null_t) != tbl[PNG()]->query_value.type()) throw runtime_error("GDAL error");
  return make_shared<detail::rowset>(&m_allocator, tbl);
} // provider::

} } // brig::gdal

#endif // BRIG_GDAL_PROVIDER_HPP
