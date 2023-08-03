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

std::vector<Node*> NodeMap::DijkstraSearch(Node* startNode, Node* endNode)
{
    // Validate the input

    if (startNode == nullptr || endNode == nullptr)
    {
        std::cout << "Error Invalid Start or End Node \n" << std::endl;
        return std::vector<Node*>();
    }
    else if (startNode == endNode)
    {
        return std::vector<Node*>();
    }
    // Initialise the starting node
    startNode->gScore == 0;
    startNode->previous == nullptr;

    // Create our temporary lists for storing nodes we’re visiting/visited
    std::vector<Node*> openList;
    std::vector<Node*> closeList;
    openList.push_back(startNode);
    while (!openList.empty())
    {
        //bubble sort to get the smallest gScore to the index 0
        for (size_t i = 0; i < openList.size() - 1; i++)
        {
            bool swapped = false;
            for (size_t j = 0; j < openList.size() - i - 1; j++)
            {
                if (openList[j]->gScore > openList[j + 1]->gScore)
                {
                    //Swapped
                    Node* temp = openList[j];
                    openList[j] = openList[j + 1];
                    openList[j + 1] = temp;
                    swapped = true;
                }
            }
            if (swapped == false)
                break;
        }
        Node* currentNode = openList[0];
        // If we visit the endNode, then we can exit early.
        if (currentNode == endNode)
        {
            break;
        }
        // Sorting the openList above guarantees the shortest path is found,
        // given no negative costs (a prerequisite of the algorithm).
        // This is an optional optimisation that improves performance,
        // but doesn’t always guarantee the shortest path.
        // Remove currentNode from OpenList
        auto it = std::find(openList.begin(), openList.end(), currentNode);
        openList.erase(it);
        //  Add currentNode to closedList
        closeList.push_back(currentNode);
        for (const Edge& edge : currentNode->connections)
        {
            Node* neighbor = edge.target;
            if (std::find(closeList.begin(), closeList.end(), neighbor) == closeList.end())
            {
                float gScore = currentNode->gScore + edge.cost;
                // Have not yet visited the node.
                // So calculate the Score and update its parent.
                // Also add it to the openList for processing.
                if (std::find(openList.begin(), openList.end(), neighbor) == openList.end()) // If c.target not in openList
                {
                    neighbor->gScore = gScore;
                    neighbor->previous = currentNode;
                    openList.push_back(neighbor);
                    // Node is already in the openList with a valid Score.
                    // So compare the calculated Score with the existing
                    // to find the shorter path.
                }
                else if ((gScore < neighbor->gScore))
                {
                    neighbor->gScore = gScore;
                    neighbor->previous = currentNode;
                }
            }
        }
    }

    // Create Path in reverse from endNode to startNode
    std::vector<Node*> pathList;
    Node* currentNode = endNode;
    while (currentNode != nullptr)
    {
        pathList.push_back(currentNode);
        currentNode = currentNode->previous;
    }
    std::reverse(pathList.begin(), pathList.end());
    // Return the path for navigation between startNode/endNode
    return pathList;
}


