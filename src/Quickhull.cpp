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
    std::vector<std::vector<uint>> pointsAboveFace;
    auto addFaceAwayFromCenter = [this, &faces, &pointsAboveFace](const vec3& center, uint A, uint B, uint C) {
        if(dot(center - points[A], cross(points[B] - points[A], points[C] - points[A])) >= 0.0f) {
            faces.emplace_back(A, C, B);
        } else {
            faces.emplace_back(A, B, C);
        }
        pointsAboveFace.emplace_back();
    };

    vec3 center = (points[left] + points[right] + points[bottom] + points[top]) / 4.0f;
    addFaceAwayFromCenter(center, top, left, bottom);
    addFaceAwayFromCenter(center, top, right, bottom);
    addFaceAwayFromCenter(center, top, left, right);
    addFaceAwayFromCenter(center, left, right, bottom);

    linesMesh.addLine(top, bottom);
    linesMesh.addLine(top, left);
    linesMesh.addLine(top, right);
    linesMesh.addLine(left, right);
    linesMesh.addLine(left, bottom);
    linesMesh.addLine(right, bottom);

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
                assignedPoints.emplace(j);
            }
        }
    }

    // vec3 colors[4]{
        // vec3(1.0f, 0.0f, 0.0f),
        // vec3(1.0f, 1.0f, 0.0f),
        // vec3(0.0f, 1.0f, 0.0f),
        // vec3(0.0f, 1.0f, 1.0f)
    // };

    // for(uint i = 0 ; i < faces.size() ; ++i) {
    //     mesh.addPosition(points[faces[i].A]);
    //     mesh.addColor(colors[i]);
    //     mesh.addPosition(points[faces[i].B]);
    //     mesh.addColor(colors[i]);
    //     mesh.addPosition(points[faces[i].C]);
    //     mesh.addColor(colors[i]);
    //
    //     for(uint index : pointsAboveFace[i]) {
    //         pointsMesh.addPosition(points[index]);
    //         pointsMesh.addColor(colors[i]);
    //     }
    // }

    for(uint i = 0 ; i < points.size() ; ++i) {
        pointsMesh.addPosition(points[i]);
    }

    for(uint i = 0 ; i < faces.size() ; ++i) {
        if(pointsAboveFace[i].empty()) { continue; }

        const vec3& A = points[faces[i].A];
        const vec3& B = points[faces[i].B];
        const vec3& C = points[faces[i].C];

        vec3 normal = normalize(cross(B - A, C - A));
        auto projectOnPlane = [&A, &normal](const vec3& P) -> vec3 {
            return P - dot(P - A, normal) * normal;
        };

        uint farthest = pointsAboveFace[i][0];
        float farthestDistance = length(projectOnPlane(points[pointsAboveFace[i][0]]) - points[pointsAboveFace[i][0]]);

        for(uint j = 1 ; j < pointsAboveFace[i].size() ; ++j) {
            uint index = pointsAboveFace[i][j];
            float distance = length(projectOnPlane(points[index]) - points[index]);
            if(distance > farthestDistance) {
                farthest = index;
                farthestDistance = distance;
            }
        }

        center = (A + B + C + points[farthest]) / 4.0f;
        addFaceAwayFromCenter(center, farthest, faces[i].A, faces[i].B);
        addFaceAwayFromCenter(center, farthest, faces[i].A, faces[i].C);
        addFaceAwayFromCenter(center, farthest, faces[i].B, faces[i].C);

        std::unordered_set<uint> assigned;
        assigned.emplace(farthest);

        for(uint j = faces.size() - 3 ; j < faces.size() ; ++j) {
            vec3 a = points[faces[j].A];
            vec3 n = cross(points[faces[j].B] - a, points[faces[j].C] - a);

            for(uint index : pointsAboveFace[i]) {
                if(!assigned.contains(index) && dot(points[index] - a, n) >= 0.0f) {
                    pointsAboveFace[i].push_back(index);
                    assigned.emplace(index);
                }
            }
        }

        linesMesh.addLine(farthest, faces[i].A);
        linesMesh.addLine(farthest, faces[i].B);
        linesMesh.addLine(farthest, faces[i].C);
    }

    for(uint i = 0 ; i < faces.size() ; ++i) {
        mesh.addPosition(points[faces[i].A]);
        mesh.addPosition(points[faces[i].B]);
        mesh.addPosition(points[faces[i].C]);
    }
}

void Quickhull::draw(Shader* shader) {
    shader->setUniform("useUniformColor", true);
    shader->setUniform("uColor", vec3(1.0f));
    pointsMesh.draw();
    glLineWidth(3.0f);
    linesMesh.draw();
    glLineWidth(1.0f);
    shader->setUniform("useUniformColor", false);

    shader->setUniform("alpha", 0.5f);
    mesh.draw();
    shader->setUniform("alpha", 1.0f);
}
