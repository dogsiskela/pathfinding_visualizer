#include <vector>
#include <iostream>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <math.h>
#include <tuple>
#include <functional>

using namespace std;

template <class T>
class DFS
{
public:
    int rows;
    int columns;
    vector<pair<int, int>> *visitedNodes;
    set<pair<int, int>> *walls;

    DFS(int rows, int columns, vector<pair<int, int>> *visitedNodes, set<pair<int, int>> *walls)
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

template <typename T>
void DFS<T>::printPairs(vector<pair<T, T>> const &input)
{
    int n = input.size();
    for (auto const &p : input)
    {
        cout << '(' << p.first << ", " << p.second << ')';
        if (--n)
        {
            cout << ", ";
        }
    }
}
// The function returns false if (x, y) is not a valid position
template <typename T>
bool DFS<T>::isValid(int x, int y, int N, set<pair<int, int>> &walls)
{
    return (x >= 0 && x < N) && (y >= 0 && y < N) && !walls.count(make_pair(x, y));
}

// Function to print the complete path from source to destination
template <typename T>
void DFS<T>::findPath(Node *node, vector<pair<int, int>> &path)
{
    if (node != nullptr)
    {
        findPath(node->parent, path);
        path.push_back(make_pair(node->x, node->y));
    }
}
template <typename T>
vector<pair<int, int>> DFS<T>::findPath(int matrixSize, int x, int y, int endX, int endY, int foodX, int foodY, std::function<void()> renderScreenCallback)
{

    // vector to store the shortest path
    vector<pair<int, int>> path;
    cout << path.size() << endl;

    int N = matrixSize;

    // base case
    if (N == 0)
    {
        return path;
    }

    // create a queue and enqueue the first node
    stack<Node *> q;
    Node *src = new Node(x, y, 1, nullptr);
    q.push(src);

    // set to check if the matrix cell is visited before or not
    set<Node *> visited;

    visited.insert(src);

    while (!q.empty())
    {
        Node *curr = q.top();
        q.pop();

        int foodLeft = curr->foodLeft;
        int i = curr->x;
        int j = curr->y;

        if (i == endX && j == endY)
        {
            findPath(curr, path);
            return path;
        }

        // check all four possible movements from the current cell
        // and recur for each valid movement
        for (int k = 0; k < 4; k++)
        {
            // get next position coordinates using the value of the current cell
            int x = i + row[k];
            int y = j + col[k];

            // check if it is possible to go to the next position
            // from the current position
            if (isValid(x, y, N, *walls))
            {

                // construct the next cell node
                Node *next = new Node(x, y, foodLeft, curr);

                auto nodeInVisited = std::find_if(visited.begin(),
                                                  visited.end(),
                                                  [&](Node *p)
                                                  { return *p == *next; });

                // if it isn't visited yet

                if (nodeInVisited == visited.end())
                {
                    q.push(next);
                    visited.insert(next);
                    (*visitedNodes).push_back(make_pair(next->x, next->y));
                    renderScreenCallback();
                }
            }
        }
    }

    // we reach here if the path is not possible
    return path;
}