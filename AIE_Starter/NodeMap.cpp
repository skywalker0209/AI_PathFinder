#include "NodeMap.h"
#include <iostream>
#include <raylib.h>

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
    this->cellSize = cellSize;
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
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            ConnectWestandSouth(x, y);
        }
    }
}

void NodeMap::Draw()
{
    Color cellColor{ 255,0,0,255 };
    Color lineColor{ 255,255,255,50 };

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

Node* NodeMap::FindNodeInList(const std::vector<Node*>& list, Node* target)
{
    auto it = std::find(list.begin(), list.end(), target);
    return it != list.end() ? *it : nullptr;
}

void NodeMap::BubbleSort(std::vector<Node*>& list)
{
    for (size_t i = 0; i < list.size() - 1; i++)
    {
        bool swapped = false;
        for (size_t j = 0; j < list.size() - i - 1; j++)
        {
            if (list[j]->fScore > list[j + 1]->fScore)
            {
                // Swap elements
                Node* temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped)
            break; // If no swap occurred in this pass, the list is already sorted.
    }
}


std::vector<Node*> NodeMap::AStarSearch(Node* startNode, Node* endNode)
{
    if (startNode == nullptr || endNode == nullptr)
    {
        throw std::runtime_error("Invalid Start or End Node");
    }
    else if (startNode == endNode)
    {
        return std::vector<Node*>();
    }

    // Initialise the starting node
    startNode->gScore = 0;
    startNode->parent = nullptr;

    // Create our temporary lists for storing nodes we’re visiting/visited
    std::vector<Node*> openList;
    std::vector<Node*> closeList;
    openList.push_back(startNode);
   
    //While openList is not empty, sort openList by node.fScore

    while (!openList.empty())
    {
        BubbleSort(openList);

        Node* currentNode = openList.front();
        // If we visit the endNode, then we can exit early.
        if (currentNode == endNode)
        {
            break;
        }
        // Remove currentNode from OpenList
        openList.erase(openList.begin());
        //  Add currentNode to closedList
        closeList.push_back(currentNode);

        for (auto &c : currentNode->connections)
        {
            if (FindNodeInList(closeList,c.target))
            {
                continue;
            }
            float gScore = currentNode->gScore + c.cost;
            float hScore = glm::distance(c.target->position, endNode->position);
            float fScore = gScore + hScore;

            if (FindNodeInList(openList, c.target))// If c.target not in openList
            {
                c.target->gScore = gScore;
                c.target->fScore = fScore;
                c.target->parent = currentNode;
                openList.push_back(c.target);
               
            }
            else if (fScore < c.target->fScore)
            {
                c.target->gScore = gScore;
                c.target->fScore = fScore;
                c.target->parent = currentNode;
            }
        }
    }

    // Create Path in reverse from endNode to startNode
    std::vector<Node*> pathList;
    Node* currentNode = endNode;
    while (currentNode != nullptr)
    {
        pathList.push_back(currentNode);
        currentNode = currentNode->parent;
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