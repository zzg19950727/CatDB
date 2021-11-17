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

#define make_aggr_stmt(stmt, function_name) \
{ \
	ExprStmt_s expr = AggrStmt::make_aggr_stmt(); \
	check(expr); \
	aggr = expr; \
	stmt = aggr; \
	if(function_name == "sum") \
		aggr->aggr_func = SUM; \
	else if(function_name == "avg") \
		aggr->aggr_func = AVG; \
	else if(function_name == "count") \
		aggr->aggr_func = COUNT; \
	else if(function_name == "min") \
		aggr->aggr_func = MIN; \
	else if(function_name == "max") \
		aggr->aggr_func = MAX; \
	else \
	{ \
		yyerror("unknow aggregate function %s", function_name.c_str()); \
		YYABORT; \
	} \
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
%token PLUS		"+"
%token MINUS	"-"
%token MUL		"*"
%token DIV		"/"
%token LP		"("
%token RP		")"
%token COMMA	","
%token PERIOD	"."
%token SEMICOLON ";"

%token<std::string>	STRING
%token<std::string>	IDENT
%token<std::string>	NUMERIC
%token<std::string>	TIMESTAMP
%token<std::string>	QB_NAME_IDENT
%token FALSE
%token TRUE
%token BOOL
%token NULLX
%token EXCEPT
%token UNION
%token INTERSECT
%token OR
%token AND
%token NOT
%token CMP_NE
%token CMP_GE
%token CMP_GT
%token CMP_EQ
%token CMP_LT
%token CMP_LE
%token LIKE
%token BETWEEN
%token ANALYZE
%token IN
%token IS
%token ANY
%token ALL
%token AS
%token ASC
%token BY
%token COLUMN
%token COLUMNS
%token CREATE
%token DATETIME
%token DATABASE
%token DATABASES
%token DELETE
%token DESC
%token DESCRIBE
%token DISTINCT
%token DOUBLE
%token DROP
%token DUAL
%token EXISTS
%token EXPLAIN
%token FLOAT
%token FROM
%token FULL
%token GROUP
%token HAVING
%token KILL
%token IF
%token INDEX
%token INNER
%token INSERT
%token INT
%token INTO
%token JOIN
%token LEFT
%token LIMIT
%token NUMBER
%token ORDER
%token ON
%token PROCESSLIST
%token RIGHT
%token ROWID
%token SAMPLE
%token SELECT
%token SET
%token SHOW
%token SIZE
%token STATIS
%token STATUS
%token TABLE
%token TABLES
%token UPDATE
%token USING
%token VALUES
%token VARCHAR
%token WHERE
%token TINYINT
%token SMALLINT
%token MEDIUMINT
%token BIGINT
%token DECIMAL
%token NUMERIC_SYM
%token REAL
%token CHAR
%token BINARY
%token VARBINARY
%token TIMESTAMP_SYM
%token DATE
%token TIME
%token CASE
%token WHEN
%token THEN
%token ELSE
%token END_SYM
%token BEGIN_HINT
%token END_HINT
%token QB_NAME
%token USE_HASH
%token USE_NL
%token LEADING
%token PARALLEL
%token ORDERED
%token NO_REWRITE
%token END 0

%type<Stmt_s>		sql_stmt stmt cmd_stmt select_stmt insert_stmt update_stmt delete_stmt explain_stmt explainable_stmt
%type<Stmt_s>		select_with_parens simple_select set_select sub_set_select
%type<Stmt_s>		show_stmt create_stmt drop_stmt desc_stmt use_stmt analyze_stmt set_var_stmt kill_stmt
%type<ExprStmt_s>	projection simple_expr arith_expr cmp_expr logical_expr in_expr column_ref expr_const func_expr query_ref_expr insert_value update_asgn_factor case_when_expr
%type<OrderStmt_s>	order_by
%type<LimitStmt_s>	opt_select_limit
%type<TableStmt_s>	table_factor sub_table_factor basic_table_factor view_table_factor joined_table_factor
%type<bool>			opt_distinct opt_asc_desc  distinct_or_all opt_if_exists
%type<int>			limit_expr data_type int_value
%type<double>		opt_sample_size
%type<std::string>	op_from_database column_label database_name relation_name opt_alias column_name function_name ident string datetime number
%type<Vector<TableStmt_s>>	from_list 
%type<BasicTableStmt_s> 	relation_factor
%type<Vector<OrderStmt_s>>	opt_order_by order_by_list
%type<Vector<ExprStmt_s>>	select_expr_list opt_groupby arith_expr_list opt_where opt_having insert_value_list update_asgn_list when_then_list1 when_then_list2
%type<ColumnDefineStmt_s>	column_definition
%type<Vector<ColumnDefineStmt_s>>	table_element_list
%type<Hint> opt_hint
%type<Vector<HintStmt_s>> hint_list
%type<HintStmt_s> single_hint
%type<Vector<String>> hint_table_list
%type<Vector<LeadingTable_s>> leading_hint_table_list
%type<LeadingTable_s> leading_hint_table
%type<OperationType> cmp_type
%type<bool> opt_split
%type<std::string> opt_qb_name opt_qb_name_single
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
	| BEGIN_HINT hint_list END_HINT
	{
		$$ = Hint();
		$$.all_hints = $2;
	}
	| BEGIN_HINT END_HINT
	{
		$$ = Hint();
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
	QB_NAME "(" ident ")"
	{
		$$ = HintStmt::make_hint_stmt(HintStmt::QB_NAME);
		$$->set_qb_name($3);
	}
	| NO_REWRITE opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(HintStmt::NO_REWRITE);
		$$->set_qb_name($2);
	}
	| ORDERED opt_qb_name_single
	{
		$$ = HintStmt::make_hint_stmt(HintStmt::LEADING);
		$$->set_qb_name($2);
		LeadingHintStmt_s leading = $$;
		leading->set_is_ordered();
	}
	| USE_HASH "(" opt_qb_name hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(HintStmt::JOIN);
		$$->set_qb_name($3);
		JoinHintStmt_s join_hint = $$;
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = $4;
	}
	| USE_NL "(" opt_qb_name hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(HintStmt::JOIN);
		$$->set_qb_name($3);
		JoinHintStmt_s join_hint = $$;
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = $4;
	}
	| LEADING "(" opt_qb_name leading_hint_table_list ")"
	{
		$$ = HintStmt::make_hint_stmt(HintStmt::LEADING);
		$$->set_qb_name($3);
		LeadingHintStmt_s leading = $$;
		leading->tables = LeadingTable::make_leading_table();
		leading->tables->is_base_table = false;
		leading->tables->table_list = $4;
	}
	| PARALLEL "(" int_value ")"
	{
		$$ = HintStmt::make_hint_stmt(HintStmt::PARALLEL);
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
   table_factor LEFT JOIN sub_table_factor ON logical_expr
   {
	   $$ = JoinedTableStmt::make_joined_table($1, $4, LeftOuter, $6);
   }
   | table_factor RIGHT JOIN sub_table_factor ON logical_expr
   {
	   $$ = JoinedTableStmt::make_joined_table($1, $4, RightOuter, $6);
   }
   | table_factor FULL JOIN sub_table_factor ON logical_expr
   {
	   $$ = JoinedTableStmt::make_joined_table($1, $4, FullOuter, $6);
   }
   | table_factor INNER JOIN sub_table_factor ON logical_expr
   {
	   $$ = JoinedTableStmt::make_joined_table($1, $4, Inner, $6);
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
  | arith_expr cmp_type ANY query_ref_expr 
	{
		SubQueryStmt_s query_expr = $4;
		query_expr->is_any = true;
		query_expr->output_one_row = false;
		//构建比较二元表达式 
		make_binary_stmt($$, $1, $4, $2);
	}
  | arith_expr cmp_type ALL query_ref_expr 
	{
		SubQueryStmt_s query_expr = $4;
		query_expr->is_all = true;
		query_expr->output_one_row = false;
		//构建比较二元表达式 
		make_binary_stmt($$, $1, $4, $2);
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
  | arith_expr IN in_expr
    {
		//构建in表达式
		make_binary_stmt($$, $1, $3, OP_IN);
    }
  | arith_expr NOT IN in_expr
    {
		//构建not in表达式
		make_binary_stmt($$, $1, $4, OP_NOT_IN);
    }
  | EXISTS query_ref_expr
    {
		SubQueryStmt_s query_expr = $2;
		query_expr->output_one_row = false;
    	make_unary_stmt($$, $2, OP_EXISTS);
    }
  | NOT EXISTS query_ref_expr
    {
		SubQueryStmt_s query_expr = $3;
		query_expr->output_one_row = false;
		//构建not一元表达式
		make_unary_stmt($$, $3, OP_NOT_EXISTS);
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

in_expr:
    query_ref_expr
    {
		SubQueryStmt_s query_expr = $1;
		query_expr->output_one_row = false;
		$$ = $1;
    }
  | "(" arith_expr_list ")"
    { 
		ListStmt_s list_stmt = ListStmt::make_list_stmt();
		Vector<ExprStmt_s> &exprs = $2;
		for (u32 i = 0; i < exprs.size(); ++i) {
			list_stmt->push_back(exprs[i]);
		}
		$$ = list_stmt;
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
  | FALSE
	{
		//构建常量表达式
		Object_s value = Bool::make_object(false);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
	}
  | TRUE
	{
		//构建常量表达式
		Object_s value = Bool::make_object(true);
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
    function_name "(" "*" ")"
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, $1);
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->set_aggr_expr(col);
		$$ = aggr;
    }
  | function_name "(" distinct_or_all arith_expr ")"
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, $1);
		aggr->distinct = $3;
		aggr->set_aggr_expr($4);
		$$ = aggr;
    }
  | function_name "(" arith_expr ")"
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, $1);
		aggr->set_aggr_expr($3);
		$$ = aggr;
    }
  ;

