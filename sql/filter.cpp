#include "filter.h"
#include "expression.h"
#include "object.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

Filter::Filter(const Expression_s & expr)
	:expr(expr)
{
}

Filter::~Filter()
{

}

bool Filter::operator()(Row_s & row)
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

Object_s Filter::get_result(const Row_s & row)
{
	return expr->get_result(row);
}

void Filter::reset(const Row_s & row)
{
	//TODO reset expression
	if (expr) {
		expr->reset(row);
	}
}

Filter_s Filter::make_filter(const Expression_s & expr)
{
	return Filter_s(new Filter(expr));
}