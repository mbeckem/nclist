#ifndef NCLIST_HPP
#define NCLIST_HPP

#include <cassert>
#include <iosfwd>
#include <vector>

/// An interval is a half open range [start, stop).
/// It contains all values x such that start <= x < stop.
/// stop >= start must always be true. If start == stop,
/// then the interval is empty.
struct interval {
    interval() {}

    interval(int start, int stop): start(start), stop(stop) {
        assert(start <= stop);
    }

    /// Returns true if the interval y is completely contained in *this.
    bool contains(const interval& y) const {
        return start <= y.start && y.stop < stop;
    }

    bool operator==(const interval &y) const {
        return start == y.start && stop == y.stop;
    }

    bool operator!=(const interval &y) const {
        return start != y.start || stop != y.stop;
    }

    int start = 0;  ///< Inclusive.
    int stop = 0;   ///< Exclusive.
};

using interval_list = std::vector<interval>;

/// A nested containment list (nclist) is a recursive, tree-like datastructure.
/// Children of the same nclist item (toplevel or deeper) are sorted by begin
/// as well as end values.
class nclist {
public:
    /// Build a nclist using the given list of intervals.
    static nclist build(const interval_list& intervals);

    /// Returns a list of all intervals that overlap with the query interval.
    interval_list overlap(const interval& query) const;

private:
    /// Every entry has a value (an interval) and a sublist of intervals
    /// which are completely contained inside the value,
    /// i.e. for every s in sublist: s is a subinterval of value.
    struct entry {
        entry(interval value): value(value) {}
        entry(interval value, std::vector<entry> sublist)
            : value(value), sublist(std::move(sublist)) {}

        interval value;
        std::vector<entry> sublist;
    };

private:
    nclist(std::vector<entry> intervals): intervals(std::move(intervals)) {}

    /// Build a sublist for the given parent (end if top level)
    /// starting at the given position until the end is reached.
    static std::vector<nclist::entry> build_sublist(
        interval_list::iterator parent,
        interval_list::iterator& pos,
        interval_list::iterator end);

    /// Finds overlapping intervals in the given sublist, recursing if necessary.
    /// Results are appended to the given interval_list.
    void overlap(const std::vector<entry>& sublist, const interval& query, interval_list& result) const;

    /// Format the list of entries.
    static void dump(std::ostream& o, const std::vector<nclist::entry>& entries, int depth = 1);

    friend std::ostream& operator<<(std::ostream& o, const nclist& n);

private:
    /// The list of top level intervals.
    std::vector<entry> intervals;
};

std::ostream& operator<<(std::ostream& o, const interval& i);
std::ostream& operator<<(std::ostream& o, const interval_list& i);
std::ostream& operator<<(std::ostream& o, const nclist& n);

#endif // NCLIST_HPP
