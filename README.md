In this lab series we’ll look at creating an implementation of Djikstra’s Shortest Path algorithm. There are three labs:
1/ Creating a Node Graph
Create a Node class (and accompanying Edge class) in a new Pathfinding.ccp/.h file like this:
#include <glm/glm.hpp>
#include <vector>
namespace AIForGames
{
    struct Node;
    struct Edge {
        Node* target;
        float cost;
    };
    struct Node {
        glm::vec2 position;
        std::vector<Edge> connections;
    };
}
Each Node contains its position in space and a dynamic array of Edges. Each Edge contains a pointer to the other Node it connects to, and the cost of getting there. 
Making an ASCII art map
One of the easiest ways to set up a relatively complicated node map is through an ASCII art array of strings describing a regular grid of nodes.
To do this we create a vector of strings (in main.cpp if you created your own project from scratch, or in AIE_Starter.cpp if you're using the AIE_Starter project), with each string representing a line of the map. We can use a code such as 0 = solid wall, 1 =  navigable space, and set up anything from a simple test shape to a complex maze for pathfinding to take place in.
    std::vector<std::string> asciiMap;
    asciiMap.push_back("000000000000");
    asciiMap.push_back("010111011100");
    asciiMap.push_back("010101110110");
    asciiMap.push_back("010100000000");
    asciiMap.push_back("010111111110");
    asciiMap.push_back("010000001000");
    asciiMap.push_back("011111111110");
    asciiMap.push_back("000000000000");
We’re assuming here that all the strings are the same length, and that we can use this to create a grid of Nodes.
To do this, we create a NodeMap class with a width, height and cell size, ie the spacing in pixels between consecutive squares in the grid. We’ll give it a function to initialize it’s data from the ASCII map declared above.
class NodeMap
{
       int m_width, m_height;
       float m_cellSize;
       Node** m_nodes;
public:
       void Initialise(std::vector<std::string> asciiMap, int cellSize);
};
The Node** variable nodes is essentially a dynamically allocated one dimensional array of Node pointers.
In the Initialise function we will allocate this array to match the width and height of the map (determined by the vector of strings passed in) and fill it with either newly allocated Nodes or null pointers for each square on the grid.
void NodeMap::Initialise(std::vector<std::string> asciiMap, int cellSize)
    {
        m_cellSize = cellSize;
        const char emptySquare = '0';
        // assume all strings are the same length, so we'll size the map according
        // to the number of strings and the length of the first one
        m_height = asciiMap.size();
        m_width = asciiMap[0].size();
        m_nodes = new Node * [m_width * m_height];
        // loop over the strings, creating Node entries as we go
        for (int y = 0; y < m_height; y++)
        {
            std::string& line = asciiMap[y];
            // report to the user that you have a mis-matched string length
            if (line.size() != m_width)
                std::cout << "Mismatched line #" << y << " in ASCII map (" << line.size() << " instead of " << m_width << ")" << std::endl;
            for (int x = 0; x < m_width; x++)
            {
                // get the x-th character, or return an empty node if the string         
                // isn't long enough
                char tile = x < line.size() ? line[x] : emptySquare;
                // create a node for anything but a '.' character
                m_nodes[x + m_width * y] = tile == emptySquare ? nullptr
                    : new Node(((float)x + 0.5f) * m_cellSize, ((float)y + 0.5f) * m_cellSize); 
            }
        }
 (Note that for convenience we’ve added a constructor to Node that takes float values for its position.)
We’re using the length of the first string to calculate the width of the rectangular node map. We put in a debug warning if any of the strings are a different length but fail gracefully if they don’t match. Extra characters will never be read. Any missing characters on the end are assumed to be not navigable, so we won’t create a node for them.
We now need to do a second pass across the grid of nodes and join them up with Edges. We can look at each Node and check its four neighbours, and create a pair of mutual Edges between them if we find an edge. For this exercise, we’ll assume that all edges are of equal cost of 1 to navigate.
Don’t forget to add a destructor that deletes the array of Node pointers we’ve allocated here.
To make this easier we’ll create a couple of helper functions:
A GetNode in NodeMap that return the Node* for a given pair of coordinates:
Node* GetNode(int x, int y) { return m_nodes[x + m_width * y]; }
And a ConnectTo function in Node to add an Edge connecting to a given Node with a given cost
void Node::ConnectTo(Node* other, float cost)
{
       connections.push_back(Edge(other, cost));
}
You’ll need to create a suitable constructor for Edge, and provide an explicit default constructor to replace the implicit one that gets created if you don’t specify any constructors.
    struct Edge
    {
        Edge() { target = nullptr; cost = 0; }
        Edge(Node* _target, float _cost) : target(_target), cost(_cost) {}
You can now add code to your NodeMap::Initialise function to create Edges between all adjacent nodes. We only need to check each node to the west (x-1,y) and south (x, y-1) since the other two connections will be set up by the node in the next column or row.
    // now loop over the nodes, creating connections between each node and its      
    // neighbour to the West and South on the grid. this will link up all nodes
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            Node* node = GetNode(x, y);
            if (node)
            {
                // see if there's a node to our west, checking for array overruns      
                // first if we're on the west-most edge
                Node* nodeWest = x == 0 ? nullptr : GetNode(x - 1, y);
                if (nodeWest)
                {
                    node->ConnectTo(nodeWest, 1); // TODO weights
                    nodeWest->ConnectTo(node, 1);
                }
                // see if there's a node south of us, checking for array index
                // overruns again
                Node* nodeSouth = y == 0 ? nullptr : GetNode(x, y - 1);
                if (nodeSouth)
                {
                    node->ConnectTo(nodeSouth, 1);
                    nodeSouth->ConnectTo(node, 1);
                }
            }
        }
    }

 
2/ Calculating a path
Using the NodeMap we created in the previous lab, we now want to write an algorithm that calculates a series of Node Pointers that take you from a start Node to an end Node.
We want a function of the form:
std::vector<Node*> DijkstrasSearch(Node* startNode, Node* endNode);
This can be a global function within a namespace, or a static member of NodeMap. It doesn’t need access to the NodeMap structure since each Node knows about all its neighbours, and we can use that to traverse the graph of nodes.
The pseudocode for Djikstra’s algorithm is shown on the next page.
We need an open and closed list, which we use to store the nodes we want to investigate still and the nodes we have investigated. All we need to do to these lists are add to them and check if a given Node is already in them. These can be std::vectors
We can add to them dynamically using push_back().
We can check if a Node* is already in a std::vector<Node*> using std::find in the <algorithm> library. It takes a start iterator and end iterator and returns either an iterator that matches the given test value, or the end iterator if it reaches the end without finding a match.
Here’s an example usage (just for illustration purposes!)
       std::vector<Node*> openList;
       bool found;
       Node* test;
       if (std::find(openList.begin(), openList.end(), test) == openList.end())
              found = false;
       else
              found = true;
==================================================================
Procedure dijkstrasSearch(startNode, endNode)
       // Validate the input
       // Initialise the starting node
       Set startNode.gScore to 0
       Set startNode.previous to null
       // Create our temporary lists for storing nodes we’re visiting/visited
       Let openList be a List of Nodes
       Let closedList be a List of Node
       Add startNode to openList
       While openList is not empty - Sort openList by Node.gScore
       Let currentNode = first item in openList
              // If we visit the endNode, then we can exit early.
              // Sorting the openList above guarantees the shortest path is found,
              // given no negative costs (a prerequisite of the algorithm).
              // This is an optional optimisation that improves performance,
              // but doesn’t always guarantee the shortest path.
              If currentNode is endNode - Exit While Loop
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
