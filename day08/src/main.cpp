#include <algorithm>
#include <cassert>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::vector;
using std::string;

using FileContents = vector<string>;

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

typedef struct {
    size_t p1, p2;
    double distance;
} PointPair;

using Distances = vector<PointPair>;

FileContents read_file(const string &filename);
int point_in_circuit(size_t p_index, vector<vector<size_t>> circuits);
void deal_with_pointpair(PointPair pp, vector<vector<size_t>>& circuits);

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

    size_t num_connections = 10;
    if (filename == "data/input.txt") num_connections = 1000;

    vector<Point> points;
    for (auto row: file_contents)
        points.push_back(Point(row));

    Distances distances;
    for (size_t i = 0; i<points.size()-1; i++) {
        for (size_t j = i+1; j<points.size(); j++) {
            PointPair pp = (PointPair){
                .p1 = i,
                .p2 = j,
                .distance = points[i].dist(points[j]),
            };
            distances.push_back(pp);
        }
    }
    std::sort(distances.begin(), distances.end(), [](PointPair p1, PointPair p2){return p1.distance < p2.distance;});

    vector<vector<size_t>> circuits;

    for (size_t conn_num=0; conn_num<num_connections; conn_num++) {
        PointPair pp = distances[conn_num];
        deal_with_pointpair(pp, circuits);
    }
    println("Conn_num {}/{}", num_connections, distances.size());
    println("Number of circuits = {}", circuits.size());
    vector<size_t> lengths;
    for (auto c: circuits) {
        lengths.push_back(c.size());
    }
    std::sort(lengths.begin(), lengths.end());
    std::reverse(lengths.begin(), lengths.end());
    ulong part1 = (ulong)lengths[0] * (ulong)lengths[1] * (ulong)lengths[2];
    if (filename == "data/input.txt") assert(part1 == 133574);

    ulong part2 = 0;
    while (circuits.size() != 1 || circuits[0].size() < points.size()) {
        PointPair pp = distances[num_connections];
        deal_with_pointpair(pp, circuits);
        println("circuits.size() = {}", circuits.size());
        if (circuits.size() == 1) println("\tcircuits[0].size() = {}", circuits[0].size());
        part2 = points[pp.p1].x * points[pp.p2].x;
        num_connections += 1;
    }
    if (filename == "data/input.txt") assert(part2 == 2435100380);
    println("num_connections = {}", num_connections);

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

void deal_with_pointpair(PointPair pp, vector<vector<size_t>>& circuits) {
    int p1_circuit = point_in_circuit(pp.p1, circuits);
    int p2_circuit = point_in_circuit(pp.p2, circuits);
    if (p1_circuit>=0 && p2_circuit>=0) {
        if (p1_circuit == p2_circuit) return;
        circuits[p1_circuit].insert(circuits[p1_circuit].end(), circuits[p2_circuit].begin(), circuits[p2_circuit].end());
        circuits.erase(circuits.begin() + p2_circuit);
    } else if (p1_circuit<0 && p2_circuit<0) {
        vector<size_t> new_circuit;
        new_circuit.push_back(pp.p1);
        new_circuit.push_back(pp.p2);
        circuits.push_back(new_circuit);
    } else if (p1_circuit<0) {
        circuits[p2_circuit].push_back(pp.p1);
    } else {
        assert(p2_circuit<0);
        circuits[p1_circuit].push_back(pp.p2);
    }
}

int point_in_circuit(size_t p_index, vector<vector<size_t>> circuits) {
    int result = -1;
    for (size_t i=0; i<circuits.size(); i++) {
        vector<size_t> circuit = circuits[i];
        for (size_t j=0; j<circuit.size(); j++) {
            if (circuit[j] == p_index) {
                result = (int)i;
                break;
            }
        }
    }
    return result;
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

