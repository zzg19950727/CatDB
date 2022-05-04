%skeleton "lalr1.cc"
//声明命名空间
%define api.namespace {CatDB} 
//%define api.parser.class { SqlParser }
%define api.token.constructor
//使得类型与token定义可以使用各种复杂的结构与类型
%define api.value.type variant 
//开启断言功能
%define parse.assert  
%defines
%code requires
{
	#define YYDEBUG 1
	#define SqlParser parser 
	#include "select_stmt.h"
	#include "insert_stmt.h"
	#include "update_stmt.h"
	#include "delete_stmt.h"
	#include "dml_stmt.h"
	#include "cmd_stmt.h"
	#include "expr_stmt.h"
	#include "table_stmt.h"
	#include "object.h"
	#include "obj_number.h"
	#include "obj_varchar.h"
	#include "obj_datetime.h"
	#include "stmt.h"
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
	#include "dml_stmt.h"
	#include "cmd_stmt.h"
	#include "expr_stmt.h"
    #include "table_stmt.h"
	#include "object.h"
	
	/*注意：这里的参数由%parse-param决定*/
	static CatDB::SqlParser::symbol_type yylex(CatDB::SqlScanner& scanner,CatDB::SqlDriver &driver)
	{
		return scanner.get_next_token();
	}
	using namespace CatDB;
	using namespace CatDB::Parser;
	using namespace CatDB::Common;
}

