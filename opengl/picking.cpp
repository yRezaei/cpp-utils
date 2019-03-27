#include "../rendering/gl_utility.hpp"
#include "picking.h"


PickingSystem::PickingSystem()
{
	id_ = 0;
	rgbTexture = 0;
	depthTexture = 0;
}

PickingSystem::~PickingSystem()
{
	destroy();
}


void PickingSystem::setFrameSize(unsigned int WindowWidth, unsigned int WindowHeight)
{
	destroy();

	glGenFramebuffers(1, &id_);
	glBindFramebuffer(GL_FRAMEBUFFER, id_);

	// Create the texture object for the primitive information buffer
	glGenTextures(1, &rgbTexture);
	glBindTexture(GL_TEXTURE_2D, rgbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_UNSIGNED_SHORT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rgbTexture, 0);

	// Create the texture object for the depth buffer
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PickingSystem::destroy() {
	if (id_ != 0)
		glDeleteBuffers(1, &id_);
	if (rgbTexture != 0)
		glDeleteBuffers(1, &rgbTexture);
	if (depthTexture != 0)
		glDeleteBuffers(1, &depthTexture);
}

void PickingSystem::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id_);
	gl::clearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void PickingSystem::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


std::array<uint16_t, 3> PickingSystem::readPixel(unsigned int x, unsigned int y)
{
	glBindFramebuffer(GL_FRAMEBUFFER, id_);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	std::array<uint16_t, 3> pixel = { 0, 0, 0 };
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixel[0]);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	pixel[2] = (pixel[2] - 1) * 3;
	return pixel;
}

