#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <print>
#include <fstream>
#include <string>

using std::println;
using std::print;
using std::vector;

typedef enum {
    STATE_RANGES,
    STATE_ITEMS,
} State;

typedef struct {
    unsigned long int start, end;
} Range;

typedef enum {
    RNG_DISJOINT,
    RNG_OVERLAP,
    RNG_WITHIN,
    RNG_SAME,
} RngComparison;

RngComparison compare_ranges(Range r1, Range r2) {
    assert(r1.start <= r2.start);   // Already sorted
    assert(r1.start <= r1.end);    // Properly formed ranges
    assert(r2.start <= r2.end);    // Properly formed ranges
    if (r1.start == r2.start && r1.end == r2.end) return RNG_SAME;
    if (r2.start > r1.end) return RNG_DISJOINT;
    if (r2.end <= r1.end) return RNG_WITHIN;
    return RNG_OVERLAP;
}

std::string range_to_string(RngComparison r) {
    switch (r) {
        case RNG_DISJOINT: return "Disjoint";
        case RNG_OVERLAP: return "Overlap";
        case RNG_WITHIN: return "Within";
        case RNG_SAME: return "Same";
    }
}

unsigned long int sum_ranges(const vector<Range> &range) {
    unsigned long int sum = 0;
    for (auto r: range) {
        if (r.start > r.end) continue;
        unsigned long int gap = r.end - r.start + 1;
        println("Gap = {}", gap);
        sum += gap;
    }
    return sum;
}

vector<Range> fix_ranges(vector<Range> &ranges) {
    std::sort(
        ranges.begin(),
        ranges.end(),
        [](Range a, Range b){
            return b.start > a.start;
        }
    );
    vector<Range> result;
    result.push_back(ranges[0]);

    size_t rsize = ranges.size();
    for (size_t i=1; i<rsize; i++) {
        RngComparison comp = compare_ranges(result.back(), ranges[i]);
        if (comp == RNG_SAME) continue;
        else if (comp == RNG_WITHIN) continue;
        else if (comp == RNG_DISJOINT) {
            result.push_back(ranges[i]);
        } else if (comp == RNG_OVERLAP) {
            result.back().end = ranges[i].end;
        } else {
            println("ERROR: Weird return value from compare_ranges()");
            exit(1);
        }
    }

    return result;
}

int main(void) {
    // std::ifstream input_data("data/test.txt");
    // std::ifstream input_data("data/test2.txt");
    std::ifstream input_data("data/input.txt");

    unsigned long int part1=0, part2 = 0;

    State state = STATE_RANGES;
    std::string data_line;
    vector<Range> ranges;
    while (std::getline(input_data, data_line)) {
        switch (state) {
            case STATE_RANGES: {
                if (data_line == "") {
                    state = STATE_ITEMS;
                    continue;
                }
                unsigned long int start, end;
                char dash;
                std::stringstream(data_line) >> start >> dash >> end;
                Range range(start, end);
                ranges.push_back(std::move(range));
            } break;
            case STATE_ITEMS: {
                unsigned long int test_val;
                std::stringstream(data_line) >> test_val;
                for (auto r: ranges) {
                    if (r.start <= test_val && test_val <=r.end) {
                        part1 += 1;
                        break;
                    }
                }
            }
        }
    }

    std::sort(
        ranges.begin(),
        ranges.end(),
        [](Range a, Range b){
            return b.start > a.start;
        }
    );

    println("---------------------------------");
    for (auto r: ranges)
        println("{} <--> {}", r.start, r.end);

    vector<Range> actual_ranges = fix_ranges(ranges);

    println("---------------------------------");
    for (auto r: actual_ranges)
        println("{} <--> {}", r.start, r.end);
    println("---------------------------------");

    for (auto r: actual_ranges) {
        part2 += r.end - r.start + 1;
    }
    println("Part 1: {}", part1);
    // 334806164202981 is too low
    println("Part 2: {}", part2);

    return 0;
}

