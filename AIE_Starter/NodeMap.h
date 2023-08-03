#pragma once

#include "PathFinding.h"
#include <string>
#include <iostream>
#include <raygui.h>

using namespace AIForGames;

class NodeMap
{
	int width, height;
	float cellSize;
	Node** nodes;

	void ConnectWestandSouth(int x, int y);
	static bool NodeInList(std::vector<Node*> List);

public:
	void Init(std::vector<std::string> asciiMap, int cellSize);
	Node* GetNode(int x, int y)
	{
		return nodes[x + width * y];
	}

	void Draw();
	static std::vector<Node*> AStarSearch(Node* startNode, Node* endNode);
	Node* NodeMap::GetClosestNode(glm::vec2 worldPos);
};

