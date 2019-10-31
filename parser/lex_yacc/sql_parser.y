%skeleton "lalr1.cc"
//声明命名空间
%define api.namespace {CatDB} 
%define api.parser.class { SqlParser }
%define api.token.constructor
//使得类型与token定义可以使用各种复杂的结构与类型
%define api.value.type variant 
//开启断言功能
%define parse.assert  
%defines
%code requires
{
	# define YYDEBUG 1
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
	#include "create_stmt.h"
	#include "drop_stmt.h"
	#include "show_stmt.h"
	#include "expr_stmt.h"
	#include "object.h"
	
	/*注意：这里的参数由%parse-param决定*/
	static CatDB::SqlParser::symbol_type yylex(CatDB::SqlScanner& scanner,CatDB::SqlDriver &driver)
	{
		return scanner.get_next_token();
	}
	using namespace CatDB;
	using namespace CatDB::Parser;
	using namespace CatDB::Common;
	extern String g_database;
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

#define make_list_stmt(ret, from) \
	Stmt_s stmt = ListStmt::make_list_stmt(); \
	check(stmt); \
	ListStmt* list = dynamic_cast<ListStmt*>(stmt.get()); \
	check(list); \
	list->stmt_list.push_back(from); \
	ret = stmt;

#define list_stmt_push(ret, list, from) \
	ListStmt* stmt = dynamic_cast<ListStmt*>(list.get()); \
	check(stmt); \
	stmt->stmt_list.push_back(from); \
	ret = list;

#define make_aggr_stmt(stmt, function_name) \
{ \
	stmt = AggrStmt::make_aggr_stmt(); \
	check(stmt); \
	AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get()); \
	if(function_name == "sum") \
		aggr->aggr_func = AggrStmt::SUM; \
	else if(function_name == "avg") \
		aggr->aggr_func = AggrStmt::AVG; \
	else if(function_name == "count") \
		aggr->aggr_func = AggrStmt::COUNT; \
	else if(function_name == "min") \
		aggr->aggr_func = AggrStmt::MIN; \
	else if(function_name == "max") \
		aggr->aggr_func = AggrStmt::MAX; \
	else \
	{ \
		yyerror("unknow aggregate function %s", function_name.c_str()); \
		YYABORT; \
	} \
}

#define make_unary_stmt(stmt, stmt1, op) \
	stmt = UnaryExprStmt::make_unary_stmt(); \
	check(stmt); \
	UnaryExprStmt* unary = dynamic_cast<UnaryExprStmt*>(stmt.get()); \
	unary->expr_stmt = stmt1; \
	unary->op_type = op;

#define make_binary_stmt(stmt, stmt1, stmt2, op) \
	stmt = BinaryExprStmt::make_binary_stmt(); \
	check(stmt); \
	BinaryExprStmt* binary = dynamic_cast<BinaryExprStmt*>(stmt.get()); \
	binary->first_expr_stmt = stmt1; \
	binary->second_expr_stmt = stmt2; \
	binary->op_type = op;

#define make_ternary_stmt(stmt, stmt1, stmt2, stmt3, op) \
	stmt = TernaryExprStmt::make_ternary_stmt(); \
	check(stmt); \
	TernaryExprStmt* ternary = dynamic_cast<TernaryExprStmt*>(stmt.get()); \
	ternary->first_expr_stmt = stmt1; \
	ternary->second_expr_stmt = stmt2; \
	ternary->third_expr_stmt = stmt3; \
	ternary->op_type = op;
	
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
%right UMINUS
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
%token AP		 "'"

%token<std::string>	STRING
%token<std::string>	IDENT
%token<std::string>	NUMERIC
%token<std::string>	TIMESTAMP
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
%token IN
%token IS
%token ANY
%token ALL
%token AS
%token ASC
%token BY
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
%token EXISTS
%token EXPLAIN
%token FLOAT
%token FROM
%token GROUP
%token HAVING
%token INSERT
%token INT
%token INTO
%token LIMIT
%token NUMBER
%token ORDER
%token SELECT
%token SET
%token SHOW
%token TABLE
%token TABLES
%token UPDATE
%token USING
%token VALUES
%token VARCHAR
%token WHERE
%token END 0

