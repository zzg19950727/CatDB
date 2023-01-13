/*CatDB::SqlParser::make_XXX(loc)给token添加前缀*/
%define api.token.prefix {TOKEN_}

//使得类型与token定义可以使用各种复杂的结构与类型
%define api.value.type variant

/*定义parser传给scanner的参数*/
%lex-param { CatDB::SqlScanner& scanner }
%lex-param { CatDB::SqlDriver& driver }

/*定义driver传给parser的参数*/
%parse-param { CatDB::SqlScanner& scanner }
%parse-param { CatDB::SqlDriver& driver }

//声明命名空间
%define api.namespace {CatDB} 

%define api.token.constructor

/*详细显示错误信息*/
%define parse.error verbose

//开启断言功能
%define parse.assert  

%skeleton "lalr1.cc"
%locations
%defines

%code requires
{
	#define YYDEBUG 1
	#define SqlParser parser 
	#include "select_stmt.h"
	#include "insert_stmt.h"
	#include "update_stmt.h"
	#include "delete_stmt.h"
	#include "table_stmt.h"
	#include "expr_stmt.h"
	#include "dml_stmt.h"
	#include "cmd_stmt.h"
	#include "stmt.h"
	#include "obj_datetime.h"
	#include "obj_varchar.h"
	#include "obj_number.h"
	#include "object.h"
	#include "type.h"

	/*避免包含头文件时冲突*/
	namespace CatDB {
		class SqlScanner;
		class SqlDriver;
	}
	using namespace CatDB::Parser;
}

%code top
{
	#include "sql_parser.hpp"
	#include "sql_scanner.h"
	#include "sql_driver.h"
	#include "location.hh"

	#include "select_stmt.h"
	#include "insert_stmt.h"
	#include "update_stmt.h"
	#include "delete_stmt.h"
    #include "table_stmt.h"
	#include "expr_stmt.h"
	#include "dml_stmt.h"
	#include "cmd_stmt.h"
	#include "object.h"
	
	/*注意：这里的参数由%parse-param决定*/
	static CatDB::SqlParser::symbol_type yylex(CatDB::SqlScanner& scanner,
											   CatDB::SqlDriver &driver)
	{
		return scanner.get_next_token();
	}
	using namespace CatDB;
	using namespace CatDB::Parser;
	using namespace CatDB::Common;
}

