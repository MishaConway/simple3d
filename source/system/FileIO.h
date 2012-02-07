#pragma once

#include <vector>
#include <string>
#include <fstream>

class Directory
{
public:
	static bool Exists( const std::string& path );	
	static std::string GetWorkingDirectory(); 
	static std::vector<std::string> GetFiles( const std::string& path, const bool return_absolute_paths = true );	
};

class File
{
public:
	static bool Exists( const std::string& path );
	static std::string ReadAllText( const std::string& path );
#if defined(__APPLE__) || defined(__APPLE_CC__)
    static void SetReadAllTextBlock( const char* (^read_all_text_block)(const char* path) );
#endif
private:
#if defined(__APPLE__) || defined(__APPLE_CC__)
    static const char* (^read_all_text_block)(const char* path); 
#endif
};