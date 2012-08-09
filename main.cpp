#include <iostream>  // cout

// data structures
#include <vector>
#include <map>
#include "dataStructures.h"

#include "fileParsing.cpp"
#include "a_star.cpp"

using namespace std;

// ================ MAIN =================

int main() {
  
  map<int, Intersection> intersections;
  map<int, Road> roads;

  // parsing
  //cout << "Reading files... ";
  parseIntersections(intersections);
  parseRoads(roads);
  parseSegments(roads, intersections);
  //cout << "done." << endl;


  /*
  // Some tests to check parsing got the proper results
  cout << "test intersections[5]: " << intersections[5].pos.lattitude << endl;
  cout << "test roads[34589].position: " << roads[34589].positions[0].lattitude << endl;
  cout << "test roads[3].label: " << roads[3].label << endl;
  cout << "test roads[3].segments.a.id: " << roads[3].segments[0].a->id << endl;
  
  cout << "test connections: " 
       << intersections[9655].connections[0]->length    << " "
       << intersections[9655].connections[0]->a->id     << " "
       << intersections[9655].connections[0]->b->id     << endl;
  */

  // run A*

  //bool pathFound = a_star(intersections, intersections[10120], intersections[38014]);
  //cout << "pathFound: " << pathFound << endl;
  //getNodePath(intersections, intersections[3], intersections[5858], path);

  // print nodes
  int A = 29632;
  int B = 5526;
  
  vector<Intersection*> path;
//  getNodePath(intersections, intersections[993037], intersections[38005], path);
  getNodePath(intersections, intersections[A], intersections[B], path); // should
  for (vector<Intersection*>::iterator it=path.begin(); it!=path.end(); ++it)
    cout << (*it)->id << "  ";
  cout << endl;
  
  cout << "weight: " << intersections[B].weightToHere << endl;
    
  cout << "done  :)" << endl;
  
  return 0;
}