%{
	
#define yyerror(fmt, ...) 				\
{ 										\
	char tmp[255]; 						\
	sprintf(tmp, fmt, ##__VA_ARGS__); 	\
	driver.set_sys_error(tmp); 			\
}
 
#define check(stmt) 				\
{ 									\
	if(!stmt) 						\
	{ 								\
		yyerror("make stmt error"); \
		YYABORT; 					\
	} 								\
}

#define make_unary_stmt(stmt, stmt1, op) 		\
{ 												\
	stmt = OpExprStmt::make_op_expr_stmt(op); 	\
	check(stmt); 								\
	stmt->params.push_back(stmt1); 				\
}

#define make_binary_stmt(stmt, stmt1, stmt2, op) 	\
{					 								\
	stmt = OpExprStmt::make_op_expr_stmt(op); 		\
	check(stmt); 									\
	stmt->params.push_back(stmt1); 					\
	stmt->params.push_back(stmt2); 					\
}

#define make_ternary_stmt(stmt, stmt1, stmt2, stmt3, op) 	\
{ 															\
	stmt = OpExprStmt::make_op_expr_stmt(op); 				\
	check(stmt); 											\
	stmt->params.push_back(stmt1); 							\
	stmt->params.push_back(stmt2); 							\
	stmt->params.push_back(stmt3); 							\
}
	
#define str_to_lower(str) 					\
{											\
	for(u32 i = 0; i<str.size(); ++i){		\
		if(str[i] >= 'A' && str[i] <= 'Z'){	\
			str[i] -= 'A';					\
			str[i] += 'a';					\
		}									\
	}										\
}

%}

%left	CMP_LE CMP_LT CMP_EQ CMP_GT CMP_GE CMP_NE
%left	UNION EXCEPT
%left	INTERSECT
%left 	UMINUS
%left	LIKE
%right	NOT
%left	OR
%left	AND
%left 	"+" "-"
%left 	"*" "/"
%left 	"(" ")"
%left 	"."

%nonassoc IS NULLX BOOL
%nonassoc BETWEEN
%nonassoc IN

%token<std::string>	STRING
%token<std::string>	IDENT
%token<std::string>	NUMERIC
%token<std::string>	QB_NAME_IDENT

%token ALL
%token AND
%token ANY
%token AS
%token ASC
%token AVG
%token BEGIN_HINT
%token BEGIN_OUTLINE_DATA
%token BETWEEN
%token BIGINT
%token BINARY
%token BOOL
%token BY
%token CALL
%token CASE
%token CHAR
%token CMP_EQ
%token CMP_GE
%token CMP_GT
%token CMP_LE
%token CMP_LT
%token CMP_NE
%token COLUMN
%token COLUMNS
%token COMMA ","
%token COUNT
%token CREATE
%token CSV
%token DATABASE
%token DATABASES
%token DATE
%token DATETIME
%token DAY
%token DECIMAL
%token DELETE
%token DENSE_RANK
%token DESC
%token DESCRIBE
%token DISTINCT
%token DIV "/"
%token DOUBLE
%token DROP
%token DUAL
%token ELSE
%token END_HINT
%token END_OUTLINE_DATA
%token END_SYM
%token ENGINE
%token EXCEPT
%token EXEC
%token EXISTS
%token EXPLAIN
%token EXPR_NORMALIZE
%token FIELD
%token FLOAT
%token FROM
%token FULL
%token FUNCTION
%token GROUP
%token HAVING
%token IF
%token IFNULL
%token IN
%token INDEX
%token INFILE
%token INNER
%token INSERT
%token INT
%token INTEGER
%token INTERSECT
%token INTERVAL
%token INTO
%token IS
%token JOIN
%token KILL
%token LEADING
%token LEFT
%token LIKE
%token LIMIT
%token LINE
%token LP "("
%token MAX
%token MEDIUMINT
%token MEMORY
%token MERGE
%token MIN
%token MINUS "-"
%token MONTH
%token MUL "*"
%token NOT
%token NO_EXPR_NORMALIZE
%token NO_MERGE
%token NO_REWRITE
%token NO_SIMPLIFY_SQ
%token NO_UNNEST
%token NO_USE_HASH
%token NO_USE_NL
%token NO_WIN_MAGIC
%token NULLX
%token NUMBER
%token NUMERIC_SYM
%token ON
%token OR
%token ORDER
%token ORDERED
%token OUTER
%token OVER
%token PACKAGE
%token PARALLEL
%token PARTITION
%token PERIOD "."
%token PLUS "+"
%token PROCEDURE
%token PROCESSLIST
%token RANK
%token REAL
%token REPLACE
%token RETURN
%token RIGHT
%token ROWID
%token ROW_NUMBER
%token RP ")"
%token SAMPLE
%token SELECT
%token SEMICOLON ";"
%token SET
%token SHOW
%token SIMPLIFY_SQ
%token SIZE
%token SMALLINT
%token SPLIT
%token STATIS
%token STATUS
%token SUBSTR
%token SUM
%token TABLE
%token TABLES
%token THEN
%token TIME
%token TIMESTAMP_SYM
%token TINYINT
%token TO_CHAR
%token TO_NUMBER
%token UNION
%token UNNEST
%token UPDATE
%token USE_HASH
%token USE_NL
%token USING
%token VALUES
%token VARBINARY
%token VARCHAR
%token VIEW
%token WHEN
%token WHERE
%token WIN_MAGIC
%token YEAR
%token END 0

%type<ColumnDefineStmt_s>			column_definition param
%type<BasicTableStmt_s> 			relation_factor
%type<LeadingTable_s> 				leading_hint_table
%type<LimitStmt_s>					opt_select_limit
%type<TableStmt_s>					table_factor sub_table_factor basic_table_factor view_table_factor joined_table_factor
%type<HintStmt_s> 					single_hint
%type<ExprStmt_s>					projection simple_expr arith_expr cmp_expr logical_expr column_ref 
									expr_const func_expr query_ref_expr update_asgn_factor case_when_expr
									seconds_expr order_by opt_arith_expr
%type<Stmt_s>						sql_stmt stmt cmd_stmt select_stmt insert_stmt update_stmt 
									delete_stmt explain_stmt explainable_stmt 
									 
%type<Stmt_s>						select_with_parens simple_select set_select sub_set_select
%type<Stmt_s>						show_stmt create_stmt drop_stmt desc_stmt use_stmt 
									kill_stmt create_package_stmt exec_package_stmt
%type<Hint> 						opt_hint

%type<Vector<Vector<ExprStmt_s>>>	insert_value_list
%type<Vector<FunctionDefinition_s>>	pro_or_func_list
%type<Vector<ColumnDefineStmt_s>>	table_element_list param_list opt_param_list table_type
%type<Vector<LeadingTable_s>> 		leading_hint_table_list
%type<Vector<TableStmt_s>>			from_list 
%type<Vector<ExprStmt_s>>			select_expr_list opt_groupby arith_expr_list opt_where opt_having insert_value update_asgn_list 
									when_then_list1 when_then_list2 opt_order_by order_by_list opt_partition_by opt_arith_expr_list
%type<Vector<HintStmt_s>> 			opt_hint_list hint_list
%type<Vector<String>> 				hint_table_list opt_engine_def opt_view_column_define view_column_define

%type<FunctionDefinition_s>			pro_or_func procedure_define function_define
%type<OperationType> 				cmp_type sq_cmp_type
%type<DataType>						data_type
%type<WinType>						win_type

%type<std::string>					op_from_database column_label database_name relation_name opt_alias column_name 
									ident string datetime number opt_qb_name opt_qb_name_single beg_view_define
									package_name procedure_name function_name param_name
%type<bool>							opt_distinct opt_asc_desc distinct_or_all opt_if_exists opt_split opt_outer opt_not_null
									opt_replace start_exec_package
%type<int>							limit_expr int_value opt_char_length opt_time_precision

%start sql_stmt
%%

sql_stmt:
	stmt ";"
    {
		driver.result = $1;
		YYACCEPT;
    }
	;

stmt:
  cmd_stmt				{ $$ = $1; }
  | select_stmt			{ $$ = $1; }
  | insert_stmt			{ $$ = $1; }
  | update_stmt			{ $$ = $1; }
  | delete_stmt			{ $$ = $1; }
  | explain_stmt		{ $$ = $1; }
  | /*EMPTY*/			{ yyerror("unknow SQL"); }
	;

/**************************************************************
 *
 *	select define
 *
 **************************************************************/
cmd_stmt:
	show_stmt				{ $$ = $1; }
	| create_stmt			{ $$ = $1; }
	| drop_stmt				{ $$ = $1; }
	| desc_stmt				{ $$ = $1; }
	| use_stmt				{ $$ = $1; }
	| kill_stmt				{ $$ = $1; }
	| create_package_stmt	{ $$ = $1; }
	| exec_package_stmt		{ $$ = $1; }
	;

select_stmt:
	simple_select
	{
		$$ = $1;
	}
	| set_select
	{
		$$ = $1;
	}
	;

set_select:
	sub_set_select UNION sub_set_select
    {
		//构建union二元表达式
		$$ = SetStmt::make_set_stmt($1, $3, UNION);
    }
	| set_select UNION sub_set_select
    {
		//构建union二元表达式
		$$ = SetStmt::make_set_stmt($1, $3, UNION);
    }
  	| sub_set_select UNION ALL sub_set_select
    {
		//构建union all二元表达式
		$$ = SetStmt::make_set_stmt($1, $4, UNION_ALL);
    }
	| set_select UNION ALL sub_set_select
    {
		//构建union all二元表达式
		$$ = SetStmt::make_set_stmt($1, $4, UNION_ALL);
    }
  	| sub_set_select INTERSECT sub_set_select
    {
		//构建intersect二元表达式
		$$ = SetStmt::make_set_stmt($1, $3, INTERSECT);
    }
	| set_select INTERSECT sub_set_select
    {
		//构建intersect二元表达式
		$$ = SetStmt::make_set_stmt($1, $3, INTERSECT);
    }
  	| sub_set_select EXCEPT sub_set_select
    {
		//构建except二元表达式
		$$ = SetStmt::make_set_stmt($1, $3, EXCEPT);
    }
	| set_select EXCEPT sub_set_select
    {
		//构建except二元表达式
		$$ = SetStmt::make_set_stmt($1, $3, EXCEPT);
    }
	;

sub_set_select:
	simple_select
	{
		$$ = $1;
	}
	| select_with_parens
	{
		$$ = $1;
	}
	;

select_with_parens:
	"(" select_stmt ")"		
	{
		$$ = $2;
	}
	;

simple_select:
	SELECT opt_hint opt_distinct select_expr_list
    FROM from_list
    opt_where opt_groupby opt_having
    opt_order_by opt_select_limit
    {
		//构建select stmt
		SelectStmt_s select_stmt = SelectStmt::make_select_stmt();
		select_stmt->stmt_hint = $2;
		select_stmt->is_distinct = $3;
		select_stmt->select_expr_list = $4;
		select_stmt->from_stmts = $6;
		select_stmt->where_stmt = $7;
		select_stmt->group_exprs = $8;
		select_stmt->having_stmt = $9;
		select_stmt->order_exprs = $10;
		select_stmt->limit_stmt = $11;
		$$ = select_stmt;
    }
	;

opt_hint:
	/*empty*/ { $$ = Hint(); }
	| BEGIN_HINT opt_hint_list END_HINT
	{
		$$ = Hint();
		$$.all_hints = $2;
		$$.is_outline = false;
	}
	| BEGIN_HINT BEGIN_OUTLINE_DATA opt_hint_list END_OUTLINE_DATA END_HINT
	{
		$$ = Hint();
		$$.all_hints = $3;
		$$.is_outline = true;
	}
	;

opt_hint_list:
	/*empty*/ 
	{
		$$ = Vector<HintStmt_s>();
	}
	| hint_list
	{
		$$ = $1;
	}
	;

hint_list:
	single_hint
	{
		$$ = Vector<HintStmt_s>();
		if ($1) {
			$$.push_back($1);
		}
	}
	| hint_list single_hint
	{
		$$ = $1;
		if ($2) {
			$$.push_back($2);
		}
	}
	;

single_hint:
/*transformer hint*/
	NO_REWRITE opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(NO_REWRITE);
		$$->set_qb_name($2);
	}
	| UNNEST opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(UNNEST, true);
		$$->set_qb_name($2);
	}
	| NO_UNNEST opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(UNNEST, false);
		$$->set_qb_name($2);
	}
	| MERGE opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(MERGE, true);
		$$->set_qb_name($2);
	}
	| NO_MERGE opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(MERGE, false);
		$$->set_qb_name($2);
	}
	| SIMPLIFY_SQ opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(SIMPLIFY_SQ, true);
		$$->set_qb_name($2);
	}
	| NO_SIMPLIFY_SQ opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(SIMPLIFY_SQ, false);
		$$->set_qb_name($2);
	}
	| EXPR_NORMALIZE opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(EXPR_NORMALIZE, true);
		$$->set_qb_name($2);
	}
	| NO_EXPR_NORMALIZE opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(EXPR_NORMALIZE, false);
		$$->set_qb_name($2);
	}
	| WIN_MAGIC  "(" QB_NAME_IDENT opt_split QB_NAME_IDENT ")"
	{
		$$ = HintStmt::make_hint_stmt(WIN_MAGIC, true);
		$$->set_qb_name($3);
		WinMagicHintStmt_s win_magic_hint = $$;
		win_magic_hint->dst_qb_name = $5;
	}
	| NO_WIN_MAGIC opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(WIN_MAGIC, false);
		$$->set_qb_name($2);
	}
