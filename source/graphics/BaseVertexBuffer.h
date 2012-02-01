#pragma once
#include "../geo/Geometry.h"

namespace PrimitiveType
{
	enum PRIMITIVE_TYPE
	{
		POINTLIST	= 1,
		LINELIST	= 2,
		LINESTRIP	= 3,
		TRIANGLELIST	= 4,
		TRIANGLESTRIP	= 5
	};
}

class BaseVertexBuffer
{
public:
	BaseVertexBuffer();
	void SetPrimitiveType( const PrimitiveType::PRIMITIVE_TYPE primitive_type );
	PrimitiveType::PRIMITIVE_TYPE GetPrimitiveType();
	unsigned int GetNumVertices();
	std::vector<Vertex> GetVertices();
protected:
	unsigned int num_vertices;
	std::vector<Vertex> vertices;
	PrimitiveType::PRIMITIVE_TYPE primitive_type;
};