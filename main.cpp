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

int main(int argc, char *argv[]) {
  
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


  // get start/end nodes
  int A = -1;
  int B = -1;
  while (intersections.find(A) == intersections.end()) {
    cout << "Start NodeID (eg. 29632): ";
    cin >> A;
  }
  while (intersections.find(B) == intersections.end()) {
    cout << "End   NodeID (eg.  5526): ";
    cin >> B;
  }
  cout << endl;
  
  // get travel vehicle
  Vehicle vehicle = CAR;
  int vehicleSelection;
  cout << "What kind of transport will you be using?" << endl;
  cout << "(car = 0, bicycle = 1, on foot = 2)" << endl;
  cin >> vehicleSelection;
  switch (vehicleSelection) {
    case 0: vehicle = CAR; break;
    case 1: vehicle = BICYCLE; break;
    case 2: vehicle = PEDESTRIAN; break;
    default: cout << "invalid number. presuming you meant Car..." << endl;
  }
  cout << endl;
  
  
  // here is where the A* magic happens :)
  vector<Intersection*> path;
  getNodePath(intersections, intersections[A], intersections[B], vehicle, path);
  
  
  // if we have a path
  if (path.size()) {
    if (argc > 1) {
       // DEBUG: show all nodeIDs in the path
      cout << "[Debug-info] NodeID path: " << endl;
      for (vector<Intersection*>::iterator it=path.begin(); it!=path.end(); ++it)
        cout << (*it)->id << "  ";
      cout << endl << endl;
    }

    // print out directions, summinng up multiple segments that have the same road name
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
    cout << "Could not find any path :(" << endl;
  
  return 0;
}
