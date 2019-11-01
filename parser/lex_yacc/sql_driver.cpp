#include "sql_driver.h"
using namespace CatDB;

SqlDriver::SqlDriver():
	m_scanner(*this),
	m_parser(m_scanner,*this)
{
 
}

SqlDriver::~SqlDriver()
{
 
}

void CatDB::SqlDriver::reset()
{
	sys_error_.clear();
	syntax_error_.clear();
}

int CatDB::SqlDriver::parse_sql(const String & sql)
{
	String query = sql + ";";
	query_stream.clear();
	query_stream << query;
	//m_parser.set_debug_level(1);
	//m_scanner.set_debug(1);
	m_scanner.switch_streams(&query_stream, 0);
	return m_parser.parse();
}

bool CatDB::SqlDriver::is_sys_error() const
{
	return !sys_error_.empty();
}

bool CatDB::SqlDriver::is_syntax_error() const
{
	return !syntax_error_.empty();
}

String CatDB::SqlDriver::sys_error() const
{
	return sys_error_;
}

String CatDB::SqlDriver::syntax_error() const
{
	return syntax_error_;
}

String CatDB::SqlDriver::error_position()
{
	String msg = "(<==parse error!!!) ";
	auto pos = query_stream.tellg();
	String sql;
	std::getline(query_stream, sql);
	query_stream.clear();
	query_stream.seekp(pos);
	query_stream << msg << sql;
	msg = query_stream.str();
	return msg;
}

void CatDB::SqlDriver::set_sys_error(const String & err)
{
	sys_error_ = error_position() + "\n" + err;
}

void CatDB::SqlDriver::set_syntax_error(const String & err)
{
	syntax_error_ = error_position() + "\n" + err;
}

void CatDB::SqlDriver::set_lex_debug(bool debug)
{
	m_scanner.set_debug(debug);
}

void CatDB::SqlDriver::set_yacc_debug(bool debug)
{
	m_parser.set_debug_level(debug);
}

void CatDB::SqlDriver::set_yacc_debug_ostream(std::ostream & os)
{
	m_parser.set_debug_stream(os);
}

Parser::Stmt_s CatDB::SqlDriver::parse_result() const
{
	return result;
}
