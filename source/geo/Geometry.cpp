#include "Geometry.h"
#include <algorithm>
#include "../system/FileIO.h"
#include "../string/string_utils.h"


Vertex::Vertex()
{
	colorUV.z = 0;
}
	
Vertex::Vertex( GeoFloat3 position )
{
	this->position = position;
}

Vertex::Vertex( GeoFloat3 position, GeoFloat2 uv )
{
	this->position = position;
	this->colorUV.x = uv.x;
	this->colorUV.y = uv.y;
	this->colorUV.z = 0;
}

GeoVertex::GeoVertex()
{
	perimeter = false;
}

GeoVertex::GeoVertex( GeoVector vector )
{
    perimeter = false;
    vertex.position = vector.ToGeoFloat3();
}

GeoVertex::GeoVertex( const Vertex& vertex )
{
	this->vertex = vertex;
	perimeter = false;
}
	
GeoVertex::GeoVertex( const Vertex& vertex, const bool perimeter )
{
	this->vertex = vertex;
	this->perimeter = perimeter;
}

GeoTriangle::GeoTriangle( const GeoVertex& a, const GeoVertex& b, const GeoVertex& c )
{
	vertices[0] = a;
	vertices[1] = b;
	vertices[2] = c;
}

GeoTriangle::GeoTriangle( const std::triple<GeoVertex, GeoVertex, GeoVertex>& vertices )
{
	this->vertices[0] = vertices.first;
	this->vertices[1] = vertices.second;
	this->vertices[2] = vertices.third;
}

GeoTriangle::GeoTriangle( const GeoVector& a, const GeoVector& b, const GeoVector& c )
{
    this->vertices[0] = GeoVertex(a);
	this->vertices[1] = GeoVertex(b);
	this->vertices[2] = GeoVertex(c);
}

std::triple<GeoVertex, GeoVertex, GeoVertex>  GeoTriangle::GetVertices()
{
	return std::triple<GeoVertex, GeoVertex, GeoVertex>( vertices[0], vertices[1], vertices[2] );
}

GeoVector GeoTriangle::ComputeNormal()
{
	return ( GeoVector( vertices[0].vertex.position ) - GeoVector( vertices[1].vertex.position) ).Cross( GeoVector( vertices[2].vertex.position ) - GeoVector( vertices[0].vertex.position) ).Normalize();
}

bool GeoTriangle::PointInsideXYTriangle( const GeoFloat2& point )
{
    GeoVector A = GeoVector(vertices[0].vertex.position);
    GeoVector B = GeoVector(vertices[1].vertex.position);
    GeoVector C = GeoVector(vertices[2].vertex.position);
    GeoVector P( point.x, point.y );
    
    
    
    // Compute vectors        
    GeoVector v0 = C - A;
    GeoVector v1 = B - A;
    GeoVector v2 = P - A;
    
    // Compute dot products
    float dot00 = v0.Dot(v0);
    float dot01 = v0.Dot(v1);
    float dot02 = v0.Dot(v2);
    float dot11 = v1.Dot(v1);
    float dot12 = v1.Dot(v2);
    
    // Compute barycentric coordinates
    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    
    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v < 1);
}

void GeoTriangle::ReverseWinding()
{
	GeoVertex temp = vertices[0];
	vertices[0] = vertices[2];
	vertices[2] = temp;	
}

GeoQuad::GeoQuad( const GeoVertex& a, const GeoVertex& b, const GeoVertex& c, const GeoVertex& d )
{
	vertices[0] = a;
	vertices[1] = b;
	vertices[2] = c;
	vertices[3] = d;
}

GeoQuad::GeoQuad()
{	
}

GeoQuad GeoQuad::XZQuad( GeoFloat3 a, const float width, const float length )
{
	GeoQuad quad;
	for( unsigned int i = 0; i < 4; i++ )
	{
		quad.vertices[i].vertex.position = a;
		quad.vertices[i].vertex.colorUV.x = 0;
		quad.vertices[i].vertex.colorUV.y = 1;
	}

	quad.vertices[1].vertex.position.x += width;
	quad.vertices[1].vertex.colorUV.x = 1;

	quad.vertices[2].vertex.position.x += width;
	quad.vertices[2].vertex.position.z += length;
	quad.vertices[2].vertex.colorUV.x = 1;
	quad.vertices[2].vertex.colorUV.y = 0;

	quad.vertices[3].vertex.position.z += length;
	quad.vertices[3].vertex.colorUV.y = 0;

	GeoVector normal = quad.Triangulate().first.ComputeNormal();
	for( unsigned int i = 0; i < 4; i++ )
		quad.vertices[i].vertex.normal = normal.ToGeoFloat3();
	return quad;
}

