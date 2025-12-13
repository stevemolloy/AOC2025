#include <cassert>
#include <optional>
#include <queue>
#include <ranges>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::vector;
using std::string;

using FileContents = vector<string>;

class Problem {
public:
    uint goal;
    uint state;
    uint start_state;
    size_t state_len;
    vector<uint> buttons;
    Problem(uint goal, size_t state_len, vector<uint> buttons);
    Problem(string str);
    void Apply(uint btn);
    string state_to_str(void) const;
    std::optional<int> MinBtnPresses(void);
};

Problem::Problem(uint goal, size_t state_len, vector<uint> buttons):
    goal(goal), state_len(state_len), buttons(buttons) {
    state = 0;
    start_state = state;
}

Problem::Problem(string str) {
    goal = 0;
    state = 0;
    char c;
    std::stringstream ss(str);
    ss >> c;  // [
    assert(c == '[');
    ss >> c;

    state_len = 0;
    goal = 0;
    int counter = 0;
    while (c!=']') {
        state_len += 1;
        if (c=='#')
            goal += 1 << counter;
        ss >> c;
        counter += 1;
    }
    ss >> c;
    assert(c == '(');

    uint val;
    ss >> val;
    while (c != '{') {
        vector<uint> btns;
        while (c != ')') {
            btns.push_back(val);
            ss >> c;
            if (c == ',') {
                ss >> val;
            }
        }
        ss >> c;
        uint val_to_push = 0;
        for (const auto& i: btns)
            val_to_push |= (1 << i);
        buttons.push_back(val_to_push);
        ss >> val;
    }
}

void Problem::Apply(uint btn) {
    state ^= btn;
}

string Problem::state_to_str(void) const {
    string str(state_len, '.');
    for (size_t i=0; i<state_len; i++) {
        uint mask = 1 << i;
        if ((state & mask) >= 1) {
            str[state_len - i - 1] = '#';
        }
    }
    return str;
}

std::optional<int> Problem::MinBtnPresses(void) {
    if (state == goal) {
        return 0;
    }

    struct SearchNode {
        uint state;
        int depth;
    };

    std::queue<SearchNode> queue;
    std::unordered_map<uint, int> visited;

    queue.push({state, 0});
    visited[start_state] = 0;

    while (!queue.empty()) {
        const auto [current_state, depth] = queue.front();
        queue.pop();

        for (const auto btn: buttons) {
            uint newstate = current_state ^ btn;

            if (newstate == goal) {
                return depth + 1;
            }

            if (!visited.contains(newstate)) {
                visited[newstate] = depth + 1;
                queue.push({newstate, depth+1});
            }
        }
    }

    return std::nullopt;
}

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
    vector<Problem> problems;
    for (const auto& row: file_contents) {
        println("{}", row);
        problems.emplace_back(row);
    }

    for (const auto& prob: problems) {
        println("goal = {}", prob.goal);
        std::print("\tButtons = ");
        for (const auto& btn: prob.buttons) {
            std::print("{} ", btn);
        }
        println("");
    }

    int part1 = 0;
    for (int i=0; auto& prob: problems) {
        auto min_presses = prob.MinBtnPresses();
        if (min_presses.has_value()) {
            println("{}: Min presses = {}", i, min_presses.value());
            part1 += min_presses.value();
        } else {
            println("{}: No answer :(", i);
        }
        i += 1;
    }

    size_t part2 = 0;

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

