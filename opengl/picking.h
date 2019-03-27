#ifndef PICKING_H
#define	PICKING_H

#include "tarja/core.hpp"

class PickingSystem
{
public:
	PickingSystem();
	~PickingSystem();

	void setFrameSize(unsigned int WindowWidth, unsigned int WindowHeight);
	void destroy();
	void bind();
	void unbind();
	std::array<uint16_t, 3> readPixel(unsigned int x, unsigned int y);

private:
	unsigned int id_;
	unsigned int rgbTexture;
	unsigned int depthTexture;
};


#endif	//!PICKING_SYSTEM_H