/*optiizer hint*/
	| USE_HASH "(" opt_qb_name hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(USE_JOIN, true);
		$$->set_qb_name($3);
		JoinHintStmt_s join_hint = $$;
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = $4;
	}
	| NO_USE_HASH "(" opt_qb_name hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(USE_JOIN, false);
		$$->set_qb_name($3);
		JoinHintStmt_s join_hint = $$;
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = $4;
	}
	| USE_NL "(" opt_qb_name hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(USE_JOIN, true);
		$$->set_qb_name($3);
		JoinHintStmt_s join_hint = $$;
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = $4;
	}
	| NO_USE_NL "(" opt_qb_name hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(USE_JOIN, false);
		$$->set_qb_name($3);
		JoinHintStmt_s join_hint = $$;
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = $4;
	}
	| ORDERED opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(LEADING);
		$$->set_qb_name($2);
		LeadingHintStmt_s leading = $$;
		leading->set_is_ordered();
	}
	| LEADING "(" opt_qb_name leading_hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(LEADING);
		$$->set_qb_name($3);
		LeadingHintStmt_s leading = $$;
		leading->tables = LeadingTable::make_leading_table();
		leading->tables->is_base_table = false;
		leading->tables->table_list = $4;
	}
/*global hint*/
	| PARALLEL "(" int_value ")"
	{
		$$ = HintStmt::make_hint_stmt(PARALLEL);
		ParallelHintStmt_s hint = $$;
		hint->parallel = $3;
	}
	;

opt_qb_name:
	/*empty*/ { $$ = ""; }
	| QB_NAME_IDENT
	{
		$$ = $1;
	}
	;

opt_qb_name_single:
	/*empty*/ { $$ = ""; }
	| "(" ")"
	{
		$$ = "";
	}
	| "(" QB_NAME_IDENT ")"
	{
		$$ = $2;
	}
	;

hint_table_list:
	ident
	{
		$$ = Vector<String>();
		$$.push_back($1);
	}
	| hint_table_list opt_split ident
	{
		$$ = $1;
		$$.push_back($3);
	}
	;

opt_split:
	/*empty*/ {}
	| "," {}
	;

leading_hint_table:
	ident
	{
		$$ = LeadingTable::make_leading_table();
		$$->is_base_table = true;
		$$->table_name = $1;
	}
	| "(" leading_hint_table_list ")"
	{
		$$ = LeadingTable::make_leading_table();
		$$->is_base_table = false;
		$$->table_list = $2;
	}
	;

leading_hint_table_list:
	leading_hint_table
	{
		$$ = Vector<LeadingTable_s>();
		$$.push_back($1);
	}
	| leading_hint_table_list opt_split leading_hint_table
	{
		$$ = $1;
		$$.push_back($3);
	}
	;

opt_distinct:
    /*empty*/		{ $$ = false; }
  | DISTINCT		{ $$ = true; }
  ;

select_expr_list:
    projection
    {
		$$ = Vector<ExprStmt_s>();
		$$.push_back($1);
    }
  | select_expr_list "," projection
    {
		$1.push_back($3);
		$$ = $1;
    }
  ;

from_list:
  	table_factor
    {
		$$ = Vector<TableStmt_s>();
		$$.push_back($1);
	}
  | from_list "," table_factor
    {
		$$ = $1;
		$$.push_back($3);
	}
  ;

opt_where:
    WHERE logical_expr		{ $$ = Vector<ExprStmt_s>(); $$.push_back($2); }
  | /* EMPTY */				{ $$ = Vector<ExprStmt_s>(); }
  ;

opt_groupby:
    GROUP BY arith_expr_list	{ $$ = $3; }
  | /* EMPTY */					{ $$ = Vector<ExprStmt_s>(); }
  ;

