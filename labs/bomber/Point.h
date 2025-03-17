#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point {
  int lat;  // Latitude
  int lng;  // Longitude

  Point() {
    // Beware of junk values!
  }

  Point(int lat, int lng) {
    this->lat = lat;
    this->lng = lng;
    
  }

  Point(const Point& other) {
    this->lat = other.lat;
    this->lng = other.lng;
  }
  bool operator==(const Point& other) const {
    return lat == other.lat && lng == other.lng;
  }
  Point& operator=(const Point& other) {
    if (this == &other) {
        return *this;
    }
    lat = other.lat;
    lng = other.lng;
    return *this;
}
};

namespace std {
  template <>
  struct hash<Point> {
    size_t operator()(const Point& p) const {
      size_t hashLat = std::hash<int>{}(p.lat);
      size_t hashLng = std::hash<int>{}(p.lng);
      return hashLat ^ (hashLng << 1); 
    }
  };
}


// Operators for easier (and consistent) input and output:
std::ostream& operator << (std::ostream& stream, const Point& point);
std::istream& operator >> (std::istream& stream, Point& point);

#endif
