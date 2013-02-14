// Andrew Naplavkov

#ifndef BRIG_DATABASE_ODBC_DETAIL_GET_DATA_FACTORY_HPP
#define BRIG_DATABASE_ODBC_DETAIL_GET_DATA_FACTORY_HPP

#include <brig/database/odbc/detail/get_data.hpp>
#include <brig/database/odbc/detail/get_data_blob.hpp>
#include <brig/database/odbc/detail/get_data_impl.hpp>
#include <brig/database/odbc/detail/get_data_string.hpp>
#include <brig/database/odbc/detail/lib.hpp>
#include <cstdint>
#include <stdexcept>

namespace brig { namespace database { namespace odbc { namespace detail {

inline get_data* get_data_factory(SQLSMALLINT sql_type)
{
  switch (sql_type)
  {
  // numeric
  case SQL_BIT:
  case SQL_TINYINT:
  case SQL_SMALLINT: return new get_data_impl<int16_t, SQL_C_SSHORT>();
  case SQL_INTEGER: return new get_data_impl<int32_t, SQL_C_SLONG>();
  case SQL_INFX_BIGINT:
  case SQL_BIGINT: return new get_data_impl<int64_t, SQL_C_SBIGINT>();
  case SQL_REAL: return new get_data_impl<float, SQL_C_FLOAT>();
  case SQL_DECIMAL:
  case SQL_DOUBLE:
  case SQL_FLOAT:
  case SQL_NUMERIC: return new get_data_impl<double, SQL_C_DOUBLE>();

  // string
  case SQL_DATE:
  case SQL_TIME:
  case SQL_TIMESTAMP:
  case SQL_TYPE_DATE:
  case SQL_TYPE_TIME:
  case SQL_TYPE_TIMESTAMP:
  
  case SQL_CHAR:
  case SQL_VARCHAR:
  case SQL_LONGVARCHAR:
  case SQL_WCHAR:
  case SQL_WVARCHAR:
  case SQL_WLONGVARCHAR:
  case SQL_DB2_UNICODE_CHAR:
  case SQL_DB2_UNICODE_VARCHAR:
  case SQL_DB2_UNICODE_LONGVARCHAR:
  case SQL_DB2_CLOB: return new get_data_string();

  // blob
  case SQL_BINARY:
  case SQL_VARBINARY:
  case SQL_LONGVARBINARY:
  case SQL_DB2_BLOB:
  case SQL_INFX_UDT_VARYING: return new get_data_blob();

  // SQL_UNKNOWN_TYPE
  default: throw std::runtime_error("ODBC type error");
  }
} // get_data_factory

} } } } // brig::database::odbc::detail

#endif // BRIG_DATABASE_ODBC_DETAIL_GET_DATA_FACTORY_HPP
