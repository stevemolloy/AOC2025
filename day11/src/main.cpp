#include <cassert>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <print>
#include <fstream>
#include <ranges>

using std::println;
using std::vector;
using std::string;
using std::unordered_map;

using FileContents = vector<string>;

struct Path {
    unsigned long path_count;
    bool dac;
    bool fft;
};

FileContents read_file(const string &filename);
int count_paths_part1(const std::map<std::string, std::vector<std::string>>& node_map, const std::string& node);
Path count_paths_part2(const std::map<string, vector<string>>& node_map, const string& node, bool dac, bool fft);

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

    int part1 = count_paths_part1(connections, "you");
    if (filename == "data/input.txt") assert(part1 == 786);
    Path part2_path = count_paths_part2(connections, "svr", false, false);

    unsigned long part2 = part2_path.path_count;
    if (filename == "data/input.txt") assert(part2 == 495845045016588);

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

int count_paths_part1(const std::map<string, vector<string>>& node_map, const string& node) {
    static unordered_map<std::string, int> cache;

    if (cache.contains(node)) return cache[node];

    int path_count = 0;

    for (const auto& next : node_map.at(node)) {
        if (next == "out") path_count += 1;
        else path_count += count_paths_part1(node_map, next);
    }

    cache[node] = path_count;
    return path_count;
}

Path count_paths_part2(const std::map<string, vector<string>>& node_map, const string& node, bool dac, bool fft) {
    static unordered_map<string, unordered_map<int, Path>> cache;

    int state_key = (dac ? 1 : 0) | (fft ? 2 : 0);

    if (cache.contains(node) && cache[node].contains(state_key)) return cache[node][state_key];

    Path path_details = {.path_count=0, .dac = dac, .fft = fft};

    for (const auto& next : node_map.at(node)) {
        if (next == "out") {
            if (dac && fft) path_details.path_count += 1;
        } else {
            bool new_dac = dac || (next == "dac");
            bool new_fft = fft || (next == "fft");

            Path new_details = count_paths_part2(node_map, next, new_dac, new_fft);
            path_details.path_count += new_details.path_count;
        }
    }

    cache[node][state_key] = path_details;
    return path_details;
}

