#pragma once

#include "GeoFloat.h"
#include "GeoVector.h"
#include "GeoMatrix.h"
#include "GeoQuaternion.h"
#include <vector>
#include <string>
#include <math.h>
#include "../std/functional.h"
#include "../std/triple.h"

struct Vertex
{
    GeoFloat3 position;
	GeoFloat3 colorUV;
	GeoFloat3 normal;
	GeoFloat3 tangent;
	GeoFloat3 bitangent;
	Vertex();
	Vertex( GeoFloat3 position );
	Vertex( GeoFloat3 position, GeoFloat2 uv );
};


struct GeoVertex
{
	GeoVertex();
    GeoVertex( GeoVector vector );
	GeoVertex( const Vertex& vertex );
	GeoVertex( const Vertex& vertex, const bool perimeter );
	Vertex vertex;
	bool perimeter;
};

class GeoTriangle
{
public:
	GeoTriangle( const GeoVertex& a, const GeoVertex& b, const GeoVertex& c );
    GeoTriangle( const GeoVector& a, const GeoVector& b, const GeoVector& c );
	GeoTriangle( const std::triple<GeoVertex, GeoVertex, GeoVertex>& vertices );
	std::triple<GeoVertex, GeoVertex, GeoVertex>  GetVertices();
    bool PointInsideXYTriangle( const GeoFloat2& point );
	GeoVector ComputeNormal();
	void ReverseWinding();
private:
	GeoVertex vertices[3];
};

class GeoQuad
{
public:
	static GeoQuad XZQuad( GeoFloat3 a, const float width, const float length );
	static GeoQuad XYQuad( GeoFloat3 a, const float width, const float height );
	static GeoQuad ZYQuad( GeoFloat3 a, const float length, const float height );
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

//	Geometry ExtrudePerimeter( const float distance );

#ifdef _WIN32	
	Geometry ProcessVertices( std::function<void(GeoVertex* pVertex, const GeoFloat3& normal)> process_vertex );
#endif
    
#if defined(__APPLE__) || defined(__APPLE_CC__)  
    Geometry ProcessVertices( void(^process_vertex)(GeoVertex* pVertex, const GeoFloat3& normal) );
#endif

//	std::vector<GeoVertex> SelectVertices( portable_function<void(const GeoVertex& pVertex, bool* pSelect)> select_vertex );

	Geometry Label( const std::string label );

	//Geometry InvertXZ();

	Geometry ReverseWinding( const bool invert_normal = true );
	std::vector<Vertex> GetVertices();
	

	std::pair<std::vector<Vertex>, std::vector<unsigned int>  > GetVerticesAndIndices();

private:
	std::vector<GeoVertex> vertices;
	std::vector<GeoTriangle> triangles;
	std::vector<GeoQuad> quads;
	std::vector<Geometry> geometries;
	std::vector<unsigned int> indices;
	GeoFloat3 normal;
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