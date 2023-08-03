#include "NodeMap.h"
#include <iostream>
#include <raygui.h>

using namespace AIForGames;


void NodeMap::ConnectWestandSouth(int x, int y)
{
    Node* node = GetNode(x, y);
    if (node != nullptr)
    {
        Node* nodeWest = (x == 0) ? nullptr : GetNode(x - 1, y);
        if (nodeWest != nullptr)
        {
            node->ConnectTo(nodeWest, 1);
            nodeWest->ConnectTo(node, 1);
        }

        Node* nodeSouth = (y == 0) ? nullptr : GetNode(x, y - 1);
        if (nodeSouth != nullptr)
        {
            node->ConnectTo(nodeSouth, 1);
            nodeSouth->ConnectTo(node, 1);
        }
    }
}


void NodeMap::Init(std::vector<std::string> asciiMap, int cellSize)
{
    this -> cellSize = cellSize;
    const char emptySquare = '0';

    height = asciiMap.size();
    width = asciiMap[0].size();
    
    nodes = new Node * [width * height];
    
    for (int y = 0; y < height; y++)
    {
        std::string& line = asciiMap[y];
        if (line.size() != width)
        {
            std::cout << "Mismatched Line # " << y
                << "in Ascii Map (" << line.size()
                << "instead of " << width << std::endl;        
        }  

        for (int x = 0; x < width; x++)
        {
            char tile = (x < line.size()) ? line[x] : emptySquare;

            nodes[x + width * y] = tile == emptySquare ? nullptr
            : new Node(((float)x + 0.5f) * cellSize, ((float)y + 0.5f) * cellSize);

        }
        
    }
    for (int y = 0; y< height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            ConnectWestandSouth(x, y);
        }
    }
}

void NodeMap::Draw()
{
    Color cellColor;
    cellColor.a = 255;
    cellColor.r = 0;
    cellColor.b = 255;
    cellColor.g = 0;

    Color lineColor;
    lineColor.a = 255;
    lineColor.r = 0;
    lineColor.b = 0;
    lineColor.g = 0;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Node* node = GetNode(x, y);
            if (node == nullptr)
            {
                DrawRectangle
                (
                    (int)(x * cellSize),
                    (int)(y * cellSize),
                    (int)cellSize - 1,
                    (int)cellSize - 1,
                    cellColor
                );
                continue;
            }
            for (int i = 0; i < node->connections.size(); i++)
            {
                Node* other = node->connections[i].target;
                DrawLine
                (   (x + 0.5f) * cellSize,
                    (y + 0.5f) * cellSize,
                    (int)other->position.x,
                    (int)other->position.y,
                    lineColor
                );
            }
        }
    }
}

bool NodeMap::NodeInList(std::vector<Node*> List)
{
    Node* test;
    if (std::find(List.begin(), List.end(), test) == List.end())
    {
        return false;
    }
    return true;
}

std::vector<Node*> NodeMap::DijkstrasSearch(Node* startNode, Node* endNode)
{
    std::vector<Node*> openList;
    bool found = NodeInList(openList);

    /*Procedure dijkstrasSearch(startNode, endNode)
        // Validate the input
        if startNode is null OR endNode is null
            Raise Error

            if startNode == endNode
                return empty Path

                // Initialise the starting node
                Set startNode.gScore to 0
                Set startNode.previous to null

                // Create our temporary lists for storing nodes we’re visiting/visited
                Let openList be a List of Nodes
                Let closedList be a List of Nodes

                Add startNode to openList

                While openList is not empty
                Sort openList by Node.gScore

                Let currentNode = first item in openList

                // If we visit the endNode, then we can exit early.
                // Sorting the openList above guarantees the shortest path is found,
                // given no negative costs (a prerequisite of the algorithm).
                // This is an optional optimisation that improves performance,
                // but doesn’t always guarantee the shortest path.
                If currentNode is endNode
                Exit While Loop

                Remove currentNode from openList
                Add currentNode to closedList

                For all connections c in currentNode
                If c.target not in closedList
                Let gScore = currentNode.gScore + c.cost

                // Have not yet visited the node.
                // So calculate the Score and update its parent.
                // Also add it to the openList for processing.
                If c.target not in openList
                Set c.target.gScore = gScore
                Set c.target.previous = currentNode
                Add c.target to openList

                // Node is already in the openList with a valid Score.
                // So compare the calculated Score with the existing
                // to find the shorter path.
                Else if (gScore < c.target.gScore)
                Set c.target.gScore = gScore
                Set c.target.previous = currentNode

                // Create Path in reverse from endNode to startNode
                Let Path be a list of Nodes
                Let currentNode = endNode

                While currentNode is not null
                Add currentNode to beginning of Path
                Set currentNode = currentNode.previous

                // Return the path for navigation between startNode/endNode
                Return Path
                */

    return std::vector<Node*>();
}
