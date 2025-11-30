#include <ranges>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using std::ranges::sort;
using std::ranges::views::zip_transform;
using std::ranges::fold_left;

constexpr std::string data_dir = "data/";

int main(void) {
    // std::ifstream input_data(data_dir + "test.txt");
    std::ifstream input_data(data_dir + "input.txt");

    std::string data_line;
    std::vector<int> list1, list2;
    int val;
    while (std::getline(input_data, data_line)) {
        std::stringstream ss(data_line);

        ss >> val;
        list1.push_back(val);

        ss >> val;
        list2.push_back(val);
    }

    assert(list1.size() == list2.size());

    sort(list1);
    sort(list2);

    auto fn = [](int a, int b){return std::abs(a-b);};
    auto zipped_diffs = zip_transform(fn, list1, list2);
    int res = fold_left(zipped_diffs, 0, std::plus());

    std::cout << res << std::endl;

    return 0;
}
