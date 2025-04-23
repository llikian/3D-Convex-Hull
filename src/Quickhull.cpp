/***************************************************************************************************
 * @file  Quickhull.cpp
 * @brief Implementation of the Quickhull struct
 **************************************************************************************************/

#include "Quickhull.hpp"

Quickhull::Quickhull(uint pointsAmount, float boundsMin, float boundsMax)
    : points(pointsAmount), pointsMesh(GL_POINTS) {
    uint left = 0;
    uint right = 0;
    uint bottom = 0;
    uint top = 0;

    for(uint i = 0 ; i < pointsAmount ; ++i) {
        points[i] = vec3::random(boundsMin, boundsMax);

        if(points[i].x < points[left].x) { left = i; }
        if(points[i].x > points[right].x) { right = i; }
        if(points[i].y < points[bottom].y) { bottom = i; }
        if(points[i].y > points[top].y) { top = i; }

        pointsMesh.addPosition(points[i]);
    }

    std::vector<Triangle> faces;
    faces.emplace_back(top, left, right);
    faces.emplace_back(bottom, left, right);

    std::cout << "left: " << left << "\nright: " << right << "\nbottom: " << bottom << "\ntop: " << top << '\n';
}