opt_having:
    HAVING logical_expr		{ $$ = Vector<ExprStmt_s>(); $$.push_back($2); }
  | /* EMPTY */				{ $$ = Vector<ExprStmt_s>(); }
  ;

opt_order_by:
	/*EMPTY*/             		{ $$ = Vector<ExprStmt_s>(); }
  | ORDER BY order_by_list	    { $$ = $3; }
  ;

order_by_list:
	order_by
	{
		$$ = Vector<ExprStmt_s>();
		$$.push_back($1);
	}
	| order_by_list "," order_by
	{
		$$ = $1;
		$$.push_back($3);
	}
	;

order_by:
  	arith_expr opt_asc_desc
    {
		$$ = OrderStmt::make_order_stmt($1, $2);
    }
  	;

opt_asc_desc:
    /* EMPTY */ { $$ = true; }
  | ASC			{ $$ = true; }
  | DESC		{ $$ = false; }
  ;

opt_select_limit:
	/* EMPTY */		{$$ = NULL;}
  | LIMIT limit_expr "," limit_expr
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt($4, $2);
		check(stmt);
		$$ = stmt;
    }
  | LIMIT limit_expr
	{
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt($2);
		check(stmt);
		$$ = stmt;
	}
	;

limit_expr:
    int_value
    {
		$$ = $1;
	}
	;

projection:
    arith_expr
    {
		//设置表达式别名
		$$ = $1;
		$$->alias_name = $1->to_string();
    }
  | arith_expr column_label
    {
		//设置表达式别名
		$$ = $1;
		$$->alias_name = $2;
    }
  | arith_expr AS column_label
    {
		//设置表达式别名
		$$ = $1;
		$$->alias_name = $3;
    }
  ;

basic_table_factor:
	relation_factor opt_alias
    {
		$$ = $1;
		$$->set_alias_name($2);
    }
  | DUAL opt_alias
	{
		//构建表表达式
		TableStmt_s table = BasicTableStmt::make_dual_table();
		check(table);
		$$ = table;
		$$->set_alias_name($2);
	}
	;

view_table_factor:
	"(" select_stmt ")" opt_alias
	{
		//设置表的别名
		$$ = ViewTableStmt::make_view_table($2);
		$$->set_alias_name($4);
	}
	;

joined_table_factor:
   table_factor LEFT opt_outer JOIN sub_table_factor ON logical_expr
   {
	   $$ = JoinedTableStmt::make_joined_table($1, $5, LeftOuter, $7);
   }
   | table_factor RIGHT opt_outer JOIN sub_table_factor ON logical_expr
   {
	   $$ = JoinedTableStmt::make_joined_table($1, $5, RightOuter, $7);
   }
   | table_factor FULL opt_outer JOIN sub_table_factor ON logical_expr
   {
	   $$ = JoinedTableStmt::make_joined_table($1, $5, FullOuter, $7);
   }
   | table_factor INNER JOIN sub_table_factor ON logical_expr
   {
	   $$ = JoinedTableStmt::make_joined_table($1, $4, Inner, $6);
   }
  ;

opt_outer:
	/*empty*/	
	{

	}
	| OUTER
	{

	}
	;

sub_table_factor:
	basic_table_factor
	{
		$$ = $1;
	}
	| view_table_factor
	{
		$$ = $1;
	}
	| "(" joined_table_factor ")"
	{
		$$ = $2;
	}
	;

table_factor:
  basic_table_factor
  {
	  $$ = $1;
  }
  | view_table_factor
  {
	  $$ = $1;
  }
  | joined_table_factor
  {
	  $$ = $1;
  }
  ;

opt_alias:
	/*empty*/ { $$ = ""; }
	| relation_name
	{
		$$ = $1;
	}
	| AS relation_name
	{
		$$ = $2;
	}
	;

/**************************************************************
 *
 *	expression define
 *
 **************************************************************/
simple_expr:
    column_ref
    { 
		$$ = $1;
	}
  | expr_const
    { 
		 $$ = $1;
	}
  | "(" arith_expr ")"
    { 
		$$ = $2;
	}
  | func_expr
    {
      	$$ = $1;
    }
  | query_ref_expr %prec UMINUS
    {
    	$$ = $1;
    }
  ;

