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

std::vector<Node*> NodeMap::AStarSearch(Node* startNode, Node* endNode)
{
    if (startNode == nullptr || endNode == nullptr)
    {
        std::cout << "Error: Invalid Start or End Node" << std::endl;
        return std::vector<Node*>();
    }
    else if (startNode == endNode)
    {
        return std::vector<Node*>();
    }

    std::priority_queue<std::pair<float, Node*>, std::vector<std::pair<float, Node*>>, std::greater<std::pair<float, Node*>>> openList;
    std::unordered_map<Node*, Node*> cameFrom;
    std::unordered_map<Node*, float> gScore;
    std::unordered_map<Node*, float> fScore;

    gScore[startNode] = 0.0f;
    fScore[startNode] = glm::distance(startNode->position, endNode->position);

    openList.push({ fScore[startNode], startNode });

    while (!openList.empty())
    {
        Node* current = openList.top().second;
        openList.pop();

        if (current == endNode)
        {
            std::vector<Node*> path;
            while (current != nullptr)
            {
                path.push_back(current);
                current = cameFrom[current];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const Edge& edge : current->connections)
        {
            Node* neighbor = edge.target;
            float tentative_gScore = gScore[current] + edge.cost;

            if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor])
            {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = gScore[neighbor] + glm::distance(neighbor->position, endNode->position);
                openList.push({ fScore[neighbor], neighbor });
            }
        }
    }

    std::cout << "No path found!" << std::endl;
    return std::vector<Node*>();
}

Node* NodeMap::GetNode(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return nodes[x + width * y];
    }
    return nullptr;
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
}
