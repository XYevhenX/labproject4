#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <climits>

using namespace std;

class Point {
public:
    unsigned long long row;
    unsigned long long pos;

    Point(unsigned long long r = 0, unsigned long long p = 0) : row(r), pos(p) {}

    bool operator<(const Point& other) const {
        if (row != other.row) return row < other.row;
        return pos < other.pos;
    }

    bool operator==(const Point& other) const {
        return row == other.row && pos == other.pos;
    }
};

class OrchardSolver {
private:
    set<Point> dead_trees;
    set<Point> victims;
    set<Point> replanted;
    unsigned long long initial_dead_count = 0;

    void find_isolated_trees() {
        set<Point> checked_alive;
        for (const auto& dt : dead_trees) {
            for (long long dr = -1; dr <= 1; ++dr) {
                for (long long dp = -1; dp <= 1; ++dp) {
                    if (dr == 0 && dp == 0) continue;

                    if (dr == -1 && dt.row == 0) continue;
                    if (dp == -1 && dt.pos == 0) continue;
                    if (dr == 1 && dt.row == ULLONG_MAX) continue;
                    if (dp == 1 && dt.pos == ULLONG_MAX) continue;

                    Point neighbor(dt.row + dr, dt.pos + dp);
                    if (dead_trees.find(neighbor) == dead_trees.end() && checked_alive.find(neighbor) == checked_alive.end()) {
                        if (is_threatened(neighbor)) {
                            victims.insert(neighbor);
                        }
                        checked_alive.insert(neighbor);
                    }
                }
            }
        }
    }

    bool is_threatened(const Point& p) {
        for (long long dr = -1; dr <= 1; ++dr) {
            for (long long dp = -1; dp <= 1; ++dp) {
                if (dr == 0 && dp == 0) continue;

                if (dr == -1 && p.row == 0) continue;
                if (dp == -1 && p.pos == 0) continue;
                if (dr == 1 && p.row == ULLONG_MAX) continue;
                if (dp == 1 && p.pos == ULLONG_MAX) continue;

                Point neighbor(p.row + dr, p.pos + dp);
                if (dead_trees.find(neighbor) == dead_trees.end()) {
                    return false;
                }
            }
        }
        return true;
    }

    void solve() {
        map<Point, vector<Point>> candidates;
        for (const auto& v : victims) {
            for (long long dr = -1; dr <= 1; ++dr) {
                for (long long dp = -1; dp <= 1; ++dp) {
                    if (dr == 0 && dp == 0) continue;

                    if (dr == -1 && v.row == 0) continue;
                    if (dp == -1 && v.pos == 0) continue;
                    if (dr == 1 && v.row == ULLONG_MAX) continue;
                    if (dp == 1 && v.pos == ULLONG_MAX) continue;

                    Point candidate(v.row + dr, v.pos + dp);
                    if (dead_trees.find(candidate) != dead_trees.end()) {
                        candidates[candidate].push_back(v);
                    }
                }
            }
        }

        set<Point> covered_victims;
        while (covered_victims.size() < victims.size()) {
            Point best_p;
            size_t max_cover = 0;

            for (auto it = candidates.begin(); it != candidates.end();) {
                size_t current_cover = 0;
                for (const auto& v : it->second) {
                    if (covered_victims.find(v) == covered_victims.end()) {
                        current_cover++;
                    }
                }

                if (current_cover == 0) {
                    it = candidates.erase(it);
                }
                else {
                    if (current_cover > max_cover) {
                        max_cover = current_cover;
                        best_p = it->first;
                    }
                    ++it;
                }
            }

            if (max_cover > 0) {
                replanted.insert(best_p);
                for (const auto& v : candidates[best_p]) {
                    covered_victims.insert(v);
                }
            }
            else {
                break;
            }
        }
    }

public:
    void run() {
        string line;
        cout << "Enter coordinates (row position) line by line. Empty line to finish:" << endl;
        while (getline(cin, line) && !line.empty()) {        
            stringstream ss(line);
            unsigned long long r, p;
            if (!(ss >> r >> p) || r == 0 || p == 0 || (r - 1) % 8 != 0 || (p - 1) % 8 != 0) {
                cout << "***** error" << endl;
                return;
            }

            Point normalized_point((r - 1) / 8, (p - 1) / 8);
            if (dead_trees.insert(normalized_point).second) {
                initial_dead_count++;
            }
        }

        find_isolated_trees();
        solve();

        cout << "THE WORK IS DONE" << endl;
        cout << initial_dead_count << endl;
        cout << replanted.size() << endl;
        for (const auto& p : replanted) {
            cout << (p.row * 8 + 1) << " " << (p.pos * 8 + 1) << endl;
        }
    }
};

void greeting() {
    cout << "The author of this program is Ziuzin Yevhen.\n";
    cout << "This program finds a minimal replanting scheme for a cherry orchard. Variant 105.\n";
}

int main() {
    greeting();

    OrchardSolver solver;
    solver.run();

    return 0;
}