#include <iostream>

#include "nclist.hpp"

int main(int argc, char *argv[])
{
    (void) argc, (void) argv;

    // The list of input intervals.
    const interval_list intervals = {
        {1, 5},
        {3, 6},
        {3, 6},
        {3, 5},
        {4, 5},
        {3, 8},
        {8, 11},
        {4, 7},
        {1, 2},
        {7, 8},
    };
    std::cout << "Input intervals:\n"
              << intervals << "\n\n"
              << std::flush;

    // The nested containment list that contains the intervals.
    const nclist n = nclist::build(intervals);

    std::cout << "Resulting nclist:\n"
              << n << "\n\n"
              << std::flush;

    // Some example query intervals.
    // For every query interval, we search for all overlapping intervals
    // in the nested containment list.
    const interval_list query_intervals = {
        {1, 7}, {7, 9}, {3, 4}, {8, 99}, {99, 199}
    };
    for (size_t i = 0; i < query_intervals.size(); ++i) {
        const interval& query = query_intervals[i];
        std::cout << "Overlapping intervals for " << query << ":\n"
                  << n.overlap(query) << "\n"
                  << std::flush;
    }


    return 0;
}
