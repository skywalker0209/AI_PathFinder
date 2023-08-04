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
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "PathFinding");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    NodeMap nodeMap;
    int cellSize = 50;
    std::vector<std::string> asciiMap; // 0 wall 1 path
    asciiMap.push_back("000000000000");
    asciiMap.push_back("010111011100");
    asciiMap.push_back("010101110110");
    asciiMap.push_back("010100000000");
    asciiMap.push_back("010111111110");
    asciiMap.push_back("010000001000");
    asciiMap.push_back("011111111110");
    asciiMap.push_back("000000000000");
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
        }

        if (IsMouseButtonPressed(1))
        {
            Vector2 mousePos = GetMousePosition();
            Node* target = nodeMap.GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
            end = target == nullptr ? end : target;
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