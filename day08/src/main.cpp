#include <algorithm>
#include <optional>
#include <cassert>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::vector;
using std::string;

using FileContents = vector<string>;

constexpr size_t TEST_CONNS = 10;
constexpr size_t INPUT_CONNS = 1000;

class Point {
    public:
        Point(double x, double y, double z);
        Point(const std::string& str);
        ~Point() =default;
        double dist(const Point& other) const;
        void dump() const;
        double x, y, z;
};

Point::Point(double x, double y, double z): x(x), y(y), z(z) {}

Point::Point(const std::string& str) {
    // 162,817,812
    char c;
    std::stringstream(str) >> x >> c >> y >> c >> z;
}

double Point::dist(const Point& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    double dz = z - other.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

void Point::dump() const {
    println("x = {} :: y = {} :: z = {}", x, y, z);
}

struct PointPair {
    size_t p1, p2;
    double distance;
};

FileContents read_file(const string &filename);
std::optional<size_t> point_in_circuit(size_t p_index, const vector<vector<size_t>>& circuits);
void add_pointpair_to_circuits(const PointPair& pp, vector<vector<size_t>>& circuits);

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

    size_t num_connections = TEST_CONNS;
    if (filename == "data/input.txt") num_connections = INPUT_CONNS;

    vector<Point> points;
    points.reserve(file_contents.size());
    for (const auto& row: file_contents)
        points.emplace_back(row);

    vector<PointPair> distances;
    distances.reserve((points.size() * (points.size()-1))/2);
    for (size_t i = 0; i<points.size()-1; i++) {
        for (size_t j = i+1; j<points.size(); j++) {
            distances.emplace_back(i, j, points[i].dist(points[j]));
        }
    }
    std::sort(
            distances.begin(),
            distances.end(),
            [](const PointPair& p1, const PointPair& p2){return p1.distance < p2.distance;}
        );

    vector<vector<size_t>> circuits;
    for (size_t conn_num=0; conn_num<num_connections; conn_num++) {
        const PointPair& pp = distances[conn_num];
        add_pointpair_to_circuits(pp, circuits);
    }

    vector<size_t> lengths;
    lengths.reserve(circuits.size());
    for (const auto& c: circuits)
        lengths.push_back(c.size());

    std::sort(lengths.begin(), lengths.end(), std::greater<size_t>());

    size_t part1 = lengths[0] * lengths[1] * lengths[2];
    if (filename == "data/input.txt") assert(part1 == 133574);

    size_t part2 = 0;
    while (circuits.size() != 1 || circuits[0].size() < points.size()) {
        const PointPair& pp = distances[num_connections];
        add_pointpair_to_circuits(pp, circuits);
        part2 = points[pp.p1].x * points[pp.p2].x;
        num_connections += 1;
    }
    if (filename == "data/input.txt") assert(part2 == 2435100380);

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

void add_pointpair_to_circuits(const PointPair& pp, vector<vector<size_t>>& circuits) {
    auto p1_circuit = point_in_circuit(pp.p1, circuits);
    auto p2_circuit = point_in_circuit(pp.p2, circuits);
    if (p1_circuit && p2_circuit) {
        if (*p1_circuit == *p2_circuit) return;
        circuits[*p1_circuit].insert(      // Both are in existing circuits
                circuits[*p1_circuit].end(),  // This new connection joins those
                circuits[*p2_circuit].begin(), // circuits into one.
                circuits[*p2_circuit].end()
            );
        circuits.erase(circuits.begin() + *p2_circuit);  // Erase the extraneous circuit
    } else if (!p1_circuit && !p2_circuit) {  // A brand new circuit
        circuits.push_back({pp.p1, pp.p2});
    } else if (!p1_circuit) {                    // Add the new node to the old circuit
        circuits[*p2_circuit].push_back(pp.p1);
    } else {
        circuits[*p1_circuit].push_back(pp.p2);
    }
}

std::optional<size_t> point_in_circuit(size_t p_index, const vector<vector<size_t>>& circuits) {
    for (size_t i=0; i<circuits.size(); i++) {
        if (std::find(circuits[i].begin(), circuits[i].end(), p_index) != circuits[i].end())
            return i;
    }
    return std::nullopt;
}

vector<string> read_file(const string &filename) {
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

