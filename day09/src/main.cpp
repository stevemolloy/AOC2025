#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <sstream>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::print;
using std::vector;
using std::string;

using FileContents = vector<string>;

FileContents read_file(const string &filename);

class Point {
    public:
    Point(long x, long y);
    Point(const std::string& str);
    long x, y;
};

Point::Point(long x, long y): x(x), y(y) {}

Point::Point(const std::string& str) {
    // 7,1
    char c;
    std::stringstream(str) >> x >> c >> y;
}

class Rect {
public:
    Rect(Point p1, Point p2);
    Point p1, p2;
    long area;
};

Rect::Rect(Point p1, Point p2): p1(p1), p2(p2) {
    long width = std::abs(p1.x - p2.x + 1);
    long height = std::abs(p1.y - p2.y + 1);
    area = width*height;
}

enum CheckState {
    CS_STILL_ON_BOUNDARY,
    CS_INSIDE,
    CS_OUTSIDE,
    CS_BACK_ON_BOUNDARY,
};

CheckState advance_state(CheckState state, bool grid_pt) {
    switch (state) {
        case CS_STILL_ON_BOUNDARY: {
            if (grid_pt)
                return CS_STILL_ON_BOUNDARY;
            return CS_INSIDE;
        } break;
        case CS_INSIDE: {
            if (!grid_pt)
                return CS_INSIDE;
            return CS_BACK_ON_BOUNDARY;
        } break;
        case CS_BACK_ON_BOUNDARY: {
            if (!grid_pt)
                state = CS_OUTSIDE;
            return CS_STILL_ON_BOUNDARY;
        } break;
        case CS_OUTSIDE: {
            return CS_OUTSIDE;
        } break;
    }
    assert(false && "Unreachable!");
    return CS_OUTSIDE;
}

bool check_rect_in_grid(Rect rect, vector<vector<bool>> grid) {
    CheckState state = CS_STILL_ON_BOUNDARY;
    int x_dir = rect.p1.x <= rect.p2.x ? 1 : -1;
    // int y_dir = rect.p1.y <= rect.p2.y ? 1 : -1;

    // First edge
    int y = rect.p1.y;
    for (int x=rect.p1.x; x!=rect.p2.x+x_dir; x+=x_dir) {
        state = advance_state(state, grid[y][x]);
        if (state == CS_OUTSIDE)
            return false;
    }

    return true;
}

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
    vector<Point> points;
    points.reserve(file_contents.size());
    for (const auto& line: file_contents)
        points.emplace_back(line);

    vector<Rect> rects;
    rects.reserve((points.size() * (points.size() - 1)) / 2);
    for (size_t i=0; i<points.size()-1; i++)
        for (size_t j=1; j<points.size(); j++)
            rects.emplace_back(points[i], points[j]);

    std::sort(rects.begin(), rects.end(), [](Rect r1, Rect r2){return r1.area > r2.area;});

    size_t part1 = rects[0].area;
    if (filename=="data/input.txt") assert(part1 == 4786902990);

    long max_x = LONG_MIN, max_y = LONG_MIN;
    long min_x = LONG_MAX, min_y = LONG_MAX;
    for (const auto& point: points) {
        if (point.x > max_x) max_x = point.x;
        if (point.y > max_y) max_y = point.y;
        if (point.x < min_x) min_x = point.x;
        if (point.y < min_y) min_y = point.y;
    }
    min_x -= 1;
    min_y -= 1;
    max_x += 1;
    max_y += 1;

    assert(min_x>=0);
    assert(min_y>=0);

    vector<vector<bool>> grid;
    grid.reserve(max_y);
    for (long i=0; i<max_y; i++) {
        grid.emplace_back(max_x, false);
    }
    for (const auto& pt: points) {
        grid[pt.y][pt.x] = true;
    }
    for (size_t i=0; i<points.size(); i++) {
        Point& this_pt = points[i];
        Point& next_pt = points[(i+1) % points.size()];
        if (this_pt.x == next_pt.x) {
            if (this_pt.y < next_pt.y) {
                for (size_t j=this_pt.y; j<(size_t)next_pt.y; j++) 
                    grid[j][this_pt.x] = true;
            } else {
                for (size_t j=next_pt.y; j<(size_t)this_pt.y; j++) 
                    grid[j][this_pt.x] = true;
            }
        }
        if (this_pt.y == next_pt.y) {
            if (this_pt.x < next_pt.x) {
                for (size_t j=this_pt.x; j<(size_t)next_pt.x; j++) 
                    grid[this_pt.y][j] = true;
            } else {
                for (size_t j=next_pt.x; j<(size_t)this_pt.x; j++) 
                    grid[this_pt.y][j] = true;
            }
        }
    }

    size_t part2 = 0;

    if (filename == "data/input.txt") {
        for (const auto& rect: rects) {
            if (rect.p1.x == 94768 || rect.p2.x == 94768) {
                println("Area = {}", rect.area);
            }
        }
    }

    if (filename == "data/test.txt") {
        for (const auto& row: grid) {
            for (const bool& c: row) {
                if (c) print("#");
                else print(".");
            }
            println("");
        }
    }

    // for (const auto& rect: rects) {
    //     if (check_rect_in_grid(rect, grid)) {
    //         part2 = rect.area;
    //         break;
    //     }
    // }

    println("Part 1: {}", part1);
    // 2991926324 is too high
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