GeoQuad GeoQuad::XYQuad( GeoFloat3 a, const float width, const float height )
{
	GeoQuad quad;
	for( unsigned int i = 0; i < 4; i++ )
	{
		quad.vertices[i].vertex.position = a;
		quad.vertices[i].vertex.colorUV.x = quad.vertices[i].vertex.colorUV.y = 0;
	}

	quad.vertices[1].vertex.position.x += width;
	quad.vertices[1].vertex.colorUV.x = 1;

	quad.vertices[2].vertex.position.x += width;
	quad.vertices[2].vertex.position.y += height;
	quad.vertices[2].vertex.colorUV.x = quad.vertices[2].vertex.colorUV.y = 1;

	quad.vertices[3].vertex.position.y += height;
	quad.vertices[3].vertex.colorUV.y = 1;

	GeoVector normal = quad.Triangulate().first.ComputeNormal();
	for( unsigned int i = 0; i < 4; i++ )
		quad.vertices[i].vertex.normal = normal.ToGeoFloat3();
	return quad;
}

GeoQuad GeoQuad::ZYQuad( GeoFloat3 a, const float length, const float height )
{
	GeoQuad quad;
	for( unsigned int i = 0; i < 4; i++ )
	{
		quad.vertices[i].vertex.position = a;
		quad.vertices[i].vertex.colorUV.x = quad.vertices[i].vertex.colorUV.y = 0;
	}

	quad.vertices[1].vertex.position.z += length;
	quad.vertices[1].vertex.colorUV.x = 1;

	quad.vertices[2].vertex.position.y += height;
	quad.vertices[2].vertex.position.z += length;
	quad.vertices[2].vertex.colorUV.x = quad.vertices[2].vertex.colorUV.y = 1;

	quad.vertices[3].vertex.position.y += height;
	quad.vertices[3].vertex.colorUV.y = 1;
	quad.ReverseWinding();

	GeoVector normal = quad.Triangulate().first.ComputeNormal();
	for( unsigned int i = 0; i < 4; i++ )
		quad.vertices[i].vertex.normal = normal.ToGeoFloat3();
	return quad;
}

std::pair<GeoTriangle,GeoTriangle> GeoQuad::Triangulate()
{
	return std::pair<GeoTriangle,GeoTriangle>(GeoTriangle( vertices[2], vertices[1], vertices[0] ), GeoTriangle( vertices[0], vertices[3], vertices[2] ));
}

void GeoQuad::ReverseWinding()
{
	GeoVertex temp = vertices[2];
	vertices[2] = vertices[0];
	vertices[0] = temp;
}

std::vector<GeoVertex> GeoQuad::GetVertices()
{
	std::vector<GeoVertex> vertices;
	for( unsigned int i = 0; i < 4; i++ )
		vertices.push_back( this->vertices[i] );
	return vertices;
}

Geometry::Geometry()
{
}
	
Geometry::Geometry( const std::vector<Vertex>& vertices )
{
	for( unsigned int i = 0; i < vertices.size(); i++ )
		this->vertices.push_back( GeoVertex( vertices[i] ) );
}

void Geometry::Clear()
{
	vertices.clear();
	indices.clear();
	triangles.clear();
	quads.clear();
	geometries.clear();
}

Geometry& Geometry::operator+=(const Geometry &rhs)
{
	if( geometries.empty() )
	{
		Geometry copy = *this; 	
		Clear();
		geometries.push_back( copy );
	}
	geometries.push_back( rhs );
	return *this;
}
	
const Geometry Geometry::operator+(const Geometry &other) const
{
	Geometry result = *this;     
    result += other;            
    return result;          
}

