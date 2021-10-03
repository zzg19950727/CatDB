#include "bit_set.h"

BitSet::BitSet()
    :data(4,0)
{
}

BitSet::~BitSet()
{
}

void BitSet::add_member(u32 member)
{
    while (data.size() < (member >> 3)+1) {
        data.push_back(0);
    }
    data[member >> 3] |= 1 << (member & 7);
}

void BitSet::del_member(u32 member)
{
    if (data.size() >= (member >> 3)+1) {
        data[member >> 3] &= ~(1 << (member & 7));
    }
}

void BitSet::add_members(const BitSet& other)
{
    u32 i = 0;
    while (i < data.size() && i < other.data.size()) {
        data[i] |= other.data[i];
        ++i;
    }
    while (i < other.data.size()) {
        data.push_back(other.data[i]);
        ++i;
    }
}

void BitSet::del_members(const BitSet& other)
{
    u32 i = 0;
    while (i < data.size() && i < other.data.size()) {
        data[i] &= ~other.data[i];
        ++i;
    }
}

bool BitSet::has_member(u32 member) const
{
    if (data.size() >= (member >> 3)+1) {
        return data[member >> 3] & (1 << (member & 7));
    } else {
        return false;
    }
}

bool BitSet::is_empty() const
{
    bool empty = true;
    for (u32 i = 0; empty && i < data.size(); ++i) {
        empty = data[i] == 0;
    }
    return empty;
}

bool BitSet::is_equal(const BitSet& other) const
{
    u32 i = 0;
    bool equal = true;
    while (equal && i < data.size() && i < other.data.size()) {
        equal = data[i] == other.data[i];
        ++i;
    }
    while (equal && i < data.size()) {
        equal = data[i] == 0;
        ++i;
    }
    while (equal && i < other.data.size()) {
        equal = other.data[i] == 0;
        ++i;
    }
    return equal;
}

bool BitSet::is_subset(const BitSet& other) const
{
    u32 i = 0;
    bool subset = true;
    while (subset && i < data.size() && i < other.data.size()) {
        subset = data[i] == (other.data[i] & data[i]);
        ++i;
    }
    while (subset && i < data.size()) {
        subset = data[i] == 0;
        ++i;
    }
    return subset;
}

bool BitSet::is_superset(const BitSet& other) const
{
    u32 i = 0;
    bool superset = true;
    while (superset && i < data.size() && i < other.data.size()) {
        superset = other.data[i] == (other.data[i] & data[i]);
        ++i;
    }
    while (superset && i < other.data.size()) {
        superset = other.data[i] == 0;
        ++i;
    }
    return superset;
}

bool BitSet::overlap(const BitSet& other) const
{
    u32 i = 0;
    bool overlap = false;
    while (!overlap && i < data.size() && i < other.data.size()) {
        overlap = (other.data[i] & data[i]);
        ++i;
    }
    return overlap;
}

void BitSet::to_list(Vector<u32> &members) const
{
    u32 i = 0;
    while (i < data.size()) {
        u32 j = 0;
        while (j < 8) {
            if (data[i] & (1<<j)) {
                members.push_back( (i<<3) + j);
            }
            ++j;
        }
        ++i;
    }
}

void BitSet::intersect(const BitSet& lhs, const BitSet& rhs)
{
    u32 i = 0;
    while (i < lhs.data.size() && i < rhs.data.size()) {
        if (i < data.size()) {
            data[i] = lhs.data[i] & rhs.data[i];
        } else {
            data.push_back(lhs.data[i] & rhs.data[i]);
        }
        ++i;
    }
}

void BitSet::clear()
{
    u32 i = 0;
    while (i < data.size()) {
        data[i] = 0;
        ++i;
    }
}

String BitSet::to_kv_string() const
{
    Vector<u32> members;
    to_list(members);
    return ::to_kv_string(members);
}
