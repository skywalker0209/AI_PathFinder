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
    Color cellColor{255,0,0,255};
    Color lineColor{255,255,255,50};

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
                ((x + 0.5f) * cellSize,
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
        #pragma region BubbleSort
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
        #pragma endregion

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
        openList.erase(openList.begin());
        //  Add currentNode to closedList
        closeList.push_back(currentNode);
        for (auto c : currentNode->connections)
        {
            if (std::find(closeList.begin(), closeList.end(), c.target) == closeList.end()) continue;
            float gScore = currentNode->gScore + c.cost;
            // Have not yet visited the node.
            // So calculate the Score and update its parent.
            // Also add it to the openList for processing.
            if (std::find(openList.begin(), openList.end(), c.target) == openList.end()) // If c.target not in openList
            {
                c.target->gScore = gScore;
                c.target->previous = currentNode;
                openList.push_back(c.target);
                // Node is already in the openList with a valid Score.
                // So compare the calculated Score with the existing
                // to find the shorter path.
            }
            else if (gScore < c.target->gScore)
            {
                c.target->gScore = gScore;
                c.target->previous = currentNode;
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

Node* NodeMap::GetClosestNode(glm::vec2 worldPos)
{
    int i = static_cast<int>(worldPos.x / cellSize);
    if (i < 0 || i >= width) return nullptr;

    int j = static_cast<int>(worldPos.y / cellSize);
    if (j < 0 || j >= height) return nullptr;

    return GetNode(i, j);
}
void NodeMap::DrawPath(const std::vector<Node*>& path, Color& color)
{
    if (path.size() < 2)
        return;
    for (size_t i = 0; i < path.size() - 1; i++)
    {
        Node* nodeA = path[i];
        Node* nodeB = path[i + 1];

        DrawLine(
            (int)nodeA->position.x,
            (int)nodeA->position.y,
            (int)nodeB->position.x,
            (int)nodeB->position.y,
            color
        );
    }
}

