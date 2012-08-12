/****************************************************\
|                                                    |
| This file is part of a A* route finding program    |
| intended for academic purposes                     |
|                                                    |
| Author: Dominik Schmid                             |
|                                                    |
\****************************************************/


#include <iostream> // cout   TODO: remove me
#include <limits> // double.infinity

#include <algorithm> // reverse

#include <vector>
#include <queue>  // priority queue

#include "dataStructures.h"

using namespace std;

typedef priority_queue<DirectedSegment*, vector<DirectedSegment*>, DirectedSegment::Comparator> priorityQ;



// helper method: pushes all edges from the node onto the queue
void pushNodeConnections(Intersection *node, Intersection &finalNode, Vehicle tansport, priorityQ &q) {
  for (vector<DirectedSegment>::iterator it=node->connections.begin(); it!=node->connections.end(); ++it) {
    if (it->to->visited) continue;
    if (! it->road->allowsVehicle(tansport)) { continue;}
    it->calculateWeights(&finalNode);  // note: we need to calculate priority before we push
    q.push(&(*it));
  }
}


/****************************************************\
|                                                    |
| This function will use the A* algorithm to the     |
| shortest path from A to B using the given map of   |
| nodes.                                             |
|                                                    |
| Note: please use the method(s) below for useful    |
| data such as the node-path                         |
|                                                    |
| The final path will be the linked list from B to A |
| when following the trail of 'from' pointers        |
|                                                    |
| The return value indicates whether the path has    |
| been found.                                        |
|                                                    |
\****************************************************/

bool a_star(map<int, Intersection> &nodes, Intersection &A, Intersection &B, Vehicle tansport) {
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
  
  A.costToHere = 0
  A.cameFrom ← A

  push all of A.connections onto fringe
  
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
        branch.priority = branch.weightToTarget + branch.estimate
        enqueue branch into fringe

  return path-not-possible (false)
  
  */
  
  // Initialize
  for (map<int, Intersection>::iterator i=nodes.begin(); i!=nodes.end(); ++i) { 
    Intersection &node = i->second;
    node.visited = false;
    node.from = 0;
    node.weightToHere = numeric_limits<double>::infinity();
  }
  
  priorityQ fringe(DirectedSegment::Comparator(true));

  A.weightToHere = 0;
  A.visited = true;

  pushNodeConnections(&A, B, tansport, fringe);
  
  while (! fringe.empty()) {
    // dequeue
    DirectedSegment *edge = fringe.top();
    fringe.pop();
    
    if (edge->to->visited) continue;
    edge->to->visited = true;
    edge->to->from = edge;
    edge->to->weightToHere = edge->weightToTarget;
    if (edge->to == &B) return true;
    
    pushNodeConnections(edge->to, B, tansport, fringe);
  }
  return false;
}



/****************************************************\
|                                                    |
| This function will put the path of the above       |
| result into a nice vector starting from A to B.    |
|                                                    |
\****************************************************/
void getNodePath(map<int, Intersection> &nodes, Intersection &A, Intersection &B, Vehicle tansport, vector<Intersection *> &dataOut) {
  bool pathFound = a_star(nodes, A, B, tansport);
  if (! pathFound) return;
  
  // build it from back to front and reverse later.
  dataOut.push_back(&B);
  while (dataOut.back() != &A)
    dataOut.push_back(dataOut.back()->from->from); // 2 froms because one for node one for segment
  
  reverse(dataOut.begin(), dataOut.end());
}


/* note: this function has not been properly tested as it is redundant. dragons be here
void getSegmentPath(map<int, Intersection> &nodes, Intersection &A, Intersection &B, vector<DirectedSegment *> &dataOut) {
  bool pathFound = a_star(nodes, A, B);
  if (! pathFound) return;
  
  Intersection *current = &B;
  // build it from back to front and reverse later.
  while (current != &A) {
    dataOut.push_back(current->from); // 2 froms because one for node one for segment
    current = current->from->from;
  }
  
  reverse(dataOut.begin(), dataOut.end());
} */
