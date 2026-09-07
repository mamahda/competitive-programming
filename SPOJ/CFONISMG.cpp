#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <utility>

using namespace std;

class Student {
private:
    string name;
    string status;

public:
    Student() = default;

    Student(const string& name, const string& status)
        : name(name), status(status) {}

    const string& getName() const {
        return name;
    }

    const string& getStatus() const {
        return status;
    }

    bool isDisruptive() const {
        return status == "DISRUPTIVE";
    }
};

class Classroom {
private:
    int rows;
    int cols;

    vector<int> assigned;

    vector<vector<int>> backNeighbors;

public:
    Classroom(int rows, int cols)
        : rows(rows),
          cols(cols),
          assigned(rows * cols, -1),
          backNeighbors(rows * cols) {

        buildNeighbors();
    }

    int getRowCount() const {
        return rows;
    }

    int getColCount() const {
        return cols;
    }

    int getCellCount() const {
        return rows * cols;
    }

    const vector<int>& getBackNeighbors(int cell) const {
        return backNeighbors[cell];
    }

    int getStudent(int cell) const {
        return assigned[cell];
    }

    void setStudent(int cell, int studentId) {
        assigned[cell] = studentId;
    }

private:
    void buildNeighbors() {
        vector<vector<int>> indexOf(
            rows,
            vector<int>(cols)
        );

        int index = 0;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                indexOf[r][c] = index++;
            }
        }

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int current = indexOf[r][c];

                if (r > 0) {
                    backNeighbors[current].push_back(
                        indexOf[r - 1][c]
                    );
                }

                if (c > 0) {
                    backNeighbors[current].push_back(
                        indexOf[r][c - 1]
                    );
                }
            }
        }
    }
};

class SeatingSolver {
private:
    int studentCount;
    int friendCount;
    vector<Student> students;
    vector<int> friendMask;
    vector<int> rivalMask;
    Classroom classroom;
    int usedMask;
    int best;

public:
    SeatingSolver(
        int rows,
        int cols,
        const vector<Student>& students,
        const vector<pair<int, int>>& friends,
        const vector<pair<int, int>>& rivals
    )
        : studentCount(static_cast<int>(students.size())),
          friendCount(static_cast<int>(friends.size())),
          students(students),
          classroom(rows, cols),
          usedMask(0),
          best(0) {

        buildRelationshipMasks(friends, rivals);
    }

    int solve() {
        if (studentCount > classroom.getCellCount()) {
            return -1;
        }

        backtrack(
            0,
            0,
            0,
            0
        );

        return best;
    }

private:
    static int popcount(int value) {
        int count = 0;

        while (value != 0) {
            value &= (value - 1);
            ++count;
        }

        return count;
    }

    void buildRelationshipMasks(
        const vector<pair<int, int>>& friends,
        const vector<pair<int, int>>& rivals
    ) {
        friendMask.assign(studentCount, 0);
        rivalMask.assign(studentCount, 0);

        for (const auto& [u, v] : friends) {
            friendMask[u] |= (1 << v);
            friendMask[v] |= (1 << u);
        }

        for (const auto& [u, v] : rivals) {
            rivalMask[u] |= (1 << v);
            rivalMask[v] |= (1 << u);
        }
    }

    bool isValidPlacement(
        int student,
        const vector<int>& neighborStudents
    ) const {
        for (int neighbor : neighborStudents) {

            if ((rivalMask[student] >> neighbor) & 1) {
                return false;
            }

            if (
                students[student].isDisruptive() &&
                students[neighbor].isDisruptive()
            ) {
                return false;
            }
        }

        return true;
    }

    int calculateAddedFriends(
        int student,
        const vector<int>& neighborStudents
    ) const {
        int count = 0;

        for (int neighbor : neighborStudents) {
            if ((friendMask[student] >> neighbor) & 1) {
                ++count;
            }
        }

        return count;
    }

    int calculateLostFriends(
        int student,
        int usedMaskValue,
        int adjacentMask
    ) const {
        int alreadyUsedFriends =
            friendMask[student] & usedMaskValue;

        int lostFriends =
            alreadyUsedFriends & ~adjacentMask;

        return popcount(lostFriends);
    }

    void backtrack(
        int cellPos,
        int currentUsedMask,
        int currentCount,
        int lostTotal
    ) {
        if (cellPos == classroom.getCellCount()) {
            best = max(best, currentCount);
            return;
        }

        int bound =
            studentCount - lostTotal;

        if (bound <= best) {
            return;
        }

        vector<int> neighborStudents;
        int adjacentMask = 0;

        for (int neighborCell :
             classroom.getBackNeighbors(cellPos)) {

            int student =
                classroom.getStudent(neighborCell);

            if (student != -1) {
                neighborStudents.push_back(student);
                adjacentMask |= (1 << student);
            }
        }

        vector<pair<int, int>> candidates;

        int allStudentMask =
            (1 << studentCount) - 1;

        int remainMask =
            (~currentUsedMask) & allStudentMask;

        for (int student = 0;
             student < studentCount;
             ++student) {

            if (!((remainMask >> student) & 1)) {
                continue;
            }

            int score =
                popcount(
                    friendMask[student] &
                    adjacentMask
                );

            candidates.push_back({
                -score,
                student
            });
        }

        sort(
            candidates.begin(),
            candidates.end()
        );

        for (const auto& candidate : candidates) {

            int student = candidate.second;

            if (!isValidPlacement(
                    student,
                    neighborStudents
                )) {
                continue;
            }

            int addedFriends =
                calculateAddedFriends(
                    student,
                    neighborStudents
                );

            int lostIncrement =
                calculateLostFriends(
                    student,
                    currentUsedMask,
                    adjacentMask
                );

            classroom.setStudent(
                cellPos,
                student
            );

            backtrack(
                cellPos + 1,
                currentUsedMask | (1 << student),
                currentCount + addedFriends,
                lostTotal + lostIncrement
            );


            classroom.setStudent(
                cellPos,
                -1
            );
        }

        backtrack(
            cellPos + 1,
            currentUsedMask,
            currentCount,
            lostTotal
        );
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int rows, cols;
    cin >> rows >> cols;

    int studentCount;
    cin >> studentCount;

    vector<Student> students;
    students.reserve(studentCount);

    unordered_map<string, int> nameToId;
    nameToId.reserve(studentCount * 2);

    for (int i = 0;
         i < studentCount;
         ++i) {

        string name;
        string status;

        cin >> name >> status;

        students.emplace_back(
            name,
            status
        );

        nameToId[name] = i;
    }

    int friendCount;
    cin >> friendCount;

    vector<pair<int, int>> friends;
    friends.reserve(friendCount);

    for (int i = 0;
         i < friendCount;
         ++i) {

        string a, b;
        cin >> a >> b;

        int u = nameToId[a];
        int v = nameToId[b];

        friends.push_back({
            u,
            v
        });
    }

    int rivalCount;
    cin >> rivalCount;

    vector<pair<int, int>> rivals;
    rivals.reserve(rivalCount);

    for (int i = 0;
         i < rivalCount;
         ++i) {

        string a, b;
        cin >> a >> b;

        int u = nameToId[a];
        int v = nameToId[b];

        rivals.push_back({
            u,
            v
        });
    }

    SeatingSolver solver(
        rows,
        cols,
        students,
        friends,
        rivals
    );

    int result = solver.solve();

    if (result == -1) {
        cout << "Impossible\n";
    } else {
        cout << result << '\n';
    }

    return 0;
}