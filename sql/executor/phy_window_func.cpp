#include "phy_window_func.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

PhyWindowFunc::PhyWindowFunc(PhyOperator_s & child,
								Vector<Expression_s> &win_items,
                                Vector<Expression_s> &partition_by_exprs)
	:SingleChildPhyOperator(child),
	WindowFuncCalculator(win_items),
    partition_by_exprs(partition_by_exprs),
    row_pos(-1)
{
}

PhyWindowFunc::~PhyWindowFunc()
{
}

PhyOperator_s PhyWindowFunc::make_window_func(PhyOperator_s & child,
												Vector<Expression_s> &win_items,
                                                Vector<Expression_s> &partition_by_exprs)
{
	PhyWindowFunc* op = new PhyWindowFunc(child, win_items, partition_by_exprs);
	return PhyOperator_s(op);
}

u32 PhyWindowFunc::inner_open()
{
	u32 ret = SUCCESS;
	CHECK(init_win_func(exec_ctx));
	win_result_row = Row::make_row(win_funcs.size());
	win_result_row->set_op_id(operator_id);
	CHECK(child->open());
	return ret;
}

u32 PhyWindowFunc::close()
{
	return child->close();
}

u32 PhyWindowFunc::reset()
{
	u32 ret = SUCCESS;
    row_pos = -1;
    last_row.reset();
    partition_by_values.clear();
    cur_window_rows.clear();
	CHECK(reset_win_func());
	CHECK(child->reset());
	return ret;
}

u32 PhyWindowFunc::inner_get_next_row()
{
	u32 ret = SUCCESS;
    if (row_pos < 0) {
        //get new window
        if (last_row) {
            CHECK(add_row_to_win_func(exec_ctx, last_row));
            cur_window_rows.push_back(last_row);
        }
        bool is_new = false;
        while (SUCC(child->get_next_row(last_row))) {
            last_row = Row::deep_copy(last_row);
            set_input_rows(last_row);
            CHECK(is_new_window(is_new));
            if (is_new) {
                row_pos = 0;
                break;
            } else {
                CHECK(add_row_to_win_func(exec_ctx, last_row));
                cur_window_rows.push_back(last_row);
            }
        }
        if (NO_MORE_ROWS == ret) {
            //last window
            if (!cur_window_rows.empty()) {
                row_pos = 0;
                last_row.reset();
                ret = SUCCESS;
            }
        }
    }
    if (SUCC(ret)) {
        set_input_rows(cur_window_rows[row_pos]);
        CHECK(calc_win_func(exec_ctx, win_result_row));
		set_input_rows(cur_window_rows[row_pos++], win_result_row);
        if (row_pos >= cur_window_rows.size()) {
            row_pos = -1;
            cur_window_rows.clear();
            CHECK(reset_win_func());
        }
    }
	return ret;
}

u32 PhyWindowFunc::type() const
{
	return PhyOperator::WINDOWN_FUNCTION;
}

u32 PhyWindowFunc::is_new_window(bool &is_new)
{
    u32 ret = SUCCESS;
    is_new = false;
    if (partition_by_exprs.empty()) {
        //do nothing
    } else if (partition_by_values.empty()) {
        for (u32 i = 0; i < partition_by_exprs.size(); ++i) {
            CHECK(partition_by_exprs[i]->get_result(exec_ctx));
            partition_by_values.push_back(exec_ctx->output_result);
        }
    } else {
        for (u32 i = 0; i < partition_by_exprs.size(); ++i) {
            CHECK(partition_by_exprs[i]->get_result(exec_ctx));
            if (!is_new) {
                int res = 0;
                CHECK(exec_ctx->output_result->compare(partition_by_values[i], res));
                if (res != CMP_RES_EQ) {
                    is_new = true;
                }
            }
            partition_by_values[i] = exec_ctx->output_result;
        }
    }
    return ret;
}