seconds_expr:
  INTERVAL string DAY
  {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_day(std::stoi($2), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
  }
  | INTERVAL string MONTH
  {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_month(std::stoi($2), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
  }
  | INTERVAL string YEAR
  {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_year(std::stoi($2), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
  }
  ;

arith_expr:
    simple_expr   
	{ 
		$$ = $1;
	}
  | "+" arith_expr %prec UMINUS
    {
		//正数表达式
		$$ = $2;
    }
  | "-" arith_expr %prec UMINUS
    {
		//负数表达式
		make_unary_stmt($$, $2, OP_MINUS);
    }
  | arith_expr "+" arith_expr
	{
		//构建二元表达式 
		make_binary_stmt($$, $1, $3, OP_ADD);
	}
  | arith_expr "-" arith_expr
	{
		//构建二元表达式 
		make_binary_stmt($$, $1, $3, OP_SUB);
	}
  | arith_expr "*" arith_expr 
	{
		//构建二元表达式
		make_binary_stmt($$, $1, $3, OP_MUL);
	}
  | arith_expr "/" arith_expr 
	{
		//构建二元表达式
		make_binary_stmt($$, $1, $3, OP_DIV);
	}
  | arith_expr "+" seconds_expr
  {
	  	//构建二元表达式
		make_binary_stmt($$, $1, $3, OP_DATE_ADD);
  }
  | arith_expr "-" seconds_expr
  {
	  	//构建二元表达式
		make_binary_stmt($$, $1, $3, OP_DATE_SUB);
  }
  | case_when_expr
	{
		$$ = $1;
	}

  ;

arith_expr_list:
    arith_expr
    {
		//构建表达式列表
		$$ = Vector<ExprStmt_s>();
		$$.push_back($1);
    }
  | arith_expr_list "," arith_expr
    {
		//将新的表达式加入到表达式列表
		$$ = $1;
		$$.push_back($3);
    }
  ;

cmp_expr:
	arith_expr cmp_type arith_expr 
	{
		//构建比较二元表达式 
		make_binary_stmt($$, $1, $3, $2);
	}
  | arith_expr IS NULLX
    {
		//构建is null表达式
		make_unary_stmt($$, $1, OP_IS_NULL);
    }
  | arith_expr IS NOT NULLX
    {
		//构建is not null表达式
		make_unary_stmt($$, $1, OP_IS_NOT_NULL);
    }
  | arith_expr BETWEEN arith_expr AND arith_expr %prec BETWEEN
    {
		//构建between and三元表达式
		make_ternary_stmt($$, $1, $3, $5, OP_BETWEEN);
    }
  | arith_expr NOT BETWEEN arith_expr AND arith_expr %prec BETWEEN
    {
		//构建not between and三元表达式
		make_ternary_stmt($$, $1, $4, $6, OP_NOT_BETWEEN);
    }
  | arith_expr IN "(" arith_expr_list ")"
    {
		//构建in表达式
		ExprStmt_s stmt = OpExprStmt::make_op_expr_stmt(OP_IN_LIST);
		check(stmt);
		stmt->params.push_back($1);
		ExprStmt_s list = ListStmt::make_list_stmt();
		list->params = $4;
		stmt->params.push_back(list);
		$$ = stmt;
    }
  | arith_expr NOT IN "(" arith_expr_list ")"
    {
		//构建not in表达式
		ExprStmt_s stmt = OpExprStmt::make_op_expr_stmt(OP_NOT_IN_LIST);
		check(stmt);
		stmt->params.push_back($1);
		ExprStmt_s list = ListStmt::make_list_stmt();
		list->params = $5;
		stmt->params.push_back(list);
		$$ = stmt;
    }
  | arith_expr IN query_ref_expr
    {
		//构建in表达式
		make_binary_stmt($$, $1, $3, OP_IN);
    }
  | arith_expr NOT IN query_ref_expr
    {
		//构建not in表达式
		make_binary_stmt($$, $1, $4, OP_NOT_IN);
    }
  | EXISTS query_ref_expr
    {
    	make_unary_stmt($$, $2, OP_EXISTS);
    }
  | NOT EXISTS query_ref_expr
    {
		//构建not一元表达式
		make_unary_stmt($$, $3, OP_NOT_EXISTS);
    }
  | arith_expr sq_cmp_type query_ref_expr 
	{
		//构建比较二元表达式 
		make_binary_stmt($$, $1, $3, $2);
	}
  ;

cmp_type:
	CMP_LE  
	{
		$$ = OP_LE;
	}
  | CMP_LT 
	{
		$$ = OP_LT;
	}
  | CMP_EQ
	{
		$$ = OP_EQ;
	}
  | CMP_GE 
	{
		$$ = OP_GE;
	}
  | CMP_GT 
	{
		$$ = OP_GT;
	}
  | CMP_NE 
	{
		$$ = OP_NE;
	}
  | LIKE 
	{
		$$ = OP_LIKE;
	}
  | NOT LIKE 
	{
		$$ = OP_NOT_LIKE;
	}
	;

sq_cmp_type:
	CMP_LE ANY
	{
		$$ = OP_LE_ANY;
	}
  | CMP_LT ANY
	{
		$$ = OP_LT_ANY;
	}
  | CMP_EQ ANY
	{
		$$ = OP_EQ_ANY;
	}
  | CMP_GE ANY
	{
		$$ = OP_GE_ANY;
	}
  | CMP_GT ANY
	{
		$$ = OP_GT_ANY;
	}
  | CMP_NE ANY
	{
		$$ = OP_NE_ANY;
	}
  |	CMP_LE ALL
	{
		$$ = OP_LE_ALL;
	}
  | CMP_LT ALL
	{
		$$ = OP_LT_ALL;
	}
  | CMP_EQ ALL
	{
		$$ = OP_EQ_ALL;
	}
  | CMP_GE ALL
	{
		$$ = OP_GE_ALL;
	}
  | CMP_GT ALL
	{
		$$ = OP_GT_ALL;
	}
  | CMP_NE ALL
	{
		$$ = OP_NE_ALL;
	}
	;

query_ref_expr:
	select_with_parens
    {
		SubQueryStmt_s query_expr = SubQueryStmt::make_query_stmt();
		query_expr->query_stmt = $1;
		$$ = query_expr;
    }
	;

case_when_expr:
	CASE arith_expr when_then_list1 ELSE arith_expr END_SYM
	{
		$$ = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		$$->params.push_back($2);
		append($$->params, $3);
		$$->params.push_back($5);
	}
	| CASE when_then_list2 ELSE arith_expr END_SYM
	{
		$$ = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		append($$->params, $2);
		$$->params.push_back($4);
	}
	;

when_then_list1:
	WHEN arith_expr THEN arith_expr
	{
		$$ = Vector<ExprStmt_s>();
		$$.push_back($2);
		$$.push_back($4);
	}
	| when_then_list1 WHEN arith_expr THEN arith_expr
	{
		$$ = $1;
		$$.push_back($3);
		$$.push_back($5);
	}
	;

when_then_list2:
	WHEN logical_expr THEN arith_expr
	{
		$$ = Vector<ExprStmt_s>();
		$$.push_back($2);
		$$.push_back($4);
	}
	| when_then_list2 WHEN logical_expr THEN arith_expr
	{
		$$ = $1;
		$$.push_back($3);
		$$.push_back($5);
	}
	;

logical_expr:
	cmp_expr
	{
		$$ = $1;
	}
  | "(" logical_expr ")"
	{
		$$ = $2;
	}
  | logical_expr AND logical_expr %prec AND
    {
		//构建and二元表达式
		make_binary_stmt($$, $1, $3, OP_AND);
    }
  | logical_expr OR logical_expr %prec OR
    {
		//构建or二元表达式
		make_binary_stmt($$, $1, $3, OP_OR);
    }
	;

column_ref:
    column_name
    { 
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", $1);
		check(col);
		$$ = col;
	}
  | "*"
	{
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		$$ = col;
	}
  | relation_name "." column_name
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt($1, $3);
		check(col);
		$$ = col;
    }
  |
    relation_name "." "*"
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt($1, "*");
		check(col);
		$$ = col;
    }
  | ROWID
    { 
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt("", "ROWID");
		check(col);
		col->is_row_id = true;
		$$ = col;
	}
  | relation_name "." ROWID
    {
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt($1, "ROWID");
		check(col);
		col->is_row_id = true;
		$$ = col;
    }
  ;

expr_const:
     string
	{
		//构建常量表达式
		Object_s value = Varchar::make_object($1);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
	}
  | datetime
	{
		//构建常量表达式
		if (!DateTime::is_valid_datetime($1)) {
			yyerror("invalid datetime");
			YYABORT;
		}
		Object_s value = DateTime::make_object($1);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
	}
  | number
	{
		//构建常量表达式
		Object_s value = Number::make_object($1);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
    }
  | NULLX
	{
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
	}
  ;
  
func_expr:
  win_type "(" distinct_or_all opt_arith_expr ")"
  {
	if (!$4) {
		yyerror("expect aggr expr");
		YYABORT;
	}
	AggrType type;
	if (WIN_COUNT == $1) {
		type = COUNT;
	} else if (WIN_SUM == $1) {
		type = SUM;
	} else if (WIN_AVG == $1) {
		type = AVG;
	} else if (WIN_MIN == $1) {
		type = MIN;
	} else if (WIN_MAX == $1) {
		type = MAX;
	}
	AggrStmt_s expr = AggrStmt::make_aggr_stmt();
	check(expr);
	expr->aggr_func = type;
	expr->distinct = $3;
	expr->set_aggr_expr($4);
	$$ = expr;
  }
  | win_type "(" distinct_or_all opt_arith_expr ")" OVER "(" opt_partition_by opt_order_by ")" 
  {
	WinExprStmt_s expr = WinExprStmt::make_win_expr_stmt($1);
	check(expr);
	expr->is_distinct = $3;
	if ($4) {
		expr->set_win_func_expr($4);
	}
	expr->set_win_part_by_exprs($8);
	expr->set_win_order_by_exprs($9);
	$$ = expr;
  }
  | TO_CHAR "(" arith_expr ")"
  {
	make_unary_stmt($$, $3, OP_TO_CHAR);
  }
  | TO_CHAR "(" arith_expr "," string ")"
  {
	Object_s value = Varchar::make_object($5);
	check(value);
	ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
	check(stmt);
	make_binary_stmt($$, $3, stmt, OP_TO_CHAR);
  }
  | SUBSTR "(" arith_expr "," number "," number ")"
  {
	Object_s value1 = Number::make_object($5);
	check(value1);
	ExprStmt_s stmt1 = ConstStmt::make_const_stmt(value1);
	check(stmt1);
	Object_s value2 = Number::make_object($7);
	check(value2);
	ExprStmt_s stmt2 = ConstStmt::make_const_stmt(value2);
	check(stmt2);
	make_ternary_stmt($$, $3, stmt1, stmt2, OP_SUBSTR);
  }
  | IFNULL "(" arith_expr "," arith_expr ")"
  {
	make_binary_stmt($$, $3, $5, OP_IFNULL);
  }
  | TO_NUMBER "(" arith_expr ")"
  {
	make_unary_stmt($$, $3, OP_TO_NUMBER);
  }
  ;


distinct_or_all:
	/*empty*/
	{
		$$ = false;
	}
  | ALL
    {
		$$ = false;
    }
  | DISTINCT
    {
		$$ = true;
    }
  ;

win_type:
	COUNT
	{
		$$ = WIN_COUNT;
	}
	| SUM
	{
		$$ = WIN_SUM;
	}
	| AVG
	{
		$$ = WIN_AVG;
	}
	| MIN
	{
		$$ = WIN_MIN;
	}
	| MAX
	{
		$$ = WIN_MAX;
	}
	| RANK
	{
		$$ = WIN_RANK;
	}
	| DENSE_RANK
	{
		$$ = WIN_DENSE_RANK;
	}
	| ROW_NUMBER
	{
		$$ = WIN_ROW_NUMBER;
	}
	;

opt_arith_expr:
	/*empty*/
	{
		$$ = ExprStmt_s();
	}
	| arith_expr
	{
		$$ = $1;
	}
	;

opt_partition_by:
	/*empty*/
	{
		$$ = Vector<ExprStmt_s>();
	}
	| PARTITION BY arith_expr_list
	{
		$$ = $3;
	}
	;

/**************************************************************
 *
 *	insert dml define
 *
 **************************************************************/
insert_stmt:
    INSERT INTO relation_factor VALUES insert_value_list
    {
		//构建insert stmt
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = $3;
		insert_stmt->value_list = $5;
		$$ = insert_stmt;
    }
  | INSERT INTO relation_factor select_stmt
    {
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = $3;
		insert_stmt->query_values = $4;
		$$ = insert_stmt;
    }
  ;

insert_value_list:
    "(" insert_value ")"
    {
		$$ = Vector<Vector<ExprStmt_s>>();
		$$.push_back($2);
    }
  | insert_value_list "," "(" insert_value ")" 
	{
		$$ = $1;
		$$.push_back($4);
	}
	;

insert_value:
    arith_expr 
	{ 
		$$ = Vector<ExprStmt_s>();
		$$.push_back($1);
	}
  | insert_value "," arith_expr
    {
		$$ = $1;
		$$.push_back($3);
    }
  ;

/**************************************************************
 *
 *	update dml define
 *
 **************************************************************/
 update_stmt:
    UPDATE opt_hint relation_factor SET update_asgn_list opt_where
    {
		UpdateStmt_s update_stmt = UpdateStmt::make_update_stmt();
		check(update_stmt);
		update_stmt->stmt_hint = $2;
		update_stmt->table = $3;
		update_stmt->from_stmts.push_back($3);
		update_stmt->update_assign_stmt = $5;
		update_stmt->where_stmt = $6;
		$$ = update_stmt;
    }
	| UPDATE opt_hint relation_factor SET update_asgn_list FROM from_list opt_where
    {
		UpdateStmt_s update_stmt = UpdateStmt::make_update_stmt();
		check(update_stmt);
		update_stmt->stmt_hint = $2;
		update_stmt->table = $3;
		update_stmt->update_assign_stmt = $5;
		update_stmt->from_stmts = $7;
		update_stmt->where_stmt = $8;
		$$ = update_stmt;
    }
  ;

update_asgn_list:
    update_asgn_factor
    {
		//构建值列表
		$$ = Vector<ExprStmt_s>();
		$$.push_back($1);
    }
  | update_asgn_list "," update_asgn_factor
    {
		//将新的表达式加入到表达式列表
		$$ = $1;
		$$.push_back($3);
    }
  ;

update_asgn_factor:
    column_name CMP_EQ arith_expr
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", $1);
		check(col);
		make_binary_stmt($$, col, $3, OP_ASSIGN);
    }
  ;

 /**************************************************************
 *
 *	delete define
 *
 **************************************************************/
