// Homework2_VRU
// Implement Dijkstra's Algorithm

#include <iostream>
#include <cstdlib>
using namespace std;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/////////////////    Class: Graph                    ///////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// CLASS DEFINITION: graph contains adjacency matrix and generates it randomly given
// number of vertices, density and min and max distances
class Graph {
  public:
    Graph(int num_elems, float density, float min_dist, float max_dist); //constructor
    void print(); //print graph
    ~Graph(); //destructor
    int size() { return num_elems;};
    bool isAdjacent(int x, int y) { return connectivity_matrix[x][y];};
    float getDistance(int x, int y) {return cost_matrix[x][y];};


  private:
    int num_elems=0; // number of vertices
    float prob(); // get a random probability value
    float get_dist(float min, float max); //get random distance for an edge
    bool ** connectivity_matrix; //matrix with connectivity between vertices
    float ** cost_matrix; //matric with length between vertices when connected

};
////////////////////////////////////////////////////////////////////////////
// FUNCTION: Constructor
Graph::Graph(int num_elems, float density, float min_dist, float max_dist)
{

  this->num_elems = num_elems;

  //create graph matrix
  connectivity_matrix = new bool*[num_elems];
  cost_matrix = new float*[num_elems];
  for (int i=0; i < num_elems; i++)
  {
    connectivity_matrix[i] = new bool[num_elems];
    cost_matrix[i] = new float[num_elems];
  }

  //fill in graph matrix according to density
  for (int i=0; i < num_elems; i++)
  {
    for (int j=0; j < i; j++)
    {
      if ( prob()  < density) 
      {
        connectivity_matrix[i][j] = connectivity_matrix[j][i]  = true;
        //only when connected do we assign a cost/distance
        cost_matrix[i][j] = cost_matrix[j][i]  = get_dist(min_dist, max_dist);
      }
      else
      {
        connectivity_matrix[i][j] = connectivity_matrix[j][i]  = false;
      }
    }
    //nodes not connected to themselves
    connectivity_matrix[i][i] = false;
  }
}


////////////////////////////////////////////////////////////////////////////
// FUNCTION:Destructor
Graph::~Graph()
{
  for (int i=0; i < num_elems; i++)
  {
    delete [] connectivity_matrix[i];
    delete [] cost_matrix[i];
  }
  delete [] connectivity_matrix;
  delete [] cost_matrix;

}



////////////////////////////////////////////////////////////////////////////
// FUNCTION: print matrix
void Graph::print()
{
   cout << "\t";
   for (int j=0; j < num_elems; j++)
   {
     cout << j << "\t";
   }
   cout << "\n";
   for (int i= 0; i < num_elems; i++)
   {
     cout << i << "\t";
     for (int j=0; j < num_elems; j++)
     {
       if (connectivity_matrix[i][j] == true)
       {
         cout << cost_matrix[i][j] << "\t";
       }
       else
       {
         cout << "---" << "\t";
       }
     }
     cout << "\n";
   }
}

////////////////////////////////////////////////////////////////////////////
// FUNCTION: get a random number between 0 and 1.0
float Graph::prob()
{
  return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}

