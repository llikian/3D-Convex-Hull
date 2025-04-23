/***************************************************************************************************
 * @file  Quickhull.hpp
 * @brief Declaration of the Quickhull struct
 **************************************************************************************************/

#pragma once

#include <vector>
#include <glad/glad.h>
#include <sys/types.h>
#include "maths/vec3.hpp"
#include "mesh/Mesh.hpp"

/**
 * @struct Quickhull
 * @brief
 */
struct Quickhull {
    struct Triangle {
        uint A;
        uint B;
        uint C;
    };

    Quickhull(uint pointsAmount, float boundsMin, float boundsMax);

    void draw() {
        pointsMesh.draw();
    }

    std::vector<vec3> points;
    Mesh pointsMesh;
};
