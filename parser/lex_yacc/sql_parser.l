%option c++
%option noyywrap debug
%option yyclass="SqlScanner"
%option prefix="CatDB_"

%{
	#include "sql_parser.hpp"	//包含由parser.l生成的头文件
	#include "sql_scanner.h" 	//包含yyFlexLexer子类的头文件
	#include "location.hh" 	//包含位置调试信息头文件
	# define YY_USER_ACTION  loc.columns (yyleng);
	#define yyterminate() CatDB::SqlParser::make_END(loc);
	# undef yywrap
  	# define yywrap() 1
	#define YY_NO_UNISTD_H
  	static CatDB::location loc;	//声明位置实例
%}

letter 	[A-Za-z_]
digit	[0-9]
num1    {digit}+(\.)?([eE][-]?{digit}+)?
num2    {digit}*(\.)?{digit}+([eE][-]?{digit}+)?
number  {num1}|{num2}
str		{letter}({letter}|{digit})*
date	[0-9]{4}(-[0-9]{1,2}){2}
time	[0-9]{2}(:[0-9]{1,2}){2}
space	[ \t\n\r\f]+
timestamp	[0-9]{4}(-[0-9]{1,2}){2}[ ][0-9]{2}(:[0-9]{1,2}){2}
%%

"bool"			{ return CatDB::SqlParser::make_BOOL(loc); }
"null"			{ return CatDB::SqlParser::make_NULLX(loc); }
"false"			{ return CatDB::SqlParser::make_FALSE(loc); }
"true"			{ return CatDB::SqlParser::make_TRUE(loc); }
"except"		{ return CatDB::SqlParser::make_EXCEPT(loc); }
"union"			{ return CatDB::SqlParser::make_UNION(loc); }
"intersect"		{ return CatDB::SqlParser::make_INTERSECT(loc); }
"or"			{ return CatDB::SqlParser::make_OR(loc); }
"and"			{ return CatDB::SqlParser::make_AND(loc); }
"not"			{ return CatDB::SqlParser::make_NOT(loc); }
"!="			{ return CatDB::SqlParser::make_CMP_NE(loc); }
">="			{ return CatDB::SqlParser::make_CMP_GE(loc); }
">"				{ return CatDB::SqlParser::make_CMP_GT(loc); }
"="				{ return CatDB::SqlParser::make_CMP_EQ(loc); }
"<"				{ return CatDB::SqlParser::make_CMP_LT(loc); }
"<="			{ return CatDB::SqlParser::make_CMP_LE(loc); }
"like"			{ return CatDB::SqlParser::make_LIKE(loc); }
"between"		{ return CatDB::SqlParser::make_BETWEEN(loc); }
"in"			{ return CatDB::SqlParser::make_IN(loc); }
"is"			{ return CatDB::SqlParser::make_IS(loc); }
"any"			{ return CatDB::SqlParser::make_ANY(loc); }
"all"			{ return CatDB::SqlParser::make_ALL(loc); }
"as"			{ return CatDB::SqlParser::make_AS(loc); }
"asc"			{ return CatDB::SqlParser::make_ASC(loc); }
"by"			{ return CatDB::SqlParser::make_BY(loc); }
"create"		{ return CatDB::SqlParser::make_CREATE(loc); }
"datetime"		{ return CatDB::SqlParser::make_DATETIME(loc); }
"delete"		{ return CatDB::SqlParser::make_DELETE(loc); }
"desc"			{ return CatDB::SqlParser::make_DESC(loc); }
"describe"		{ return CatDB::SqlParser::make_DESCRIBE(loc); }
"distinct"		{ return CatDB::SqlParser::make_DISTINCT(loc); }
"double"		{ return CatDB::SqlParser::make_DOUBLE(loc); }
"drop"			{ return CatDB::SqlParser::make_DROP(loc); }
"exists"		{ return CatDB::SqlParser::make_EXISTS(loc); }
"explain"		{ return CatDB::SqlParser::make_EXPLAIN(loc); }
"float"			{ return CatDB::SqlParser::make_FLOAT(loc); }
"from"			{ return CatDB::SqlParser::make_FROM(loc); }
"group"			{ return CatDB::SqlParser::make_GROUP(loc); }
"having"		{ return CatDB::SqlParser::make_HAVING(loc); }
"insert"		{ return CatDB::SqlParser::make_INSERT(loc); }
"into"			{ return CatDB::SqlParser::make_INTO(loc); }
"limit"			{ return CatDB::SqlParser::make_LIMIT(loc); }
"number"		{ return CatDB::SqlParser::make_NUMBER(loc); }
"order"			{ return CatDB::SqlParser::make_ORDER(loc); }
"select"		{ return CatDB::SqlParser::make_SELECT(loc); }
"set"			{ return CatDB::SqlParser::make_SET(loc); }
"show"			{ return CatDB::SqlParser::make_SHOW(loc); }
"table"			{ return CatDB::SqlParser::make_TABLE(loc); }
"tables"		{ return CatDB::SqlParser::make_TABLES(loc); }
"update"		{ return CatDB::SqlParser::make_UPDATE(loc); }
"using"			{ return CatDB::SqlParser::make_USING(loc); }
"values"		{ return CatDB::SqlParser::make_VALUES(loc); }
"varchar"		{ return CatDB::SqlParser::make_VARCHAR(loc); }
"where"			{ return CatDB::SqlParser::make_WHERE(loc); }
"("				{ return CatDB::SqlParser::make_LP(loc); }
")"				{ return CatDB::SqlParser::make_RP(loc); }
","				{ return CatDB::SqlParser::make_COMMA(loc); }
"."				{ return CatDB::SqlParser::make_PERIOD(loc); }
";"				{ return CatDB::SqlParser::make_SEMICOLON(loc); }
"+"				{ return CatDB::SqlParser::make_PLUS(loc); }
"-"				{ return CatDB::SqlParser::make_MINUS(loc); }
"*"				{ return CatDB::SqlParser::make_MUL(loc); }
"/"				{ return CatDB::SqlParser::make_DIV(loc); }
"'"				{ return CatDB::SqlParser::make_AP(loc); }
"\""			{ return CatDB::SqlParser::make_DQ(loc); }

{space}
{number}		{ return CatDB::SqlParser::make_NUMERIC(yytext,loc);	}
{str}			{ return CatDB::SqlParser::make_STRING(yytext,loc);	} 
{date}			{ return CatDB::SqlParser::make_TIMESTAMP(yytext,loc);	} 
{time}			{ return CatDB::SqlParser::make_TIMESTAMP(yytext,loc);	} 
{timestamp}		{ return CatDB::SqlParser::make_TIMESTAMP(yytext,loc);	}
<<EOF>>			{ yyterminate(); }
%%