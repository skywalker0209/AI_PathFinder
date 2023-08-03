#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <queue>

namespace AIForGames
{
    struct Node;

    struct Edge
    {
        Node* target;
        float cost;

        Edge() { target = nullptr; cost = 0; }

        Edge(Node* _target, float _cost) : target(_target), cost(_cost) {}
    };

    struct Node
    {
        glm::vec2 position;
        std::vector<Edge> connections;
        Node* previous; // For storing the previous node in the path
        float gScore;   // Cost from start node to this node
        float hScore;   // Estimated cost from this node to the goal node
        float fScore;   // fScore = gScore + hScore

        Node(float x, float y) : position(x, y), previous(nullptr), gScore(0), hScore(0), fScore(0) {}

        void ConnectTo(Node* other, float cost)
        {
            connections.push_back(Edge(other, cost));
        }
    };

    // A* Pathfinding function
    std::vector<Node*> AStarSearch(Node* startNode, Node* endNode);
}
