#include "BaseEffect.h"
#include "../system/FileIO.h"
#include "../string/string_utils.h"

BaseEffect::BaseEffect()
{
	
}

BaseEffect::BaseEffect( const std::string& effect_path )
{
printf("went here...\n" );
}

std::vector< std::pair< std::string, std::string > > BaseEffect::ParseTechniqueFile( const std::string& technique_file )
{
	std::vector< std::pair< std::string, std::string > > techniques;

	std::vector<std::string> effect_path_parts = ExplodeString( technique_file, "/\\");
	effect_path_parts.pop_back();
	const std::string effect_directory = ImplodeVector( effect_path_parts, "/" ) + "/";	
	const std::string source = File::ReadAllText( technique_file );
    
    std::vector<std::string> regex_matches = GetRegexMatches( source, "technique([^}]+)" );
    for( unsigned int i = 0; i < regex_matches.size(); i++ )
    {
        const std::vector<std::string> split = ExplodeString( StringReplace( regex_matches[i], "technique", "" ), "{" );
        const std::string technique_name = Trim(split[0]);
        const std::vector<std::string> shaders = ExplodeString( split[1], "," );
        const std::string vertex_shader = Trim(shaders[0]);
        const std::string pixel_shader = Trim( shaders[1]);

		techniques.push_back( std::pair< std::string, std::string >( vertex_shader, pixel_shader ) );
	}

	return techniques;
}


bool BaseEffect::SetColor( const std::string& variable_name, Color color )
{
    SetFloatArray( variable_name, GeoVector( ((float)color.r) / 255.0f, ((float)color.g) / 255.0f, ((float)color.b) / 255.0f, ((float)color.a) / 255.0f ) );
    return true;
}

