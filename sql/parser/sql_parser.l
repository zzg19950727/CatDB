%option yyclass="SqlScanner"
%option prefix="CatDB_"
%option noyywrap debug
%option caseless
%option c++

%{
	#include "sql_parser.hpp"	//包含由parser.l生成的头文件
	#include "sql_scanner.h" 	//包含yyFlexLexer子类的头文件
	#include "location.hh" 	//包含位置调试信息头文件
	#define yyterminate() return CatDB::SqlParser::make_END(loc);
	#define YY_USER_ACTION  loc.columns (yyleng);
	#undef yywrap
  	#define yywrap() 1
	#define YY_NO_UNISTD_H
%}

digit	[0-9]
letter 	[A-Za-z_]
space	[ \t\n\r\f]+
number  ({num1}|{num2})
date	[0-9]{4}(-[0-9]{1,2}){2}
time	[0-9]{2}(:[0-9]{1,2}){2}
ident	{letter}({letter}|{digit})*
num1    {digit}+(\.)?([eE][-]?{digit}+)?
num2    {digit}*(\.)?{digit}+([eE][-]?{digit}+)?
start_comment	\/\*
end_comment		\*\/
qb_name_ident 	@{letter}({letter}|{digit})*
timestamp		[0-9]{4}(-[0-9]{1,2}){2}[ ][0-9]{2}(:[0-9]{1,2}){2}

%x beg_str
%x beg_str2
%x beg_ident
%x beg_comment
%%

