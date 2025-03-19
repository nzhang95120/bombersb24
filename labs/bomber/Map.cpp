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

    const int bomb_reward = 2;

    if (src.lat < 0 || src.lng < 0 ||
        src.lat >= static_cast<int>(map.size()) || src.lng >= static_cast<int>(map[0].size()) ||
        walls.find(src) != walls.end() || waters.find(src) != waters.end()){
        throw PointError(src); 
    }
    if (dst.lat < 0 || dst.lng < 0 ||
        dst.lat >= static_cast<int>(map.size()) || dst.lng >= static_cast<int>(map[0].size()) ||
        waters.find(dst) != waters.end()) {
        throw PointError(dst);
    }

    size_t initialBombCount = 0;
    unordered_set<Point> initialBombs = bombs;
    if (initialBombs.find(src) != initialBombs.end()){
        initialBombs.erase(src);
        initialBombCount++;
    }

    int initial_h = abs(src.lat - dst.lat) + abs(src.lng - dst.lng);
    Node cur = Node{src, initialBombCount, "", initialBombs, walls, initial_h - static_cast<int>(bomb_reward * initialBombCount)}; 
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
            unordered_set<Point> newBombs = current.current_bombs;
            unordered_set<Point> newWalls = current.current_walls;

            if (newBombs.find(newPoint) != newBombs.end()) {
                newBombs.erase(newPoint);
                newBombCount++;
            }

            if (newWalls.find(newPoint) != newWalls.end() && newBombCount > 0) {
                newWalls.erase(newPoint);
                newBombCount--;
            }

            string newPath = current.path + direction;
            int new_g = newPath.size();
            if (newBombCount < current.bombCount) {
                new_g += 1;
            }
            int new_h = abs(newPoint.lat - dst.lat) + abs(newPoint.lng - dst.lng);
            int new_priority = (new_g + new_h) - static_cast<int>(bomb_reward * newBombCount);
            Node visiting = {newPoint, newBombCount, newPath, newBombs, newWalls, new_priority};

            auto visitIt = visited.find(visiting);
            if (visitIt == visited.end()) {
                visited[visiting] = new_g;
                exploration.push(visiting);
            } else if (new_g < static_cast<int>(visitIt->second)) {
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
    if(cur.current_walls.find(dst) != cur.current_walls.end() && cur.bombCount == 0){
        return false;
    }
    if(waters.find(dst) != waters.end()){
        return false;
    }
    return true;
}