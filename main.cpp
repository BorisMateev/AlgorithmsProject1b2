// Project 1b: Solving graph coloring using exhaustive search
//

#include <iostream>
#include <list>
#include <fstream>
#include <queue>
#include <sstream>
#include <math.h>
#include <ctime>

#include "headers/d_except.h"
#include "headers/graph.h"
#include "headers/knapsack.h"

void greedyColoring(graph &g);
void printColorSolution(graph g);
bool isValidColor(graph g, int n, int c);

unsigned long long convertBase(unsigned long long n, int base);
int getNumConflicts(graph g);


namespace patch
// Patch to fix to_string so it works in visual studio
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
		stm << n ;
        return stm.str() ;
    }
}

using namespace std;


int main()
{
    ifstream fin;
    int numColors;

    // Enter path to input file for testing
    string path = "C:\\Users\\Boris\\Documents\\Visual Studio 2010\\Projects\\caimat-2b\\caimat-2b\\instances";
    string fileName = path + "\\color24-5.input";

    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
		system("pause");
        exit(1);
    }

    try
    {
        cout << "Reading number of colors" << endl;
        fin >> numColors;

        cout << "Reading graph" << endl;
        graph g(fin);

        cout << "Num colors: " << numColors << endl;
        cout << g;
	
	// Run greedy algorithm
       greedyColoring(g);
	   
       printColorSolution(g);

	   cout<<"Number of conflicts detected: "<<getNumConflicts(g)<<endl<<endl;
    }
    catch (indexRangeError &ex)
    {
		system("pause");
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError &ex)
    {
		system("pause");
        cout << ex.what() << endl; exit(1);
    }

	system("pause");
	return 0;
}

void greedyColoring(graph &g)
// Greedy algorithm that finds minimum number of conflicts for a
// numColors coloring of graph g by iteratively going trough all
// nodes and choosing the lowest color that does not conflict with 
// neighboring nodes.
{
	// Get the total number of nodes
    int nodeCount = g.numNodes();

	// Go through and color all nodes
	for(int currNode = 0; currNode < nodeCount; currNode++)
	{
		bool isColored = false; // Keep track of when to end coloring loop
		int currColor = 1; // Try to color each node with lowest possible color

		// Greedily color node with lowest possible color
		while(isColored == false)
		{
		   	if(isValidColor(g, currNode, currColor))
			// If this color works, use it
			{
				g.setNodeWeight(currNode, currColor);
				isColored = true;
			}

			// Otherwise try the next one
			else
				currColor++;
		}
	}
}
 
bool isValidColor(graph g, int n, int c)
// Check if node n has any neighbors of color c
// Return value of true means no neighbors of color c currently exist
{
	bool status = true;
	int neighborColor;
	int numNodes = g.numNodes();

	// Iterate through all nodes, checking for neighbors
	for (int i = 0; i < numNodes; i++)
    {		
		// Check if nodes have conflicting color, because of 
		// how the graph is constructed we only need to check in one 
		// direction, from smaller to larger node
		if(g.isEdge(i, n))
		{
			neighborColor = g.getNodeWeight(i);
			if(neighborColor == c)
			{
				status = false;
				return status;
			}
		}
	}

	return status;
}
	
void printColorSolution(graph g)
// print color of each node and total number of colors used
{
	int numNodes = g.numNodes();
	int numColors = 0;
	int currColor;
	int biggestColor = 0;

	cout<<"\nSolution for greedy coloring of this graph:\n\n";

	for(int i = 0; i < numNodes; i++)
	{
		currColor = g.getNodeWeight(i);
		cout<<"Color of node "<<i<<": "<<currColor<<"\n";
		if (currColor > biggestColor)
			biggestColor = currColor;
	}

	cout<<"\nA total of " << biggestColor<< " colors were used.\n\n";
}

int getNumConflicts(graph g)
// Get the total number of conflicts on our graph.
{
    int numConflicts = 0;

	// Iterate through all nodes
    for (int i = 0; i < g.numNodes(); i++)
    {
		// Checking for edges, because of way graph is constructed
		// we only need to chec in one direction, from smaller to larger node
        for (int j = 0; j < g.numNodes(); j++)
        {
            // If neighboring nodes have same color, add a conflict
            if (g.isEdge(i, j))
            {
                if (g.getNode(i).getWeight() == g.getNode(j).getWeight())
					numConflicts++;
			}
		}
    }
    return numConflicts;
}
