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
class AStar
{
public:
    int rows;
    int columns;
    vector<pair<int, int>> *visitedNodes;
    set<pair<int, int>> *walls;

    AStar(int rows, int columns, vector<pair<int, int>> *visitedNodes, set<pair<int, int>> *walls)
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
void AStar<T>::printPairs(vector<pair<T, T>> const &input)
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
bool AStar<T>::isValid(int x, int y, int N, set<pair<int, int>> &walls)
{
    return (x >= 0 && x < N) && (y >= 0 && y < N) && !walls.count(make_pair(x, y));
}

// Function to print the complete path from source to destination
template <typename T>
void AStar<T>::findPath(Node *node, vector<pair<int, int>> &path)
{
    if (node != nullptr)
    {
        findPath(node->parent, path);
        path.push_back(make_pair(node->x, node->y));
    }
}
template <typename T>
vector<pair<int, int>> AStar<T>::findPath(int matrixSize, int x, int y, int endX, int endY, int foodX, int foodY, std::function<void()> renderScreenCallback)
{

    // vector to store the shortest path
    vector<pair<int, int>> path;

    int N = matrixSize;

    // base case
    if (N == 0)
    {
        return path;
    }

    // create a queue and enqueue the first node
    set<Node *> q;
    Node *src = new Node(x, y, abs(endX - x) + abs(endY - y), 0, 1, nullptr);

    q.insert(src);

    // set to check if the matrix cell is visited before or not
    set<Node *> visited;

    visited.insert(src);

    while (!q.empty())
    {
        int least = 999999;
        Node *curr;
        for (Node *currentNode : q)
        {
            if (currentNode->weight < least)
            {
                curr = currentNode;
                least = currentNode->weight;
            }
        }

        q.erase(curr);

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
                Node *next = new Node(x, y, abs(endX - x) + abs(endY - y) + curr->startToCurrent + 1, curr->startToCurrent + 1, foodLeft, curr);

                auto nodeInVisited = std::find_if(visited.begin(),
                                                  visited.end(),
                                                  [&](Node *p)
                                                  { return (p->x == next->x) && (p->y == next->y) && (p->weight <= next->weight); });

                auto nodeInQueue = std::find_if(q.begin(),
                                                q.end(),
                                                [&](Node *p)
                                                { return (p->x == next->x) && (p->y == next->y) && (p->weight <= next->weight); });

                // if it isn't visited yet
                if ((nodeInVisited == visited.end()) && (nodeInQueue == q.end()))
                {

                    q.insert(next);
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