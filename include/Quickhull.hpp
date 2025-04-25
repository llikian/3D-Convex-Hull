/***************************************************************************************************
 * @file  Quickhull.hpp
 * @brief Declaration of the Quickhull struct
 **************************************************************************************************/

#pragma once

#include <vector>
#include <glad/glad.h>
#include <sys/types.h>
#include "engine/Shader.hpp"
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

    void create(uint pointsAmount, float boundsMin, float boundsMax);

    void draw(Shader* shader);

    std::vector<vec3> points;
    Mesh pointsMesh;
    Mesh linesMesh;
    Mesh mesh;

    std::vector<std::vector<uint>> pointsAboveFace;
};
