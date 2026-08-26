#include <iostream>
#include <vector>
#include <string>
#include "point.h"
#include "kdtree.h"
#include "httplib.h"
int main() {
    //testing point.h
    std::vector<Point> points = loadPoints("data/spotify.csv");
    for(const Point& city : points){
        std::string title = city.name + " " + std::to_string(city.x)+ " " + std::to_string(city.y);
        std::cout << title << std::endl; 
    }
    
    //testing buildTree
    KDNode* kdtree = buildTree(points,0);
    std::cout << kdtree->point.name << std::endl;
    

    //testing findNearest
    const Point query = {"query", 47.0, -120.0};
    Point nearest = nearestNeighbor(kdtree, query);
    std::cout << "Nearest: " << nearest.name << std::endl;

    //testing clusters
    double r = 15.0; // tweak this — try a few values
    std::vector<std::vector<Point>> clusters = clusterPoints(points, kdtree, r);

    std::cout << "Found " << clusters.size() << " clusters:" << std::endl;
    for (size_t i = 0; i < clusters.size(); i++) {
        std::cout << "Cluster " << i << ": ";
        for (const Point& p : clusters[i]) {
            std::cout << p.name << " ";
        }
        std::cout << std::endl;
    }
    //brute force
    //correctness
    /*
    Point treeResult = nearestNeighbor(kdtree, query);
    Point bruteResult = bruteForceNearestNeighbor(points, query);
    if (treeResult.name == bruteResult.name) {
        std::cout << "Correctness check passed: both agree on " << treeResult.name << std::endl;
    } else {
        std::cout << "MISMATCH: tree=" << treeResult.name << " brute=" << bruteResult.name << std::endl;
    }
    //speed
    
    auto bStart = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000; i++) {
        bruteForceNearestNeighbor(points, query);
    }
    auto bEnd = std::chrono::high_resolution_clock::now();
    auto bElapsed = std::chrono::duration_cast<std::chrono::microseconds>(bEnd - bStart);
    std::cout << "Brute Force Time: " << bElapsed.count() << std::endl;
    
    //kdtree
    auto kStart = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000; i++) {
        nearestNeighbor(kdtree, query);
    }

    auto kEnd = std::chrono::high_resolution_clock::now();
    auto kElapsed = std::chrono::duration_cast<std::chrono::microseconds>(kEnd - kStart);
    std::cout << "Kdtree Time: " << kElapsed.count() << std::endl;

    double ratio = ((double)bElapsed.count()/kElapsed.count()-1) * 100;
    std::cout << "The brute force method took " << ratio << "% longer than the KDtree method for finding the nearest neighbor" << std::endl;
    */

    //server code using http lib.h
    httplib:: Server svr;
    svr.Get("/nearest", [&](const httplib::Request &req, httplib::Response &res) {
        double qx = std::stod(req.get_param_value("x"));
        double qy = std::stod(req.get_param_value("y"));
        Point query = {"query", qx, qy};
        Point result = nearestNeighbor(kdtree, query);

        std::string json = "{\"name\":\"" + result.name + "\",\"x\":" + std::to_string(result.x) + ",\"y\":" + std::to_string(result.y) + "}";
        res.set_content(json, "application/json");
    });

    svr.Get("/radius", [&](const httplib::Request &req, httplib::Response &res) {
        double qx = std::stod(req.get_param_value("x"));
        double qy = std::stod(req.get_param_value("y"));
        double r = std::stod(req.get_param_value("radius"));
        Point query = {"query", qx, qy};
        std::vector<Point> results;
        radiusQuery(kdtree, query, r, 0, results);
        std::string json = "[";
        for(size_t i = 0; i < results.size(); i++) {
            json += "{\"name\":\"" + results[i].name + "\",\"x\":" + std::to_string(results[i].x) + ",\"y\":" + std::to_string(results[i].y) + "}";
            if (i != results.size() - 1) {
                json += ",";
            }
        }
        json += "]";
        res.set_content(json, "application/json");
    });

    svr.Get("/cluster", [&](const httplib::Request &req, httplib::Response &res) {
        double r = std::stod(req.get_param_value("radius"));
        std::vector<std::vector<Point>> clusters;
        clusters = clusterPoints(points, kdtree, r);
        std::string json = "[";
        for (size_t i = 0; i < clusters.size(); i++) {
            json += "[";
            for (size_t j = 0; j < clusters[i].size(); j++) {
                json += "{\"name\":\"" + clusters[i][j].name + "\",\"x\":" + std::to_string(clusters[i][j].x) + ",\"y\":" + std::to_string(clusters[i][j].y) + "}";
                if (j != clusters[i].size() - 1) {
                    json += ",";
                }
            }
            json += "]";
            if (i != clusters.size() - 1) {
                json += ",";
            }
        }
        json += "]";
        res.set_content(json, "application/json");
    });

    svr.Get("/points", [&](const httplib::Request &req, httplib::Response &res) {
    std::string json = "[";
    for (size_t i = 0; i < points.size(); i++) {
        json += "{\"name\":\"" + points[i].name + "\",\"x\":" + std::to_string(points[i].x) + ",\"y\":" + std::to_string(points[i].y) + "}";
        if (i != points.size() - 1) {
            json += ",";
        }
    }
    json += "]";
    res.set_content(json, "application/json");
    });
    std::cout << "Server running at http://localhost:8080" << std::endl;
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"}
    });
    svr.listen("localhost", 8080);
    return 0;
}

