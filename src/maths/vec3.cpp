/***************************************************************************************************
 * @file  vec3.cpp
 * @brief Implementation of the vec3 struct
 **************************************************************************************************/

#include "maths/vec3.hpp"

#include <random>

vec3::vec3() : x(0.0f), y(0.0f), z(0.0f) { }

vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) { }

vec3::vec3(float scalar) : x(scalar), y(scalar), z(scalar) { }

vec3& vec3::operator +=(const vec3& vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;

    return *this;
}

vec3& vec3::operator -=(const vec3& vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;

    return *this;
}

vec3& vec3::operator *=(const vec3& vec) {
    x *= vec.x;
    y *= vec.y;
    z *= vec.z;

    return *this;
}

vec3& vec3::operator /=(const vec3& vec) {
    x /= vec.x;
    y /= vec.y;
    z /= vec.z;

    return *this;
}

vec3& vec3::operator +=(float scalar) {
    x += scalar;
    y += scalar;
    z += scalar;

    return *this;
}

vec3& vec3::operator -=(float scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;

    return *this;
}

vec3& vec3::operator *=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;
}

vec3& vec3::operator /=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;

    return *this;
}

vec3 vec3::random(float min, float max) {
    static std::random_device seed;
    static std::default_random_engine generator(seed());

    return vec3(std::uniform_real_distribution<float>(min, max)(generator),
                std::uniform_real_distribution<float>(min, max)(generator),
                std::uniform_real_distribution<float>(min, max)(generator));
}

vec3 vec3::random(const vec3& min, const vec3& max) {
    static std::random_device seed;
    static std::default_random_engine generator(seed());

    return vec3(std::uniform_real_distribution<float>(min.x, max.x)(generator),
                std::uniform_real_distribution<float>(min.y, max.y)(generator),
                std::uniform_real_distribution<float>(min.z, max.z)(generator));
}

std::ostream& operator <<(std::ostream& stream, const vec3& vec) {
    stream << "( " << vec.x << " ; " << vec.y << " ; " << vec.z << " )";
    return stream;
}

std::istream& operator >>(std::istream& stream, vec3& vec) {
    stream >> vec.x >> vec.y >> vec.z;
    return stream;
}

vec3 operator +(const vec3& left, const vec3& right) {
    return vec3(
        left.x + right.x,
        left.y + right.y,
        left.z + right.z
    );
}

vec3 operator -(const vec3& left, const vec3& right) {
    return vec3(
        left.x - right.x,
        left.y - right.y,
        left.z - right.z
    );
}

vec3 operator *(const vec3& left, const vec3& right) {
    return vec3(
        left.x * right.x,
        left.y * right.y,
        left.z * right.z
    );
}

vec3 operator /(const vec3& left, const vec3& right) {
    return vec3(
        left.x / right.x,
        left.y / right.y,
        left.z / right.z
    );
}

vec3 operator +(const vec3& vec, float scalar) {
    return vec3(
        vec.x + scalar,
        vec.y + scalar,
        vec.z + scalar
    );
}

vec3 operator -(const vec3& vec, float scalar) {
    return vec3(
        vec.x - scalar,
        vec.y - scalar,
        vec.z - scalar
    );
}

vec3 operator *(const vec3& vec, float scalar) {
    return vec3(
        vec.x * scalar,
        vec.y * scalar,
        vec.z * scalar
    );
}

vec3 operator *(float scalar, const vec3& vec) {
    return vec3(
        scalar * vec.x,
        scalar * vec.y,
        scalar * vec.z
    );
}

vec3 operator /(const vec3& vec, float scalar) {
    return vec3(
        vec.x / scalar,
        vec.y / scalar,
        vec.z / scalar
    );
}

bool operator ==(const vec3& left, const vec3& right) {
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

bool operator !=(const vec3& left, const vec3& right) {
    return left.x != right.x || left.y != right.y || left.z != right.z;
}