delete_stmt:
    DELETE opt_hint FROM relation_factor opt_alias opt_where
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = $2;
		delete_stmt->table = $4;
		$4->set_alias_name($5);
		delete_stmt->from_stmts.push_back($4);
		delete_stmt->where_stmt = $6;
		$$ = delete_stmt;
    }
	| DELETE opt_hint "*" FROM relation_factor opt_alias opt_where
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = $2;
		delete_stmt->table = $5;
		$5->set_alias_name($6); 
		delete_stmt->from_stmts.push_back($5);
		delete_stmt->where_stmt = $7;
		$$ = delete_stmt;
    }
	| DELETE opt_hint relation_factor FROM from_list opt_where
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = $2;
		delete_stmt->table = $3;
		delete_stmt->from_stmts = $5;
		delete_stmt->where_stmt = $6;
		$$ = delete_stmt;
    }
  ;
 
 /**************************************************************
 *
 *	explain define
 *
 **************************************************************/
explain_stmt:
    EXPLAIN explainable_stmt
    {
		$$ = $2;
		DMLStmt_s dml = $$;
		dml->is_explain = true;
    }
  ;

explainable_stmt:
    select_stmt         { $$ = $1; }
  | delete_stmt         { $$ = $1; }
  | insert_stmt         { $$ = $1; }
  | update_stmt         { $$ = $1; }
  ;

