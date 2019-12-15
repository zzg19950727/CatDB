%option c++
%option noyywrap debug
%option yyclass="SqlScanner"
%option prefix="CatDB_"
%option caseless
%{
	#include "sql_parser.hpp"	//包含由parser.l生成的头文件
	#include "sql_scanner.h" 	//包含yyFlexLexer子类的头文件
	#include "location.hh" 	//包含位置调试信息头文件
	#include <string>
	# define YY_USER_ACTION  loc.columns (yyleng);
	#define yyterminate() CatDB::SqlParser::make_END(loc);
	# undef yywrap
  	# define yywrap() 1
	#define YY_NO_UNISTD_H
  	static CatDB::location loc;	//声明位置实例
	std::string parse_str;
	char* tmp_ptr;
%}

letter 	[A-Za-z_]
digit	[0-9]
space	[ \t\n\r\f]+
num1    {digit}+(\.)?([eE][-]?{digit}+)?
num2    {digit}*(\.)?{digit}+([eE][-]?{digit}+)?
number  ({num1}|{num2})
ident	{letter}({letter}|{digit})*
date	[0-9]{4}(-[0-9]{1,2}){2}
time	[0-9]{2}(:[0-9]{1,2}){2}
timestamp	[0-9]{4}(-[0-9]{1,2}){2}[ ][0-9]{2}(:[0-9]{1,2}){2}
%x beg_str
%x beg_str2
%x beg_ident
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
"<>"			{ return CatDB::SqlParser::make_CMP_NE(loc); }
">="			{ return CatDB::SqlParser::make_CMP_GE(loc); }
">"				{ return CatDB::SqlParser::make_CMP_GT(loc); }
"="				{ return CatDB::SqlParser::make_CMP_EQ(loc); }
"<"				{ return CatDB::SqlParser::make_CMP_LT(loc); }
"<="			{ return CatDB::SqlParser::make_CMP_LE(loc); }
"like"			{ return CatDB::SqlParser::make_LIKE(loc); }
"between"		{ return CatDB::SqlParser::make_BETWEEN(loc); }
"analyze"		{ return CatDB::SqlParser::make_ANALYZE(loc); }
"in"			{ return CatDB::SqlParser::make_IN(loc); }
"is"			{ return CatDB::SqlParser::make_IS(loc); }
"any"			{ return CatDB::SqlParser::make_ANY(loc); }
"all"			{ return CatDB::SqlParser::make_ALL(loc); }
"as"			{ return CatDB::SqlParser::make_AS(loc); }
"asc"			{ return CatDB::SqlParser::make_ASC(loc); }
"by"			{ return CatDB::SqlParser::make_BY(loc); }
"columns"		{ return CatDB::SqlParser::make_COLUMNS(loc); }
"create"		{ return CatDB::SqlParser::make_CREATE(loc); }
"database"		{ return CatDB::SqlParser::make_DATABASE(loc); }
"databases"		{ return CatDB::SqlParser::make_DATABASES(loc); }
"date"			{ return CatDB::SqlParser::make_DATE(loc); }
"datetime"		{ return CatDB::SqlParser::make_DATETIME(loc); }
"delete"		{ return CatDB::SqlParser::make_DELETE(loc); }
"desc"			{ return CatDB::SqlParser::make_DESC(loc); }
"describe"		{ return CatDB::SqlParser::make_DESCRIBE(loc); }
"distinct"		{ return CatDB::SqlParser::make_DISTINCT(loc); }
"double"		{ return CatDB::SqlParser::make_DOUBLE(loc); }
"drop"			{ return CatDB::SqlParser::make_DROP(loc); }
"dual"			{ return CatDB::SqlParser::make_DUAL(loc); }
"exists"		{ return CatDB::SqlParser::make_EXISTS(loc); }
"explain"		{ return CatDB::SqlParser::make_EXPLAIN(loc); }
"float"			{ return CatDB::SqlParser::make_FLOAT(loc); }
"from"			{ return CatDB::SqlParser::make_FROM(loc); }
"group"			{ return CatDB::SqlParser::make_GROUP(loc); }
"having"		{ return CatDB::SqlParser::make_HAVING(loc); }
"insert"		{ return CatDB::SqlParser::make_INSERT(loc); }
"index"			{ return CatDB::SqlParser::make_INDEX(loc); }
"int"			{ return CatDB::SqlParser::make_INT(loc); }
"into"			{ return CatDB::SqlParser::make_INTO(loc); }
"limit"			{ return CatDB::SqlParser::make_LIMIT(loc); }
"number"		{ return CatDB::SqlParser::make_NUMBER(loc); }
"order"			{ return CatDB::SqlParser::make_ORDER(loc); }
"select"		{ return CatDB::SqlParser::make_SELECT(loc); }
"set"			{ return CatDB::SqlParser::make_SET(loc); }
"show"			{ return CatDB::SqlParser::make_SHOW(loc); }
"status"			{ return CatDB::SqlParser::make_STATUS(loc); }
"table"			{ return CatDB::SqlParser::make_TABLE(loc); }
"tables"		{ return CatDB::SqlParser::make_TABLES(loc); }
"update"		{ return CatDB::SqlParser::make_UPDATE(loc); }
"use"			{ return CatDB::SqlParser::make_USING(loc); }
"values"		{ return CatDB::SqlParser::make_VALUES(loc); }
"varchar"		{ return CatDB::SqlParser::make_VARCHAR(loc); }
"where"			{ return CatDB::SqlParser::make_WHERE(loc); }
"tinyint"		{ return CatDB::SqlParser::make_TINYINT(loc); }
"smallint"		{ return CatDB::SqlParser::make_SMALLINT(loc); }
"mediumint"		{ return CatDB::SqlParser::make_MEDIUMINT(loc); }
"bigint"		{ return CatDB::SqlParser::make_BIGINT(loc); }
"decimal"		{ return CatDB::SqlParser::make_DECIMAL(loc); }
"numeric"		{ return CatDB::SqlParser::make_NUMERIC_SYM(loc); }
"real"			{ return CatDB::SqlParser::make_REAL(loc); }
"char"			{ return CatDB::SqlParser::make_CHAR(loc); }
"binary"		{ return CatDB::SqlParser::make_BINARY(loc); }
"varbinary"		{ return CatDB::SqlParser::make_VARBINARY(loc); }
"timestamp"		{ return CatDB::SqlParser::make_TIMESTAMP_SYM(loc); }
"time"			{ return CatDB::SqlParser::make_TIME(loc); }

