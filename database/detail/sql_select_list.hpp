// Andrew Naplavkov

#ifndef BRIG_DATABASE_DETAIL_SQL_SELECT_LIST_HPP
#define BRIG_DATABASE_DETAIL_SQL_SELECT_LIST_HPP

#include <brig/database/command.hpp>
#include <brig/database/detail/dialect.hpp>
#include <brig/column_def.hpp>
#include <iterator>
#include <string>
#include <vector>

namespace brig { namespace database { namespace detail {

inline std::string sql_select_list(dialect* dct, command* cmd, const std::vector<column_def>& cols)
{
  using namespace std;

  string sql;
  for (auto col(begin(cols)); col != end(cols); ++col)
  {
    if (col != begin(cols)) sql += ", ";
    sql += dct->sql_column(cmd, *col);
  }
  return sql;
}

} } } // brig::database::detail

#endif // BRIG_DATABASE_DETAIL_SQL_SELECT_LIST_HPP