%type<Stmt_s>		sql_stmt stmt select_stmt select_with_parens select_no_parens
%type<int>			limit_expr data_type
%type<bool>			opt_distinct opt_asc_desc  distinct_or_all
%type<Stmt_s>		select_expr_list from_list opt_where opt_groupby opt_having
%type<Stmt_s>		opt_order_by opt_select_limit order_by
%type<Stmt_s>		projection table_factor relation_factor 
%type<Stmt_s>		expr_list expr simple_expr arith_expr in_expr column_ref expr_const func_expr
%type<Stmt_s>		insert_stmt insert_vals_list insert_vals
%type<Stmt_s>		update_stmt update_asgn_list update_asgn_factor delete_stmt show_stmt
%type<Stmt_s>		explain_stmt explainable_stmt drop_stmt desc_stmt use_stmt
%type<Stmt_s>		create_stmt table_element_list table_element column_definition
%type<std::string>	column_label database_name relation_name column_name function_name ident string datetime
%type<double>		number
%start sql_stmt
%%

sql_stmt:
	stmt
    {
		driver.result = $1;
		YYACCEPT;
    }
	;

stmt:
    select_stmt			{ $$ = $1; }
  | insert_stmt			{ $$ = $1; }
  | update_stmt			{ $$ = $1; }
  | delete_stmt			{ $$ = $1; }
  | explain_stmt		{ $$ = $1; }
  | show_stmt			{ $$ = $1; }
  | create_stmt			{ $$ = $1; }
  | drop_stmt			{ $$ = $1; }
  | desc_stmt			{ $$ = $1; }
  | use_stmt			{ $$ = $1; }
  | /*EMPTY*/			{ yyerror("unknow stmt"); }
	;

/**************************************************************
 *
 *	select define
 *
 **************************************************************/
select_stmt:
	select_no_parens				
	{ 
		$$ = $1;
	}
	;

select_with_parens:
	"(" select_no_parens ")"		
	{
		Stmt_s stmt = QueryStmt::make_query_stmt();
		QueryStmt* query = dynamic_cast<QueryStmt*>(stmt.get());
		query->query_stmt = $2;
		$$ = stmt;
	}
	;

select_no_parens:
	SELECT opt_distinct select_expr_list
    FROM from_list
    opt_where opt_groupby opt_having
    opt_order_by opt_asc_desc opt_select_limit
    {
		//构建select stmt
		Stmt_s stmt = SelectStmt::make_select_stmt();
		SelectStmt* select_stmt = dynamic_cast<SelectStmt*>(stmt.get());
		select_stmt->is_distinct = $2;
		select_stmt->select_expr_list = $3;
		select_stmt->from_stmts = $5;
		select_stmt->where_stmt = $6;
		select_stmt->group_columns = $7;
		select_stmt->having_stmt = $8;
		select_stmt->order_columns = $9;
		select_stmt->asc_desc = $10;
		select_stmt->limit_stmt = $11;
		$$ = stmt;
    }
  | select_with_parens UNION select_with_parens
    {
		//构建union二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_UNION);
    }
  | select_with_parens UNION ALL select_with_parens
    {
		//构建union all二元表达式
		make_binary_stmt($$, $1, $4, ExprStmt::OP_UNION_ALL);
    }
  | select_with_parens INTERSECT select_with_parens
    {
		//构建intersect二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_INTERSECT);
    }
  | select_with_parens EXCEPT select_with_parens
    {
		//构建except二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_EXCEPT);
    }
	;

opt_distinct:
    /* EMPTY */		{ $$ = false; }
  | DISTINCT		{ $$ = true; }
  ;

select_expr_list:
    projection
    {
		//构建select list
		make_list_stmt($$, $1);
    }
  | select_expr_list "," projection
    {
		//把输出表达式加入到select list
		list_stmt_push($$, $1, $3);
    }
  ;

from_list:
  	table_factor
    {
		//构建from list 
		make_list_stmt($$, $1);
	}
  | from_list "," table_factor
    {
		//把表加入到from list
		list_stmt_push($$, $1, $3);
	}
  ;

opt_where:
    WHERE expr		{ $$ = $2; }
  | /* EMPTY */		{ $$ = NULL; }
  ;

opt_groupby:
    GROUP BY expr_list	{ $$ = $3; }
  | /* EMPTY */			{ $$ = NULL; }
  ;

opt_having:
    HAVING expr		{ $$ = $2; }
  | /* EMPTY */		{ $$ = NULL; }
  ;

opt_order_by:
	/*EMPTY*/             { $$ = NULL; }
  | order_by	          { $$ = $1; }
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
		Stmt_s stmt = LimitStmt::make_limit_stmt($2, $4);
		check(stmt);
		$$ = stmt;
    }
  | LIMIT limit_expr
	{
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt($2);
		check(stmt);
		$$ = stmt;
	}
	;