"("				{ return CatDB::SqlParser::make_LP(loc); }
")"				{ return CatDB::SqlParser::make_RP(loc); }
","				{ return CatDB::SqlParser::make_COMMA(loc); }
"."				{ return CatDB::SqlParser::make_PERIOD(loc); }
";"				{ return CatDB::SqlParser::make_SEMICOLON(loc); }
"+"				{ return CatDB::SqlParser::make_PLUS(loc); }
"-"				{ return CatDB::SqlParser::make_MINUS(loc); }
"*"				{ return CatDB::SqlParser::make_MUL(loc); }
"/"				{ return CatDB::SqlParser::make_DIV(loc); }

\"				{parse_str.clear();BEGIN(beg_str);}
<beg_str>\"         {
						BEGIN(INITIAL);
						return CatDB::SqlParser::make_STRING(parse_str,loc);
					}
<beg_str>\\n		parse_str += '\n';
<beg_str>\\t		parse_str += '\t';
<beg_str>\\r		parse_str += '\r';
<beg_str>\\b		parse_str += '\b';
<beg_str>\\f		parse_str += '\f';
<beg_str>\\(.|\n)	parse_str += yytext[1];
<beg_str>[^\\\n\"]+ {
						tmp_ptr = yytext;
						while( *tmp_ptr )
							parse_str += *tmp_ptr++;
					}

'					{parse_str.clear();BEGIN(beg_str2);}
<beg_str2>'         {
						BEGIN(INITIAL);
						return CatDB::SqlParser::make_STRING(parse_str,loc);
					}
<beg_str2>\\n		parse_str += '\n';
<beg_str2>\\t		parse_str += '\t';
<beg_str2>\\r		parse_str += '\r';
<beg_str2>\\b		parse_str += '\b';
<beg_str2>\\f		parse_str += '\f';
<beg_str2>\\(.|\n)	parse_str += yytext[1];
<beg_str2>[^\\\n']+ {
						tmp_ptr = yytext;
						while( *tmp_ptr )
							parse_str += *tmp_ptr++;
					}

`					{parse_str.clear();BEGIN(beg_ident);}
<beg_ident>`    	{
						BEGIN(INITIAL);
						return CatDB::SqlParser::make_IDENT(parse_str,loc);
					}
<beg_ident>[^\\\n`]+  {
						tmp_ptr = yytext;
						while( *tmp_ptr )
							parse_str += *tmp_ptr++;
					}

{space}
{number}		{ return CatDB::SqlParser::make_NUMERIC(yytext,loc);	}
{ident}			{ return CatDB::SqlParser::make_IDENT(yytext,loc);	} 
<<EOF>>			{ yyterminate(); }
%%