#ifndef SQL_SCANNER_H
#define SQL_SCANNER_H
#if ! defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer CatDB_FlexLexer
#include "FlexLexer.h"
#endif
#undef YY_DECL
#define YY_DECL CatDB::SqlParser::symbol_type CatDB::SqlScanner::get_next_token()
#include "sql_parser.hpp"
#include <string>

namespace CatDB {
	class SqlDriver;
	  	class SqlScanner:public yyFlexLexer 
		{
	  	private:
			SqlDriver& m_driver;
	 
	  	public:
			SqlScanner (SqlDriver& driver):m_driver(driver)
			{
			}
		virtual CatDB::SqlParser::symbol_type get_next_token();
		virtual ~SqlScanner (){}
		void set_debug(int level) { yyFlexLexer::set_debug(level); }
	  };
}

#endif	//SQL_SCANNER_H