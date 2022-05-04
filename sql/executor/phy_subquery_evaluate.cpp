#include "phy_subquery_evaluate.h"
#include "object.h"
#include "error.h"
#include "log.h"
using namespace CatDB::Sql;

PhySubqueryEvaluate::PhySubqueryEvaluate(const PhyOperator_s& first_child)
	:MultiChildPhyOperator()
{
    add_child(first_child);
}

PhySubqueryEvaluate::~PhySubqueryEvaluate()
{
}

PhyOperator_s PhySubqueryEvaluate::make_subquery_evaluate(const PhyOperator_s& first_child)
{
	PhySubqueryEvaluate* op = new PhySubqueryEvaluate(first_child);
	return PhyOperator_s(op);
}

u32 PhySubqueryEvaluate::inner_open()
{
	u32 ret = SUCCESS;
    for (u32 i = 0; i < children.size(); ++i) {
        CHECK(children[i]->open());
    }
    return ret;
}

u32 PhySubqueryEvaluate::close()
{
	u32 ret = SUCCESS;
    for (u32 i = 0; i < children.size(); ++i) {
        u32 ret2 = children[i]->close();
        if (FAIL(ret2)) {
            ret = ret2;
        }
    }
    return ret;
}

u32 PhySubqueryEvaluate::reset()
{
	u32 ret = SUCCESS;
    for (u32 i = 0; i < children.size(); ++i) {
        CHECK(children[i]->reset());
    }
    return ret;
}

u32 PhySubqueryEvaluate::inner_get_next_row()
{
	u32 ret = SUCCESS;
    Row_s row;
    ret = children[0]->get_next_row(row);
    set_input_rows(row);
    exec_ctx->cur_op = this;
    return ret;
}

u32 PhySubqueryEvaluate::type() const
{
	return PhyOperator::SUBQUERY_EVALUATE;
}

u32 PhySubqueryEvaluate::reset_subplan(u32 subplan_id)
{
    u32 ret = SUCCESS;
    CHECK(children[subplan_id]->reset());
    return ret;
}

u32 PhySubqueryEvaluate::get_subplan_next_row(u32 subplan_id, Row_s &row)
{
    u32 ret = SUCCESS;
    CHECK(children[subplan_id]->get_next_row(row));
    return ret;
}