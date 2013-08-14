#pragma once

#define WINDOWS_STORE_APP 1

#include <vector>
#include <string>
#include <fstream>
#include <cstdarg>

class Directory
{
public:
	static bool Exists( const std::string& path );	
	static std::string GetWorkingDirectory(); 
	static std::vector<std::string> GetFiles( const std::string& path, const bool return_absolute_paths = true );
#if defined(__APPLE__) || defined(__APPLE_CC__)
    static void SetGetFilesInDirectoryBlock( std::vector<std::string> (^get_files_in_directory_block)(const char* path) );
#endif
private:
#if defined(__APPLE__) || defined(__APPLE_CC__)
    static std::vector<std::string> (^get_files_in_directory_block)(const char* path); 
#endif
	
};

class File
{
public:
	static bool Exists( const std::string& path );
	static std::string ReadAllText( const std::string& path );
	static std::vector<unsigned char> ReadAllBytes( const std::string& path );
#if defined(__APPLE__) || defined(__APPLE_CC__)
    static void SetReadAllTextBlock( std::string (^read_all_text_block)(const char* path) );
    static void SetFileExistsBlock( bool (^file_exists_block)(const char* path) );
#endif
private:
#if defined(__APPLE__) || defined(__APPLE_CC__)
    static std::string (^read_all_text_block)(const char* path);
    static bool (^file_exists_block)(const char* path);
#endif
};

#ifdef WINDOWS_STORE_APP
std::wstring MetrifyPath( const std::string& path );
#endif

/*
//void debug_print( const char * format, ... );
template <typename... Ts>
void debug_print(int dbg_lvl, char *fmt, Ts... ts)
{
	char buffer[10000];
	sprintf_s( buffer, 10000, fmt, ... );
	OutputDebugStringA( buffer );
}
*/

void debug_print( const char * format, ... );