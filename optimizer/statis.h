#ifndef STATIS_H
#define STATIS_H
#include "type.h"
namespace CatDB {
	namespace Parser {
		DECLARE(TableStmt);
	}
	namespace Sql {
		DECLARE(Expression);
		using Parser::TableStmt;
		u32 get_table_row_count(const Vector<TableStmt*>& table_list);
		u32 cal_table_select_rows(const Vector<TableStmt*>& table_list, 
								const HashMap<TableStmt*, Expression_s>& table_filters);
	}
}

#endif	//STATIS_H