limit_expr:
    number
    {
		$$ = $1;
	}
	;

order_by:
  	ORDER BY expr_list
    {
		$$ = $3;
    }
  ;

projection:
    expr
    {
	//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>($1.get());
		check(stmt);
		stmt->alias_name = stmt->to_string();
		$$ = $1;
    }
  | expr column_label
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>($1.get());
		check(stmt);
		stmt->alias_name = $2;
		$$ = $1;
    }
  | expr AS column_label
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>($1.get());
		check(stmt);
		stmt->alias_name = $3;
		$$ = $1;
    }
  ;

table_factor:
    relation_factor
    {
		$$ = $1;
    }
  | relation_factor AS relation_name
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>($1.get());
		check(stmt);
		stmt->alias_name = $3;
		$$ = $1;
    }
  | relation_factor relation_name
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>($1.get());
		check(stmt);
		stmt->alias_name = $2;
		$$ = $1;
    }
  | select_with_parens AS relation_name
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>($1.get());
		check(stmt);
		stmt->alias_name = $3;
		$$ = $1;
    }
  | select_with_parens relation_name
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>($1.get());
		check(stmt);
		stmt->alias_name = $2;
		$$ = $1;
    }
  ;
/**************************************************************
 *
 *	expression define
 *
 **************************************************************/
 expr_list:
    expr
    {
		//构建表达式列表
		make_list_stmt($$, $1);
    }
  | expr_list "," expr
    {
		//将新的表达式加入到表达式列表
		list_stmt_push($$, $1, $3);
    }
  ;

expr:
    simple_expr   
	{ 
		$$ = $1;
	}
  | "+" expr %prec UMINUS
    {
		//正数表达式
		$$ = $2;
    }
  | "-" expr %prec UMINUS
    {
		//负数表达式
		make_unary_stmt($$, $2, ExprStmt::OP_MINUS);
    }
  | expr "+" expr 
	{
		//构建加法二元表达式 
		make_binary_stmt($$, $1, $3, ExprStmt::OP_ADD);
	}
  | expr "-" expr 
	{
		//构建减法二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_SUB);
	}
  | expr "*" expr 
	{
		//构建乘法二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_MUL);
	}
  | expr "/" expr 
	{
		//构建除法二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_DIV);
	}
  | expr CMP_LE expr 
	{
		//构建比较二元表达式 
		make_binary_stmt($$, $1, $3, ExprStmt::OP_LE);
	}
  | expr CMP_LT expr 
	{
		//构建比较二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_LT);
	}
  | expr CMP_EQ expr
	{
		//构建比较二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_EQ);
	}
  | expr CMP_GE expr 
	{
		//构建比较二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_GE);
	}
  | expr CMP_GT expr 
	{
		//构建比较二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_GT);
	}
  | expr CMP_NE expr 
	{
		//构建比较二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_NE);
	}
  | expr LIKE expr 
	{
		//构建比较二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_LIKE);
	}
  | expr NOT LIKE expr 
	{
		//构建比较二元表达式
		make_binary_stmt($$, $1, $4, ExprStmt::OP_NOT_LIKE);
	}
  | expr AND expr %prec AND
    {
		//构建and二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_AND);
    }
  | expr OR expr %prec OR
    {
		//构建or二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_OR);
    }
  | NOT expr
    {
		//构建not一元表达式
		make_unary_stmt($$, $2, ExprStmt::OP_NOT);
    }
  | expr IS NULLX
    {
		//构建is null表达式
		make_unary_stmt($$, $1, ExprStmt::OP_IS_NULL);
    }
  | expr IS NOT NULLX
    {
		//构建is not null表达式
		make_unary_stmt($$, $1, ExprStmt::OP_IS_NOT_NULL);
    }
  | expr BETWEEN arith_expr AND arith_expr %prec BETWEEN
    {
		//构建between and三元表达式
		make_ternary_stmt($$, $1, $3, $5, ExprStmt::OP_BETWEEN);
    }
  | expr NOT BETWEEN arith_expr AND arith_expr %prec BETWEEN
    {
		//构建not between and三元表达式
		make_ternary_stmt($$, $1, $4, $6, ExprStmt::OP_NOT_BETWEEN);
    }
  | expr IN in_expr
    {
		//构建in表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_IN);
    }
  | expr NOT IN in_expr
    {
		//构建not in表达式
		make_binary_stmt($$, $1, $4, ExprStmt::OP_NOT_IN);
    }
  ;

