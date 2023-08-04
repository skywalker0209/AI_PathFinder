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

public:
	void Init(std::vector<std::string> asciiMap, int cellSize);
	Node* GetNode(int x, int y)
	{
		return nodes[x + width * y];
	}
	void Draw();
	static std::vector<Node*> AStarSearch(Node* startNode, Node* endNode);
	static void DrawPath(const std::vector<Node*>& path, Color& color);
	Node* GetClosestNode(glm::vec2 worldPos);

private:
	static void BubbleSort(std::vector<Node*>& list);
	static Node* FindNodeInList(const std::vector<Node*>& list, Node* target); // Function declaration for FindNodeInList
};
