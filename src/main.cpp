#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <OpenGLPrj.hpp>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <math.h>
#include <tuple>
#include "algorithms/node.h"
#include "algorithms/bfs.h"
#include "algorithms/dfs.h"
#include "algorithms/astar.h"
#include "world/grid.h"
#include "shaders.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, int &currentX, int &currentY);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void renderFrame(GLFWwindow *window, Shader cellShader);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void process_mouse_hold(GLFWwindow *window);
void saveCells();
void loadCells();

vector<pair<int, int>> path;
set<pair<int, int>> walls;

enum cellType
{
    start_cell,
    end_cell,
    food_cell,
    other
};

enum searchAlg
{
    a_star,
    bfs,
    dfs
};

int matrixSize = 80;
// Matrix size
int columns = matrixSize;
int rows = matrixSize;
// float cellWidth = 0.05f;

// Starting cell position
int startPosX = 0;
int startPosY = 0;

// End goal position
int endPosX = 6;
int endPosY = 4;

// Food location position
int foodPosX = 3;
int foodPosY = 3;

bool startAlg = false;
bool isRunningAlg = false;

int paintWidth = 0;

searchAlg currentSearchAlg = bfs;

float cellWidth = (float)2 / columns;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

vector<pair<int, int>> visitedNodes;

BFS<int> searchAlgBFS = BFS<int>(rows, columns, &visitedNodes, &walls);
DFS<int> searchAlgDFS = DFS<int>(rows, columns, &visitedNodes, &walls);
AStar<int> searchAlgAStar = AStar<int>(rows, columns, &visitedNodes, &walls);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pathfinder", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // Build shader
    Shader cellShader("../res/shaders/cellShader.vert", "../res/shaders/cellShader.frag");

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    Grid newGrid = Grid(rows, columns);
    newGrid.init();

    loadCells();

    while (!glfwWindowShouldClose(window))
    {

        if (startAlg == true && !isRunningAlg)
        {
            startAlg = false;
            isRunningAlg = true;
            if (currentSearchAlg == bfs)
            {

                path = searchAlgBFS.findPath(columns, startPosX, startPosY, endPosX, endPosY, foodPosX, foodPosY, [window, cellShader]()
                                             { renderFrame(window, cellShader); });
            }
            else if (currentSearchAlg == dfs)
            {
                path = searchAlgDFS.findPath(columns, startPosX, startPosY, endPosX, endPosY, foodPosX, foodPosY, [window, cellShader]()
                                             { renderFrame(window, cellShader); });
            }
            else
            {
                path = searchAlgAStar.findPath(columns, startPosX, startPosY, endPosX, endPosY, foodPosX, foodPosY, [window, cellShader]()
                                               { renderFrame(window, cellShader); });
            }
            isRunningAlg = false;
        }
        else
        {
            renderFrame(window, cellShader);
        }
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
bool holdButtonDownLeft = false;
bool holdButtonDownRight = false;
void renderFrame(GLFWwindow *window, Shader cellShader)
{
    // input
    // -----
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // render
    // ------
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    process_mouse_hold(window);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            auto currentCellIsInVisited = std::find_if(visitedNodes.begin(),
                                                       visitedNodes.end(),
                                                       [&](const pair<int, int> &p)
                                                       { return p.first == j && p.second == i; });

            auto currentCellIsInFoundPath = std::find_if(path.begin(),
                                                         path.end(),
                                                         [&](const pair<int, int> &p)
                                                         { return p.first == j && p.second == i; });

            bool isVisitedCell = currentCellIsInVisited != visitedNodes.end();
            bool isPathCell = currentCellIsInFoundPath != path.end();
            bool isStartCell = j == startPosX && i == startPosY;
            bool isEndCell = j == endPosX && i == endPosY;
            bool isFoodCell = j == foodPosX && i == foodPosY;
            bool isWallCell = walls.count(make_pair(j, i));
            cellShader.use();

            if (isStartCell)
            {
                cellShader.setVec4("cellColor", 0.0f, 0.6f, 0.84f, 1.0f);
            }
            else if (isEndCell)
            {
                cellShader.setVec4("cellColor", 0.8f, 0.6f, 0.84f, 1.0f);
            }

            else if (isFoodCell)
            {
                cellShader.setVec4("cellColor", 0.0f, 1.0f, 0.0f, 1.0f);
            }
            else if (isVisitedCell)
            {
                if (isPathCell)
                {
                    cellShader.setVec4("cellColor", 0.0f, 1.0f, 0.0f, 1.0f);
                }
                else
                {
                    cellShader.setVec4("cellColor", 0.8f, 0.4f, 0.0f, 1.0f);
                }
            }
            else if (isWallCell)
            {
                cellShader.setVec4("cellColor", 0.4f, 0.4f, 0.4f, 1.0f);
            }
            else
            {
                cellShader.setVec4("cellColor", 0.0f, 0.0f, 0.0f, 1.0f);
            }

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)((i * columns + j) * 6 * sizeof(GLuint)));
        }
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
}

