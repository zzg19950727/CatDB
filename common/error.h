#ifndef ERROR_H
#define ERROR_H
#define SUCCESS					0x0
#define ERR_COL_DESC			0x1
#define ROW_ID_NOT_FOUND		0x2
#define ERR_COL_INDEX			0x3
#define ERR_ROW_DESC			0x4
#define ROW_DATA_TOO_LONG		0x5
#define NO_MORE_PAGE_FREE_SPACE	0x6
#define NO_MORE_ROWS			0x7
#define TABLE_FILE_NOT_EXISTS	0x8
#define UNKNOWN_PAGE_OFFSET		0x9
#define BAD_PAGE_IN_FILE		0xa
#define WRITE_PAGE_ERROR		0xb
#define END_OF_TABLE_SPACE		0xc
#define ROW_ID_DELETED			0xd
#define EMPTY_TABLE_SPACE		0xe
#define WRONG_CALC_FOR_OP		0xf
#define INVALID_OBJECT			0x10
#define OPERATION_NOT_SUPPORT	0x11
#define DEVISOR_IS_ZERO			0x12
#define ROW_NOT_FOUND			0x13
#define ERROR_OPERATION_IN_EUQAL_JOIN_CONDITION	0x14
#define ERR_EXPR_TYPE			0x15
#define ERR_BUCKET_IDX			0x16
#define UNKNOWN_AGG_FUNC		0x17
#define EXPR_CALC_ERR			0x18
#define ERROR_LEX_STMT			0x19
#define ERROR_COLUMN_VALUE		0x1a
#define PLAN_NOT_BUILD			0x1b
#define COLUMN_NOT_EXISTS		0x1c
#define TABLE_NOT_EXISTS		0x1d
#define TABLE_REDEFINE			0x1e
#define JOIN_TABLES_NOT_EXISTS	0x1f
#define NOT_UNIQUE_TABLE		0x20
#define NO_TABLE_FOR_SELECT		0x21
#define HAVING_ERROR			0x22
#define HAVE_ALL_COLUMN_STMT	0x23
#define ERROR_INDEX				0x24
#define SYSTEM_SCHEMA_ERROR		0x25
#define DATABASE_EXISTS			0x26
#define DATABASE_NOT_EXISTS		0x27
#define TABLE_EXISTS			0x28
#define SUBQUERY_RESULT_MUST_BE_ONE_COLUMN	0x29
#define SUBQUERY_RESULT_MUST_BE_ONE_ROW	0x2a
#define END_OF_BUCKETS			0x2b
#define SET_ROW_DESC_ERROR		0x2c
#define CAN_NOT_ANALYZE_SYSTEM	0x2d
#define CAST_DATA_ERROR			0x2e
#define CAN_NOT_REWRITE			0x2f
#define UNKNOWN_PAGE_BUFFER		0x30
#define SIZE_OVERFLOW			0x31
#define OBJ_TYPE_ERROR			0x32
#define ERR_UNEXPECTED			0x33
#define INVALID_ARGUMENT		0x34
#define INVALID_CMD_TYPE        0x35
#define IS_ALL_COLUMN_STMT	    0x36
#define MARK_ROW                0x37
#define NO_MORE_MEMORY          0x38
#define MORE_THAN_ONE_ROW       0x39
#define QB_NAME_EXISTS          0x3a
#define MAX_ERROR_CODE			0x3b

#include "type.h"
#include "log.h"
const char* err_string(u32 err_code);

#define SUCC(code)   ((code) == SUCCESS)
#define FAIL(code)   ((code) != SUCCESS)
#define CHECK(function) ret=function; \
                        if (FAIL(ret)) { \
                            LOG_ERR("catch error code", K(ret));\
                            return ret;\
                        }

bool is_false();

template<typename T, typename ...ARGS>
bool is_false(const T &value, ARGS... args)
{
    return !value || is_false(args...);
}

#define MY_ASSERT(...) do { if (is_false(__VA_ARGS__)) { \
                            ret = ERR_UNEXPECTED;\
                            LOG_ERR("param check failed", K(ret));\
                            return ret;\
                        } } while(0);

#endif	//ERROR_H