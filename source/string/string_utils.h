#include <string>
#include <vector>

std::string ImplodeVector( const std::vector< std::string>& vecstr, const std::string& separator );
std::vector< std::string> ExplodeString( const std::string& str, const std::string& charDelimiters, const bool PreserveDelimiters = false );
std::string Trim( const std::string& trim );
std::string StripString( std::string str, std::string charsToStrip );
std::string StringReplace( std::string result, const std::string& replaceWhat, const std::string& replaceWithWhat);


std::vector<std::string> GetRegexMatches( std::string str, std::string pattern );

#if defined(__APPLE__) || defined(__APPLE_CC__)
void SetGetRegexMatchesBlock( char** (^get_regex_matches_block)(const char* str, const char* pattern ) );
#endif