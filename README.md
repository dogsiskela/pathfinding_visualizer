<h3 align="center"> Pathfinding Visualizer With OPENGL</h3>

---

<p align="center"> Visualization of popular pathfinding algorithms
    <br> 
</p>





## About <a name = "about"></a>
This project brings the background processing of pathfinding algorithms and renders a visual representation in a 2D matrix. This interface allows for map state modification by changing the start and end position as well as adding and removing walls and cells that the path has to go through. 

## Getting Started <a name = "getting_started"></a>
- Open the project as a CMake build from the CMakeLists.txt
- An .exe file is located in the build/pathfinder folder

## Usage <a name="usage"></a>
The window is consisted of NxN cells grid. There are 7 types of cells:
![1](https://user-images.githubusercontent.com/45862325/153929977-8e0f60d5-772f-4cb1-94e5-ccde02486a7e.png)

- Empty cell (Dark)
- Wall cell (Light gray)
- Starting cell (Blue)
- End cell (Pink)
- Cell that the path must go through (Only implemented in BFS) (Green)
- Visited cell by the algorithm (Orange)
- Found path (Green)

![image](https://user-images.githubusercontent.com/45862325/153930158-243580a9-7947-46fd-93aa-d8a519b0ffe9.png)


## Modifying the grid <a name="inputs"></a>
- To start the algorithm, press right click on the starting cell.
- To add new wall to the grid, hold and drag the cursor through the empty cells. The paint size can be 1px to 3px. Modify the wall painting size by using the mouse wheel.
- To change the position of the start and end cells, click and drag the cell to the new position.
- To change the current algorithm press the corresponding key:
    - B - Breadth First Search
    - D - Depth First Search
    - A - A* 
- Press R to clean the cells from the last run algorithm.

## Demo <a name="demo"></a>
https://user-images.githubusercontent.com/45862325/153942576-cc6327a2-c84e-4fe1-9404-f0e58569ff59.mp4

## Project goals
- [x] Generate cells grid
- [x] BFS Algorithm
- [x] DFS Algorithm
- [x] A* Algorithm 
- [x] Map state modification
- [x] Import and export map state
- [ ] User Interface
- [ ] Generate random maps

## Libraries used <a name = "libraries"></a>
- GLAD
- GLFW
- GLM
