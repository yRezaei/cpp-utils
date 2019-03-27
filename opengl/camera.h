#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "tarja/core.hpp"
#include "tarja/mathematics.hpp"


class Camera
{
public:
	enum ProjectionMode 
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

private:
	float fov_;
	float zoom_ratio_;
	float near_;
	float far_;
	glm::ivec2 viewport_;
	glm::vec3 position_;
	glm::vec3 target_;
	ProjectionMode projection_mode_;

public:
	Camera();
	Camera(const glm::vec3& position, const glm::vec3& target, const glm::ivec2& viewport, float near, float far, ProjectionMode projection_mode);
	~Camera();

private:
	void init(const glm::vec3& position, const glm::vec3& target, const glm::ivec2& viewport, float near, float far, ProjectionMode projection_mode);
	void set_functions();

public:
	std::function<glm::mat4(void)> projection;
	std::function<void(const glm::vec3&)> translate;
	std::function<void(void)> zoom_in;
	std::function<void(void)> zoom_out;

	void position(const glm::vec3& new_position);
	const glm::vec3& position() const;

	void target(const glm::vec3& new_target);
	const glm::vec3& target() const;

	const glm::ivec2& viewport() const;
	void viewport(const glm::ivec2& new_veiwport);

	float aspect_ratio();
	void clipping_plane(float near, float far);
	void near_plane(float near);
	void far_plane(float far);
	glm::mat4 view();

};


#endif // !CAMERA_H