distinct_or_all:
    ALL
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
		$$ = Vector<ExprStmt_s>();
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
		//构建值列表
		make_list($$, $1);
	}
  | insert_value "," arith_expr
    {
		//将新的表达式加入到表达式列表
		list_push($$, $1, $3);
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
		make_binary_stmt($$, col, $3, OP_EQ);
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
    CREATE TABLE relation_factor "(" table_element_list ")"
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::CreateTable);
		check(cmd_stmt);
		cmd_stmt->params.create_table_params.table = $3;
		cmd_stmt->params.create_table_params.column_define_list = $5;
		$$ = cmd_stmt;
    }
	| CREATE DATABASE database_name
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::CreateDatabase);
		check(cmd_stmt);
		cmd_stmt->params.create_database_params.database = $3;
		$$ = cmd_stmt;
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
    column_name data_type
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt($1, $2);
		check(stmt);
		$$ = stmt;
    }
  ;

data_type:
  TINYINT
    { $$ = ColumnDefineStmt::NUMBER; }
  | SMALLINT
    { $$ = ColumnDefineStmt::NUMBER; }
  | MEDIUMINT
    { $$ = ColumnDefineStmt::NUMBER; }
  | INT
    { $$ = ColumnDefineStmt::NUMBER; }
  | BIGINT
    { $$ = ColumnDefineStmt::NUMBER; }
  | DECIMAL opt_decimal
    { $$ = ColumnDefineStmt::NUMBER; }
  | NUMERIC_SYM opt_decimal
    { $$ = ColumnDefineStmt::NUMBER; }
  | BOOL
    { $$ = ColumnDefineStmt::NUMBER; }
  | FLOAT opt_float
    { $$ = ColumnDefineStmt::NUMBER; }
  | REAL
    { $$ = ColumnDefineStmt::NUMBER; }
  | DOUBLE opt_decimal
    { $$ = ColumnDefineStmt::NUMBER; }
  | CHAR opt_char_length
    { $$ = ColumnDefineStmt::VARCHAR; }
  | BINARY opt_char_length
    { $$ = ColumnDefineStmt::VARCHAR; }
  | VARCHAR opt_char_length
    { $$ = ColumnDefineStmt::VARCHAR; }
  | VARBINARY opt_char_length
    { $$ = ColumnDefineStmt::VARCHAR; }
  | TIMESTAMP_SYM opt_time_precision
    { $$ = ColumnDefineStmt::DATETIME; }
  | DATETIME
    { $$ = ColumnDefineStmt::DATETIME; }
  | DATE
    { $$ = ColumnDefineStmt::DATETIME; }
  | TIME opt_time_precision
    { $$ = ColumnDefineStmt::DATETIME; }
  ;
  
