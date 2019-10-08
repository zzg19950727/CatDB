#include "filter.h"
#include "expression.h"
#include "object.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

CatDB::Sql::Filter::Filter(const Expression_s & expr)
	:expr(expr)
{
}

CatDB::Sql::Filter::~Filter()
{

}

bool CatDB::Sql::Filter::operator()(Row_s & row)
{
	if (expr) {
		Object_s result = expr->get_result(row);
		if (result->get_type() == T_ERROR_RESULT) {
			Log(LOG_ERR, "Filter", "get error result");
		}
		return result->bool_value();
	}
	else {
		return true;
	}
}

void CatDB::Sql::Filter::reset(const Row_s & row)
{
	//TODO reset expression
	expr->reset(row);
}

Filter_s CatDB::Sql::Filter::make_filter(const Expression_s & expr)
{
	return Filter_s(new Filter(expr));
}