in_expr:
    select_with_parens
    {
    	$$ = $1;
    }
  | "(" expr_list ")"
    { 
		$$ = $2;
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
		make_unary_stmt($$, $2, ExprStmt::OP_MINUS);
    }
  | arith_expr "+" arith_expr 
	{
		//构建二元表达式 
		make_binary_stmt($$, $1, $3, ExprStmt::OP_ADD);
	}
  | arith_expr "-" arith_expr 
	{
		//构建二元表达式 
		make_binary_stmt($$, $1, $3, ExprStmt::OP_SUB);
	}
  | arith_expr "*" arith_expr 
	{
		//构建二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_MUL);
	}
  | arith_expr "/" arith_expr 
	{
		//构建二元表达式
		make_binary_stmt($$, $1, $3, ExprStmt::OP_DIV);
	}
  ;

simple_expr:
    column_ref
    { 
		$$ = $1;
	}
  | expr_const
    { 
		 $$ = $1;
	}
  | "(" expr ")"
    { 
		$$ = $2;
	}
  | func_expr
    {
      	$$ = $1;
    }
  | select_with_parens %prec UMINUS
    {
    	$$ = $1;
    }
  | EXISTS select_with_parens
    {
    	make_unary_stmt($$, $2, ExprStmt::OP_EXISTS);
    }
  ;

column_ref:
    column_name
    { 
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", $1);
		check(col);
		$$ = col;
	}
  | "*"
	{
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		$$ = col;
	}
  | relation_name "." column_name
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt($1, $3);
		check(col);
		$$ = col;
    }
  |
    relation_name "." "*"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt($1, "*");
		check(col);
		$$ = col;
    }
  ;

expr_const:
     string
	{
		//构建常量表达式
		Object_s value = Varchar::make_object($1);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
	}
  | datetime
	{
		//构建常量表达式
		Object_s value = DateTime::make_object($1);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
	}
  | number
	{
		//构建常量表达式
		Object_s value = Number::make_object($1);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
	}
  | FALSE
	{
		//构建常量表达式
		Object_s value = Bool::make_object(false);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
	}
  | TRUE
	{
		//构建常量表达式
		Object_s value = Bool::make_object(true);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
	}
  | NULLX
	{
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		$$ = stmt;
	}
  ;
  
func_expr:
    function_name "(" "*" ")"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, $1);
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->aggr_expr = col;
		$$ = stmt;
    }
  | function_name "(" distinct_or_all expr ")"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, $1);
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->distinct = $3;
		aggr->aggr_expr = $4;
		$$ = stmt;
    }
  | function_name "(" expr ")"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, $1);
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->aggr_expr = $3;
		$$ = stmt;
    }
  | function_name "(" ")"
    {
		//sys function
		yyerror("system function %s not support yet", $1.c_str());
		YYABORT;
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
    INSERT INTO relation_factor VALUES insert_vals_list
    {
		//构建insert stmt
		Stmt_s stmt = InsertStmt::make_insert_stmt();
		check(stmt);
		InsertStmt* insert_stmt = dynamic_cast<InsertStmt*>(stmt.get());
		insert_stmt->table = $3;
		insert_stmt->values = $5;
		$$ = stmt;
    }
  | INSERT INTO relation_factor select_stmt
    {
		yyerror("insert into table from query not support yet!");
		YYABORT;
    }
  ;
insert_vals_list:
    "(" insert_vals ")"
    {
		make_list_stmt($$, $2);
    }
  | insert_vals_list "," "(" insert_vals ")" 
	{
		list_stmt_push($$, $1, $4);
	}

insert_vals:
    expr 
	{ 
		//构建值列表
		make_list_stmt($$, $1);
	}
  | insert_vals "," expr
    {
		//将新的表达式加入到表达式列表
		list_stmt_push($$, $1, $3);
    }
  ;

/**************************************************************
 *
 *	update dml define
 *
 **************************************************************/
 update_stmt:
    UPDATE relation_factor SET update_asgn_list opt_where
    {
		Stmt_s stmt = UpdateStmt::make_update_stmt();
		check(stmt);
		UpdateStmt* update_stmt = dynamic_cast<UpdateStmt*>(stmt.get());
		update_stmt->table = $2;
		update_stmt->update_asgn_stmt = $4;
		update_stmt->where_stmt = $5;
		$$ = stmt;
    }
  ;

update_asgn_list:
    update_asgn_factor
    {
		//构建值列表
		make_list_stmt($$, $1);
    }
  | update_asgn_list ',' update_asgn_factor
    {
		//将新的表达式加入到表达式列表
		list_stmt_push($$, $1, $3);
    }
  ;

update_asgn_factor:
    column_name CMP_EQ expr
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", $1);
		check(col);
		make_binary_stmt($$, col, $3, ExprStmt::OP_EQ);
    }
  ;

 /**************************************************************
 *
 *	delete define
 *
 **************************************************************/
