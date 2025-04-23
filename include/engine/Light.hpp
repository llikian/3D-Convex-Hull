/***************************************************************************************************
 * @file  Light.hpp
 * @brief Declaration of the Light structs
 **************************************************************************************************/

#pragma once

#include "maths/vec3.hpp"

/**
 * @struct Light
 * @brief The base class for all light sources.
 */
struct Light {
    vec3 ambient;  ///< The ambient component's color.
    vec3 diffuse;  ///< The diffuse component's color.
    vec3 specular; ///< The specular component's color.
};

/**
 * @struct DirectionalLight
 * @brief A directinal light. A light source that is designed to simulate an infinitely far away.
 * For example: the sun.
 */
struct DirectionalLight : public Light {
    vec3 direction; ///< Direction of the light.
};

/**
 * @struct PointLight
 * @brief A point light. A light source that illuminates in all directions in a limited radius.
 */
struct PointLight : public Light {
    vec3 position; ///< The light's position.

    float constant;  ///< The attenuation formula's constant component.
    float linear;    ///< The attenuation formula's linear component.
    float quadratic; ///< The attenuation formula's quadratic component.
};

/**
 * @struct SpotLight
 * @brief A spotlight. A light source that illuminates in a specific direction where only objects
 * in the radius of the spotlight are lit. Just like a point light, its strength is attenuated over
 * distance.
 */
struct SpotLight : public Light {
    vec3 position;  ///< The spotlight's position.
    vec3 direction; ///< The spotlight's direction.

    float cutOff;      ///< The cosinus of the radius of the spotlight.
    float outerCutOff; ///< The cosinus of the radius of the outer edge of the spotlight.
                       ///< Soft edges will appear between the cutoff and the outer cutoff angles.

    float constant;  ///< The attenuation formula's constant component.
    float linear;    ///< The attenuation formula's linear component.
    float quadratic; ///< The attenuation formula's quadratic component.
};

/**
 * @struct FlashLight
 * @brief A spotlight that is located at the camera's position and looks in the same direction as
 * the camera.
 */
struct FlashLight : public Light {
    float cutOff;      ///< The cosinus of the radius of the spotlight.
    float outerCutOff; ///< The cosinus of the radius of the outer edge of the spotlight.
                       ///< Soft edges will appear between the cutoff and the outer cutoff angles.

    float constant;  ///< The attenuation formula's constant component.
    float linear;    ///< The attenuation formula's linear component.
    float quadratic; ///< The attenuation formula's quadratic component.
};