"("						{ return CatDB::SqlParser::make_LP(loc); }
")"						{ return CatDB::SqlParser::make_RP(loc); }
","						{ return CatDB::SqlParser::make_COMMA(loc); }
"."						{ return CatDB::SqlParser::make_PERIOD(loc); }
";"             		{ return CatDB::SqlParser::make_SEMICOLON(loc); }
"+"						{ return CatDB::SqlParser::make_PLUS(loc); }
"-"						{ return CatDB::SqlParser::make_MINUS(loc); }
"*"						{ return CatDB::SqlParser::make_MUL(loc); }
"/"						{ return CatDB::SqlParser::make_DIV(loc); }
"/*+"					{ return CatDB::SqlParser::make_BEGIN_HINT(loc); }
"*/"					{ return CatDB::SqlParser::make_END_HINT(loc); }
"!="					{ return CatDB::SqlParser::make_CMP_NE(loc); }
"<"						{ return CatDB::SqlParser::make_CMP_LT(loc); }
"<="					{ return CatDB::SqlParser::make_CMP_LE(loc); }
"<>"					{ return CatDB::SqlParser::make_CMP_NE(loc); }
"="						{ return CatDB::SqlParser::make_CMP_EQ(loc); }
">"						{ return CatDB::SqlParser::make_CMP_GT(loc); }
">="					{ return CatDB::SqlParser::make_CMP_GE(loc); }
"all"					{ return CatDB::SqlParser::make_ALL(loc); }
"analyze"				{ return CatDB::SqlParser::make_ANALYZE(loc); }
"and"					{ return CatDB::SqlParser::make_AND(loc); }
"any"					{ return CatDB::SqlParser::make_ANY(loc); }
"as"					{ return CatDB::SqlParser::make_AS(loc); }
"asc"					{ return CatDB::SqlParser::make_ASC(loc); }
"avg"					{ return CatDB::SqlParser::make_AVG(loc); }
"begin_outline_data"	{ return CatDB::SqlParser::make_BEGIN_OUTLINE_DATA(loc); }
"between"				{ return CatDB::SqlParser::make_BETWEEN(loc); }
"bigint"				{ return CatDB::SqlParser::make_BIGINT(loc); }
"binary"				{ return CatDB::SqlParser::make_BINARY(loc); }
"bool"					{ return CatDB::SqlParser::make_BOOL(loc); }
"by"					{ return CatDB::SqlParser::make_BY(loc); }
"case"					{ return CatDB::SqlParser::make_CASE(loc); }
"char"					{ return CatDB::SqlParser::make_CHAR(loc); }
"column"				{ return CatDB::SqlParser::make_COLUMN(loc); }
"columns"				{ return CatDB::SqlParser::make_COLUMNS(loc); }
"count"					{ return CatDB::SqlParser::make_COUNT(loc); }
"create"				{ return CatDB::SqlParser::make_CREATE(loc); }
"csv"					{ return CatDB::SqlParser::make_CSV(loc); }
"database"				{ return CatDB::SqlParser::make_DATABASE(loc); }
"databases"				{ return CatDB::SqlParser::make_DATABASES(loc); }
"date"					{ return CatDB::SqlParser::make_DATE(loc); }
"datetime"				{ return CatDB::SqlParser::make_DATETIME(loc); }
"day"					{ return CatDB::SqlParser::make_DAY(loc); }
"decimal"				{ return CatDB::SqlParser::make_DECIMAL(loc); }
"delete"				{ return CatDB::SqlParser::make_DELETE(loc); }
"dense_rank"			{ return CatDB::SqlParser::make_DENSE_RANK(loc); }
"desc"					{ return CatDB::SqlParser::make_DESC(loc); }
"describe"				{ return CatDB::SqlParser::make_DESCRIBE(loc); }
"distinct"				{ return CatDB::SqlParser::make_DISTINCT(loc); }
"double"				{ return CatDB::SqlParser::make_DOUBLE(loc); }
"drop"					{ return CatDB::SqlParser::make_DROP(loc); }
"dual"					{ return CatDB::SqlParser::make_DUAL(loc); }
"else"					{ return CatDB::SqlParser::make_ELSE(loc); }
"end"					{ return CatDB::SqlParser::make_END_SYM(loc); }
"end_outline_data"		{ return CatDB::SqlParser::make_END_OUTLINE_DATA(loc); }
"engine"				{ return CatDB::SqlParser::make_ENGINE(loc); }
"except"				{ return CatDB::SqlParser::make_EXCEPT(loc); }
"exec"					{ return CatDB::SqlParser::make_EXEC(loc); }
"exists"				{ return CatDB::SqlParser::make_EXISTS(loc); }
"explain"				{ return CatDB::SqlParser::make_EXPLAIN(loc); }
"expr_normalize"		{ return CatDB::SqlParser::make_EXPR_NORMALIZE(loc); }
"field"					{ return CatDB::SqlParser::make_FIELD(loc); }
"float"					{ return CatDB::SqlParser::make_FLOAT(loc); }
"from"					{ return CatDB::SqlParser::make_FROM(loc); }
"full"					{ return CatDB::SqlParser::make_FULL(loc); }
"function"				{ return CatDB::SqlParser::make_FUNCTION(loc);}
"group"					{ return CatDB::SqlParser::make_GROUP(loc); }
"having"				{ return CatDB::SqlParser::make_HAVING(loc); }
"if"					{ return CatDB::SqlParser::make_IF(loc); }
"ifnull"				{ return CatDB::SqlParser::make_IFNULL(loc); }
"in"					{ return CatDB::SqlParser::make_IN(loc); }
"index"					{ return CatDB::SqlParser::make_INDEX(loc); }
"infile"				{ return CatDB::SqlParser::make_INFILE(loc); }
"inner"					{ return CatDB::SqlParser::make_INNER(loc); }
"insert"				{ return CatDB::SqlParser::make_INSERT(loc); }
"int"					{ return CatDB::SqlParser::make_INT(loc); }
"integer"				{ return CatDB::SqlParser::make_INTEGER(loc); }
"intersect"				{ return CatDB::SqlParser::make_INTERSECT(loc); }
"interval"				{ return CatDB::SqlParser::make_INTERVAL(loc); }
"into"					{ return CatDB::SqlParser::make_INTO(loc); }
"is"					{ return CatDB::SqlParser::make_IS(loc); }
"join"					{ return CatDB::SqlParser::make_JOIN(loc); }
"kill"					{ return CatDB::SqlParser::make_KILL(loc); }
"leading"       		{ return CatDB::SqlParser::make_LEADING(loc); }
"left"					{ return CatDB::SqlParser::make_LEFT(loc); }
"like"					{ return CatDB::SqlParser::make_LIKE(loc); }
"limit"					{ return CatDB::SqlParser::make_LIMIT(loc); }
"line"					{ return CatDB::SqlParser::make_LINE(loc); }
"max"					{ return CatDB::SqlParser::make_MAX(loc); }
"mediumint"				{ return CatDB::SqlParser::make_MEDIUMINT(loc); }
"memory"				{ return CatDB::SqlParser::make_MEMORY(loc); }
"merge"					{ return CatDB::SqlParser::make_MERGE(loc); }
"min"					{ return CatDB::SqlParser::make_MIN(loc); }
"month"					{ return CatDB::SqlParser::make_MONTH(loc); }
"no_expr_normalize"		{ return CatDB::SqlParser::make_NO_EXPR_NORMALIZE(loc); }
"no_merge"				{ return CatDB::SqlParser::make_NO_MERGE(loc); }
"no_rewrite"    		{ return CatDB::SqlParser::make_NO_REWRITE(loc); }
"no_simplify_sq" 		{ return CatDB::SqlParser::make_NO_SIMPLIFY_SQ(loc); }
"no_unnest"     		{ return CatDB::SqlParser::make_NO_UNNEST(loc); }
"no_use_hash"   		{ return CatDB::SqlParser::make_NO_USE_HASH(loc); }
"no_use_nl"     		{ return CatDB::SqlParser::make_NO_USE_NL(loc); }
"no_win_magic"			{ return CatDB::SqlParser::make_NO_WIN_MAGIC(loc); }
"not"					{ return CatDB::SqlParser::make_NOT(loc); }
"null"					{ return CatDB::SqlParser::make_NULLX(loc); }
"numeric"				{ return CatDB::SqlParser::make_NUMERIC_SYM(loc); }
"on"					{ return CatDB::SqlParser::make_ON(loc); }
"or"					{ return CatDB::SqlParser::make_OR(loc); }
"order"					{ return CatDB::SqlParser::make_ORDER(loc); }
"ordered"       		{ return CatDB::SqlParser::make_ORDERED(loc); }
"outer"					{ return CatDB::SqlParser::make_OUTER(loc); }
"over"					{ return CatDB::SqlParser::make_OVER(loc); }
"package"				{ return CatDB::SqlParser::make_PACKAGE(loc);}
"parallel"      		{ return CatDB::SqlParser::make_PARALLEL(loc); }
"partition"				{ return CatDB::SqlParser::make_PARTITION(loc); }
"procedure"				{ return CatDB::SqlParser::make_PROCEDURE(loc);}
"processlist"			{ return CatDB::SqlParser::make_PROCESSLIST(loc); }
"rank"					{ return CatDB::SqlParser::make_RANK(loc); }
"real"					{ return CatDB::SqlParser::make_REAL(loc); }
"replace"				{ return CatDB::SqlParser::make_REPLACE(loc);}
"return"				{ return CatDB::SqlParser::make_RETURN(loc);}
"right"					{ return CatDB::SqlParser::make_RIGHT(loc); }
"row_number"			{ return CatDB::SqlParser::make_ROW_NUMBER(loc); }
"rowid"					{ return CatDB::SqlParser::make_ROWID(loc); }
"sample"				{ return CatDB::SqlParser::make_SAMPLE(loc); }
"select"				{ return CatDB::SqlParser::make_SELECT(loc); }
"set"					{ return CatDB::SqlParser::make_SET(loc); }
"show"					{ return CatDB::SqlParser::make_SHOW(loc); }
"simplify_sq"			{ return CatDB::SqlParser::make_SIMPLIFY_SQ(loc); }
"size"					{ return CatDB::SqlParser::make_SIZE(loc); }
"smallint"				{ return CatDB::SqlParser::make_SMALLINT(loc); }
"split"					{ return CatDB::SqlParser::make_SPLIT(loc); }
"statis"				{ return CatDB::SqlParser::make_STATIS(loc); }
"status"				{ return CatDB::SqlParser::make_STATUS(loc); }
"substr"				{ return CatDB::SqlParser::make_SUBSTR(loc); }
"sum"					{ return CatDB::SqlParser::make_SUM(loc); }
"table"					{ return CatDB::SqlParser::make_TABLE(loc); }
"tables"				{ return CatDB::SqlParser::make_TABLES(loc); }
"then"					{ return CatDB::SqlParser::make_THEN(loc); }
"time"					{ return CatDB::SqlParser::make_TIME(loc); }
"timestamp"				{ return CatDB::SqlParser::make_TIMESTAMP_SYM(loc); }
"tinyint"				{ return CatDB::SqlParser::make_TINYINT(loc); }
"to_char"				{ return CatDB::SqlParser::make_TO_CHAR(loc); }
"to_number"				{ return CatDB::SqlParser::make_TO_NUMBER(loc); }
"union"					{ return CatDB::SqlParser::make_UNION(loc); }
"unnest"      			{ return CatDB::SqlParser::make_UNNEST(loc); }
"update"				{ return CatDB::SqlParser::make_UPDATE(loc); }
"use"					{ return CatDB::SqlParser::make_USING(loc); }
"use_hash"      		{ return CatDB::SqlParser::make_USE_HASH(loc); }
"use_nl"        		{ return CatDB::SqlParser::make_USE_NL(loc); }
"values"				{ return CatDB::SqlParser::make_VALUES(loc); }
"varbinary"				{ return CatDB::SqlParser::make_VARBINARY(loc); }
"varchar"				{ return CatDB::SqlParser::make_VARCHAR(loc); }
"view"					{ return CatDB::SqlParser::make_VIEW(loc); }
"when"					{ return CatDB::SqlParser::make_WHEN(loc); }
"where"					{ return CatDB::SqlParser::make_WHERE(loc); }
"win_magic"				{ return CatDB::SqlParser::make_WIN_MAGIC(loc); }
"year"					{ return CatDB::SqlParser::make_YEAR(loc); }
{number}				{ return CatDB::SqlParser::make_NUMERIC(yytext,loc);	}
{ident}					{ return CatDB::SqlParser::make_IDENT(yytext,loc);	} 
{qb_name_ident}			{ return CatDB::SqlParser::make_QB_NAME_IDENT(yytext+1,loc);	} 

