#pragma once
#include "RenderableObject.h"
#include "Quad.h"


class Nebula : public RenderableObject
{
public:
	Nebula();
	virtual bool Render();	
private:
	std::vector<RenderableObject> quads;	
};