opt_decimal:
    "(" number "," number ")" 	{ }
  | "(" number ")" 				{ }
  | /*EMPTY*/ 					{ }
  ;

opt_float:
    "(" number ")"    { }
  | /*EMPTY*/         { }
  ;

opt_time_precision:
    "(" number ")"    { }
  | /*EMPTY*/         { }
  ;

opt_char_length:
    "(" number ")"    { }
  | /*EMPTY*/         { }
  ;

/**************************************************************
 *
 *	drop define
 *
 **************************************************************/
 drop_stmt:
    DROP TABLE opt_if_exists table_factor
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DropTable);
		check(cmd_stmt);
		cmd_stmt->params.drop_table_params.ignore_not_exists = $3;
		cmd_stmt->params.drop_table_params.table = $4;
		$$ = cmd_stmt;
    }
	| DROP DATABASE opt_if_exists database_name
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DropDatabase);
		check(cmd_stmt);
		cmd_stmt->params.drop_database_params.ignore_not_exists = $3;
		cmd_stmt->params.drop_database_params.database = $4;
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
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = false;
		$$ = cmd_stmt;
 	}
	| SHOW FULL TABLES op_from_database
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = $4;
		$$ = cmd_stmt;
	}
	| SHOW TABLES op_from_database
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = $3;
		$$ = cmd_stmt;
	}
	| SHOW COLUMNS FROM relation_factor
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
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
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = $4;
		cmd_stmt->params.desc_table_params.is_show_table_statis = true;
		$$ = cmd_stmt;
	}
	| SHOW COLUMN STATIS relation_factor
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = $4;
		cmd_stmt->params.desc_table_params.is_show_column_statis = true;
		$$ = cmd_stmt;
	}
	| SHOW PROCESSLIST
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowProcesslist);
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
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::UseDatabase);
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
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = $2;
		$$ = cmd_stmt;
    }
	| DESC relation_factor
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
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
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = $3;
		cmd_stmt->params.analyze_params.table = $5;
		cmd_stmt->params.analyze_params.sample_size = $6;
		$$ = cmd_stmt;
    }
  | ANALYZE TABLE database_name "." "*" opt_sample_size
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = $3;
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = $6;
		$$ = cmd_stmt;
    }
  | ANALYZE TABLE "*" "." "*" opt_sample_size
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::Analyze);
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
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::SetVar);
		check(cmd_stmt);
		cmd_stmt->params.set_var_params.var_name = $2;
		cmd_stmt->params.set_var_params.var_value = $4;
		$$ = cmd_stmt;
	}
	;

kill_stmt:
	KILL int_value
	{
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::Kill);
		check(cmd_stmt);
		cmd_stmt->params.kill_params.pid = $2;
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

function_name:
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
