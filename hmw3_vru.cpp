// Homework3_VRU
// Calculate minimum spanning tree in a graph from a given data set 
// I have chosen to use Kursal's algorithm

#include <iostream>
#include <cstdlib>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/////////////////    Class: Edge                     ///////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// CLASS DEFINITION: Edge: from, to and cost
//
class Edge {
  public:
    int i;
    int j;
    int cost;
};


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/////////////////    Class: Graph                    ///////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// CLASS DEFINITION: graph contains list of edges with their associated cost
class Graph {
  public:
    Graph(int num_elems, float density, float min_dist, float max_dist); //constructor
    Graph(string filename);
    void print(); //print graph
    ~Graph(); //destructor
    vector<Edge> getEdges() { return edges;};
    int size() { return num_elems;};


  private:
    int num_elems=0; // number of vertices
    vector<Edge> edges;

};
////////////////////////////////////////////////////////////////////////////
// FUNCTION: Constructor
//fill in graph from file in the style of homework 
Graph::Graph(string filename)
{
  ifstream file (filename);
  istream_iterator<int> start(file), end;

  this->num_elems = *start++;
  vector<int> filenrs(start, end);

  int index = 0;
  Edge *e = new Edge;
  for (auto nr : filenrs)
  {
    switch (index)
    {
      case 0: 
        e->i = nr;
        
      case 1: 
        e->j = nr;
        break;
      case 2: 
        e->cost =nr;
        edges.push_back(*e);
        break;
    }
    index = (index+1)%3;
  }
  delete e;

}


////////////////////////////////////////////////////////////////////////////
// FUNCTION:Destructor
Graph::~Graph()
{
  edges.clear();
}

////////////////////////////////////////////////////////////////////////////
// FUNCTION: print matrix
void Graph::print()
{
  for (auto it : edges)
  {
    cout << it.i << ":" << it.j << ":" << it.cost << endl;
  }
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/////////////////    Class: Kruskal                  ///////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// CLASS DEFINITION: Kruskal contains functions to calcylate MST from a given graph
//
class Kruskal {
  public:
    Kruskal(Graph * g);
    ~Kruskal();
    int getCost() { return accum_cost;};
    vector<Edge> getTree() { return mst_edges;};
    bool checkDisjointed();
    void setRoot(int i, int j);
  private:
    int accum_cost;
    vector<Edge> mst_edges;
    int getRoot(int i);
    int * root;
    int graph_size;
};

////////////////////////////////////////////////////////////////////////////
// FUNCTION:sortEdgesByCost function parameter for sort
bool sortEdgesByCost(Edge a, Edge b) {return (a.cost < b.cost);}

////////////////////////////////////////////////////////////////////////////
// FUNCTION:Constructor
Kruskal::Kruskal(Graph * g) {
  graph_size = g->size();

  //get all edges from graph and sort them
  vector<Edge> all_edges = g->getEdges();
  sort(all_edges.begin(), all_edges.end(),sortEdgesByCost);

  //get array of roots to check for loops
  root = new int[graph_size];
  for (int i=0; i < graph_size; i++)
  {
    root[i] = i;
  }
  //initialize cost
  accum_cost = 0;

  //go through sorted edges
  while (all_edges.size() != 0)
  {
    Edge e = all_edges.front();
    //if the two vertices in this edge have a different root, they do not form a loop
    if (getRoot(e.i) != getRoot(e.j))
    {
      mst_edges.push_back(e);
      accum_cost += e.cost;
      // make sure we are always getting the same root for each disjointed tree by adding the new one as its root
      setRoot(e.j, e.i); 
    }
    //remove this edge which has already been reviewed from vector
    all_edges.erase(all_edges.begin());
  }
}

////////////////////////////////////////////////////////////////////////////
// FUNCTION:Destructor
Kruskal::~Kruskal() {
  delete [] root;
}

////////////////////////////////////////////////////////////////////////////
// FUNCTION: look for root in a tree
int Kruskal::getRoot(int i)
{
  while (root[i] != i)
  {
    i = root[i];
  }
  return i;

}
////////////////////////////////////////////////////////////////////////////
// FUNCTION: set new root
void Kruskal::setRoot(int i, int j)
{
  root[getRoot(j)] = getRoot(i);
}


////////////////////////////////////////////////////////////////////////////
// FUNCTION:Check that MST actually includes all vertices by checking their roots
bool Kruskal::checkDisjointed()
{
  int unique_root = getRoot(0);

  for (int i=1; i < graph_size; i++)
  {
    if (getRoot(i) != unique_root)
    {
      cout << "vertex " << i << " is disjointed from 0." << endl;
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/////////////////    Main                            ///////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// FUNCTION: main
//
int main()
{
  // generate a new adjacency matrix (or graph) given input data
  Graph * mygraph = new Graph("data.txt");

  //print the graph 
  //mygraph->print();
 
  Kruskal * k = new Kruskal(mygraph); 


  //check that tree is actually possible
  if (k->checkDisjointed())
  {
    cout << "MST is disjointed" << endl;
  }
  else
  {
    //print cost and edges
    cout << "MST cost is : " << k->getCost() << endl;

    vector<Edge> tree = k->getTree();
    cout << "MST tree is : " <<  endl;
    for (auto edge : tree)
    {
      cout << edge.i << " : " << edge.j << " : " << edge.cost << endl;
    }
  }
  delete mygraph;


  return 0;

}


