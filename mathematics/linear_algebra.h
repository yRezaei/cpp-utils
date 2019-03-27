#pragma once
#ifndef CORE_LINEAR_ALGEBRA_H
#define CORE_LINEAR_ALGEBRA_H

#include "../core.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/intersect.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtc/noise.hpp"
#include "glm/gtc/type_ptr.hpp"


constexpr int		MaxInt					= std::numeric_limits<int>::max();
constexpr float		MaxFloat				= std::numeric_limits<float>::max();
constexpr double	MaxDouble				= std::numeric_limits<double>::max();

constexpr float		EpsilonFloat			= std::numeric_limits<float>::epsilon();
constexpr double	EpsilonDouble			= std::numeric_limits<double>::epsilon();

template <class T> constexpr T PI			= T(3.14159265358979323846264338327950288L);

template <class T> constexpr T PIover180	= PI<T> / T(180);
template <class T> constexpr T l80overPI	= T(180) / PI<T>;



template <class T> inline T ToRadian(T angle_in_degree) { return angle_in_degree * PIover180<T>; }
template <class T> inline T ToDegree(T angle_in_degree) { return angle_in_degree * l80overPI<T>; }

template <class T>
struct RandIntegral
{
private:
	std::random_device rand_dev;
	std::mt19937 generator;
	std::uniform_int_distribution<T>  distribution;

public:
	RandIntegral() { generator = std::mt19937(rand_dev()); distribution = std::uniform_int_distribution<T>(0, std::numeric_limits<T>::max()); }
	~RandIntegral() { }
	T operator()() { return distribution(generator); }
	T operator()(const T& min_val, const T& max_val) { return min_val + (distribution(generator) % (max_val - min_val)); }
};

template <class T>
struct RandReal
{
private:
	T maximum_value;
	std::random_device rand_dev;
	std::mt19937 generator;
	std::uniform_real_distribution<T>  distribution;

public:
	RandReal() { maximum_value = std::numeric_limits<T>::max(); generator = std::mt19937(rand_dev()); distribution = std::uniform_real_distribution<T>(0, maximum_value); }
	~RandReal() { }
	T operator()() { return distribution(generator); }
	T operator()(const T& min_val, const T& max_val) { return (min_val + (distribution(generator) / maximum_value) * (max_val - min_val)); }
};

inline glm::vec3 upVec();
inline glm::vec3 upVec(const glm::quat& quaternion);
inline glm::vec3 upVec(const glm::mat3& mat);
inline glm::vec3 upVec(const glm::mat4& mat);

inline glm::vec3 rightVec();
inline glm::vec3 rightVec(const glm::quat& quaternion);
inline glm::vec3 rightVec(const glm::mat3& mat);
inline glm::vec3 rightVec(const glm::mat4& mat);

inline glm::vec3 frontVec();
inline glm::vec3 frontVec(const glm::quat& quaternion);
inline glm::vec3 frontVec(const glm::mat3& mat);
inline glm::vec3 frontVec(const glm::mat4& mat);

inline glm::quat quat(const glm::mat3& mat);
inline glm::quat quat(const glm::mat4& mat);

inline glm::mat3 mat3(const glm::quat& quaternion);
inline glm::mat3 mat3(const glm::mat4& mat);

inline glm::mat4 mat4(const glm::vec3& position);
inline glm::mat4 mat4(const glm::quat& quaternion);
inline glm::mat4 mat4(const glm::mat3& mat);
inline glm::mat4 mat4(const glm::vec3& position, const glm::mat3& mat);
inline glm::mat4 mat4(const glm::vec3& position, const glm::quat& quaternion);

//##########################################################################

inline glm::vec3 upVec() {
	return glm::vec3(0.0f, 0.0f, 1.0f);
}

inline glm::vec3 upVec(const glm::quat& quaternion)
{
	return normalize(mat3(quaternion)[2]);
}

inline glm::vec3 upVec(const glm::mat3& mat) {
	return glm::normalize(mat[2]);
}

inline glm::vec3 upVec(const glm::mat4& mat) {
	return normalize(mat[2]);
}

inline glm::vec3 rightVec() {
	return glm::vec3(1.0f, 0.0f, 0.0f);
}

inline glm::vec3 rightVec(const glm::quat& quaternion)
{
	return normalize(mat3(quaternion)[0]);
}

inline glm::vec3 rightVec(const glm::mat3& mat) {
	return normalize(mat[0]);
}

inline glm::vec3 rightVec(const glm::mat4& mat) {
	return normalize(mat[0]);
}

inline glm::vec3 frontVec() {
	return glm::vec3(0.0f, 1.0f, 0.0f);
}

inline glm::vec3 frontVec(const glm::quat& quaternion) {
	return normalize(mat3(quaternion)[1]);
}

inline glm::vec3 frontVec(const glm::mat3& mat) {
	return normalize(mat[1]);
}

inline glm::vec3 frontVec(const glm::mat4& mat) {
	return normalize(mat[1]);
}

//################################################################################

inline glm::quat quat(const glm::mat4& mat) {
	return glm::quat_cast(mat);
}

inline glm::quat quat(const glm::mat3& mat) {
	return glm::quat_cast(mat);
}

inline glm::mat3 mat3(const glm::quat& quat) {
	return glm::mat3_cast(quat);
}

inline glm::mat3 mat3(const glm::mat4& mat) {
	glm::mat3 result;
	result[0] = glm::vec3(mat[0]);
	result[1] = glm::vec3(mat[1]);
	result[2] = glm::vec3(mat[2]);
	return result;
}

inline glm::mat4 mat4(const glm::vec3& position) {
	return glm::translate(glm::mat4(1.0f), position);
}

inline glm::mat4 mat4(const glm::quat& quat) {
	return glm::mat4_cast(quat);
}

inline glm::mat4 mat4(const glm::mat3& mat) {
	glm::mat4 result;
	result[0] = glm::vec4(mat[0], 0.0f);
	result[1] = glm::vec4(mat[1], 0.0f);
	result[2] = glm::vec4(mat[2], 0.0f);
	return result;
}

inline glm::mat4 mat4(const glm::vec3& position, const glm::quat& quat) {
	return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(quat);
}

inline glm::mat4 mat4(const glm::vec3& position, const glm::mat3& mat) {
	glm::mat4 result = mat4(mat);
	result[3] = glm::vec4(position, 1.0f);
	return result;
}


#endif // End of CORE_LINEAR_ALGEBRA_H
