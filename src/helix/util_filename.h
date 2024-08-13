//
// Copyright (c) 2013 NutronLabs
//

#pragma once

#include "util_string.h"

namespace util
{
	static void ToUpper( std::string &str );
	static void ToLower( std::string &str );

	bool Exists( const char* filename );
	bool IsDirectory( const char* filename );
	bool IsFile( const char* filename );

	class Filename
	{
	public:
		static const char s_preferredDividerChar;

	public:
		static bool IsDivider( const char c );
		static bool EndsWithDivider( const char * str );
		static bool StartsWithDivider( const char * str );
		static void MakeAllDividers( std::string& str, char new_divider );
		static void RemoveRedundantDividers( std::string& str );

		static std::string MakeAbsolutePath( const char* path );
		static std::string Normalize( const char* path );

		static void getPathAndFilename( const std::string& string, std::string& path, std::string& filename );
		static std::string GetFilename( const std::string & str );
		static std::string GetPath( const std::string &str );

		static void ReplaceDirectory( const std::string &origDir, const std::string &newDir, std::string &path );
		static void RemoveDir( const std::string &origDir, std::string &path );

		static void StripDrive( const std::string& path, std::string& outpath );
		static std::string GetExtension( const std::string& str, bool includeDot = true );

		static std::string ReplaceFilenameExtension( const std::string& filename, const std::string& newext );
		static std::string StripFilenameExtension( const std::string& filename );

		static bool Rename( StringList& old_list, StringList& new_list, const std::string& pattern, int starting_index );
	};

} // namespace util