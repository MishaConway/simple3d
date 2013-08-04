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