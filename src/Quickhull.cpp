/***************************************************************************************************
 * @file  Quickhull.cpp
 * @brief Implementation of the Quickhull struct
 **************************************************************************************************/

#include "Quickhull.hpp"

#include <unordered_set>
#include "maths/geometry.hpp"

Quickhull::Quickhull(uint pointsAmount, float boundsMin, float boundsMax)
    : points(pointsAmount), pointsMesh(GL_POINTS), linesMesh(GL_LINES), mesh(GL_TRIANGLES) {
    create(pointsAmount, boundsMin, boundsMax);
}

void Quickhull::create(uint pointsAmount, float boundsMin, float boundsMax) {
    points.resize(pointsAmount);
    pointsMesh.clear();
    linesMesh.clear();
    mesh.clear();
    pointsAboveFace.clear();

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

        linesMesh.addPosition(points[i]);
    }

    // TODO : Check if two indices are the same

    std::vector<Triangle> faces;

    const vec3 center = (points[left] + points[right] + points[bottom] + points[top]) / 4.0f;

    auto addFaceAwayFromCenter = [this, &center, &faces](uint A, uint B, uint C) {
        vec3 normal = cross(points[B] - points[A], points[C] - points[A]);
        if(dot(center - points[A], normal) >= 0.0f) {
            faces.emplace_back(A, C, B);
        } else {
            faces.emplace_back(A, B, C);
        }
    };

    addFaceAwayFromCenter(top, left, bottom);
    addFaceAwayFromCenter(top, right, bottom);
    addFaceAwayFromCenter(top, left, right);
    addFaceAwayFromCenter(left, right, bottom);

    linesMesh.addLine(top, bottom);
    linesMesh.addLine(top, left);
    linesMesh.addLine(top, right);
    linesMesh.addLine(left, right);
    linesMesh.addLine(left, bottom);
    linesMesh.addLine(right, bottom);

    pointsAboveFace.resize(faces.size());

    std::unordered_set<uint> assignedPoints;
    assignedPoints.emplace(left);
    assignedPoints.emplace(right);
    assignedPoints.emplace(bottom);
    assignedPoints.emplace(top);

    for(uint i = 0 ; i < faces.size() ; ++i) {
        const vec3& A = points[faces[i].A];
        const vec3& B = points[faces[i].B];
        const vec3& C = points[faces[i].C];
        vec3 normal = cross(B - A, C - A);

        for(uint j = 0 ; j < pointsAmount ; ++j) {
            if(!assignedPoints.contains(j) && dot(points[j] - A, normal) >= 0.0f) {
                pointsAboveFace[i].push_back(j);
            }
        }
    }

    vec3 colors[4] {
        vec3(1.0f, 0.0f, 0.0f),
        vec3(1.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 1.0f)
    };

    for(uint i = 0 ; i < faces.size() ; ++i) {
        mesh.addPosition(points[faces[i].A]);
        mesh.addColor(colors[i]);
        mesh.addPosition(points[faces[i].B]);
        mesh.addColor(colors[i]);
        mesh.addPosition(points[faces[i].C]);
        mesh.addColor(colors[i]);

        for(uint index : pointsAboveFace[i]) {
            pointsMesh.addPosition(points[index]);
            pointsMesh.addColor(colors[i]);
        }
    }
}

void Quickhull::draw(Shader* shader) {
    pointsMesh.draw();

    glLineWidth(3.0f);
    shader->setUniform("useUniformColor", true);
    shader->setUniform("uColor", vec3(1.0f));
    linesMesh.draw();
    shader->setUniform("useUniformColor", false);
    glLineWidth(1.0f);

    shader->setUniform("alpha", 0.5f);
    mesh.draw();
    shader->setUniform("alpha", 1.0f);
}
