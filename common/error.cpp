#include "error.h"

const char * err_string(u32 err_code)
{
	switch (err_code)
	{
	case SUCCESS:
		return "success";
		case ERR_COL_DESC:
			return "column id error";
		case ROW_ID_NOT_FOUND:
			return "row id not found";
		case ERR_COL_INDEX:
			return "column index error";
		case ERR_ROW_DESC:
			return "row desc error";
		case ROW_DATA_TOO_LONG:
			return "update none fix data type not support yet";
		case NO_MORE_PAGE_FREE_SPACE:
			return "no more page free space";
		case NO_MORE_ROWS:
			return "no more rows";
		case TABLE_FILE_NOT_EXISTS:
			return "table file not exists";
		case UNKNOWN_PAGE_OFFSET:
			return "unknown page offset";
		case BAD_PAGE_IN_FILE:
			return "bad page in table file";
		case WRITE_PAGE_ERROR:
			return "write page error";
		case END_OF_TABLE_SPACE:
			return "end of table space";
		case ROW_ID_DELETED:
			return "row id have deleted";
		case EMPTY_TABLE_SPACE:
			return "table space is empty";
		case WRONG_CALC_FOR_OP:
			return "unknown operation";
		case INVALID_OBJECT:
			return "invalid object";
		case OPERATION_NOT_SUPPORT:
			return "operation not support";
		case DEVISOR_IS_ZERO:
			return "devisor is zero";
		case ROW_NOT_FOUND:
			return "row not found";
		case ERROR_OPERATION_IN_EUQAL_JOIN_CONDITION:
			return "error operation in equal join condition";
		case ERR_EXPR_TYPE:
			return "error expression type";
		case ERR_BUCKET_IDX:
			return "error bucket index in hash table";
		case UNKNOWN_AGG_FUNC:
			return "unknown aggregate function";
		case EXPR_CALC_ERR:
			return "expression calculate error";
		case ERROR_LEX_STMT:
			return "error lex statement";
		case ERROR_COLUMN_VALUE:
			return "error column value";
		case PLAN_NOT_BUILD:
			return "plan not build yet";
		case COLUMN_NOT_EXISTS:
			return "column not exists";
		case TABLE_NOT_EXISTS:
			return "table not exists";
		case TABLE_REDEFINE:
			return "many table have this column";
		case JOIN_TABLES_NOT_EXISTS:
			return "join tables not exists";
		case NOT_UNIQUE_TABLE:
			return "not unique table alias";
		case NO_TABLE_FOR_SELECT:
			return "there is no table for select";
		case HAVING_ERROR:
			return "having stmt error";
		case HAVE_ALL_COLUMN_STMT:
			return "have * column stmt";
		case ERROR_INDEX:
			return "error index";
		case SYSTEM_SCHEMA_ERROR:
			return "schema check error";
		case DATABASE_EXISTS:
			return "database exists";
		case DATABASE_NOT_EXISTS:
			return "database not exists";
		case TABLE_EXISTS:
			return "table exists"; 
		case SUBQUERY_RESULT_MUST_BE_ONE_COLUMN:
			return "subquery`s result must be one column";
		case SUBQUERY_RESULT_MUST_BE_ONE_ROW:
			return "subquery`s result must be one row";
		case END_OF_BUCKETS:
			return "end of hash table buckets";
		case SET_ROW_DESC_ERROR:
			return "row desc error in set operation";
		case CAN_NOT_ANALYZE_SYSTEM:
			return "can not analyze system database";
		case CAST_DATA_ERROR:
			return "cast data error";
		case CAN_NOT_REWRITE:
			return "can not rewrite sql";
		case UNKNOWN_PAGE_BUFFER:
			return "unknown page buffer";
		case SIZE_OVERFLOW:
			return "size overflow";
		case OBJ_TYPE_ERROR:
			return "object type error";
		case ERR_UNEXPECTED:
			return "error unexpect";
		case INVALID_ARGUMENT:
			return "invalid argument";
		case INVALID_CMD_TYPE:
			return "invalid cmd type";
		case IS_ALL_COLUMN_STMT	:
			return "is all column stmt";
		case MARK_ROW:
			return "mark row";
		case NO_MORE_MEMORY:
			return "no more memory";
		case MORE_THAN_ONE_ROW:
			return "subquery output more than one row";
		case QB_NAME_EXISTS:
			return "qb name exists";
		case QUERY_KILLED:
			return "query is killed";
		default:
			return "error";
	}
}

bool is_false()
{
    return false;
}