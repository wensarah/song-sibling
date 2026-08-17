#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#pragma once
struct Point {
    std::string name;
    double x;
    double y;
};

std::vector<Point> loadPoints(const std::string& filename) { //filename is a reference
    // Open the file using the parameter variable passed into the function
    std::ifstream file(filename);

    std::vector<Point> points;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string nameStr, xStr, yStr;

        // Extract the 3 comma-separated tokens from stringstream
        std::getline(ss, nameStr, ',');
        std::getline(ss, xStr, ',');
        std::getline(ss, yStr, ',');

        // Convert numeric strings to doubles using std::stod()
        double xVal = std::stod(xStr);
        double yVal = std::stod(yStr);

        // Instantiate Point, assign fields, and add to vector
        Point p;
        p.name = nameStr;
        p.x = xVal;
        p.y = yVal;

        points.push_back(p);
    }
    return points;
}