/**************************************************************
 *
 *	create define
 *
 **************************************************************/
create_stmt:
    CREATE TABLE relation_factor "(" table_element_list ")" opt_engine_def
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateTable);
		check(cmd_stmt);
		CreateTableParam_s param = CreateTableParam::make_create_table_param();
		param->database_name = $3->database;
		param->table_name = $3->table_name;
		param->column_define_list = $5;
		param->engine_args = $7;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
    }
	| CREATE DATABASE database_name
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateDatabase);
		check(cmd_stmt);
		CreateDBParam_s param = CreateDBParam::make_create_DB_param();
		param->database = $3;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
	| CREATE VIEW ident opt_view_column_define AS beg_view_define select_stmt
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateView);
		check(cmd_stmt);
		CreateViewParam_s param = CreateViewParam::make_create_view_param();
		param->database = driver.get_global_database();
		param->view_name = $3;
		param->column_define = $4;
		param->view_define_sql = $6;
		param->ref_query = $7;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
  ;

opt_view_column_define:
	/*empty*/
	{
		$$ = Vector<String>();
	}
	| "(" view_column_define ")"
	{
		$$ = $2;
	}
	;

view_column_define:
	column_name
	{
		$$ = Vector<String>();
		$$.push_back($1);
	}
	| view_column_define "," column_name
	{
		$$ = $1;
		$$.push_back($3);
	}
	;

beg_view_define:
	/*empty*/
	{
		$$ = driver.left_string();
	}
	;

table_element_list:
    column_definition
    {
		$$ = Vector<ColumnDefineStmt_s>();
		$$.push_back($1);
    }
  | table_element_list "," column_definition
    {
		$$ = $1;
		$$.push_back($3);
    }
  ;

column_definition:
    column_name data_type opt_not_null
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt($1, $2);
		check(stmt);
		$$ = stmt;
    }
  ;

opt_not_null:
	/*empty*/
	{

	}
	| NOT NULLX
	{
		
	}
	;

data_type:
  TINYINT
    { $$ = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
  | SMALLINT
    { $$ = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
  | MEDIUMINT
    { $$ = DataType(T_NUMBER, MAX_MEDIUMINT_PREC, 0); }
  | INT
    { $$ = DataType(T_NUMBER, MAX_INT_PREC, 0); }
  | INTEGER
    { $$ = DataType(T_NUMBER, MAX_INT_PREC, 0); }
  | BIGINT
    { $$ = DataType(T_NUMBER, MAX_BIGINT_PREC, 0); }
  | DECIMAL 
    { $$ = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
  | NUMERIC_SYM 
    { $$ = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
  | DECIMAL "(" int_value "," int_value ")"
    { $$ = DataType(T_NUMBER, $3, $5); }
  | NUMBER
  	{ $$ = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
  | NUMBER "(" int_value "," int_value ")"
  	{ $$ = DataType(T_NUMBER, $3, $5); }
  | NUMERIC_SYM "(" int_value "," int_value ")"
    { $$ = DataType(T_NUMBER, $3, $5); }
  | BOOL
    { $$ = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
  | FLOAT
    { $$ = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
  | REAL
    { $$ = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
  | DOUBLE
    { $$ = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
  | CHAR opt_char_length
    { $$ = DataType(T_VARCHAR, $2); }
  | BINARY opt_char_length
    { $$ = DataType(T_VARCHAR, $2); }
  | VARCHAR opt_char_length
    { $$ = DataType(T_VARCHAR, $2); }
  | VARBINARY opt_char_length
    { $$ = DataType(T_VARCHAR, $2); }
  | TIMESTAMP_SYM opt_time_precision
    { $$ = DataType(T_DATETIME, TIMESTAMP, $2); }
  | DATETIME
    { $$ = DataType(T_DATETIME, DATETIME); }
  | DATE
    { $$ = DataType(T_DATETIME, DATE); }
  | TIME opt_time_precision
    { $$ = DataType(T_DATETIME, TIME); }
  ;

opt_time_precision:
	/*EMPTY*/         	{ $$ = MAX_TIME_SCALE; }
  	| "(" int_value ")"   
	{ 
		if ($2 > MAX_TIME_SCALE) {
			yyerror("max time prec:%d", MAX_TIME_SCALE);
			YYABORT;
		}
		$$ = $2; 
	}
  ;

opt_char_length:
	/*EMPTY*/         	{ $$ = MAX_STR_LENGTH; }
  	| "(" int_value ")"   
	  { 
		  if ($2 > MAX_STR_LENGTH) {
			yyerror("max char length:%d", MAX_STR_LENGTH);
			YYABORT;
		}
		$$ = $2; 
	  }
  ;

opt_engine_def:
	/*empty*/		
	{ 
		$$ = Vector<String>(); 
		$$.push_back("CAT");
		$$.push_back(" ");
		$$.push_back(" ");
		$$.push_back(" ");
	}
	| ENGINE CMP_EQ CSV "," INFILE CMP_EQ string "," FIELD SPLIT BY string "," LINE SPLIT BY string
	{
		$$ = Vector<String>();
		$$.push_back("CSV");
		$$.push_back($7);
		$$.push_back($12);
		$$.push_back("new line");
	}
	;

/**************************************************************
 *
 *	drop define
 *
 **************************************************************/
 drop_stmt:
    DROP TABLE opt_if_exists relation_factor
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropTable);
		check(cmd_stmt);
		DropTableParam_s param = DropTableParam::make_drop_table_param();
		param->database_name = $4->database;
		param->table_name = $4->table_name;
		param->ignore_not_exists = $3;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
    }
	| DROP DATABASE opt_if_exists database_name
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropDatabase);
		check(cmd_stmt);
		DropDBParam_s param = DropDBParam::make_drop_DB_param();
		param->database = $4;
		param->ignore_not_exists = $3;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
	| DROP VIEW opt_if_exists ident
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropView);
		check(cmd_stmt);
		DropViewParam_s param = DropViewParam::make_drop_view_param();
		param->database = driver.get_global_database();
		param->view_name = $4;
		param->ignore_not_exists = $3;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
  ;

opt_if_exists:
	/*empty*/	{ $$ = false; }
	| IF EXISTS { $$ = true; }
	;

/**************************************************************
 *
 *	show define
 *
 **************************************************************/
 show_stmt:
	SHOW DATABASES
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		$$ = cmd_stmt;
 	}
	| SHOW FULL TABLES op_from_database
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		ShowTablesParam_s param = ShowTablesParam::make_show_tables_param();
		param->database = $4;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
	| SHOW TABLES op_from_database
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		ShowTablesParam_s param = ShowTablesParam::make_show_tables_param();
		param->database = $3;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
	| SHOW COLUMNS FROM relation_factor
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		DescTableParam_s param = DescTableParam::make_desc_table_param();
		param->database_name = $4->database;
		param->table_name = $4->table_name;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
	| SHOW PROCESSLIST
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowProcesslist);
		check(cmd_stmt);
		$$ = cmd_stmt;
	}
	| SHOW MEMORY
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowMemory);
		check(cmd_stmt);
		$$ = cmd_stmt;
	}
  ;
 
