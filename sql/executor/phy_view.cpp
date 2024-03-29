#include "phy_view.h"
#include "phy_expression.h"
#include "object.h"
#include "error.h"
#include "log.h"
#include "row.h"

using namespace CatDB::Common;
using namespace CatDB::Sql;

PhyView::PhyView(PhyOperator_s& child)
	:SingleChildPhyOperator(child)
{
}

PhyOperator_s PhyView::make_view(PhyOperator_s& child)
{
	return PhyOperator_s(new PhyView(child));
}

u32 PhyView::inner_open()
{
	return child->open();
}

u32 PhyView::close()
{
	return child->close();
}

u32 PhyView::reset()
{
	return child->reset();
}

u32 PhyView::inner_get_next_row()
{
	u32 ret = SUCCESS;
	Row_s row;
	while (SUCC(child->get_next_row(row))) {
		set_input_rows(row);
		return ret;
	}
	return ret;
}

u32 PhyView::type() const
{
	return PhyOperator::VIEW;
}