#include "../Scene.h"

class SimpleSceneUsingBasicShaderTechnique : public Scene
{
public:
	SimpleSceneUsingBasicShaderTechnique();
	SimpleSceneUsingBasicShaderTechnique( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z ); 
};