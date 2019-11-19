#include "phy_operator.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

PhyOperator::PhyOperator()
	:access_desc(0),
	output_desc(0)
{
}

PhyOperator::~PhyOperator()
{
}

void PhyOperator::set_access_desc(const RowDesc& desc)
{
	access_desc = desc;
}

void PhyOperator::set_output_desc(const RowDesc& desc)
{
	output_desc = desc;
}

SingleChildPhyOperator::SingleChildPhyOperator(const PhyOperator_s & child)
	:child(child)
{
	set_access_desc(child->output_desc);
	set_output_desc(child->output_desc);
}

SingleChildPhyOperator::~SingleChildPhyOperator()
{
}

DoubleChildPhyOperator::DoubleChildPhyOperator(const PhyOperator_s & left, const PhyOperator_s & right)
	:left_child(left),
	right_child(right)
{
	ColumnDesc col_desc;
	RowDesc new_output_desc;
	for (u32 i = 0; i < left->output_desc.get_column_num(); ++i) {
		left->output_desc.get_column_desc(i, col_desc);
		new_output_desc.add_column_desc(col_desc);
	}
	for (u32 i = 0; i < right->output_desc.get_column_num(); ++i) {
		right->output_desc.get_column_desc(i, col_desc);
		new_output_desc.add_column_desc(col_desc);
	}
	set_access_desc(new_output_desc);
	set_output_desc(new_output_desc);
}

DoubleChildPhyOperator::~DoubleChildPhyOperator()
{
}

JoinPhyOperator::JoinPhyOperator(const PhyOperator_s & left, const PhyOperator_s & right)
	:DoubleChildPhyOperator(left, right),
	type(JoinPhyOperator::Join)
{
}

JoinPhyOperator::~JoinPhyOperator()
{
}

JoinPhyOperator::JoinType JoinPhyOperator::join_type() const
{
	return type;
}

void JoinPhyOperator::set_join_type(JoinType type)
{
	this->type = type;
}
