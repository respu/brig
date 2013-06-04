// Andrew Naplavkov

#ifndef BRIG_DATABASE_MYSQL_DETAIL_LIB_HPP
#define BRIG_DATABASE_MYSQL_DETAIL_LIB_HPP

#include <brig/detail/dynamic_loading.hpp>
#include <brig/global.hpp>
#include <mysql.h>

namespace brig { namespace database { namespace mysql { namespace detail {

class lib {
  lib();

public:
  decltype(mysql_close) *p_mysql_close;
  decltype(mysql_error) *p_mysql_error;
  decltype(mysql_fetch_field_direct) *p_mysql_fetch_field_direct;
  decltype(mysql_free_result) *p_mysql_free_result;
  decltype(mysql_get_client_info) *p_mysql_get_client_info;
  decltype(mysql_init) *p_mysql_init;
  decltype(mysql_next_result) *p_mysql_next_result;
  decltype(mysql_num_fields) *p_mysql_num_fields;
  decltype(mysql_query) *p_mysql_query;
  decltype(mysql_real_connect) *p_mysql_real_connect;
  decltype(mysql_set_character_set) *p_mysql_set_character_set;
  decltype(mysql_stmt_bind_param) *p_mysql_stmt_bind_param;
  decltype(mysql_stmt_bind_result) *p_mysql_stmt_bind_result;
  decltype(mysql_stmt_close) *p_mysql_stmt_close;
  decltype(mysql_stmt_execute) *p_mysql_stmt_execute;
  decltype(mysql_stmt_fetch) *p_mysql_stmt_fetch;
  decltype(mysql_stmt_fetch_column) *p_mysql_stmt_fetch_column;
  decltype(mysql_stmt_init) *p_mysql_stmt_init;
  decltype(mysql_stmt_prepare) *p_mysql_stmt_prepare;
  decltype(mysql_stmt_result_metadata) *p_mysql_stmt_result_metadata;
  decltype(mysql_store_result) *p_mysql_store_result;

  bool empty() const  { return p_mysql_store_result == 0; }
  static lib& singleton()  { static lib s; return s; }
}; // lib

inline lib::lib() : p_mysql_store_result(0)
{
  auto handle = BRIG_DL_LIBRARY(LibMysqlWin, LibMysqlLin);
  if (  handle
    && (p_mysql_close = BRIG_DL_FUNCTION(handle, mysql_close))
    && (p_mysql_error = BRIG_DL_FUNCTION(handle, mysql_error))
    && (p_mysql_fetch_field_direct = BRIG_DL_FUNCTION(handle, mysql_fetch_field_direct))
    && (p_mysql_free_result = BRIG_DL_FUNCTION(handle, mysql_free_result))
    && (p_mysql_get_client_info = BRIG_DL_FUNCTION(handle, mysql_get_client_info))
    && (p_mysql_init = BRIG_DL_FUNCTION(handle, mysql_init))
    && (p_mysql_next_result = BRIG_DL_FUNCTION(handle, mysql_next_result))
    && (p_mysql_num_fields = BRIG_DL_FUNCTION(handle, mysql_num_fields))
    && (p_mysql_query = BRIG_DL_FUNCTION(handle, mysql_query))
    && (p_mysql_real_connect = BRIG_DL_FUNCTION(handle, mysql_real_connect))
    && (p_mysql_set_character_set = BRIG_DL_FUNCTION(handle, mysql_set_character_set))
    && (p_mysql_stmt_bind_param = BRIG_DL_FUNCTION(handle, mysql_stmt_bind_param))
    && (p_mysql_stmt_bind_result = BRIG_DL_FUNCTION(handle, mysql_stmt_bind_result))
    && (p_mysql_stmt_close = BRIG_DL_FUNCTION(handle, mysql_stmt_close))
    && (p_mysql_stmt_execute = BRIG_DL_FUNCTION(handle, mysql_stmt_execute))
    && (p_mysql_stmt_fetch = BRIG_DL_FUNCTION(handle, mysql_stmt_fetch))
    && (p_mysql_stmt_fetch_column = BRIG_DL_FUNCTION(handle, mysql_stmt_fetch_column))
    && (p_mysql_stmt_init = BRIG_DL_FUNCTION(handle, mysql_stmt_init))
    && (p_mysql_stmt_prepare = BRIG_DL_FUNCTION(handle, mysql_stmt_prepare))
    && (p_mysql_stmt_result_metadata = BRIG_DL_FUNCTION(handle, mysql_stmt_result_metadata))
      ) p_mysql_store_result = BRIG_DL_FUNCTION(handle, mysql_store_result);
} // lib::

} } } } // brig::database::mysql::detail

#endif // BRIG_DATABASE_MYSQL_DETAIL_LIB_HPP
