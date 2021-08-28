#include "phy_filter.h"
#include "phy_expression.h"
#include "object.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

PhyFilter::PhyFilter(const Expression_s & expr)
{
	exprs.push_back(expr);
}

PhyFilter::PhyFilter(const Vector<Expression_s> & exprs)
	:exprs(exprs)
{
}

PhyFilter::~PhyFilter()
{

}

bool PhyFilter::operator()(Row_s & row)
{
	bool ret = true;
	if (!exprs.empty()) {
		for (u32 i = 0; ret && i < exprs.size(); ++i) {
			Object_s result = exprs[i]->get_result(row);
			if (result->get_type() == T_ERROR_RESULT) {
				LOG_ERR("get error result", K(result));
			}
			ret = result->bool_value();
		}
	}
	return ret;
}

PhyFilter_s PhyFilter::make_filter(const Expression_s & expr)
{
	return PhyFilter_s(new PhyFilter(expr));
}

PhyFilter_s PhyFilter::make_filter(const Vector<Expression_s> & exprs)
{
	return PhyFilter_s(new PhyFilter(exprs));
}