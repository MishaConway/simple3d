#include "string_utils.h"
#include <sstream>

 std::string ImplodeVector( const std::vector< std::string>& vecstr, const std::string& separator )
{
	std::string ImplodedString;
	for( size_t i = 0; i < vecstr.size(); i++ )
	{
		ImplodedString += vecstr[i];
		if( i < vecstr.size() - 1 )
			ImplodedString += separator;
	}

	return ImplodedString;
}

std::vector< std::string> ExplodeString( const std::string& str, const std::string& charDelimiters, const bool PreserveDelimiters  )
{
	std::vector<std::string> words;

	std::string currentWord;

	for( size_t i = 0; i < str.size(); i++ )
	{		
		bool hitToken = false;
		for( size_t j = 0; j < charDelimiters.size(); j++ )
			if( str[i] == charDelimiters[j] )
			{
				hitToken = true;
				break;
			}

		if( hitToken )
		{
			//if( currentWord.size() > 0 )
			{
				words.push_back( currentWord );
				if( PreserveDelimiters )
				{
					std::stringstream stream;
					stream << str[i];
					words.push_back( stream.str() );
				}
				currentWord.clear();
			}
		}
		else
			currentWord += str[i];
	}

	//if( currentWord.size() > 0 )
		words.push_back( currentWord ); 

	return words;
}

std::string Trim( const std::string& str )
{
	std::string s = str;
	std::stringstream trimmer;
	trimmer << s;
	s.clear();
	trimmer >> s;
	return s;
}