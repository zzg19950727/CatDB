#ifndef SQL_DEFINE_H
#define SQL_DEFINE_H
#include "enum_type.def"
#define COPY_DEFAULT			0x0
#define COPY_SHARE				0x1
#define COPY_ON_WRITE			0x2

//PAGE_SIZE跟row_id的结构设计相关，改大后注意是否向前兼容row_id
//详见row_id设计
#define PAGE_SIZE 16*1024

#define ENABLE_COPY_SHARE(flag)		((flag) & COPY_SHARE)
#define ENABLE_COPY_ON_WRITE(flag)	((flag) & COPY_ON_WRITE)


ENUM_DEFINE(StmtType,
			Select,
			Update,
			Insert,
			Delete,
			SetOperation,
			DoCMD
		);

ENUM_DEFINE(CMDType,
			NONE,
			CreateTable,
			DropTable,
			CreateDatabase,
			DropDatabase,
			ShowTables,
			ShowDatabases,
			DescTable,
			UseDatabase,
			Analyze,
			SetVar,
			ShowProcesslist,
			Kill,
			ShowMemory
		);

ENUM_DEFINE(StmtFlag,
			INVALID_FLAG,
			IS_COLUMN,
			HAS_COLUMN,
			IS_CONST,
			HAS_CONST,
			IS_SET_EXPR,
			HAS_SET_EXPR,
			IS_AGG,
			HAS_AGG,
			IS_SUBQUERY,
			HAS_SUBQUERY,
			IS_LIST,
			IS_OP_EXPR,
			IS_EXEC_PARAM,
			HAS_EXEC_PARAM
		);

ENUM_DEFINE(TableType,
			BasicTable, 
			JoinedTable, 
			ViewTable
		);

ENUM_DEFINE(ExprType,
			CONST,
			EXEC_PARAM,
			COLUMN,
			SET_EXPR,
			AGG_EXPR,
			SUBQUERY,
			EXPR_LIST,
			OP_EXPR
		);

ENUM_DEFINE(OperationType,
			OP_INVALID,
			OP_MINUS,
			OP_ADD,
			OP_SUB,
			OP_MUL,
			OP_DIV,
			OP_EQ,
			OP_NE,
			OP_GE,
			OP_GT,
			OP_LE,
			OP_LT,
			OP_EQ_ANY,
			OP_NE_ANY,
			OP_GE_ANY,
			OP_GT_ANY,
			OP_LE_ANY,
			OP_LT_ANY,
			OP_EQ_ALL,
			OP_NE_ALL,
			OP_GE_ALL,
			OP_GT_ALL,
			OP_LE_ALL,
			OP_LT_ALL,
			OP_BETWEEN,
			OP_NOT_BETWEEN,
			OP_IS_NULL,
			OP_IS_NOT_NULL,
			OP_IN,
			OP_NOT_IN,
			OP_IN_LIST,
			OP_NOT_IN_LIST,
			OP_EXISTS,
			OP_NOT_EXISTS,
			OP_AND,
			OP_OR,
			OP_NOT,
			OP_LIKE,
			OP_NOT_LIKE,
			OP_IFNULL,
			OP_CASE_WHEN,
			OP_CAST,
			OP_TO_CHAR,
			OP_SUBSTR,
			OP_TO_NUMBER,
			OP_ASSIGN,
			OP_DATE_ADD,
			OP_DATE_SUB,
			OP_MAX
		);

ENUM_DEFINE(AggrType,
			SUM,
			AVG,
			COUNT,
			MIN,
			MAX
		);

ENUM_DEFINE(SetOpType,
			UNION_ALL, 
			UNION, 
			INTERSECT, 
			EXCEPT
		);

ENUM_DEFINE(JoinType,
			Inner,
			LeftOuter, 
			RightOuter, 
			FullOuter, 
			LeftSemi, 
			RightSemi, 
			LeftAnti, 
			RightAnti
		);

static JoinType ReverseJoinType[] = {
			Inner,
			RightOuter,
			LeftOuter, 
			FullOuter,
			RightSemi,  
			LeftSemi, 
			RightAnti,
			LeftAnti
		};

ENUM_DEFINE(JoinAlgo,
			HASH_JOIN, 
			NL_JOIN
		);


ENUM_DEFINE(HintType,
			INVALID_HINT,
			NO_REWRITE,
			UNNEST,
			JOIN, 
			LEADING, 
			PARALLEL);


ENUM_DEFINE(LogOperatorType,
			LOG_TABLE_SCAN,
			LOG_JOIN,
			LOG_SORT,
			LOG_DISTINCT,
			LOG_GROUP_BY,
			LOG_SCALAR_GROUP,
			LOG_SET,
			LOG_LIMIT,
			LOG_VIEW,
			LOG_INSERT,
			LOG_UPDATE,
			LOG_DELETE,
			LOG_EXPR_VALUE,
			LOG_DUAL_TABLE,
			LOG_SUBQUERY_EVALUATE
		);

ENUM_DEFINE(TimeType,
			TIME, 
			DATE, 
			DATETIME, 
			TIMESTAMP
			);
enum OBJ_TYPE {T_NUMBER = 0, T_VARCHAR, T_DATETIME, T_BOOL, T_MAX_TYPE};
static const char* obj_type_str[] = {"NUMBER", "VARCHAR", "DATETIME", "BOOL", "INVALID_TYPE"};
#endif //SQL_DEFINE_H