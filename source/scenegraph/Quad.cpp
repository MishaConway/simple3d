#include "Quad.h"
#include "../geo/Geometry.h"
VertexBuffer Quad::unit_square;

Quad::Quad( Texture t) 
{
	tex = t;
}

VertexBuffer& Quad::GetVertexBuffer()
{
	if( !unit_square.IsValid() )
		unit_square = VertexBuffer( GeometryFactory().GenerateXYUnitSquare().GetVertices() );
	return unit_square;
}