std::vector<Vertex> Geometry::GetVertices()
{
	std::vector<Vertex> aggregated_vertices;
	if( geometries.empty() )
	{
		for( unsigned int i = 0; i < vertices.size(); i++ )
			aggregated_vertices.push_back( vertices[i].vertex );
		for( unsigned int i = 0; i < triangles.size(); i++ )
		{
			std::triple<GeoVertex, GeoVertex, GeoVertex> verts = triangles[i].GetVertices();
			aggregated_vertices.push_back( verts.first.vertex );
			aggregated_vertices.push_back( verts.second.vertex );
			aggregated_vertices.push_back( verts.third.vertex );
		}
		for( unsigned int i = 0; i < quads.size(); i++ )
		{
			std::pair<GeoTriangle, GeoTriangle> triangles = quads[i].Triangulate();
			std::triple<GeoVertex, GeoVertex, GeoVertex> verts = triangles.first.GetVertices();
			aggregated_vertices.push_back( verts.first.vertex );
			aggregated_vertices.push_back( verts.second.vertex);
			aggregated_vertices.push_back( verts.third.vertex );
			verts = triangles.second.GetVertices();
			aggregated_vertices.push_back( verts.first.vertex );
			aggregated_vertices.push_back( verts.second.vertex);
			aggregated_vertices.push_back( verts.third.vertex );
		}
	}
	else
	for( unsigned int geo_index = 0; geo_index < geometries.size(); geo_index++ )
	{		
		std::vector<Vertex> verts = geometries[geo_index].GetVertices();
		for( unsigned int i = 0; i < verts.size(); i++ )
			aggregated_vertices.push_back( verts[i] );	
	}
	return aggregated_vertices;
}
/*
Geometry Geometry::ExtrudePerimeter( const float distance )
{
	Geometry g = *this;
	
	if( g.geometries.empty() )
	{
		Geometry original = g;
		std::vector<GeoVertex> perimeter_vertices = SelectVertices([](const GeoVertex& vertex, bool *pSelect){*pSelect = vertex.perimeter;});	
		//for( unsigned int i = 0; i < g.vertices.size(); i++ )
		//	if( g.vertices[i].perimeter )
		//		perimeter_vertices.push_back( g.vertices[i] ); 
		
		g = g.ReverseWinding(true);
		if( !perimeter_vertices.empty() )
		for( unsigned int i = 0; i < perimeter_vertices.size() - 1; i++ )
		{			
			GeoVertex a = perimeter_vertices[i];
			GeoVertex b = perimeter_vertices[i+1];
			GeoVertex c = a;
			c.vertex.position.x += original.normal.x * distance;
			c.vertex.position.y += original.normal.y * distance;
			c.vertex.position.z += original.normal.z * distance;
			GeoVertex d = b;
			d.vertex.position.x += original.normal.x * distance;
			d.vertex.position.y += original.normal.y * distance;
			d.vertex.position.z += original.normal.z * distance;

			g.vertices.push_back( a );
			g.vertices.push_back( b );
			g.vertices.push_back( c );
		
			g.vertices.push_back( b );
			g.vertices.push_back( d );
			g.vertices.push_back( c );
		}

		g = g + original.TranslateAlongNormal( distance );
	}
	else
	for( unsigned int i = 0; i < geometries.size(); i++ )
		g.geometries[i] = g.geometries[i].ExtrudePerimeter( distance );
	
	return g;
}
*/

Geometry Geometry::UniformScale( const float scale_factor )
{
	return Scale( scale_factor, scale_factor, scale_factor ); 
}


