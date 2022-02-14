#include <vector>
#include <iostream>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <math.h>
#include <tuple>
#include <functional>

template <class T>
class SearchAlgorithm
{
public:
    int rows;
    int columns;
    vector<pair<int, int>> *visitedNodes;
    set<pair<int, int>> *walls;

    SearchAlgorithm(int rows, int columns, vector<pair<int, int>> *visitedNodes, set<pair<int, int>> *walls)
    {
        this->rows = rows;
        this->columns = columns;
        this->visitedNodes = visitedNodes;
        this->walls = walls;
    }
    void printPairs(vector<pair<T, T>> const &input);
    bool isValid(int x, int y, int N, set<pair<int, int>> &walls);
    void findPath(Node *node, vector<pair<int, int>> &path);
    vector<pair<int, int>> findPath(int matrixSize, int x, int y, int endX, int endY, int foodX, int foodY, std::function<void()> callback);
};
