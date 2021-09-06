#ifndef SQL_DEFINE_H
#define SQL_DEFINE_H

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
#endif //SQL_DEFINE_H