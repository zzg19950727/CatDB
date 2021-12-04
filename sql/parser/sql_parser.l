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
	#define yyterminate() return CatDB::SqlParser::make_END(loc);
	# undef yywrap
  	# define yywrap() 1
	#define YY_NO_UNISTD_H
%}

letter 	[A-Za-z_]
digit	[0-9]
space	[ \t\n\r\f]+
num1    {digit}+(\.)?([eE][-]?{digit}+)?
num2    {digit}*(\.)?{digit}+([eE][-]?{digit}+)?
number  ({num1}|{num2})
ident	{letter}({letter}|{digit})*
qb_name_ident @{letter}({letter}|{digit})*
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
"column"		{ return CatDB::SqlParser::make_COLUMN(loc); }
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
"full"          { return CatDB::SqlParser::make_FULL(loc); }
"group"			{ return CatDB::SqlParser::make_GROUP(loc); }
"having"		{ return CatDB::SqlParser::make_HAVING(loc); }
"kill"			{ return CatDB::SqlParser::make_KILL(loc); }
"if"			{ return CatDB::SqlParser::make_IF(loc); }
"insert"		{ return CatDB::SqlParser::make_INSERT(loc); }
"index"			{ return CatDB::SqlParser::make_INDEX(loc); }
"inner"			{ return CatDB::SqlParser::make_INNER(loc); }
"int"			{ return CatDB::SqlParser::make_INT(loc); }
"into"			{ return CatDB::SqlParser::make_INTO(loc); }
"join"			{ return CatDB::SqlParser::make_JOIN(loc); }
"left"			{ return CatDB::SqlParser::make_LEFT(loc); }
"limit"			{ return CatDB::SqlParser::make_LIMIT(loc); }
"memory"		{ return CatDB::SqlParser::make_MEMORY(loc); }
"number"		{ return CatDB::SqlParser::make_NUMBER(loc); }
"on"			{ return CatDB::SqlParser::make_ON(loc); }
"order"			{ return CatDB::SqlParser::make_ORDER(loc); }
"processlist"	{ return CatDB::SqlParser::make_PROCESSLIST(loc); }
"right"			{ return CatDB::SqlParser::make_RIGHT(loc); }
"rowid"			{ return CatDB::SqlParser::make_ROWID(loc); }
"sample"		{ return CatDB::SqlParser::make_SAMPLE(loc); }
"select"		{ return CatDB::SqlParser::make_SELECT(loc); }
"set"			{ return CatDB::SqlParser::make_SET(loc); }
"show"			{ return CatDB::SqlParser::make_SHOW(loc); }
"size"			{ return CatDB::SqlParser::make_SIZE(loc); }
"statis"		{ return CatDB::SqlParser::make_STATIS(loc); }
"status"		{ return CatDB::SqlParser::make_STATUS(loc); }
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
"case"			{ return CatDB::SqlParser::make_CASE(loc); }
"when"			{ return CatDB::SqlParser::make_WHEN(loc); }
"then"			{ return CatDB::SqlParser::make_THEN(loc); }
"else"			{ return CatDB::SqlParser::make_ELSE(loc); }
"end"			{ return CatDB::SqlParser::make_END_SYM(loc); }
"qb_name"       { return CatDB::SqlParser::make_QB_NAME(loc); }
"use_hash"      { return CatDB::SqlParser::make_USE_HASH(loc); }
"use_nl"        { return CatDB::SqlParser::make_USE_NL(loc); }
"leading"       { return CatDB::SqlParser::make_LEADING(loc); }
"parallel"      { return CatDB::SqlParser::make_PARALLEL(loc); }
"ordered"       { return CatDB::SqlParser::make_ORDERED(loc); }
"no_rewrite"    { return CatDB::SqlParser::make_NO_REWRITE(loc); }
"engine"		{ return CatDB::SqlParser::make_ENGINE(loc); }
"infile"		{ return CatDB::SqlParser::make_INFILE(loc); }
"field"			{ return CatDB::SqlParser::make_FIELD(loc); }
"line"			{ return CatDB::SqlParser::make_LINE(loc); }
"split"			{ return CatDB::SqlParser::make_SPLIT(loc); }
"csv"			{ return CatDB::SqlParser::make_CSV(loc); }

"("				{ return CatDB::SqlParser::make_LP(loc); }
")"				{ return CatDB::SqlParser::make_RP(loc); }
","				{ return CatDB::SqlParser::make_COMMA(loc); }
"."				{ return CatDB::SqlParser::make_PERIOD(loc); }
";"             { return CatDB::SqlParser::make_SEMICOLON(loc); }
"+"				{ return CatDB::SqlParser::make_PLUS(loc); }
"-"				{ return CatDB::SqlParser::make_MINUS(loc); }
"*"				{ return CatDB::SqlParser::make_MUL(loc); }
"/"				{ return CatDB::SqlParser::make_DIV(loc); }
"/*+"			{ return CatDB::SqlParser::make_BEGIN_HINT(loc); }
"*/"			{ return CatDB::SqlParser::make_END_HINT(loc); }
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
{qb_name_ident}	{ return CatDB::SqlParser::make_QB_NAME_IDENT(yytext+1,loc);	} 
<<EOF>>			{ yyterminate(); }
%%
