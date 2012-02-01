#pragma once
#include "RenderableObject.h"

class Quad : public RenderableObject
{
public:
	Quad( Texture t );
protected:
	virtual VertexBuffer& GetVertexBuffer();
private:
	static VertexBuffer unit_square;
};