////////////////////////////////////////////////////////////////////////////
// FUNCTION: get a random distance float between given min and max
float Graph::get_dist(float min, float max)
{
  return ((static_cast <float> (rand()) ) / (static_cast <float>(RAND_MAX) / (max-min))) + min;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/////////////////    Class: ShortestPath             ///////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// CLASS DEFINITION: graph contains adjacency matrix and everything needed to calculate best path
class ShortestPath {
  public:
    ShortestPath(Graph *g, int from, int to); //get shortest path from/to given nodes
    ~ShortestPath();
    float GetCost() { return shortest_path_cost;};
    bool GetFound() { return found;}

  private:
    Graph * g;
    bool * closed; //closed node set
    bool * open ; //open node set
    float * cost;  //current min distance to a node (valid when in closed set)
    bool found=false;
    float shortest_path_cost;

    void OpenNodesAdd(int closed_node, float closed_dist); //add nodes to open set that are adjacent to a given closed node
    int OpenSetMin();  //find shortest distance node in open set to move it to closed set

};


////////////////////////////////////////////////////////////////////////////
// FUNCTION: Constructor
ShortestPath::ShortestPath(Graph *g, int from, int to)
{
  //initialization of private variables
  this->g = g;

  this->closed = new bool[g->size()];
  this->open = new bool[g->size()];
  this->cost = new float[g->size()];

  found=false;

  for (int i= 0; i < g->size(); i++) 
  {
    this->closed[i] = false;
    this->open[i] = false;
  }
  //start node is in open set with cost 0
  this->open[from] = true;
  this->cost[from] = 0;

  //look for more open nodes
  this->OpenNodesAdd(from, 0);
  int new_node = this->OpenSetMin();
  while ((new_node != to) && (new_node != -1))
  {
    this->open[new_node] = false;
    this->closed[new_node] = true;
    this->OpenNodesAdd(new_node, this->cost[new_node]);
    new_node = this->OpenSetMin();
    //cout << "new node for closed set " << new_node << " with cost " << this->cost[new_node] << "\n";
  }
  if (new_node != -1) 
  {
    found = true;
    shortest_path_cost = this->cost[new_node];
  }

}


////////////////////////////////////////////////////////////////////////////
// FUNCTION:Destructor
ShortestPath::~ShortestPath()
{
  delete [] closed;
  delete [] open;
  delete [] cost;

}


////////////////////////////////////////////////////////////////////////////
// FUNCTION: add (open) nodes that are adjacent to a given closed node 
void ShortestPath::OpenNodesAdd(int closed_node, float closed_dist)
{

  for (int i=0; i < g->size(); i++)
  {
    //check first that i is not already in the closed set
    if (closed[i] == true) continue;
  
    if (g->isAdjacent(i,closed_node) == true)
    {
      //this node is adjacent, add new cost if it is improved
      //check if already in the open matrix
      if (open[i] == true) 
      {
        if (g->getDistance(i,closed_node) + closed_dist < cost[i])
        {
          cost[i] = g->getDistance(i,closed_node) + closed_dist;
        } 
      }
      else
      {
        open[i] = true;
        cost[i] = g->getDistance(i,closed_node) + closed_dist;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////
// FUNCTION: get the current minimum distance node in open set list
int ShortestPath::OpenSetMin()
{
  float min_dist = 0;
  int index_min_dist = -1;
  for (int i=0; i < g->size(); i++)
  {
    if (open[i] && ((cost[i] < min_dist)  || (index_min_dist == -1)))
    {
      min_dist = cost[i];
      index_min_dist = i;
    }
  }
  return index_min_dist;
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
  int num_elems = 50;
  int start_node = 0;
  int end_node = num_elems - 1;
  float density = 0.40;
  
  srand(time(0)); //seed srand

  // generate a new adjacency matrix (or graph) given a number of nodes, density and min and max distance
  Graph * mygraph = new Graph(num_elems,density,1.0, 10.0);

  //print the graph 
  //mygraph->print();

  //calculate average distances from start node to all other nodes. 
  int num_paths = 0; //keep track of number of paths to calculate average, in case any path does not exist
  float avge_dist = 0; //accum of distances to calculate average

  for (int i=1; i < num_elems; i++)
  {

    ShortestPath *shortest_path  = new ShortestPath(mygraph, 0, i);

    if (shortest_path->GetFound()) //don't include non existing paths
    {
      //cout << "shortest path to " << i << " is " <<  shortest_path->GetCost() << "\n";
      avge_dist += shortest_path->GetCost();
      num_paths++;
    }
    delete shortest_path;
  }
  avge_dist = avge_dist / (static_cast <float>(num_paths));
  cout << "Out of " << num_paths << " paths existing from node 0, the average length is " << avge_dist << "\n";

  delete mygraph;


  return 0;

}


