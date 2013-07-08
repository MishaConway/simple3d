#include "../Scene.h"

class SpriteScene : public Scene
{
public:
	SpriteScene();
	SpriteScene( WINDOW_TYPE hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const std::string& root_shader_path, const std::string& root_assets_path );
};