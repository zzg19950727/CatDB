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
}

SingleChildPhyOperator::~SingleChildPhyOperator()
{
}

DoubleChildPhyOperator::DoubleChildPhyOperator(const PhyOperator_s & left, const PhyOperator_s & right)
	:left_child(left),
	right_child(right)
{
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
