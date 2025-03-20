#include "Map.h"
#include "Errors.h"
#include <sstream>
#include <string>

// Member Functions


Map::Map(std::istream& stream){
    string line;
    int row = 0;
    int col = 0;
    while(getline(stream, line)){
        istringstream ss(line);
        vector<Point> newRow; 
        map.push_back(newRow);
        for(char c : line){
            Point newPoint = Point(row, col);
            if (c == '*'){
                bombs.insert(newPoint);
            } else if (c == '#'){
                walls.insert(newPoint);
            } else if (c == '~'){
                waters.insert(newPoint);
            }
            map[row].push_back(newPoint);
            col++;
        }
        col = 0;
        row++;
    }


}

std::string Map::route(Point src, Point dst){


    if (src.lat < 0 || src.lng < 0 ||
        src.lat >= static_cast<int>(map.size()) || src.lng >= static_cast<int>(map[0].size())){
        throw PointError(src); 
    }
    if(waters.find(src) != waters.end() || walls.find(src) != walls.end()){
        throw RouteError(src, dst);
    }
    if (dst.lat < 0 || dst.lng < 0 ||
        dst.lat >= static_cast<int>(map.size()) || dst.lng >= static_cast<int>(map[0].size()) ||
        waters.find(dst) != waters.end()) {
        throw PointError(dst);
    }

    size_t initialBombCount = 0;
    unordered_set<Point> collected_bombs;
    if (bombs.count(src)) {
        collected_bombs.insert(src);
        initialBombCount = 1;
    }

    int initial_h = abs(src.lat - dst.lat) + abs(src.lng - dst.lng);
    const int bomb_reward = std::max(3, initial_h / 5);
    Node cur = Node{src, initialBombCount, "", collected_bombs, {}, initial_h - static_cast<int>(bomb_reward * initialBombCount)}; 
    visited[cur] = 0;
    exploration.push(cur);

    while(!exploration.empty()){
        Node current = exploration.top();
        exploration.pop();
        
        if (current.position == dst) {
            while (!exploration.empty()){
                exploration.pop();
            }
            visited.clear();
            return current.path;
        }

        auto it = visited.find(current);
        if (it != visited.end() && current.path.size() > it->second) {
            continue;
        }

        for (size_t i = 0; i < 4; ++i) {
            int newLat = current.position.lat + dy[i];
            int newLng = current.position.lng + dx[i];
            char direction = dir[i];

            if (newLat < 0 || newLng < 0 || newLat >= static_cast<int>(map.size()) || newLng >= static_cast<int>(map[0].size())) {
                continue;
            }

            Point newPoint = map[newLat][newLng];
            if (!isValid(current, newPoint)) {
                continue;
            }

            size_t newBombCount = current.bombCount;
            unordered_set<Point> new_collected = current.collected_bombs;
            unordered_set<Point> new_destroyed = current.destroyed_walls;

            if (bombs.count(newPoint) && !new_collected.count(newPoint)) {
                new_collected.insert(newPoint);
                newBombCount++;
            }

            if (walls.count(newPoint) && !new_destroyed.count(newPoint) && newBombCount > 0) {
                new_destroyed.insert(newPoint);
                newBombCount--;
            }


            string newPath = current.path + direction;
            int new_g = newPath.size();
            int new_h = abs(newPoint.lat - dst.lat) + abs(newPoint.lng - dst.lng);
            int new_priority = (new_g + new_h) - static_cast<int>(bomb_reward * newBombCount);
            Node visiting = {newPoint, newBombCount, newPath, new_collected, new_destroyed, new_priority};

            auto visitIt = visited.find(visiting);
            if (visitIt == visited.end() || new_g < static_cast<int>(visitIt->second)) {
                visited[visiting] = new_g;
                exploration.push(visiting);
            }
        }
    }

    while (!exploration.empty()) {
        exploration.pop();
    }
    visited.clear();
    throw RouteError(src, dst);
}

bool Map::isValid(Node cur, Point dst){
    if(dst.lat < 0 || dst.lng < 0 || dst.lat >= static_cast<int>(map.size()) || dst.lng >= static_cast<int>(map[0].size())){
        return false;
    }
    if (walls.count(dst) && !cur.destroyed_walls.count(dst) && cur.bombCount == 0){
        return false;
    }

    if(waters.find(dst) != waters.end()){
        return false;
    }
    return true;
}