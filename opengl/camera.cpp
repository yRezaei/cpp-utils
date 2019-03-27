#include "camera.h"

Camera::Camera()
{
	init(glm::vec3(0.0f), glm::vec3(0, 0, -1), glm::ivec2(800,600), 0.0f, 100000.0f, PERSPECTIVE);
}

Camera::Camera(const glm::vec3 & new_position, const glm::vec3 & new_target, const glm::ivec2 & new_viewport, float new_near, float new_far, ProjectionMode new_projection_mode)
{
	init(new_position, new_target, new_viewport, new_near, new_far, new_projection_mode);
}


Camera::~Camera()
{
}

void Camera::init(const glm::vec3 & new_position, const glm::vec3 & new_target, const glm::ivec2 & new_viewport, float new_near, float new_far, ProjectionMode new_projection_mode)
{
	position_ = new_position;
	target_ = new_target;
	near_ = new_near;
	far_ = new_far;
	viewport_ = new_viewport;
	projection_mode_ = new_projection_mode;
	set_functions();
}

void Camera::set_functions()
{
	switch (projection_mode_)
	{
	case Camera::PERSPECTIVE:
	{
		fov_ = 75.0f;
		zoom_ratio_ = 0.5f;

		projection = [&]() { 
			return glm::perspective(ToRadian(fov_), viewport_.x / (float)viewport_.y, near_, far_);
		};
		
		translate = [&](const glm::vec3& offset) { 
			target_ += offset; 
			position_ += offset;
		};
		
		zoom_in = [&]() { 
			fov_ -= zoom_ratio_; 
			if (fov_ > 179.0f)
				fov_ = 179.0f;
			else if (fov_ < 1.0f)
				fov_ = 1.0f;
		};
		
		zoom_out = [&]() { 
			fov_ += zoom_ratio_;
			if (fov_ > 179.0f)
				fov_ = 179.0f;
			else if (fov_ < 1.0f)
				fov_ = 1.0f;
		};
		break;
	}
	case Camera::ORTHOGRAPHIC:
	{
		zoom_ratio_ = 0.5f;

		projection = [&]() { 
			glm::vec2 v(viewport_.x * zoom_ratio_, viewport_.y * zoom_ratio_);
			return glm::ortho((float)-v.x, (float)v.x, (float)-v.y, (float)v.y);
			//return glm::ortho(0.0f, static_cast<float>(viewport_.x), 0.0f, static_cast<float>(viewport_.y));
		};

		translate = [&](const glm::vec3& offset) { 
			target_ += offset;
			position_ += offset;
		};

		zoom_in = [&]() { 
			zoom_ratio_ -= 0.005f;
			if (zoom_ratio_ > 1.0f)
				zoom_ratio_ = 1.0f;
			else if (zoom_ratio_ < 0.0f)
				zoom_ratio_ = 0.0f;
		};

		zoom_out = [&]() { 
			zoom_ratio_ += 0.005f;
			if (zoom_ratio_ > 1.0f)
				zoom_ratio_ = 1.0f;
			else if (zoom_ratio_ < 0.0f)
				zoom_ratio_ = 0.0f;
		};
		break;
	}
	default:
		break;
	}
}

void Camera::position(const glm::vec3 & new_position)
{
	position_ = new_position;
	target_ = glm::vec3(new_position.x, new_position.y, 0);
}

const glm::vec3 & Camera::position() const
{
	return position_;
}

void Camera::target(const glm::vec3 & new_target)
{
	target_ = new_target;
}

const glm::vec3 & Camera::target() const
{
	return target_;
}

const glm::ivec2 & Camera::viewport() const
{
	return viewport_;
}

void Camera::viewport(const glm::ivec2 & new_veiwport)
{
	viewport_ = new_veiwport;
}

float Camera::aspect_ratio()
{
	return viewport_.x / (float)viewport_.y;
}

void Camera::clipping_plane(float near, float far)
{
	near_ = near;
	far_ = far;
}

void Camera::near_plane(float near)
{
	near_ = near;
}

void Camera::far_plane(float far)
{
	far_ = far;
}

glm::mat4 Camera::view()
{
	return glm::lookAt(position_, target_, frontVec());
}
