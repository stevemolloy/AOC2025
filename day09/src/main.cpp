#include <algorithm>
#include <cassert>
#include <sstream>
#include <vector>
#include <print>
#include <fstream>

using std::println;
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

class Rectangle {
public:
    Rectangle(Point p1, Point p2);
    Point p1, p2;
    long area;
};

Rectangle::Rectangle(Point p1, Point p2): p1(p1), p2(p2) {
    long width = std::abs(p1.x - p2.x + 1);
    long height = std::abs(p1.y - p2.y + 1);
    area = width*height;
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
    for (const auto& line: file_contents) {
        points.emplace_back(line);
    }

    vector<Rectangle> rects;
    rects.reserve((points.size() * (points.size() - 1)) / 2);
    for (size_t i=0; i<points.size()-1; i++) {
        for (size_t j=1; j<points.size(); j++) {
            rects.emplace_back(points[i], points[j]);
        }
    }

    std::sort(rects.begin(), rects.end(), [](Rectangle r1, Rectangle r2){return r1.area > r2.area;});
    println("{}", rects[0].area);

    size_t part1 = rects[0].area;
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

