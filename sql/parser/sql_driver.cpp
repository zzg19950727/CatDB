#include "sql_driver.h"
using namespace CatDB;

SqlDriver::SqlDriver():
	m_scanner(*this),
	m_parser(m_scanner,*this),
	global_database("test")
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
	for (u32 i = 0; i < query.size(); ++i) {
		if (query[i] == '\n')
			query[i] = ' ';
	}
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
	String msg[] = { "(—_—?)", "(๑•_•๑)", "(￣ε(#￣)", "(⊙ˍ⊙)", "╮(╯▽╰)╭","(°ʖ°)✧" };
	auto pos = query_stream.tellg();
	String sql;
	std::getline(query_stream, sql);
	query_stream.clear();
	query_stream.seekp(pos);
	query_stream << msg[rand()%6] << " " << sql;
	return query_stream.str();
}

String CatDB::SqlDriver::left_string()
{
	String sql;
	std::getline(query_stream, sql);
	for (int i = sql.size()-1; i >= 0; --i)
		query_stream.putback(sql[i]);
	return sql;
}

void CatDB::SqlDriver::set_global_database(const String & database)
{
	global_database = database;
}

const String & CatDB::SqlDriver::get_global_database()
{
	return global_database;
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
