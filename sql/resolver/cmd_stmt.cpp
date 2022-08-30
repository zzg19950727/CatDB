#include "cmd_stmt.h"
#include "error.h"

using namespace CatDB::Parser;
using namespace CatDB::Common;

CreateTableParam_s CreateTableParam::make_create_table_param()
{
	return CreateTableParam_s(new CreateTableParam);
}

DropTableParam_s DropTableParam::make_drop_table_param()
{
	return DropTableParam_s(new DropTableParam);
}

CreateDBParam_s CreateDBParam::make_create_DB_param()
{
	return CreateDBParam_s(new CreateDBParam);
}

DropDBParam_s DropDBParam::make_drop_DB_param()
{
	return DropDBParam_s(new DropDBParam);
}

ShowTablesParam_s ShowTablesParam::make_show_tables_param()
{
	return ShowTablesParam_s(new ShowTablesParam);
}

DescTableParam_s DescTableParam::make_desc_table_param()
{
	return DescTableParam_s(new DescTableParam);
}

UseDBParam_s UseDBParam::make_use_DB_param()
{
	return UseDBParam_s(new UseDBParam);
}

KillSessionParam_s KillSessionParam::make_kill_session_param()
{
	return KillSessionParam_s(new KillSessionParam);
}

CreateViewParam_s CreateViewParam::make_create_view_param()
{
	return CreateViewParam_s(new CreateViewParam);
}

DropViewParam_s DropViewParam::make_drop_view_param()
{
	return DropViewParam_s(new DropViewParam);
}

FunctionDefinition_s FunctionDefinition::make_func_define()
{
	return FunctionDefinition_s(new FunctionDefinition);
}

CreatePackageParam_s CreatePackageParam::make_create_package_param()
{
	return CreatePackageParam_s(new CreatePackageParam);
}

ExecFunctionParam_s ExecFunctionParam::make_exec_func_param()
{
	return ExecFunctionParam_s(new ExecFunctionParam);
}

CMDStmt::CMDStmt()
{
    cmd_type = NONE;
}

CMDStmt::~CMDStmt()
{
}

StmtType CMDStmt::stmt_type() const
{
	return DoCMD;
}

Stmt_s CMDStmt::make_cmd_stmt(CMDType cmd_type)
{
	CMDStmt* stmt = new CMDStmt();
	stmt->cmd_type = cmd_type;
	return Stmt_s(stmt);
}

u32 CMDStmt::formalize()
{
	return SUCCESS;
}