cellType cellType = other;

void process_mouse_hold(GLFWwindow *window)
{
    if (holdButtonDownLeft || holdButtonDownRight)
    {

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int column = (((xpos / SCR_WIDTH) * 2)) / (cellWidth);
        int row = rows - (((ypos / SCR_HEIGHT) * 2)) / (cellWidth);

        bool isStartCell = column == startPosX && row == startPosY;
        bool isEndCell = column == endPosX && row == endPosY;
        bool isFoodCell = column == foodPosX && row == foodPosY;
        bool isWallCell = walls.count(make_pair(column, row));

        if (holdButtonDownLeft && cellType == end_cell)
        {
            if (endPosX != column && endPosY != rows && !isWallCell && !isStartCell && !isFoodCell)
            {

                endPosX = column;
                endPosY = row;
            }
        }
        else if (holdButtonDownLeft && cellType == start_cell)
        {
            if (startPosX != column && startPosY != rows && !isWallCell && !isEndCell && !isFoodCell)
            {

                startPosX = column;
                startPosY = row;
            }
        }
        else if (holdButtonDownLeft && cellType == food_cell)
        {
            if (startPosX != column && startPosY != rows && !isWallCell && !isEndCell)
            {

                foodPosX = column;
                foodPosY = row;
            }
        }
        else if (holdButtonDownLeft && cellType == other && !walls.count(make_pair(column, row)) && !isStartCell && !isEndCell)
        {
            for (int i = -paintWidth; i <= paintWidth; i++)
            {
                for (int j = -paintWidth; j <= paintWidth; j++)
                {
                    if (!walls.count(make_pair(column + i, row + j)))
                    {

                        walls.insert(make_pair(column + i, row + j));
                    }
                }
            }
        }
        else if (holdButtonDownRight && walls.count(make_pair(column, row)))
        {
            walls.erase(make_pair(column, row));
        }
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int column = (((xpos / SCR_WIDTH) * 2)) / (cellWidth);
    int row = rows - (((ypos / SCR_HEIGHT) * 2)) / (cellWidth);

    bool isStartAlgCell = column == startPosX && row == startPosY;
    bool isEndCell = column == endPosX && row == endPosY;
    bool isFoodCell = column == foodPosX && row == foodPosY;

    if (GLFW_PRESS == action)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {

            if (isEndCell)
            {
                cellType = end_cell;
            }
            else if (isFoodCell)
            {
                cellType = food_cell;
            }
            else if (isStartAlgCell)
            {
                cellType = start_cell;
            }
            holdButtonDownLeft = true;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (isStartAlgCell && !isRunningAlg)
            {
                saveCells();
                path.clear();
                visitedNodes.clear();
                startAlg = true;
            }
            else
            {
                holdButtonDownRight = true;
            }
        }
    }
    else if (GLFW_RELEASE == action)
    {

        holdButtonDownLeft = false;
        holdButtonDownRight = false;
        cellType = other;
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_R && !isRunningAlg)
    {
        path.clear();
        visitedNodes.clear();
        startAlg = false;
    }
    if (key == GLFW_KEY_B)
    {

        currentSearchAlg = bfs;
    }
    else if (key == GLFW_KEY_D)
    {
        currentSearchAlg = dfs;
    }
    else if (key == GLFW_KEY_A)
    {
        currentSearchAlg = a_star;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    cout << yoffset;
    if (yoffset == 1 && yoffset + paintWidth <= 2)
    {
        paintWidth += 1;
    }
    else if (yoffset == -1 && yoffset + paintWidth >= 0)
    {
        paintWidth -= 1;
    }
}

void loadCells()
{
    ifstream inFile;
    inFile.open("walls.txt");
    if (!inFile)
    {
        cerr << "Unable to open file datafile.txt";
    }
    else
    {
        inFile >> startPosX;
        inFile >> startPosY;
        inFile >> endPosX;
        inFile >> endPosY;
        inFile >> foodPosX;
        inFile >> foodPosY;

        int x;
        while (inFile >> x)
        {
            int y;
            inFile >> y;
            walls.insert(make_pair(x, y));
        }
    }
}

void saveCells()
{

    std::ofstream fout("walls.txt");
    fout << startPosX << " " << startPosY << '\n';
    fout << endPosX << " " << endPosY << '\n';
    fout << foodPosX << " " << foodPosY << '\n';
    for (auto const &x : walls)
        fout << x.first << " " << x.second << '\n';
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}