%{
	
#define yyerror(fmt, ...) \
{ \
	char tmp[255]; \
	sprintf(tmp, fmt, ##__VA_ARGS__); \
	driver.set_sys_error(tmp); \
}
 
#define check(stmt) \
{ \
	if(!stmt) \
	{ \
		yyerror("make stmt error"); \
		YYABORT; \
	} \
}

#define make_list(ret, from) \
{ \
	ListStmt_s list = ListStmt::make_list_stmt(); \
	check(list); \
	list->push_back(from); \
	ret = list; \
}

#define list_push(ret, list, from) \
{ \
	ListStmt_s stmt = list; \
	check(stmt); \
	stmt->push_back(from); \
	ret = list; \
}

#define make_unary_stmt(stmt, stmt1, op) \
{ \
	stmt = OpExprStmt::make_op_expr_stmt(op); \
	check(stmt); \
	stmt->params.push_back(stmt1); \
}

#define make_binary_stmt(stmt, stmt1, stmt2, op) \
{ \
	stmt = OpExprStmt::make_op_expr_stmt(op); \
	check(stmt); \
	stmt->params.push_back(stmt1); \
	stmt->params.push_back(stmt2); \
}

#define make_ternary_stmt(stmt, stmt1, stmt2, stmt3, op) \
{ \
	stmt = OpExprStmt::make_op_expr_stmt(op); \
	check(stmt); \
	stmt->params.push_back(stmt1); \
	stmt->params.push_back(stmt2); \
	stmt->params.push_back(stmt3); \
}
	
#define str_to_lower(str) \
{\
	for(u32 i = 0; i<str.size(); ++i){\
		if(str[i] >= 'A' && str[i] <= 'Z'){\
			str[i] -= 'A';\
			str[i] += 'a';\
		}\
	}\
}
%}

/*定义parser传给scanner的参数*/
%lex-param { CatDB::SqlScanner& scanner }
%lex-param { CatDB::SqlDriver& driver }
/*定义driver传给parser的参数*/
%parse-param { CatDB::SqlScanner& scanner }
%parse-param { CatDB::SqlDriver& driver }
 
%locations
//%define parse-trace
/*详细显示错误信息*/
%define parse.error verbose
/*CatDB::SqlParser::make_XXX(loc)给token添加前缀*/
%define api.token.prefix {TOKEN_}

%left	UNION EXCEPT
%left	INTERSECT
%left	OR
%left	AND
%right	NOT
%left	CMP_LE CMP_LT CMP_EQ CMP_GT CMP_GE CMP_NE
%left	 LIKE
%nonassoc BETWEEN
%nonassoc IN
%nonassoc IS NULLX BOOL
%left "+" "-"
%left "*" "/"
%left UMINUS
%left "(" ")"
%left "."

%token<std::string>	STRING
%token<std::string>	IDENT
%token<std::string>	NUMERIC
%token<std::string>	QB_NAME_IDENT

%token ALL
%token ANALYZE
%token AND
%token ANY
%token AS
%token ASC
%token BEGIN_HINT
%token BEGIN_OUTLINE_DATA
%token BETWEEN
%token BIGINT
%token BINARY
%token BOOL
%token BY
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
%token CREATE
%token CSV
%token DATABASE
%token DATABASES
%token DATE
%token DATETIME
%token DAY
%token DECIMAL
%token DELETE
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
%token EXISTS
%token EXPLAIN
%token EXPR_NORMALIZE
%token FIELD
%token FLOAT
%token FROM
%token FULL
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
%token INTERSECT
%token INTERVAL
%token INTEGER
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
%token MEDIUMINT
%token MEMORY
%token MERGE
%token MINUS "-"
%token MONTH
%token MUL "*"
%token NOT
%token NO_REWRITE
%token NO_UNNEST
%token NO_MERGE
%token NO_USE_HASH
%token NO_USE_NL
%token NO_SIMPLIFY_SQ
%token NO_EXPR_NORMALIZE
%token NULLX
%token NUMERIC_SYM
%token ON
%token OR
%token ORDER
%token ORDERED
%token OUTER
%token PARALLEL
%token PARTITION
%token PERIOD "."
%token PLUS "+"
%token PROCESSLIST
%token REAL
%token RIGHT
%token ROWID
%token RP ")"
%token SAMPLE
%token SELECT
%token SEMICOLON ";"
%token SET
%token SHOW
%token SIZE
%token SIMPLIFY_SQ
%token SMALLINT
%token SPLIT
%token STATIS
%token STATUS
%token SUBSTR
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
%token YEAR
//aggr function
%token SUM
%token COUNT
%token AVG
%token MIN
%token MAX
%token RANK
%token DENSE_RANK
%token ROW_NUMBER
%token OVER
%token END 0

%type<Stmt_s>						sql_stmt stmt cmd_stmt select_stmt insert_stmt update_stmt 
									delete_stmt explain_stmt explainable_stmt
%type<Stmt_s>						select_with_parens simple_select set_select sub_set_select
%type<Stmt_s>						show_stmt create_stmt drop_stmt desc_stmt use_stmt analyze_stmt 
									set_var_stmt kill_stmt
%type<ExprStmt_s>					projection simple_expr arith_expr cmp_expr logical_expr column_ref 
									expr_const func_expr query_ref_expr update_asgn_factor case_when_expr
									seconds_expr aggr_expr
%type<OrderStmt_s>					order_by
%type<LimitStmt_s>					opt_select_limit
%type<TableStmt_s>					table_factor sub_table_factor basic_table_factor view_table_factor joined_table_factor
%type<bool>							opt_distinct opt_asc_desc distinct_or_all opt_if_exists opt_split opt_outer opt_not_null
%type<int>							limit_expr int_value opt_char_length opt_time_precision
%type<DataType>						data_type
%type<double>						opt_sample_size
%type<std::string>					op_from_database column_label database_name relation_name opt_alias column_name 
									ident string datetime number opt_qb_name opt_qb_name_single beg_view_define
%type<Vector<TableStmt_s>>			from_list 
%type<BasicTableStmt_s> 			relation_factor
%type<Vector<OrderStmt_s>>			opt_order_by order_by_list
%type<Vector<ExprStmt_s>>			select_expr_list opt_groupby arith_expr_list opt_where opt_having insert_value update_asgn_list 
									when_then_list1 when_then_list2
%type<Vector<Vector<ExprStmt_s>>>	insert_value_list
%type<ColumnDefineStmt_s>			column_definition
%type<Vector<ColumnDefineStmt_s>>	table_element_list
%type<Hint> 						opt_hint
%type<Vector<HintStmt_s>> 			opt_hint_list hint_list
%type<HintStmt_s> 					single_hint
%type<Vector<String>> 				hint_table_list opt_engine_def opt_view_column_define view_column_define
%type<Vector<LeadingTable_s>> 		leading_hint_table_list
%type<LeadingTable_s> 				leading_hint_table
%type<OperationType> 				cmp_type sq_cmp_type
%type<AggrType>						aggr_type
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
  | /*EMPTY*/			{ yyerror("unknow stmt"); }
	;

/**************************************************************
 *
 *	select define
 *
 **************************************************************/
cmd_stmt:
	show_stmt			{ $$ = $1; }
	| create_stmt		{ $$ = $1; }
	| drop_stmt			{ $$ = $1; }
	| desc_stmt			{ $$ = $1; }
	| use_stmt			{ $$ = $1; }
	| analyze_stmt		{ $$ = $1; }
	| set_var_stmt		{ $$ = $1; }
	| kill_stmt			{ $$ = $1; }
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
/*optiizer hint*/
	| USE_HASH "(" opt_qb_name hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(JOIN, true);
		$$->set_qb_name($3);
		JoinHintStmt_s join_hint = $$;
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = $4;
	}
	| NO_USE_HASH "(" opt_qb_name hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(JOIN, false);
		$$->set_qb_name($3);
		JoinHintStmt_s join_hint = $$;
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = $4;
	}
	| USE_NL "(" opt_qb_name hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(JOIN, true);
		$$->set_qb_name($3);
		JoinHintStmt_s join_hint = $$;
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = $4;
	}
	| NO_USE_NL "(" opt_qb_name hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(JOIN, false);
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
	/*EMPTY*/             		{ $$ = Vector<OrderStmt_s>(); }
  | ORDER BY order_by_list	    { $$ = $3; }
  ;