Geometry Geometry::Scale( const float scale_x, const float scale_y, const float scale_z )
{
#ifdef _WIN32		
	return (*this).ProcessVertices( [&scale_x, &scale_y, &scale_z](GeoVertex* vertex, const GeoFloat3& normal){ 
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
    return (*this).ProcessVertices( ^(GeoVertex* vertex, const GeoFloat3& normal){
#endif
		vertex->vertex.position.x *= scale_x;
		vertex->vertex.position.y *= scale_y;
		vertex->vertex.position.z *= scale_z;
	});
}

Geometry Geometry::Label( const std::string label )
{
	Geometry g = *this;
	g.label = label;
	return g;
}

Geometry Geometry::SetTextureIndex( const unsigned int texture_index )
{
#ifdef _WIN32		
	return (*this).ProcessVertices( [&texture_index](GeoVertex* vertex, const GeoFloat3& normal){ 
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
        return (*this).ProcessVertices( ^(GeoVertex* vertex, const GeoFloat3& normal){
#endif
		vertex->vertex.colorUV.z = (float) texture_index;
	});	
}

Geometry Geometry::Translate( const float translation_x, const float translation_y, const float translation_z )
{
#ifdef _WIN32		
	return (*this).ProcessVertices( [&translation_x, &translation_y, &translation_z](GeoVertex* vertex, const GeoFloat3& normal){ 
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
    return (*this).ProcessVertices( ^(GeoVertex* vertex, const GeoFloat3& normal){
#endif
		vertex->vertex.position.x += translation_x;
		vertex->vertex.position.y += translation_y;
		vertex->vertex.position.z += translation_z;
	});
}
/*
std::vector<GeoVertex> Geometry::SelectVertices( portable_function<void(const GeoVertex& pVertex, bool* pSelect)> select_vertex )
{
	bool select = false;
	std::vector<GeoVertex> selected_vertices;
	if( geometries.empty() )
	{
		//process all triangles
		for( unsigned int i = 0; i < triangles.size(); i++ )
		{
			auto verts = triangles[i].GetVertices();
			select = false;
			select_vertex( verts.first, &select );
			if( select )
				selected_vertices.push_back( verts.first );
			select = false;
			select_vertex( verts.second, &select );
			if( select )
				selected_vertices.push_back( verts.second );
			select = false;
			select_vertex( verts.third, &select );
			if( select )
				selected_vertices.push_back( verts.third );
		}

		//process all quads
		for( unsigned int i = 0; i < quads.size(); i++ )
		{
			auto verts = quads[i].GetVertices();
			select = false;
			select_vertex( verts[0], &select );
			if( select )
				selected_vertices.push_back( verts[0] );
			
			select = false;
			select_vertex( verts[1], &select );
			if( select )
				selected_vertices.push_back( verts[1] );
			
			select = false;
			select_vertex( verts[2], &select );
			if( select )
				selected_vertices.push_back( verts[2] );
			
			select = false;
			select_vertex( verts[3], &select );
			if( select )
				selected_vertices.push_back( verts[3] );
		}
		
		for( unsigned int i = 0; i < vertices.size(); i++ )
		{
			select = false;
			select_vertex( vertices[i], &select );
			if( select )
				selected_vertices.push_back( vertices[i] );
		}
	}
	else
	for( unsigned int i = 0; i < geometries.size(); i++ )
	{
		std::vector<GeoVertex> sel_vertices = geometries[i].SelectVertices( select_vertex );
		for( unsigned int j = 0; j < sel_vertices.size(); j++ )
			selected_vertices.push_back( sel_vertices[j] );
	}
	return selected_vertices;
}
*/


#ifdef _WIN32	
Geometry Geometry::ProcessVertices( std::function<void(GeoVertex* pVertex, const GeoFloat3& normal)> process_vertex )
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
Geometry Geometry::ProcessVertices( void(^process_vertex)(GeoVertex* pVertex, const GeoFloat3& normal) )
#endif
{
	Geometry g = *this;
	
	if( g.geometries.empty() )
	{
		//process all triangles
		for( unsigned int i = 0; i < g.triangles.size(); i++ )
		{
			std::triple<GeoVertex, GeoVertex, GeoVertex> verts = g.triangles[i].GetVertices();
			process_vertex( &verts.first, g.normal );
			process_vertex( &verts.second, g.normal );
			process_vertex( &verts.third, g.normal );
			g.triangles[i] = GeoTriangle( verts );
		}

		//process all quads
		for( unsigned int i = 0; i < g.quads.size(); i++ )
		{
			std::vector<GeoVertex> verts = g.quads[i].GetVertices();
			process_vertex( &verts[0], g.normal );
			process_vertex( &verts[1], g.normal );
			process_vertex( &verts[2], g.normal );
			process_vertex( &verts[3], g.normal );
			g.quads[i] = GeoQuad( verts[0], verts[1], verts[2], verts[3] );
		}
		
		for( unsigned int i = 0; i < g.vertices.size(); i++ )
			process_vertex( &g.vertices[i], g.normal );
	}
	else
	for( unsigned int i = 0; i < geometries.size(); i++ )
		g.geometries[i] = g.geometries[i].ProcessVertices( process_vertex );
	return g;
}




Geometry Geometry::TranslateAlongNormal( const float distance )
{
#ifdef _WIN32		
	return (*this).ProcessVertices( [&distance](GeoVertex* vertex, const GeoFloat3& normal){
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
    return (*this).ProcessVertices( ^(GeoVertex* vertex, const GeoFloat3& normal){
#endif
		vertex->vertex.position.x += normal.x * distance;
		vertex->vertex.position.y += normal.y * distance;
		vertex->vertex.position.z += normal.z * distance;
	});
}

Geometry Geometry::ReverseWinding( const bool invert_normal )
{
	Geometry g = *this;

	if( g.geometries.empty() )
	{
		for( unsigned int tri = 0; tri < g.vertices.size() / 3; tri++ )
		{
			GeoVertex temp = g.vertices[tri*3];
			g.vertices[tri*3] = g.vertices[tri*3+2];
			g.vertices[tri*3+2] = temp;
		}
		for( unsigned int i = 0; i < g.triangles.size(); i++ )
			g.triangles[i].ReverseWinding();
		for( unsigned int i = 0; i < g.quads.size(); i++ )
			g.quads[i].ReverseWinding();
	}
	else
	for( unsigned int i = 0; i < g.geometries.size(); i++ )
		g.geometries[i] = g.geometries[i].ReverseWinding(invert_normal);

	if( invert_normal )
	{
		g.normal.x *= -1;
		g.normal.y *= -1;
		g.normal.z *= -1;
	}
	return g;
}

/*
Geometry Geometry::InvertXZ()
{
	return (*this).ProcessVertices( [](GeoVertex* vertex, const GeoFloat3& normal){ 
		const float temp = vertex->vertex.position.x;
		vertex->vertex.position.x = vertex->vertex.position.z;
		vertex->vertex.position.z = temp;
	});
}
*/

GeometryFactory::GeometryFactory()
{
	generate_indices = false;
	separate_texture_index_per_side = true;
}

GeometryFactory::GeometryFactory( const bool generate_indices )
{
	this->generate_indices = generate_indices;
}

Geometry GeometryFactory::GenerateXZUnitSquare()
{	
	Geometry xz_quad;
	xz_quad.quads.push_back( GeoQuad::XZQuad( GeoFloat3( -0.5f, 0, 0.5f ), 1.0f, -1.0f ) ); 
	xz_quad.normal = GeoFloat3( 0, 1, 0 );
	GeoVector normal =  GeoQuad::XZQuad( GeoFloat3( -0.5f, 0, 0.5f ), 1.0f, -1.0f ).Triangulate().first.ComputeNormal();
	return xz_quad;
}

Geometry GeometryFactory::GenerateYZUnitSquare()
{
	Geometry yz_quad;
	yz_quad.quads.push_back( GeoQuad::ZYQuad( GeoFloat3( 0, -0.5f, -0.5f  ), 1.0f, 1.0f ) ); 
	yz_quad.normal = GeoFloat3( 1, 0, 0 );
	GeoVector normal =  GeoQuad::ZYQuad( GeoFloat3( 0, -0.5f, -0.5f  ), 1.0f, 1.0f ).Triangulate().first.ComputeNormal();
	return yz_quad;
}
	
Geometry GeometryFactory::GenerateXYUnitSquare()
{
	Geometry xy_quad;
	xy_quad.quads.push_back( GeoQuad::XYQuad( GeoFloat3( -0.5f, -0.5f, 0 ), 1.0f, 1.0f ) ); 
	xy_quad.normal = GeoFloat3( 0, 0, 1 );
	GeoVector normal =  GeoQuad::XYQuad( GeoFloat3( -0.5f, -0.5f, 0 ), 1.0f, 1.0f ).Triangulate().first.ComputeNormal();
	return xy_quad;
}
	
Geometry GeometryFactory::GenerateUnitCube()
{
	Geometry unit_cube = GenerateXZUnitSquare().TranslateAlongNormal( 0.5f ).SetTextureIndex( 1 ).Label( "top" ) + GenerateXZUnitSquare().ReverseWinding().TranslateAlongNormal(  0.5f ).Label( "bottom" ) +
		GenerateXYUnitSquare().TranslateAlongNormal( 0.5f ).Label( "front" ).SetTextureIndex( 2 ) + GenerateXYUnitSquare().ReverseWinding().TranslateAlongNormal( 0.5f ).Label( "back" ) +
		GenerateYZUnitSquare().TranslateAlongNormal( 0.5f ).Label( "right" ) + GenerateYZUnitSquare().ReverseWinding().TranslateAlongNormal( 0.5f ).Label( "left" );	
	return unit_cube;
}

Geometry GeometryFactory::GenerateUnitXZCircle()
{
	Geometry xz_circle;
	xz_circle.normal = GeoFloat3( 0, 1, 0 );
	
	const float radius = 1.0f;
	const float inscribed_square_length = radius * sqrtf( 2.0f );
	const float half_inscribed_square_length = 0.5f * inscribed_square_length;
	const float num_wedges = 260;
	const float angle_per_wedge = 360.0f / num_wedges;
	float last_right_angle = 0;
	for( unsigned int i = 0; i < num_wedges; i++ )
	{
		const float left_angle = std::min( -45 + angle_per_wedge * (float) i, last_right_angle );
		float right_angle = -45 + angle_per_wedge * (float) (i+1);
		if( left_angle >= -45 && left_angle < 45 )
		{
			if( right_angle >= 45 )
				right_angle = 45;
		}
		else if( left_angle >= 45 && left_angle < 135 )
		{
			if( right_angle >= 135 )
				right_angle = 135;
		}
		else if( left_angle >= 135 && left_angle < 135+90   )
		{
			if( right_angle >= 135+90 )
				right_angle = 135+90;
		}
		else if( left_angle >= 135+90 && left_angle < 360-45)
		{
			if( right_angle >= 360-45 )
				right_angle = 360-45;
		}
		last_right_angle = right_angle;

		const GeoFloat2 left_top( radius * cos( GeoConvertToRadians(left_angle)), radius  * sin( GeoConvertToRadians(left_angle) ) );
		const GeoFloat2 right_top( radius  * cos( GeoConvertToRadians(right_angle)), radius  * sin( GeoConvertToRadians(right_angle) ) );

		GeoFloat2 left_bottom = left_top;
		GeoFloat2 right_bottom = right_top;
		if( left_angle >= -45 && left_angle < 45  )
			left_bottom.x = right_bottom.x = half_inscribed_square_length;
		else if( left_angle >= 45 && left_angle < 135 )
			left_bottom.y = right_bottom.y = half_inscribed_square_length;
		else if( left_angle >= 135 && left_angle < 135+90 )
			left_bottom.x = right_bottom.x = -half_inscribed_square_length;
		else if( left_angle >= 135+90 && left_angle < 360-45 )
			left_bottom.y = right_bottom.y = -half_inscribed_square_length;
		
		GeoVertex left_bottom_vertex( Vertex(GeoFloat3(left_bottom.x, 0, left_bottom.y)) );
		GeoVertex right_bottom_vertex( Vertex(GeoFloat3(right_bottom.x, 0, right_bottom.y)) );
		GeoVertex left_top_vertex( Vertex(GeoFloat3(left_top.x, 0, left_top.y)), true );
		GeoVertex right_top_vertex( Vertex(GeoFloat3(right_top.x, 0, right_top.y)), true );
		
		if( fabs( right_bottom.x - right_top.x ) < 0.01f && fabs( right_bottom.y - right_top.y ) < 0.01f )
			xz_circle.triangles.push_back( GeoTriangle( left_top_vertex, right_bottom_vertex, left_bottom_vertex ) );
		else
		if( fabs( left_bottom.x - left_top.x ) < 0.01f && fabs( left_bottom.y - left_top.y ) < 0.01f )
			xz_circle.triangles.push_back( GeoTriangle( right_top_vertex, right_bottom_vertex, left_bottom_vertex ) );
		else
			xz_circle.quads.push_back( GeoQuad( left_bottom_vertex, right_bottom_vertex, right_top_vertex, left_top_vertex  ) );
	}
	
	xz_circle = xz_circle + GenerateXZUnitSquare().UniformScale( sqrtf(2) );	
	
	#ifdef _WIN32	
	xz_circle = xz_circle.ProcessVertices( [this, &radius](GeoVertex* vertex, const GeoFloat3& normal){ 
    #endif
    #if defined(__APPLE__) || defined(__APPLE_CC__)  
    xz_circle = xz_circle.ProcessVertices( ^(GeoVertex* vertex, const GeoFloat3& normal){
    #endif
		vertex->vertex.colorUV.x = (vertex->vertex.position.x) / (2*radius );
		vertex->vertex.colorUV.y = (-vertex->vertex.position.z) / (2*radius );
	});	
	return xz_circle;
}

Geometry GeometryFactory::GenerateUnitCylinder()
{
	const float height = 1;
	const unsigned int num_wedges = 260;
	
	Geometry cylinder;
	cylinder.normal = GeoFloat3( 0, 1, 0 );
	for( unsigned int i = 0; i < num_wedges; i++ )
	{
		const float left_angle = GeoConvertToRadians( i * 360.0f / (float) num_wedges );
		const float right_angle = GeoConvertToRadians( (i+1) * 360.0f / (float) num_wedges );
		GeoVertex a( Vertex( GeoFloat3( cos(left_angle), -height / 2.0f, sin(left_angle) ), GeoFloat2( 50*left_angle / GEO_2PI, 0 ) ), true );
		GeoVertex b( Vertex( GeoFloat3( cos(right_angle), -height / 2.0f, sin(right_angle) ), GeoFloat2( 50*right_angle / GEO_2PI, 0 ) ), true );
		GeoVertex c( Vertex( GeoFloat3( cos(right_angle), height / 2.0f, sin(right_angle) ), GeoFloat2( 50*right_angle / GEO_2PI, 1 ) ), true );
		GeoVertex d( Vertex( GeoFloat3( cos(left_angle), height / 2.0f, sin(left_angle) ), GeoFloat2( 50*left_angle / GEO_2PI, 1 ) ), true );
		cylinder.quads.push_back( GeoQuad( d, c, b, a ) );
	}
	return cylinder;
}

Geometry GeometryFactory::GenerateUnitCappedCylinder()
{
	return GenerateUnitCylinder().Scale(0.999f, 1, 0.999f ) + GenerateUnitXZCircle().TranslateAlongNormal( 0.5f ).SetTextureIndex(1) + GenerateUnitXZCircle().TranslateAlongNormal( -0.5f ).ReverseWinding().SetTextureIndex(2);
}

Geometry GeometryFactory::GenerateUnitSphere()
{
	const float radius = 1.0f;
	const float num_iterations = 4;
	unsigned int i,it;
	float a;
	std::vector<GeoFloat3> p;
	p.push_back( GeoFloat3( 0.0,0.0,1.0 ) );
	p.push_back( GeoFloat3( 0.0,0.0,-1.0 ) );
	p.push_back( GeoFloat3( -1.0,-1.0,0.0 ) );
	p.push_back( GeoFloat3( 1.0,-1.0,0.0 ) );
	p.push_back( GeoFloat3( 1.0,1.0,0.0 ) );
	p.push_back( GeoFloat3( -1.0,1.0,0.0 ) );

	unsigned int nt = 0,ntold;

	/* Create the level 0 object */
	a = 1.0f / sqrtf(2.0f);
	for (i = 0; i < 6; i++) {
		p[i].x *= a;
		p[i].y *= a;
	}

	std::vector< std::vector<GeoFloat3> > f;

	std::vector<GeoFloat3> face;
	face.push_back( p[0] );
	face.push_back( p[3] );
	face.push_back( p[4] );
	f.push_back(face);

	face.clear();
	face.push_back( p[0] );
	face.push_back( p[4] );
	face.push_back( p[5] );
	f.push_back(face);

	face.clear();
	face.push_back( p[0] );
	face.push_back( p[5] );
	face.push_back( p[2] );
	f.push_back(face);

	face.clear();
	face.push_back( p[0] );
	face.push_back( p[2] );
	face.push_back( p[3] );
	f.push_back(face);


	face.clear();
	face.push_back( p[1] );
	face.push_back( p[4] );
	face.push_back( p[3] );
	f.push_back(face);


	face.clear();
	face.push_back( p[1] );
	face.push_back( p[5] );
	face.push_back( p[4] );
	f.push_back(face);	


	face.clear();
	face.push_back( p[1] );
	face.push_back( p[2] );
	face.push_back( p[5] );
	f.push_back(face);	


	face.clear();
	face.push_back( p[1] );
	face.push_back( p[3] );
	face.push_back( p[2] );
	f.push_back(face);	
	nt = 8;

	/* Bisect each edge and move to the surface of a unit sphere */
	for (it = 0; it < num_iterations; it++) {
		ntold = nt;
		for (i = 0; i < ntold; i++) {
			GeoFloat3 pa, pb, pc;
			pa.x = (f[i][0].x + f[i][1].x) / 2.0f;
			pa.y = (f[i][0].y + f[i][1].y) / 2.0f;
			pa.z = (f[i][0].z + f[i][1].z) / 2.0f;
			pb.x = (f[i][1].x + f[i][2].x) / 2.0f;
			pb.y = (f[i][1].y + f[i][2].y) / 2.0f;
			pb.z = (f[i][1].z + f[i][2].z) / 2.0f;
			pc.x = (f[i][2].x + f[i][0].x) / 2.0f;
			pc.y = (f[i][2].y + f[i][0].y) / 2.0f;
			pc.z = (f[i][2].z + f[i][0].z) / 2.0f;

			pa = GeoVector( pa ).Normalize().ToGeoFloat3();
			pb = GeoVector( pb ).Normalize().ToGeoFloat3();
			pc = GeoVector( pc ).Normalize().ToGeoFloat3();

			face.clear();
			face.push_back( f[i][0] );
			face.push_back( pa );
			face.push_back( pc );
			f.push_back(face);
			nt++;

			face.clear();
			face.push_back( pa );
			face.push_back( f[i][1] );
			face.push_back( pb );
			f.push_back(face);
			nt++;

			face.clear();
			face.push_back( pb );
			face.push_back( f[i][2] );
			face.push_back( pc );
			f.push_back(face);
			nt++;

			f[i][0] = pa;
			f[i][1] = pb;
			f[i][2] = pc;
		}
	}

	Geometry sphere;
	std::vector<Vertex> verts;
	for (unsigned int i = 0; i < f.size(); i++)
	{
		GeoVertex triangle_verts[3];
		for (unsigned int j = 0; j < 3; j++)
		{
			triangle_verts[j].vertex.position = triangle_verts[j].vertex.normal = f[i][j];
			triangle_verts[j].vertex.position.x *= radius * 0.5f;
			triangle_verts[j].vertex.position.y *= radius * 0.5f;
			triangle_verts[j].vertex.position.z *= radius * 0.5f;            
			triangle_verts[j].vertex.colorUV.x = atan2f( f[i][j].x, f[ i][j].z ) / ( 2.0f * 3.14f) + 0.5f;
			triangle_verts[j].vertex.colorUV.y = asinf( f[i][j].y / 3.14f + 0.5f );
		}

		sphere.triangles.push_back( GeoTriangle( triangle_verts[0], triangle_verts[1], triangle_verts[2] ) );
	}
	return sphere;
}
                                          
  
Geometry GeometryFactory::FromSimpleModel( const std::string& filepath )
{
    std::string data = File::ReadAllText( filepath );
    std::vector< std::string > data_lines = ExplodeString( data, "\n\r" );
    std::vector< GeoVertex > vertices;
    for( int i = 0; i < data_lines.size(); i++ )
    {
        const std::string line = StripString( data_lines[i], " \n\t\r" );
        if( line.size() > 0 && '#' != line[0] )
        {
            GeoVertex vertex;            
            //  printf( "line %i is: %s\n", i, line.c_str() );
            std::vector< std::string > components = ExplodeString( line, "," );
            for( int j = 0; j < components.size(); j++ )
            {
                // printf( "\tcomponent:%s\n", components[j].c_str() );
                if( "p" == components[j] || "n" == components[j] )
                {
                    GeoFloat3 float3;
                    float3.x = StringToNumber<float>( components[j+1] );
                    float3.y = StringToNumber<float>( components[j+2] );
                    float3.z = StringToNumber<float>( components[j+3] );
                    
                    if( "p" == components[j] )
                        vertex.vertex.position = float3;
                    else if( "n" == components[j] )
                        vertex.vertex.normal = float3;
                    
                }
            }
            vertices.push_back(vertex);
        }
    }
    
    Geometry geometry;
    for( int tri = 0; tri < vertices.size() / 3; tri++ )
    {
        GeoTriangle triangle( vertices[3*tri], vertices[3*tri+1], vertices[3*tri+2]);
        triangle.ReverseWinding();
        geometry.triangles.push_back(triangle);
    }
                                              
    return geometry;
}


