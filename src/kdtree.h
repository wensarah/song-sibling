#include <iostream>
#include "point.h"
#include <algorithm>
#include <cmath>
#include <set>
#include <queue>

//Creates the struct KDNode
struct KDNode {
    Point point;
    KDNode* left;
    KDNode* right;
};

//builds a KDTree given a vector of points
KDNode* buildTree(std:: vector<Point> points, int depth = 0) {
    if(points.empty()){
        return nullptr;
    } else if( depth % 2 == 0){ //split on x
        std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
        });
       
    } else { //split on y
        std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.y < b.y;
        });
    }
    int medianIndex = points.size()/2;
    Point median = points[medianIndex];
    std::vector<Point> leftPoints(points.begin(), points.begin() + medianIndex);
    std::vector<Point> rightPoints(points.begin() + 1 + medianIndex, points.end());
    KDNode* node = new KDNode();
    node->point = median;
    node->left = buildTree(leftPoints, depth + 1);
    node->right = buildTree(rightPoints, depth + 1);
    return node;    
}

// calculates the euclidean distance between two points.
double distance(const Point& a, const Point& b) {
    return std::sqrt(std::pow((a.x - b.x),2) + std::pow((a.y - b.y),2));
}


Point nearestNeighbor(KDNode* node, const Point& query, int depth, Point best){
    if(node == nullptr) {
        return best;
    }
    //calculate current best
    double currDistance = distance(query, node->point);
    if(distance(query, best) > currDistance){
        best = node->point;
    }
    
    double queryVal = (depth % 2 == 0) ? query.x : query.y;
    double nodeVal  = (depth % 2 == 0) ? node->point.x : node->point.y;

    KDNode* goodSide;
    KDNode* otherSide;
    if (queryVal < nodeVal) {
        goodSide = node->left;
        otherSide = node->right;
    } else {
        goodSide = node->right;
        otherSide = node->left;
    }

    best = nearestNeighbor(goodSide, query, depth + 1, best);
    
    if(std::abs(nodeVal - queryVal) < distance(query,best)) {
        best = nearestNeighbor(otherSide, query, depth + 1, best);
    }
    return best;
}

Point nearestNeighbor(KDNode* root, const Point& query) {
    return nearestNeighbor(root, query, 0, root->point);
}

void radiusQuery(KDNode* node, const Point& query, double r, int depth, std::vector<Point>& results) {
    if(node == nullptr){
        return;
    }
    if(distance(query, node->point) <=r) {
        results.push_back(node->point);
    }
    double queryVal = (depth % 2 == 0) ? query.x : query.y;
    double nodeVal = (depth % 2 == 0) ? node->point.x : node->point.y;
    if (std::abs(nodeVal - queryVal) <= r) {
        // could have points within r on both sides — check both
        radiusQuery(node->left, query, r, depth + 1, results);
        radiusQuery(node->right, query, r, depth + 1, results);
    } else if (queryVal < nodeVal) {
        radiusQuery(node->left, query, r, depth + 1, results);
    } else {
        radiusQuery(node->right, query, r, depth + 1, results);
    }
}

std::vector<std::vector<Point>> clusterPoints (const std::vector<Point>& points, KDNode* root, double r) {
    std::set<std::string> visited;
    std::vector<std::vector<Point>> clusters;

    for(const Point& p: points){
        std::string key = p.name;

        if(visited.find(key) == visited.cend()) { //its unvisited
            std::queue<Point> q; 
            visited.insert(key);
            q.push(p);
            std::vector<Point> cluster; 
            while(!q.empty()) {
                Point curr = q.front();
                q.pop();
                cluster.push_back(curr);
                std::vector<Point> withinRs; 
                radiusQuery(root,curr,r,0,withinRs); //getting the points neighbors.
                for(Point neigh : withinRs) {
                    if(visited.find(neigh.name) == visited.cend()) { //its unvisited
                        q.push(neigh);
                        visited.insert(neigh.name);
                    }
                }
            }
            clusters.push_back(cluster);
        }
    }
    return clusters;
}

//brute force 
Point bruteForceNearestNeighbor(const std::vector<Point>& points, const Point& query) {
    Point closest = points.front();
    double bestD = distance(closest,query);
    for(Point curr: points) {
        if(distance(curr, query) < bestD){
            closest = curr;
            bestD = distance(closest,query);
        }
    }
    return closest;
}



