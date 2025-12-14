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

constexpr int PACKAGE_TYPES = 6;
constexpr int ROWS = 3;

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

    string line;
    vector<int> p_sizes;
    for (size_t i=0; i<PACKAGE_TYPES; i++) {
        int line_index = i*(PACKAGE_TYPES-1) + 1;
        int p_size = 0;
        for (size_t l=0; l<ROWS; l++) {
            line = file_contents[line_index + l];
            for (size_t j=0; j<ROWS; j++) {
                if (line[j] == '#') p_size += 1;
                else if (line[j] != '.') {
                    println("ERROR: Unexpected character: '{}'", line[j]);
                    exit(1);
                }
            }
        }
        p_sizes.push_back(p_size);
    }

    int part1 = 0;
    for (size_t i=30; i<file_contents.size(); i++) {
        char x;
        int x_size, y_size, amount;
        vector<int> amounts;
        std::stringstream ss(file_contents[i]);
        ss >> x_size >> x >> y_size >> x;
        for (size_t s=0; s<PACKAGE_TYPES; s++) {
            ss >> amount;
            amounts.push_back(amount);
        }

        int lazy = 0, tight = 0;
        for (int j = 0; auto amount: amounts) {
            lazy += ROWS*ROWS * amount;
            tight += p_sizes[j++] * amount;
        }
        int total_size = x_size * y_size;
        if (total_size < tight) continue;         // Impossible. Don't even try.
        else if (lazy <= total_size) part1 += 1;  // Trivial
        else assert(false && "You need to implement more complex logic for this one");
    }

    if (filename == "data/input.txt") assert(part1 == 460);

    println("Part 1: {}", part1);

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