op_from_database:
	FROM database_name
	{
		$$ = $2;
	}
	|/*EMPTY*/
	{
		$$ = driver.get_global_database();
	}
	;

 use_stmt:
	USING database_name
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(UseDatabase);
		check(cmd_stmt);
		UseDBParam_s param = UseDBParam::make_use_DB_param();
		param->database = $2;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
	;
  
/**************************************************************
 *
 *	desc define
 *
 **************************************************************/
 desc_stmt:
    DESCRIBE relation_factor
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		DescTableParam_s param = DescTableParam::make_desc_table_param();
		param->database_name = $2->database;
		param->table_name = $2->table_name;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
    }
	| DESC relation_factor
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		DescTableParam_s param = DescTableParam::make_desc_table_param();
		param->database_name = $2->database;
		param->table_name = $2->table_name;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
  ;

/**************************************************************
 *
 *	kill session task define
 *
 **************************************************************/

kill_stmt:
	KILL int_value
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Kill);
		check(cmd_stmt);
		KillSessionParam_s param = KillSessionParam::make_kill_session_param();
		param->session_id = $2;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
	;

 /**************************************************************
 *
 *	create package define
 *
 **************************************************************/
create_package_stmt:
    CREATE opt_replace PACKAGE package_name IS
    pro_or_func_list
    END_SYM
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreatePackage);
		check(cmd_stmt);
		CreatePackageParam_s param = CreatePackageParam::make_create_package_param();
		param->is_replace = $2;
		param->name = $4;
		param->functions = $6;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
	;

pro_or_func_list:
    pro_or_func
	{
		$$ = Vector<FunctionDefinition_s>();
		$$.push_back($1);
	}
    | pro_or_func_list pro_or_func
	{
		$$ = $1;
		$$.push_back($2);
	}
	;

pro_or_func:
    procedure_define
	{
		$$ = $1;
	}
    | function_define
	{
		$$ = $1;
	}
	;

procedure_define:
    PROCEDURE procedure_name "(" opt_param_list ")" ";"
	{
		$$ = FunctionDefinition::make_func_define();
		$$->name = $2;
		$$->param_list = $4;
	}
	;

function_define:
    FUNCTION function_name "(" opt_param_list ")" 
    RETURN table_type ";"
	{
		$$ = FunctionDefinition::make_func_define();
		$$->name = $2;
		$$->param_list = $4;
		$$->return_type_list = $7;
	}
	| FUNCTION function_name "(" opt_param_list ")" 
    RETURN data_type ";"
	{
		$$ = FunctionDefinition::make_func_define();
		$$->name = $2;
		$$->param_list = $4;
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt($2, $7);
		$$->return_type_list.push_back(stmt);
	}
	;

table_type:
    TABLE "(" param_list ")"
	{
		$$ = $3;
	}
	;

opt_param_list:
	/**/
	{
		$$ = Vector<ColumnDefineStmt_s>();
	}
	| param_list
	{
		$$ = $1;
	}
	;

param_list:
    param
	{
		$$ = Vector<ColumnDefineStmt_s>();
		$$.push_back($1);
	}
    | param_list "," param
	{
		$$ = $1;
		$$.push_back($3);
	}
	;

param:
    param_name data_type
	{
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt($1, $2);
		check(stmt);
		$$ = stmt;
	}
	;

opt_replace:
	/*empty*/		{ $$ = false; }
	| OR REPLACE 	{ $$ = true; }
	;

package_name:
	ident		{ $$ = $1; }
  ;

procedure_name:
	ident		{ $$ = $1; }
  ;

function_name:
	ident		{ $$ = $1; }
  ;

param_name:
	ident		{ $$ = $1; }
  ;

exec_package_stmt:
	start_exec_package package_name "." function_name "(" opt_arith_expr_list ")"
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ExecFunction);
		check(cmd_stmt);
		ExecFunctionParam_s param = ExecFunctionParam::make_exec_func_param();
		param->package_name = $2;
		param->function_name = $4;
		param->param_list = $6;
		cmd_stmt->param = param;
		$$ = cmd_stmt;
	}
	;

start_exec_package:
	EXEC
	{ $$ = true; }
	| CALL
	{ $$ = true; }
	;

opt_arith_expr_list:
	/**/
	{
		$$ = Vector<ExprStmt_s>();
	}
	| arith_expr_list
	{
		$$ = $1;
	}
	;

 /**************************************************************
 *
 *	name define
 *
 **************************************************************/
relation_factor:
	relation_name
	{
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(driver.get_global_database(), $1);
		check(table);
		table->alias_name = $1;
		$$ = table;
	}
    | database_name "." relation_name
	{
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table($1, $3);
		check(table);
		table->alias_name = $3;
		$$ = table;
	}
  ;

database_name:
	ident		{ $$ = $1; }
  ;

relation_name:
	ident		{ $$ = $1; }
  ;

column_name:
	ident		{ $$ = $1; }
	;

column_label:
	ident		{ $$=$1; }
	;

ident:
	IDENT		{ str_to_lower($1);$$ = $1; }
	;

datetime:
	DATETIME string	{ $$ = $2; }
  | DATE string	{ $$ = $2; }
  ;
 
string:
	STRING 		{ $$ = $1; }
  ;	

number:
	NUMERIC		{ $$ = $1; }
	;

int_value:
	number
    {
		$$ = std::stoi($1);
	}
%%

void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
