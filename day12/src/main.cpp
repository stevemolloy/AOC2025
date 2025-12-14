#include <cassert>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::vector;
using std::string;

using FileContents = vector<string>;

FileContents read_file(const string &filename);

int main(void) {
    // string filename = "data/test.txt";
    string filename = "data/input.txt";
    FileContents file_contents;
    try {
        file_contents = read_file(filename);
    } catch (std::runtime_error &e) {
        println("ERROR: {}", e.what());
        println("ERROR: Filename = '{}'", filename);
        return 1;
    }

    vector<long> p_sizes;
    for (size_t i=0; i<6; i++) {
        int line_index = i*5 + 1;

        string line;
        long p_size = 0;
        for (size_t l=0; l<3; l++) {
            line = file_contents[line_index + l];
            for (size_t j=0; j<3; j++) {
                if (line[j] == '#') {
                    p_size += 1;
                } else if (line[j] != '.') {
                    println("ERROR: Unexpected character: '{}'", line[j]);
                    exit(1);
                }
            }
        }
        p_sizes.push_back(p_size);
    }
    for (auto s: p_sizes) {
        println("{}", s);
    }

    long part1 = 0;

    for (size_t i=30; i<file_contents.size(); i++) {
        char x;
        long x_size, y_size, amount;
        vector<long> amounts;
        std::stringstream ss(file_contents[i]);
        ss >> x_size >> x >> y_size >> x;
        for (size_t s=0; s<6; s++) {
            ss >> amount;
            amounts.push_back(amount);
        }

        long lazy = 0;
        long tight = 0;
        for (int i = 0; auto amount: amounts) {
            lazy += 9 * amount;
            tight += p_sizes[i++] * amount;
        }
        long total_size = x_size * y_size;
        if (total_size < tight) {
            continue;
        } else if (lazy <= total_size) {
            part1 += 1;
        } else {
            assert(false && "You need to implement more complex logic for this one");
        }
    }

    if (filename == "data/input.txt") assert(part1 == 460);

    long part2 = 0;

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

FileContents read_file(const string &filename) {
    std::ifstream file_stream(filename);
    if (!file_stream.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    vector<string> file_contents;
    string line;
    while (std::getline(file_stream, line))
        file_contents.push_back(std::move(line));
    return file_contents;
}

