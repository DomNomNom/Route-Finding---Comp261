/****************************************************\
|                                                    |
| This file is part of a A* route finding program    |
| intended for academic purposes                     |
|                                                    |
| Author: Dominik Schmid                             |
|                                                    |
\****************************************************/

#ifndef FILEPARSING // include guards
#define FILEPARSING

#include <iostream> // cout
#include <fstream>  // file reading
#include <stdlib.h> // C-style parsing (FILE, fscanf)

// for line-by-line parsing
#include <sstream>
#include <string>

// data structures
#include <vector>
#include <map>

#include "dataStructures.h"

using namespace std;

// creates intersectoins from the file and puts it in the given map
void parseIntersections(map<int, Intersection> &intersections) {
  ifstream intersectionsFile("nodeID-lat-lon.tab");
  if (intersectionsFile.is_open()) {
    int id; 
    double lattitude, longitude;
    while (intersectionsFile >> id >> lattitude >> longitude) // parsing. (not fail-safe)
      intersections[id] = Intersection(id, Position(lattitude, longitude));
    intersectionsFile.close();
  }
  else cout << "Unable to open node file" << endl; 
}

void _parseRoadInfo(map<int, Road> &roads) {
  // note: we are useing C-style parsing here
  int id;
  int type;
  char label[99];
  char city[99];
  int oneway;
  int speed;
  int roadclass;
  int notforcar, notforpede, notforbicy;
  
  FILE *roadsFile = fopen("roadID-roadInfo.tab","r");
  int ret = fscanf(roadsFile, "%*[^\n]"); // throw away the first line
  while (ret != EOF) {

    // scanf magic curtesy of TrueShiftBlue (Ben)
    ret = fscanf(
      roadsFile,
      "%d %d %99[^\t] %99[^\t] %d %d %d %d %d %d",
      &id, &type, label, city, &oneway, &speed, &roadclass, &notforcar, &notforpede, &notforbicy
    );
    
    if (roads.find(id) == roads.end()) { // check for non-existent IDs
      //cout << "ParserWarning: ID " << id << " hasn't been defined" << endl;
      continue;
    }

    roads[id].setRoadInfo(type, label, city, oneway, speed, roadclass, notforcar, notforpede, notforbicy);
  }
}


void parseRoads(map<int, Road> &roads) {
  ifstream roadsFile("roadID-coords.tab");
  if (roadsFile.is_open()) {
    string line;
    while (getline(roadsFile, line)) {
      int id;
      double lattitude, longitude;
      vector<Position> positions;

      istringstream iss(line);  // our parser over the line
      iss >> id;
      
      while (iss >> lattitude >> longitude)
        positions.push_back(Position(lattitude, longitude));

      if (roads.find(id) != roads.end()) // check for duplicate IDs
        cout << "OMG, ID " << id << " is duplicated" << endl;

      roads[id] = Road(id, positions);
    }
  }
  else cout << "Unable to open road file" << endl; 

  _parseRoadInfo(roads);
}


void parseSegments(map<int, Road> &roads, map<int, Intersection> &intersections) {
  ifstream segmentFile("roadSeg-roadID-length-nodeID-nodeID.tab");
  if (segmentFile.is_open()) {
    string line;
    getline(segmentFile, line); // throw away the first line
    while (getline(segmentFile, line)) {
      int roadID;
      double segmentLength;
      int nodeID1, nodeID2;
      
      istringstream iss(line);  // our parser over the line

      iss >> roadID >> segmentLength >> nodeID1 >> nodeID2;

      map<int, Road>::iterator roadItem = roads.find(roadID); // find the road we want to append the segment to
      if (roadItem == roads.end()) { // check for invalid IDs
        cout << "ParserWarning: ID " << roadID << " hasn't been defined" << endl;
        continue;
      }
      
      // add to the list of segments the road has
      roadItem->second.segments.push_back( Segment(
        segmentLength,
        &(intersections.find(nodeID1)->second),
        &(intersections.find(nodeID2)->second),
        &(roadItem->second)
      ));
      
      // notify our intersections that we connect them using DirectedSegments
      Segment &newSegment = roadItem->second.segments.back();
      intersections.find(nodeID1)->second.connections.push_back(
        DirectedSegment(newSegment, false)
      );
      if (! newSegment.road->oneWay) // Only add the back-link if our road is not one-way
        intersections.find(nodeID2)->second.connections.push_back(
          DirectedSegment(newSegment, true)
        );
    }
  }
  else cout << "Unable to open road file" << endl; 
}


#endif
