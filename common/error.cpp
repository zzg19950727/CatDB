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
			return "row data too long";
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
		default:
			return "";
	}
}
