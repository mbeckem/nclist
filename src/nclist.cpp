#include "nclist.hpp"

#include <algorithm>
#include <iostream>

nclist nclist::build(const interval_list& input)
{
    // Comparison function described in section 2.7.
    // Intervals that are not properly contained within others are sorted
    // by start and end.
    // Contained intervals follow after their containing interval, i.e.
    // if x and y have the same start but x stops after y then x is less than y.
    auto less = [](const interval& x, const interval& y) {
        return x.start < y.start || (x.start == y.start && x.stop > y.stop);
    };

    // Make a copy. A more efficient variant of this function could be implemented
    // by taking a non-const reference to the input list and sorting it in-place.
    interval_list intervals = input;
    std::sort(intervals.begin(), intervals.end(), less);

    // Call the recursive build_sublist algorithm and use the result list
    // as the toplevel sublist for the new nclist.
    auto pos = intervals.begin();
    auto end = intervals.end();
    return nclist{
        build_sublist(end, pos, end)
    };
}

// Constructs a tree of interval lists.
// This is a more naive implemention of the technique described in the paper.
// Instead of using many nested lists, the paper's authors use two arrays
// linked together by indices (called L and H).
std::vector<nclist::entry> nclist::build_sublist(
        interval_list::iterator parent,     // parent of this call, end if at top level
        interval_list::iterator& pos,       // the current position in the list
        interval_list::iterator end)        // the end of the list
{
    std::vector<nclist::entry> result;
    while (pos != end) {
        // If there is no parent, add it always (we are at top level).
        // Otherwise, only add it if its contained within the parent.
        if (parent != end && (!parent->contains(*pos) || *parent == *pos)) {
            // By returning from the loop we let the recursion level
            // above us continue.
            break;
        }

        const auto current = pos++;
        nclist::entry e(*current);
        e.sublist = build_sublist(current, pos, end);
        result.push_back(std::move(e));
    }
    return result;
}

interval_list nclist::overlap(const interval& query) const {
    interval_list result;
    overlap(intervals, query, result);
    return result;
}

// Implements Algorithm 1 of the paper.
void nclist::overlap(const std::vector<entry>& sublist,
                     const interval& query,
                     interval_list& result) const
{
    // Find the first interval in "sublist" that has stop > query.start.
    auto i = std::upper_bound(sublist.begin(), sublist.end(), query,
                     [&](const interval& q, const entry& x) {
        return q.start < x.value.stop;
    });

    // Iterate until the end of the list or until
    // the current entry no longer overlaps with the query interval.
    // Recurse into every sublist on the way.
    auto end = sublist.end();
    while (i != end && i->value.start < query.stop) {
        result.push_back(i->value);
        overlap(i->sublist, query, result);
        ++i;
    }
}

std::ostream& operator<<(std::ostream& o, const interval& i) {
    return o << "[" << i.start << ", " << i.stop << ")";
}

std::ostream& operator<<(std::ostream& o, const interval_list& ls) {
    o << "[";

    auto begin = ls.begin();
    auto end = ls.end();
    for (auto it = begin; it != end; ++it) {
        if (it != begin) {
            o << ", ";
        }
        o << *it;
    }

    o << "]";
    return o;
}

void nclist::dump(std::ostream& o, const std::vector<nclist::entry>& entries, int depth) {
    auto line = [&]() -> std::ostream& {
        for (int i = 0; i < depth; ++i) {
            o << "  ";
        }
        return o;
    };

    for (auto& entry : entries) {
        line() << entry.value;
        auto& sub = entry.sublist;
        if (!sub.empty()) {
            o << ", sublist = [\n";
            dump(o, sub, depth + 1);
            line() << "]\n";
        } else {
            o << "\n";
        }
    }
}

std::ostream& operator<<(std::ostream& o, const nclist& n) {
    o << "toplevel = [\n";
    nclist::dump(o, n.intervals);
    o << "]";
    return o;
}
