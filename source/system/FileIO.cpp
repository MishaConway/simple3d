#include "FileIO.h"
#include "../string/string_utils.h"

#define WINDOWS_STORE_APP 1

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

#include <wrl/client.h>
#include <ppl.h>
#include <ppltasks.h>
#include <agile.h>
using namespace Windows::UI::Core;



#else
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <wchar.h>
#endif

#if defined(__APPLE__) || defined(__APPLE_CC__)
std::vector<std::string> (^Directory::get_files_in_directory_block)(const char* path) = 0; 
std::string (^File::read_all_text_block)(const char* path) = 0;
bool (^File::file_exists_block)(const char* path) = 0;
#endif


bool Directory::Exists( const std::string& path )
{    	
#if defined(_WINDOWS_)
	//this part not supported in windows store app
	//const DWORD dwResult = GetFileAttributesA( path.c_str() );
	//return dwResult != INVALID_FILE_ATTRIBUTES && (dwResult & FILE_ATTRIBUTE_DIRECTORY);
	return false;
#else
	struct stat directoryDescription;
	if( 0 == stat( path.c_str(), &directoryDescription ) )
		return directoryDescription.st_mode & S_IFDIR;
#endif
	return false;
}

std::string Directory::GetWorkingDirectory()
{
	#if defined(__APPLE__) || defined(__APPLE_CC__)
    return "/";
    #endif
    
    std::string working_directory;

	char buffer[1024];
#if defined(_WINDOWS_)
	//this not supported in windows store app
/*    int charsWritten = GetCurrentDirectoryA( 1024, (char*)buffer );
	if( charsWritten > 0 )
		working_directory = std::string( (char*)buffer );
	working_directory += "\\"; */
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
    std::string cleaned_path = path;

#if defined(_WINDOWS_)
	std::replace( cleaned_path.begin(), cleaned_path.end(), '/', '\\');
	if( '\\' != cleaned_path[cleaned_path.size()-1] )
		cleaned_path += "\\";
	
	const std::string regular_expression_path = cleaned_path + "*.*";
	 
	WIN32_FIND_DATAA find_file_data;
	//not supported in windows store app
/*	HANDLE hFindFile = FindFirstFileA( regular_expression_path.c_str(), &find_file_data ); 
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
		} */
#else
    std::replace( cleaned_path.begin(), cleaned_path.end(), '\\', '/');
    if( '/' != cleaned_path[cleaned_path.size()-1] )
		cleaned_path += "/";
    
	#if defined(__APPLE__) || defined(__APPLE_CC__)
    files = get_files_in_directory_block( cleaned_path.c_str() );
    #else
    DIR* directory = opendir( path.c_str() );
	if( directory == 0 )
		return files;

	std::string fext = ".";
           
	struct dirent* directoryEntry = readdir(directory);
	while ( directoryEntry )
	{
		if( directoryEntry->d_type != DT_DIR )
		{
			if (strstr(directoryEntry->d_name, fext.c_str()))
				files.push_back( std::string( directoryEntry->d_name) );
		}
        directoryEntry = readdir(directory);
	}
	(void) closedir (directory); 
    #endif
#endif

	return files;

}

#if defined(__APPLE__) || defined(__APPLE_CC__)
void Directory::SetGetFilesInDirectoryBlock( std::vector<std::string> (^_get_files_in_directory_block)(const char* path) )
{
    get_files_in_directory_block = _get_files_in_directory_block;
}
#endif


#ifdef WINDOWS_STORE_APP

std::wstring MetrifyPath( const std::string& path )
{
	std::string cleaned_path = StringReplace( path, "/", "\\" );

	if( '\\' == cleaned_path[0] )
		cleaned_path.erase(0,1);
	
	std::wstring wide_path;
	wide_path.assign( cleaned_path.begin(), cleaned_path.end() );
		
	const std::wstring installed_location = std::wstring( Windows::ApplicationModel::Package::Current->InstalledLocation->Path->Data() );
	
	return std::wstring(installed_location + L"\\Simple3D_VisualStudio2010Express\\" + wide_path);
}


#endif





bool File::Exists( const std::string& path )
{
#if defined(_WINDOWS_)
	//not supported in windows store app
	#ifdef WINDOWS_STORE_APP
	std::wstring wide_path = MetrifyPath( path );
	FILE* file;
	_wfopen_s( &file, wide_path.c_str(), L"r" );
	if( file )
	{
		fclose( file );
		return true;
	}
	else
		return false;
	#else
	DWORD dwResult = GetFileAttributesA( path.c_str() );
	if ( dwResult == INVALID_FILE_ATTRIBUTES )
		return false;
	else
	if ( !(dwResult & FILE_ATTRIBUTE_DIRECTORY) )
		return true;
	#endif
#else
    #if defined(__APPLE__) || defined(__APPLE_CC__)
    return file_exists_block( path.c_str() );
    #else
    struct stat file_description;
    return stat( path.c_str(), &file_description ) == 0 ? true : false;
    #endif
#endif

	return false;
}

#if defined(__APPLE__) || defined(__APPLE_CC__)
void File::SetFileExistsBlock( bool (^_file_exists_block)(const char* path) )
{
    file_exists_block = _file_exists_block;
}
void File::SetReadAllTextBlock( std::string (^_read_all_text_block)(const char* path) )
{
    read_all_text_block = _read_all_text_block;    
}
#endif

std::string File::ReadAllText( const std::string& path )
{
#if defined(__APPLE__) || defined(__APPLE_CC__)
    if( read_all_text_block )
        return read_all_text_block(path.c_str());
#endif
	
#ifdef WINDOWS_STORE_APP
	std::wstring wide_path = MetrifyPath( path );
    std::ifstream t(wide_path.c_str());
#else
	std::ifstream t(path.c_str());
#endif
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}

std::vector<unsigned char> File::ReadAllBytes( const std::string& path )
{
	#ifdef WINDOWS_STORE_APP
	std::wstring wide_path = MetrifyPath( path );
    std::ifstream file(wide_path, std::ifstream::in | std::ifstream::binary );

	#else
	std::ifstream file(path, std::ios::binary);
	#endif
	
	if( file.is_open() )
	{
		// get its size:
		file.seekg(0, std::ios::end);
		std::streampos file_size = file.tellg();
		file.seekg(0, std::ios::beg);

		// read the data:
		std::vector<unsigned char> bytes(file_size);
		file.read((char*) &bytes[0], file_size);
		return bytes;
	}

	return std::vector<unsigned char>();
}



