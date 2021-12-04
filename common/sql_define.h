#ifndef SQL_DEFINE_H
#define SQL_DEFINE_H
#define COPY_DEFAULT			0x1
#define COPY_SHARE				0x2
#define COPY_ON_WRITE			0x3

//PAGE_SIZE跟row_id的结构设计相关，改大后注意是否向前兼容row_id
//详见row_id设计
#define PAGE_SIZE 16*1024

#define ENABLE_COPY_SHARE(flag)		((flag) & COPY_SHARE)
#define ENABLE_COPY_ON_WRITE(flag)	((flag) & COPY_ON_WRITE)

enum StmtFlag {
	INVALID_FLAG = 0,
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
};

enum ExprType {
	CONST = 0,
	EXEC_PARAM,
	COLUMN,
	SET_EXPR,
	AGG_EXPR,
	SUBQUERY,
	EXPR_LIST,
	OP_EXPR
};

enum OperationType {
				OP_INVALID = 0,
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
				OP_BETWEEN,
				OP_NOT_BETWEEN,
				OP_IS_NULL,
				OP_IS_NOT_NULL,
				OP_IN,
				OP_NOT_IN,
				OP_EXISTS,
				OP_NOT_EXISTS,
				OP_AND,
				OP_OR,
				OP_NOT,
				OP_LIKE,
				OP_NOT_LIKE,
				OP_IFNULL,
				OP_CASE_WHEN,
				OP_CAST
			};

enum AggrType {
				SUM = 0,
				AVG,
				COUNT,
				MIN,
				MAX
			};

enum SetOpType {UNION_ALL = 0, UNION, INTERSECT, EXCEPT};

static const char* SetOpTypeString[] = {"UNION_ALL", "UNION", "INTERSECT", "EXCEPT"};

enum JoinType {Inner = 0,
			   LeftOuter, 
			   RightOuter, 
			   FullOuter, 
			   LeftSemi, 
			   RightSemi, 
			   LeftAnti, 
			   RightAnti};

static JoinType ReveriseJoinType[] = {Inner,
								RightOuter, 
								LeftOuter, 
								FullOuter, 
								RightSemi, 
								LeftSemi, 
								RightAnti, 
								LeftAnti};

static JoinType ReverseJoinType[] = {Inner,
			   RightOuter,
			   LeftOuter, 
			   FullOuter,
			   RightSemi,  
			   LeftSemi, 
			   RightAnti,
			   LeftAnti};

static const char* JoinTypeString[] = {
				"Inner",
				"LeftOuter",
				"RightOuter",
				"FullOuter",
				"LeftSemi",
				"RightSemi",
				"LeftAnti",
				"RightAnti"};
				
enum JoinAlgo {HASH_JOIN, NL_JOIN};

static const char* JoinAlgoString[] = {
				"HASH_JOIN",
				"NL_JOIN"};

#endif //SQL_DEFINE_H