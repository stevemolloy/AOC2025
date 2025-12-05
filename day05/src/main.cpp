#include <cstddef>
#include <numeric>
#include <ranges>
#include <sstream>
#include <vector>
#include <print>
#include <fstream>
#include <string>

using std::println;
using std::print;
using std::vector;
using std::ranges::views::enumerate;

typedef enum {
    STATE_RANGES,
    STATE_ITEMS,
} State;

typedef struct {
    unsigned long int start, end;
} Range;

int main(void) {
    // std::ifstream input_data("data/test.txt");
    std::ifstream input_data("data/input.txt");

    unsigned long int part1=0, part2 = 0;

    State state = STATE_RANGES;
    std::string data_line;
    vector<Range> ranges;
    while (std::getline(input_data, data_line)) {
        switch (state) {
            case STATE_RANGES: {
                if (data_line == "") {
                    for (auto i: ranges) {
                        println("From {} to {}", i.start, i.end);
                    }
                    println("");
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

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

