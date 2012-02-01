#include <string>
#include <vector>

std::string ImplodeVector( const std::vector< std::string>& vecstr, const std::string& separator );
std::vector< std::string> ExplodeString( const std::string& str, const std::string& charDelimiters, const bool PreserveDelimiters = false );
std::string Trim( const std::string& trim );