#include "BaseVertexBuffer.h"

BaseVertexBuffer::BaseVertexBuffer()
{
	primitive_type = PrimitiveType::TRIANGLELIST;
}

void BaseVertexBuffer::SetPrimitiveType( const PrimitiveType::PRIMITIVE_TYPE primitive_type )
{
	this->primitive_type = primitive_type;
}
	
PrimitiveType::PRIMITIVE_TYPE BaseVertexBuffer::GetPrimitiveType()
{
	return primitive_type;		
}

unsigned int BaseVertexBuffer::GetNumVertices()
{
	return num_vertices;
}

std::vector<Vertex> BaseVertexBuffer::GetVertices()
{
	return vertices;
}