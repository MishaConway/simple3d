#include "FileIO.h"

#include <algorithm>
#include <sstream>
#if defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#define NOSOUND
#define NOTEXTMETRIC
//#define NOGDI
#define NOGDICAPMASKS
//#define NOMSG
#define NOMINMAX
#define NOCRYPT
#include <windows.h>
#include <shlobj.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <wchar.h>
#endif



bool Directory::Exists( const std::string& path )
{    	
#if defined(_WINDOWS_)
	const DWORD dwResult = GetFileAttributesA( path.c_str() );
	return dwResult != INVALID_FILE_ATTRIBUTES && (dwResult & FILE_ATTRIBUTE_DIRECTORY);
#else
	struct stat directoryDescription;
	if( 0 == stat( path.c_str(), &directoryDescription ) )
		return directoryDescription.st_mode & S_IFDIR;
#endif
	return false;
}

std::string Directory::GetWorkingDirectory()
{
	std::string working_directory;

	char buffer[1024];
#if defined(_WINDOWS_)
    int charsWritten = GetCurrentDirectoryA( 1024, (char*)buffer );
	if( charsWritten > 0 )
		working_directory = std::string( (char*)buffer );
	working_directory += "\\";
#else
     if( getcwd( (char*) buffer, 1024 ) != 0 )
		working_directory = std::string( (char*) buffer );
#endif
	return working_directory;	
}

std::vector<std::string> Directory::GetFiles( const std::string& path, const bool return_absolute_paths )
{
	std::vector< std::string> files;
	std::string currentFile;

#if defined(_WINDOWS_)
    std::string cleaned_path = path;
	std::replace( cleaned_path.begin(), cleaned_path.end(), '/', '\\');
	if( '\\' != cleaned_path[cleaned_path.size()-1] )
		cleaned_path += "\\";
	
	const std::string regular_expression_path = cleaned_path + "*.*";
	 
	WIN32_FIND_DATAA find_file_data;
	HANDLE hFindFile = FindFirstFileA( regular_expression_path.c_str(), &find_file_data ); 
	if( hFindFile && !(find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
	{
		currentFile = std::string( find_file_data.cFileName );
		if( currentFile != "." && currentFile != ".." )
		{
			if( return_absolute_paths )
				currentFile = cleaned_path + currentFile;
			files.push_back( currentFile );
		}
	}

	while( FindNextFileA( hFindFile, &find_file_data ) )
		if( !(find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			currentFile = std::string( find_file_data.cFileName );
			if( currentFile != "." && currentFile != ".." )
			{
				if( return_absolute_paths )
					currentFile = cleaned_path + currentFile;
				files.push_back( currentFile );
			}
		}
#else
	DIR* directory = opendir( path.c_str() );
	if( directory == 0 )
		return files;

	std::string fext = ".";
           
	struct dirent* directoryEntry;
	while ( directoryEntry = readdir (directory) )
	{
		if( directoryEntry->d_type != DT_DIR )
		{
			if (strstr(directoryEntry->d_name, fext.c_str()))
				files.push_back( std::string( directoryEntry->d_name) );
		}
	}
	(void) closedir (directory);       
#endif

	return files;

}

bool File::Exists( const std::string& path )
{
#if defined(_WINDOWS_)
	DWORD dwResult = GetFileAttributesA( path.c_str() );
	if ( dwResult == INVALID_FILE_ATTRIBUTES )
		return false;
	else
	if ( !(dwResult & FILE_ATTRIBUTE_DIRECTORY) )
		return true;
#else
     struct stat file_description;
     return stat( path.c_str(), &file_description ) == 0 ? true : false;
#endif

	return false;
}

std::string File::ReadAllText( const std::string& path )
{
	std::ifstream t(path.c_str());
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}



