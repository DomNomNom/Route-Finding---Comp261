#include <iostream> // cout TODO: remove me
#include <limits> // double.infinity

#include "dataStructures.h"

using namespace std;


/****************************************************\
|                                                    |
| This function will use the A* algorithm to the     |
| shortest path from A to B using the given map of   |
| nodes.                                             |
|                                                    |
| The final path will be the linked list from B to A |
| when following the trail of 'from' pointers        |
| Note: please use the method(s) below for this      |
|                                                    |
| The return value indicates whether the path has    |
| been found.                                        |
|                                                    |
\****************************************************/

bool a_star(map<int, Intersection> &nodes, Intersection &A, Intersection &B) {
  /* ----- Pseudo code: ------
  note: Node and directed-edge refer to Intersection and DirectedEdge respectively
  
  variables:
  Node A // starting Node
  Node B // end Node
  priorityQueue<over directed-edges, sorted by (weightToTarget + target.estimate(B))>  fringe
  
  intialize:
    all nodes.visited ← false
    all nodes.cameFrom ← null
    all nodes.costToHere ← infinity
  

  push A onto fringe
  
  Repeat until fringe is empty:
    deque fringe into 'edge'
    if edge.to.visited: next repeat

    edge.to.visited ← true
    edge.to.pathTo ← from
    edge.to.weightToHere ← edge.weightToTarget
    if edge.to = goal: return path-found (true)
    for each directed-edge 'branch' coming out of edge.to:
      if not branch.to.visited:
        branch.weightToTarget = edge.weightToTarget + branch.weight
        branch.estimate = branch.target.estimate(B)
        enqueue branch into fringe

  return path-not-possible (false)
  
  */
  
  // Initialize
  for (map<int, Intersection>::iterator i=nodes.begin(); i!=nodes.end(); ++i) { 
    //cout << i->first << ": " << i->second.id << endl;
    Intersection &node = i->second;
    node.visited = false;
    node.from = 0;
    node.weightToHere = numeric_limits<double>::infinity();
  }
  
  
}


/****************************************************\
|                                                    |
| This function will put the path of the above       |
| result into a nice vector starting from A to B.    |
|                                                    |
\****************************************************/
void extractPath(map<int, Intersection> &nodes, vector<Intersection *> &target) {
  // TODO
}
