#include "Nebula.h"
#include "../system/FileIO.h"
#include <random>

Nebula::Nebula()
{	
//nebula functionality currently only implemented for windows
//TODO: port for non windows functionality
#ifdef _WIN32	
	const unsigned int num_quads = 12;
	

	std::tr1::mt19937 eng;  
	std::tr1::normal_distribution<float> dist(0, 0.25f);  

	std::vector<Texture> textures;
	std::vector<std::string> nebula_files = Directory::GetFiles( "assets\\nebula" );
	for( unsigned int i = 0; i < nebula_files.size(); i++ )
		textures.push_back( Texture( nebula_files[i] ) );

	for( unsigned int i = 0; i < num_quads; i++ )
	{
		const float random_angle = (float)(rand() % 360);
		//GeoVector random_rotation_axis( (float)(rand() % 1000), (float)(rand() % 1000), (float)(rand() % 1000) );
		GeoVector random_rotation_axis( 0, 1, 0 );
		random_rotation_axis.Normalize();
		
		RenderableObject quad( textures[rand() % textures.size()], GeometryFactory().GenerateXYUnitSquare() );
		quad.RotateInObjectspace( random_rotation_axis.ToXMFloat3(), random_angle );
		//quad.Scale( 1 + dist(eng), 1 + dist(eng), 1 + dist(eng) );
		quad.Translate( 0, 0, 2 );
		quad.Translate( dist(eng), dist(eng), dist(eng) );
		quad.SetTechnique( "AdditiveParticle" );
		quads.push_back( quad );
	}
#endif
}

bool Nebula::Render()
{
	pGraphicsDevice->GetStateManager().SetParticleRendering();
	pGraphicsDevice->GetStateManager().Lock();
	for( unsigned int i = 0; i < quads.size(); i++ )
	{
		//quads[i].SetIdentity();
		//quads[i].Scale( 0.75f, 0.75f, 0.75f );
		//quads[i].Translate( 0, 0, 2 );
		quads[i].Render();
	}
	pGraphicsDevice->GetStateManager().Unlock();
	pGraphicsDevice->GetStateManager().DisableBlending();
	return true;
}