order_by_list:
	order_by
	{
		$$ = Vector<OrderStmt_s>();
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
  aggr_expr
  {
		$$ = $1;
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

aggr_expr:
	aggr_type "(" "*" ")"
	{
		AggrStmt_s expr = AggrStmt::make_aggr_stmt();
		check(expr);
		expr->aggr_func = $1;
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		expr->set_aggr_expr(col);
		$$ = expr;
	}
	| aggr_type "(" distinct_or_all arith_expr ")"
    {
		AggrStmt_s expr = AggrStmt::make_aggr_stmt();
		check(expr);
		expr->aggr_func = $1;
		expr->distinct = $3;
		expr->set_aggr_expr($4);
		$$ = expr;
    }
	;

aggr_type:
	COUNT
	{
		$$ = COUNT;
	}
	| SUM
	{
		$$ = SUM;
	}
	| AVG
	{
		$$ = AVG;
	}
	| MIN
	{
		$$ = MIN;
	}
	| MAX
	{
		$$ = MAX;
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
  | update_asgn_list ',' update_asgn_factor
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
		cmd_stmt->params.create_table_params.table = $3;
		cmd_stmt->params.create_table_params.column_define_list = $5;
		cmd_stmt->params.create_table_params.engine_args = $7;
		$$ = cmd_stmt;
    }
	| CREATE DATABASE database_name
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateDatabase);
		check(cmd_stmt);
		cmd_stmt->params.create_database_params.database = $3;
		$$ = cmd_stmt;
	}
	| CREATE VIEW ident opt_view_column_define AS beg_view_define select_stmt
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateView);
		check(cmd_stmt);
		cmd_stmt->params.create_view_params.database = driver.get_global_database();
		cmd_stmt->params.create_view_params.view_name = $3;
		cmd_stmt->params.create_view_params.column_define = $4;
		cmd_stmt->params.create_view_params.view_define_sql = $6;
		cmd_stmt->params.create_view_params.ref_query = $7;
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
    DROP TABLE opt_if_exists table_factor
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropTable);
		check(cmd_stmt);
		cmd_stmt->params.drop_table_params.ignore_not_exists = $3;
		cmd_stmt->params.drop_table_params.table = $4;
		$$ = cmd_stmt;
    }
	| DROP DATABASE opt_if_exists database_name
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropDatabase);
		check(cmd_stmt);
		cmd_stmt->params.drop_database_params.ignore_not_exists = $3;
		cmd_stmt->params.drop_database_params.database = $4;
		$$ = cmd_stmt;
	}
	| DROP VIEW opt_if_exists ident
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropView);
		check(cmd_stmt);
		cmd_stmt->params.drop_view_params.database = driver.get_global_database();
		cmd_stmt->params.drop_view_params.ignore_not_exists = $3;
		cmd_stmt->params.drop_view_params.view_name = $4;
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
	SELECT DATABASE "(" ")"
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = true;
		$$ = cmd_stmt;
	}
    | SHOW DATABASES
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = false;
		$$ = cmd_stmt;
 	}
	| SHOW FULL TABLES op_from_database
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = $4;
		$$ = cmd_stmt;
	}
	| SHOW TABLES op_from_database
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = $3;
		$$ = cmd_stmt;
	}
	| SHOW COLUMNS FROM relation_factor
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = $4;
		$$ = cmd_stmt;
	}
	| SHOW INDEX FROM relation_factor
	{
		$$ = NULL;
	}
	| SHOW STATUS
	{
		$$ = NULL;
	}
	| SHOW TABLE STATIS relation_factor
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = $4;
		cmd_stmt->params.desc_table_params.is_show_table_statis = true;
		$$ = cmd_stmt;
	}
	| SHOW COLUMN STATIS relation_factor
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = $4;
		cmd_stmt->params.desc_table_params.is_show_column_statis = true;
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
		cmd_stmt->params.use_database_params.database = $2;
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
		cmd_stmt->params.desc_table_params.table = $2;
		$$ = cmd_stmt;
    }
	| DESC relation_factor
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = $2;
		$$ = cmd_stmt;
	}
  ;
   /**************************************************************
 *
 *	analyze table define
 *
 **************************************************************/
 analyze_stmt:
    ANALYZE TABLE database_name "." relation_name opt_sample_size
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = $3;
		cmd_stmt->params.analyze_params.table = $5;
		cmd_stmt->params.analyze_params.sample_size = $6;
		$$ = cmd_stmt;
    }
  | ANALYZE TABLE database_name "." "*" opt_sample_size
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = $3;
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = $6;
		$$ = cmd_stmt;
    }
  | ANALYZE TABLE "*" "." "*" opt_sample_size
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = "*";
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = $6;
		$$ = cmd_stmt;
    }
  ;
  
opt_sample_size:
	/*empty*/	{	$$ = 0.1; }
	| SAMPLE SIZE number
	{
		$$ = std::stod($3);
	}
	;

set_var_stmt:
	SET ident CMP_EQ string
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(SetVar);
		check(cmd_stmt);
		cmd_stmt->params.set_var_params.var_name = $2;
		cmd_stmt->params.set_var_params.var_value = $4;
		$$ = cmd_stmt;
	}
	;

kill_stmt:
	KILL int_value
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Kill);
		check(cmd_stmt);
		cmd_stmt->params.kill_params.thread_id = $2;
		$$ = cmd_stmt;
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
  | DUAL
	{
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_dual_table();
		check(table);
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
