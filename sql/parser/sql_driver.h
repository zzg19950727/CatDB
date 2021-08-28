#ifndef SQL_DRIVER_H
#define SQL_DRIVER_H
#include "sql_scanner.h"
#include "sql_parser.hpp"
#include "stmt.h"
#include "type.h"

namespace CatDB {
	class SqlDriver 
	{
		friend class SqlScanner;
		friend class SqlParser;
	  private:
		SqlParser m_parser;
		SqlScanner m_scanner;
		std::stringstream query_stream;
		Parser::Stmt_s result;
		String global_database;
		String syntax_error_;
		String sys_error_;
		
	  public:
		SqlDriver ();
		virtual ~SqlDriver();
		void reset();
		int parse_sql(const String& sql);
		bool is_sys_error()const;
		bool is_syntax_error()const;
		String sys_error()const;
		String syntax_error()const;
		String error_position();
		void set_global_database(const String& database);
		const String& get_global_database();
		void set_sys_error(const String& err);
		void set_syntax_error(const String& err);
		void set_lex_debug(bool debug);
		void set_yacc_debug(bool debug);
		void set_yacc_debug_ostream(std::ostream& os);
		Parser::Stmt_s parse_result()const;
	  };
}
#endif	//SQL_DRIVER_H
