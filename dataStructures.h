#ifndef DATASTRUCTURES // include guards
#define DATASTRUCTURES

#include <string>

using namespace std;



enum Vehicle { CAR, PEDESTRIAN, BICYCLE }; // types of transport we have data for


class Position {
public:
  double lattitude;
  double longitude;
  
  Position() { }
  Position(double lattitude, double longitude)
   : lattitude(longitude), longitude(longitude) { }
};

// forward declarations
class Segment;
class DirectedSegment;

class Intersection {
public:
  int id;
  Position pos;
  
  vector<DirectedSegment> connections;
  
  // searching stuff (externally used)
  bool visited;
  double weightToHere;
  DirectedSegment *from;
  
  Intersection() { }
  Intersection(int id, Position pos) 
    : id(id), pos(pos) { } // copy to fields
  
  double estimate(Intersection *b) {
    return 0.; // TODO: hypotinuse
  }
};


class Road {
public:

  int id;
  vector<Position> positions;
  vector<Segment> segments;
  
  // road info
  int type;
  string label;
  string city;
  bool oneWay;
  int speed;
  int roadClass;
  bool allow_car, allow_pedestrian, allow_bicycle;
  
  Road() { }
  Road(int id, vector<Position> positions) 
   : id(id), positions(positions) { }
  
  void setRoadInfo(int type, string label, string city, bool oneway, int speed, int roadclass, bool notforcar, bool notforpede, bool notforbicy) {
    this->type = type;
    this->label = label;
    this->city = city;
    this->oneWay = oneway;
    this->speed = speed;
    this->roadClass = roadclass;
    this->allow_car = ! notforcar;
    this->allow_pedestrian = !notforpede;
    this->allow_bicycle = !notforbicy;
  }

  bool allowsVehicle(Vehicle v) {
    switch (v) {
      case CAR:         return allow_car;
      case PEDESTRIAN:  return allow_pedestrian;   
      case BICYCLE:     return allow_bicycle;
      default:
        cout << "unknown vehicle: " << v << endl;
        return false;
    }
  }

};


class Segment {
public:
  double length;
  Intersection *a, *b; // our end points
  Road *road; // our parent (is set by Road constructor)
  
  Segment() { }
  Segment(double length, Intersection *a, Intersection *b, Road *road)
   : length(length), a(a), b(b), road(road) { // copy to fields
  }

  Intersection *getOther(Intersection *from) {
    if (from == a) return b;
    if (from == b) return a;
    cout << "I don't have this node D:" << endl;    
    return 0;
  }
  
    // check whether we're allowed to go from a to b using a transport method
  bool canGo(Intersection &from, Vehicle v) {
    if (&from == b  &&  road->oneWay) return false; // can't go the wrong way on a one-way street
    return road->allowsVehicle(v);
  }
};


class DirectedSegment {
public:
  double length;
  Intersection *from, *to; // our end points ('to' is our 'target')
  Road *road;
  
  // things for A* searching
  double weight;
  double weightToTarget;
  double estimate;
  double priority;
  
  DirectedSegment() { }
  DirectedSegment(Segment &toCopy, bool reverse)
   : length(toCopy.length), road(toCopy.road), weight(toCopy.length) { // TODO: better weight
    if (reverse) {  from=toCopy.b; to=toCopy.a;  }
    else         {  from=toCopy.a; to=toCopy.b;  }
  }
  
  void calculateWeights(Intersection *end) {
    weightToTarget = from->weightToHere + weight;
    priority = weightToTarget + to->estimate(end);
    //cout << "calculating Weights: " << from->id << " " << from->weightToHere << endl;
  }
  
  // for our priority queue. (see: http://www.cplusplus.com/reference/stl/priority_queue/priority_queue/)
  class Comparator {
    bool reverse;
  public:
    Comparator(const bool& revparam=false) {
      reverse = revparam;
    }
    bool operator() (const DirectedSegment *lhs, const DirectedSegment *rhs) const {
      //cout << "comparing " << lhs->weight << " " << lhs->priority << " <> " << rhs->weight  << " " << rhs->priority << endl;
      if (reverse) return (lhs->priority > rhs->priority);
      else         return (lhs->priority < rhs->priority);
    }
  };
};


#endif

