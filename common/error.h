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

#include "type.h"
const char* err_string(u32 err_code);
#endif	//ERROR_H