{start_comment}				{
								BEGIN(beg_comment);		
							}
<beg_comment>(.|\n)			{
								;
							}
<beg_comment><<EOF>>		{
								yyterminate();
							}
<beg_comment>{end_comment}	{
								BEGIN(INITIAL);
							}

\"							{
								parse_str.clear();
								BEGIN(beg_str);
							}
<beg_str>\"         		{
								BEGIN(INITIAL);
								return CatDB::SqlParser::make_STRING(parse_str,loc);
							}
<beg_str>\\n				parse_str += '\n';
<beg_str>\\t				parse_str += '\t';
<beg_str>\\r				parse_str += '\r';
<beg_str>\\b				parse_str += '\b';
<beg_str>\\f				parse_str += '\f';
<beg_str>\\(.|\n)			parse_str += yytext[1];
<beg_str>[^\\\n\"]+ 		{
								tmp_ptr = yytext;
								while( *tmp_ptr ) {
									parse_str += *tmp_ptr++;
								}
							}

'						{parse_str.clear();BEGIN(beg_str2);}
<beg_str2>'         	{
							BEGIN(INITIAL);
							return CatDB::SqlParser::make_STRING(parse_str,loc);
						}
<beg_str2>\\n			parse_str += '\n';
<beg_str2>\\t			parse_str += '\t';
<beg_str2>\\r			parse_str += '\r';
<beg_str2>\\b			parse_str += '\b';
<beg_str2>\\f			parse_str += '\f';
<beg_str2>\\(.|\n)		parse_str += yytext[1];
<beg_str2>[^\\\n']+ 	{
							tmp_ptr = yytext;
							while( *tmp_ptr ) {
								parse_str += *tmp_ptr++;
							}
						}

`						{parse_str.clear();BEGIN(beg_ident);}
<beg_ident>`    		{
							BEGIN(INITIAL);
							return CatDB::SqlParser::make_IDENT(parse_str,loc);
						}
<beg_ident>[^\\\n`]+  	{
							tmp_ptr = yytext;
							while( *tmp_ptr ) {
								parse_str += *tmp_ptr++;
							}
						}

{space}
<<EOF>>					{ yyterminate(); }
%%
