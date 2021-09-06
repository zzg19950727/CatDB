#ifndef BIT_SET_H
#define BIT_SET_H
#include "type.h"
#include "log.h"

class BitSet {
public:
    BitSet();
    ~BitSet();
    void add_member(u32 member);
    void del_member(u32 member);
    void add_members(const BitSet& other);
    void del_members(const BitSet& other);
    bool has_member(u32 member) const;
    bool is_empty() const;
    bool is_equal(const BitSet& other) const;
    bool is_subset(const BitSet& other) const;
    bool is_superset(const BitSet& other) const;
    bool overlap(const BitSet& other) const;
    void to_list(Vector<u32> &members) const;
    void intersect(const BitSet& lhs, const BitSet& rhs);
    void operator=(const BitSet &other) {data = other.data;}
    void clear();
    String to_kv_string() const;
private:
    Vector<u8> data;
};

#endif  //BIT_SET_H