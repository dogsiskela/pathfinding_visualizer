
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
class Grid
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

public:
    int columns;
    int rows;
    float startingX = -1.0f;
    float startingY = -1.0f;
    float cellWidth;
    float spaceBetween = 0.005f;
    void init();
    void dispose();

    Grid(float rows, float columns)
    {
        this->rows = rows;
        this->columns = columns;
        this->cellWidth = 2 / rows;
    }
};

void Grid::dispose()
{
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
}

void Grid::init()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            // top right
            vertices.push_back(startingX + (j * cellWidth) + cellWidth);
            vertices.push_back(startingY + cellWidth + (i * cellWidth));
            vertices.push_back(0.0f);
            // bottom right
            vertices.push_back(startingX + (j * cellWidth) + cellWidth);
            vertices.push_back(startingY + (i * cellWidth) + spaceBetween);
            vertices.push_back(0.0f);
            // bottom left
            vertices.push_back(startingX + (j * cellWidth) + spaceBetween);
            vertices.push_back(startingY + (i * cellWidth) + spaceBetween);
            vertices.push_back(0.0f);
            // top right
            vertices.push_back(startingX + (j * cellWidth) + spaceBetween);
            vertices.push_back(startingY + cellWidth + (i * cellWidth));
            vertices.push_back(0.0f);
        }
    }

    for (int i = 0; i <= (columns * rows); i++)
    {
        indices.push_back(0 + (4 * i));
        indices.push_back(1 + (4 * i));
        indices.push_back(3 + (4 * i));
        indices.push_back(1 + (4 * i));
        indices.push_back(2 + (4 * i));
        indices.push_back(3 + (4 * i));
    }
    cout << "INITAILIZED";
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
}