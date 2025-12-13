#include <cassert>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <print>
#include <fstream>
#include <ranges>

using std::println;
using std::vector;
using std::string;

using FileContents = vector<string>;

FileContents read_file(const string &filename);
int count_paths(std::map<string, vector<string>> node_map, string node, int path_count);

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

    std::map<string, vector<string>> connections;

    for (const auto& row: file_contents) {
        string key, val;
        std::stringstream ss(row);
        std::getline(ss, key, ':');
        std::getline(ss, val);
        auto words = val
            | std::views::split(' ')
            | std::views::filter([](auto&& w) { return !std::ranges::empty(w); })
            | std::views::transform([](auto&& w) {
                    return std::string(w.begin(), w.end());
                });
        std::vector<std::string> values(std::ranges::begin(words), std::ranges::end(words));
        connections[key] = values;
    }

    int part1 = count_paths(connections, "you", 0);
    if (filename == "data/input.txt") assert(part1 == 786);
    int part2 = 0;

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

int count_paths(std::map<string, vector<string>> node_map, string node, int path_count) {
    for (const auto& next: node_map[node]) {
        if (next == "out") {
            path_count += 1;
            return path_count;
        }
        path_count = count_paths(node_map, next, path_count);
    }
    return path_count;
}

