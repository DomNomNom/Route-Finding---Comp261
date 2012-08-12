/****************************************************\
|                                                    |
| This file is part of a A* route finding program    |
| intended for academic purposes                     |
|                                                    |
| Author: Dominik Schmid                             |
|                                                    |
\****************************************************/

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
  parseIntersections(intersections);
  parseRoads(roads);
  parseSegments(roads, intersections);

  /*
  // DEBUG: Some tests to check parsing got the proper results
  cout << "test intersections[5]: " << intersections[5].pos.lattitude << endl;
  cout << "test roads[34589].position: " << roads[34589].positions[0].lattitude << endl;
  cout << "test roads[3].label: " << roads[3].label << endl;
  cout << "test roads[3].segments.a.id: " << roads[3].segments[0].a->id << endl;
  
  cout << "test connections: " 
       << intersections[9655].connections[0]->length    << " "
       << intersections[9655].connections[0]->a->id     << " "
       << intersections[9655].connections[0]->b->id     << endl;
  */


  // nodes
  int A = 29632;
  int B = 5526;
  
  vector<Intersection*> path;
  getNodePath(intersections, intersections[A], intersections[B], PEDESTRIAN, path);
  
  /* // DEBUG: show all nodeIDs in the path
  cout << "NodeID path: " << endl;
  for (vector<Intersection*>::iterator it=path.begin(); it!=path.end(); ++it)
    cout << (*it)->id << "  ";
  cout << endl << endl;
  */
  
  // print out directions, summinng up multiple segments that have the same road name
  if (intersections.size()) {
    cout << "Directions: " << endl;
    Road *prevRoad = 0;
    double accumulatedRoadLength = 0;
    double totalDistance = 0;
    for (vector<Intersection*>::iterator it=path.begin(); it!=path.end(); ++it) {
      DirectedSegment *seg = (*it)->from;
      if (seg == 0) continue; // don't use the first one
      if (prevRoad == 0) 
        prevRoad = seg->road;
      if (seg->road->label.compare(prevRoad->label) != 0) { // if we now have a different road
        cout << "Follow " << prevRoad->label << " for " << accumulatedRoadLength << "km" << endl;
        prevRoad = seg->road;
        totalDistance += accumulatedRoadLength;
        accumulatedRoadLength = 0;
      }
      accumulatedRoadLength += seg->length;
    }
    if (prevRoad != 0)
      cout << "follow " << prevRoad->label << " for " << accumulatedRoadLength << " km" << endl << endl;

    cout << "Total travel distance: " << totalDistance << " km" << endl;
    cout << "minimum time (obiding speed limits): " << intersections[B].weightToHere << " hours" << endl << endl;
      
    cout << "done  :)" << endl;
  }
  else
    cout << "could not find path" << endl;
  
  return 0;
}
