/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "Pathfinding.h"
#include <string>
#include "NodeMap.h"

using namespace AIForGames;


int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 960;
    int screenHeight = 960;

    InitWindow(screenWidth, screenHeight, "PathFinding");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    NodeMap nodeMap;
    int cellSize = 40;
    std::vector<std::string> asciiMap; // 0 grass 1 path
    asciiMap.push_back("000000000000000000000000");//0
    asciiMap.push_back("010111011100000000000000");//1
    asciiMap.push_back("010101110110000000000000");//2
    asciiMap.push_back("010100100000000000000000");//3
    asciiMap.push_back("010111111110000000000000");//4
    asciiMap.push_back("010000001000000000000000");//5
    asciiMap.push_back("011111111110000000000000");//6
    asciiMap.push_back("000000000000000000000000");//7
    asciiMap.push_back("000000000000000000000000");//8
    asciiMap.push_back("000000000000000000000000");//9
    asciiMap.push_back("000000000000000000000000");//10
    asciiMap.push_back("000000000000000000000000");//11
    asciiMap.push_back("000000000000000000000000");//12
    asciiMap.push_back("000000000000000000000000");//13
    asciiMap.push_back("000000000000000000000000");//14
    asciiMap.push_back("000000000000000000000000");//15
    asciiMap.push_back("000000000000000000000000");//16
    asciiMap.push_back("000000000000000000000000");//17
    asciiMap.push_back("000000000000000000000000");//18
    asciiMap.push_back("000000000000000000000000");//10
    asciiMap.push_back("000000000000000000000000");//20
    asciiMap.push_back("000000000000000000000000");//21
    asciiMap.push_back("000000000000000000000000");//22
    nodeMap.Init(asciiMap, cellSize);

    Node* start = nodeMap.GetNode(1, 1);
    Node* end = nodeMap.GetNode(10, 2);

    std::vector<Node*> path = NodeMap::AStarSearch(start, end);
    Color lineColor = { 255, 255, 255, 255 };;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        if (IsMouseButtonPressed(0))
        {
            Vector2 mousePos = GetMousePosition();
            Node* target = nodeMap.GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
            start = target == nullptr ? start : target;
            path = NodeMap::AStarSearch(start, end);
        }

        if (IsMouseButtonPressed(1))
        {
            Vector2 mousePos = GetMousePosition();
            Node* target = nodeMap.GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
            end = target == nullptr ? end : target;
            path = NodeMap::AStarSearch(start, end);
        }

        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        nodeMap.Draw();

        NodeMap::DrawPath(path, lineColor);



        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}