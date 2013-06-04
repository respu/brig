// Andrew Naplavkov

#ifndef BRIG_DATABASE_ORACLE_DETAIL_BINDING_FACTORY_HPP
#define BRIG_DATABASE_ORACLE_DETAIL_BINDING_FACTORY_HPP

#include <brig/column_def.hpp>
#include <brig/database/oracle/detail/binding.hpp>
#include <brig/database/oracle/detail/binding_blob.hpp>
#include <brig/database/oracle/detail/binding_geometry.hpp>
#include <brig/database/oracle/detail/binding_impl.hpp>
#include <brig/database/oracle/detail/binding_string.hpp>
#include <brig/database/oracle/detail/get_charset_form.hpp>
#include <brig/database/oracle/detail/handles.hpp>
#include <brig/variant.hpp>
#include <stdexcept>

namespace brig { namespace database { namespace oracle { namespace detail {

struct binding_visitor : ::boost::static_visitor<binding*> {
  handles* hnd;
  size_t i;
  const column_def& param;
  binding_visitor(handles* hnd_, size_t i_, const column_def& param_) : hnd(hnd_), i(i_), param(param_)  {}
  binding* operator()(const null_t&) const;
  binding* operator()(int16_t v) const  { return new binding_impl<int16_t, SQLT_INT>(hnd, i, v); }
  binding* operator()(int32_t v) const  { return new binding_impl<int32_t, SQLT_INT>(hnd, i, v); }
  binding* operator()(int64_t v) const  { return new binding_impl<int64_t, SQLT_INT>(hnd, i, v); }
  binding* operator()(float v) const  { return new binding_impl<float, SQLT_FLT>(hnd, i, v); }
  binding* operator()(double v) const  { return new binding_impl<double, SQLT_FLT>(hnd, i, v); }
  binding* operator()(const std::string& r) const  { return new binding_string(hnd, i, r, get_charset_form(param.type_lcase)); }
  binding* operator()(const blob_t&) const;
}; // binding_visitor

inline binding* binding_visitor::operator()(const null_t&) const
{
  switch (param.type)
  {
    default: throw std::runtime_error("OCI type error");
    case column_type::Blob: return new binding_blob(hnd, i, 0, 0);
    case column_type::Double: return new binding_impl<double, SQLT_FLT>(hnd, i);
    case column_type::Geometry: return new binding_geometry(hnd, i, blob_t(), param.srid);
    case column_type::Integer: return new binding_impl<int64_t, SQLT_INT>(hnd, i);
    case column_type::String: return new binding_string(hnd, i, std::string(), get_charset_form(param.type_lcase));
  };
}

inline binding* binding_visitor::operator()(const blob_t& r) const
{
  if (column_type::Geometry == param.type)
    return new binding_geometry(hnd, i, r, param.srid);
  else
    return new binding_blob(hnd, i, (void*)r.data(), ub4(r.size()));
} // binding_visitor::

inline binding* binding_factory(handles* hnd, size_t order, const column_def& param)
{
  return ::boost::apply_visitor(binding_visitor(hnd, order + 1, param), param.query_value);
}

} } } } // brig::database::oracle::detail

#endif // BRIG_DATABASE_ORACLE_DETAIL_BINDING_FACTORY_HPP