delete_stmt:
    DELETE FROM relation_factor opt_where
    {
		Stmt_s stmt = DeleteStmt::make_delete_stmt();
		check(stmt);
		DeleteStmt* delete_stmt = dynamic_cast<DeleteStmt*>(stmt.get());
		delete_stmt->table = $3;
		delete_stmt->where_stmt = $4;
		$$ = stmt;
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
		yyerror("explain not support yet!");
		YYABORT;
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
		Stmt_s stmt = CreateTableStmt::make_create_table_stmt();
		check(stmt);
		CreateTableStmt* create_table_stmt = dynamic_cast<CreateTableStmt*>(stmt.get());
		create_table_stmt->table = $3;
		create_table_stmt->column_define_list = $5;
		$$ = stmt;
    }
	| CREATE DATABASE database_name
	{
		Stmt_s stmt = CreateDatabaseStmt::make_create_database_stmt();
		check(stmt);
		CreateDatabaseStmt* create_database_stmt = dynamic_cast<CreateDatabaseStmt*>(stmt.get());
		create_database_stmt->database = $3;
		$$ = stmt;
	}
  ;

table_element_list:
    table_element
    {
		make_list_stmt($$, $1);
    }
  | table_element_list "," table_element
    {
		list_stmt_push($$, $1, $3);
    }
  ;

table_element:
    column_definition
    {
		$$ = $1;
    }
  ;

column_definition:
    column_name data_type
    {
		Stmt_s stmt = ColumnDefineStmt::make_column_define_stmt($1, $2);
		check(stmt);
		$$ = stmt;
    }
  ;

data_type:
    NUMBER
    {
		$$ = ColumnDefineStmt::NUMBER;
    }
  | DATETIME
    { 
		$$ = ColumnDefineStmt::DATETIME;
	}
  | VARCHAR
    {
		$$ = ColumnDefineStmt::VARCHAR;
    }
  | INT
	{
		$$ = ColumnDefineStmt::INT;
	}
  ;

/**************************************************************
 *
 *	drop define
 *
 **************************************************************/
 drop_stmt:
    DROP TABLE table_factor
    {
		Stmt_s stmt = DropTableStmt::make_drop_table_stmt();
		check(stmt);
		DropTableStmt* drop_table_stmt = dynamic_cast<DropTableStmt*>(stmt.get());
		drop_table_stmt->table = $3;
		$$ = stmt;
    }
	| DROP DATABASE database_name
	{
		Stmt_s stmt = DropDatabaseStmt::make_drop_database_stmt();
		check(stmt);
		DropDatabaseStmt* drop_database_stmt = dynamic_cast<DropDatabaseStmt*>(stmt.get());
		drop_database_stmt->database = $3;
		$$ = stmt;
	}
  ;

/**************************************************************
 *
 *	show define
 *
 **************************************************************/
 show_stmt:
    SHOW DATABASES
    {
		$$ = ShowDatabasesStmt::make_show_databases_stmt();
    }
	| SHOW TABLES
	{
		$$ = ShowTablesStmt::make_show_tables_stmt();
	}
  ;
 use_stmt:
	USING database_name
	{
		Stmt_s stmt = UseDatabaseStmt::make_use_database_stmt();
		check(stmt);
		UseDatabaseStmt* use_database_stmt = dynamic_cast<UseDatabaseStmt*>(stmt.get());
		use_database_stmt->database = $2;
		$$ = stmt;
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
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = $2;
		$$ = stmt;
    }
	| DESC relation_factor
	{
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = $2;
		$$ = stmt;
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
		Stmt_s table = TableStmt::make_table_stmt(g_database, $1);
		check(table);
		$$ = table;
	}
  | database_name "." relation_name
	{
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt($1, $3);
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
	ident		{ str_to_lower($1);$$=$1; }
	;

ident:
	IDENT		{ $$ = $1; }
	;

string:
	STRING 		{ $$ = $1; }
  ;	

datetime:
	"'" TIMESTAMP "'"	{ $$ = $2; }
  ;

number:
	NUMERIC		{ $$ = std::stod($1); }

%%

void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
