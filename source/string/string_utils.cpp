#include "string_utils.h"
#include <sstream>

#if defined(__APPLE__) || defined(__APPLE_CC__)
char** (^get_regex_matches_block)(const char* str, const char* pattern ); 
#endif

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

std::string StripString( std::string str, std::string charsToStrip )
{
	std::string newStr;
	for( int i = 0; i < (int)str.size(); i++ )
	{
		bool foundToken = false;
		for( int j = 0; j < (int)charsToStrip.size(); j++ )
			if( str[i] == charsToStrip[j] )
			{
				foundToken = true;
				break;
			}

		if( !foundToken )
			newStr += str[i];
	}

	return newStr;
}

std::string StringReplace( std::string result, 
  const std::string& replaceWhat, 
  const std::string& replaceWithWhat)
{
  while(1)
  {
    const int pos = result.find(replaceWhat);
    if (pos==-1) break;
    result.replace(pos,replaceWhat.size(),replaceWithWhat);
  }
  return result;
}


std::vector<std::string> GetRegexMatches( std::string str, std::string _pattern )
{
    std::vector<std::string> regex_matches;
    
    #ifdef _WIN32	
    const std::tr1::regex pattern(_pattern);
	const std::tr1::sregex_token_iterator end;
	for (std::tr1::sregex_token_iterator i(str.begin(), str.end(), pattern); i != end; ++i)
        regex_matches.push_back( (*i).str() );
    #endif
    
    #if defined(__APPLE__) || defined(__APPLE_CC__)
    char** pMatches = get_regex_matches_block( str.c_str(), _pattern.c_str() );
    while( *pMatches )
    {
       // std::string s = std::string(*pMatches );
      //  printf( "it is %s\n", s.c_str() );
        regex_matches.push_back( std::string(*pMatches++) );
    }
    #endif
    
    
    return regex_matches;
}

#if defined(__APPLE__) || defined(__APPLE_CC__)
void SetGetRegexMatchesBlock( char** (^_get_regex_matches_block)(const char* str, const char* pattern )  )
{
    get_regex_matches_block = _get_regex_matches_block;
}
#endif



