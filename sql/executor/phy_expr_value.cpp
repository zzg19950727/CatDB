#include "phy_expr_value.h"
#include "phy_expression.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

PhyExprValue::PhyExprValue()
    :pos(0)
{
}

PhyExprValue::~PhyExprValue()
{
	
}

PhyOperator_s CatDB::Sql::PhyExprValue::make_expr_value()
{
	PhyExprValue* expr_value = new PhyExprValue();
	return PhyOperator_s(expr_value);
}

u32 CatDB::Sql::PhyExprValue::inner_open()
{
	return SUCCESS;
}

u32 CatDB::Sql::PhyExprValue::close()
{
	return SUCCESS;
}

u32 CatDB::Sql::PhyExprValue::reset()
{
    pos = 0;
	return SUCCESS;
}

u32 CatDB::Sql::PhyExprValue::inner_get_next_row()
{
    while(pos < value_list.size()) {
        set_input_rows(cur_row);
        output_exprs = value_list[pos];
        ++pos;
        return SUCCESS;
    }
    return NO_MORE_ROWS;
}

u32 CatDB::Sql::PhyExprValue::add_value(Vector<Expression_s> &value)
{
    value_list.push_back(value);
    return SUCCESS;
}

u32 CatDB::Sql::PhyExprValue::type() const
{
	return PhyOperator::EXPR_VALUE;
}