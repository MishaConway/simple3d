#pragma once

#include "GeoFloat.h"
#include "GeoVector.h"
#include "GeoMatrix.h"
#include <vector>
#include <string>
#include <math.h>
#include <tuple> 
#include "../std/functional.h"
#include "../std/triple.h"

struct Vertex
{
    XMFLOAT3 position;
	XMFLOAT3 colorUV;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 bitangent;
	Vertex();
	Vertex( XMFLOAT3 position );
	Vertex( XMFLOAT3 position, XMFLOAT2 uv );
};


struct GeoVertex
{
	GeoVertex();
	GeoVertex( const Vertex& vertex );
	GeoVertex( const Vertex& vertex, const bool perimeter );
	Vertex vertex;
	bool perimeter;
};

class GeoTriangle
{
public:
	GeoTriangle( const GeoVertex& a, const GeoVertex& b, const GeoVertex& c );
	GeoTriangle( const std::triple<GeoVertex, GeoVertex, GeoVertex>& vertices );
	std::triple<GeoVertex, GeoVertex, GeoVertex>  GetVertices();
	GeoVector ComputeNormal();
	void ReverseWinding();
private:
	GeoVertex vertices[3];
};

class GeoQuad
{
public:
	static GeoQuad XZQuad( XMFLOAT3& a, const float width, const float length );
	static GeoQuad XYQuad( XMFLOAT3& a, const float width, const float height );
	static GeoQuad ZYQuad( XMFLOAT3& a, const float length, const float height );
	GeoQuad();
	GeoQuad( const GeoVertex& a, const GeoVertex& b, const GeoVertex& c, const GeoVertex& d );
	std::pair<GeoTriangle,GeoTriangle> Triangulate();
	std::vector<GeoVertex> GetVertices();
	void ReverseWinding();
	std::vector<GeoQuad> Tesselate();
private:
	GeoVertex vertices[4];
};

class Geometry
{
friend class GeometryFactory;
public:
	Geometry();
	Geometry( const std::vector<Vertex>& vertices );
	Geometry& operator+=(const Geometry &rhs);
	const Geometry operator+(const Geometry &other) const;
	void Clear();
	Geometry UniformScale( const float scale_factor );
	Geometry Scale( const float scale_x, const float scale_y, const float scale_z );
	Geometry Translate( const float translation_x, const float translation_y, const float translation_z );
	Geometry TranslateAlongNormal( const float distance );

	Geometry SetTextureIndex( const unsigned int texture_index );

	Geometry ExtrudePerimeter( const float distance );

	Geometry ProcessVertices( std::function<void(GeoVertex* pVertex, const XMFLOAT3& normal)> process_vertex );

	std::vector<GeoVertex> SelectVertices( std::function<void(const GeoVertex& pVertex, bool* pSelect)> select_vertex );

	Geometry Label( const std::string label );

	Geometry InvertXZ();

	Geometry ReverseWinding( const bool invert_normal = true );
	std::vector<Vertex> GetVertices();
	

	std::pair<std::vector<Vertex>, std::vector<unsigned int>> GetVerticesAndIndices();

private:
	std::vector<GeoVertex> vertices;
	std::vector<GeoTriangle> triangles;
	std::vector<GeoQuad> quads;
	std::vector<Geometry> geometries;
	std::vector<unsigned int> indices;
	XMFLOAT3 normal;
	std::string label;
};

class GeometryFactory
{
public:
	GeometryFactory();
	GeometryFactory( const bool generate_indices );
	Geometry GenerateXZUnitSquare();
	Geometry GenerateXYUnitSquare();
	Geometry GenerateYZUnitSquare();
	Geometry GenerateUnitCube();
	Geometry GeneratedRoundedUnitCube();
	Geometry GenerateUnitCappedCylinder();
	Geometry GenerateUnitCylinder();
	Geometry GenerateUnitSphere();
	Geometry GenerateUnitXZCircle();
private:
	bool generate_indices;	
	bool separate_texture_index_per_side;
};