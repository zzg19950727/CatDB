#include "stmt.h"
using namespace CatDB::Parser;

ColumnDefineStmt::ColumnDefineStmt()
{

}

ColumnDefineStmt::~ColumnDefineStmt()
{

}

ColumnDefineStmt_s ColumnDefineStmt::make_column_define_stmt(const String& name, const DataType &data_type)
{
	ColumnDefineStmt *stmt = new ColumnDefineStmt();
	stmt->column_name = name;
	stmt->data_type = data_type;
	return ColumnDefineStmt_s(stmt);
}

Stmt::Stmt()
{
}

Stmt::~Stmt()
{
}

bool Stmt::is_select_stmt() const 
{ 
    return stmt_type() == Select || SetOperation == stmt_type(); 
}