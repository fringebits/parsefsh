/*******************************************************************************
*
* Copyright(c) 2008 Ridgeback Software
*
* $File: //depot/source/core/rbUtil/util_filename.cpp $
* $Revision: #5 $
* $DateTime: 2008/07/04 13:07:11 $
* $Author: mikeg $
*
*******************************************************************************/

#include <ctype.h>
#include "util_filename.h"
#include <algorithm>

namespace util
{

	/*static*/ void ToUpper( std::string &str )
	{
		std::transform( str.begin(), str.end(), str.begin(), toupper );
	}

	/*static*/ void ToLower( std::string &str )
	{
		std::transform( str.begin(), str.end(), str.begin(), tolower );
	}


	//static
	const char Filename::s_preferredDividerChar = '/';

	//static
	bool Filename::IsDivider( const char c )
	{
		if( c =='/' || c=='\\' )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//static
	bool Filename::EndsWithDivider( const char * str )
	{
		if( str!=NULL )
		{
			return IsDivider( str[strlen( str )-1] );
		}
		else
		{
			return false;
		}
	}

	//static
	bool Filename::StartsWithDivider( const char * str )
	{
		if( str!=NULL )
		{
			return IsDivider( str[0] );
		}
		else
		{
			return false;
		}
	}

	//static
	std::string Filename::Normalize( const char* string )
	{
		std::string result = string;

		ToLower( result );
		MakeAllDividers( result, '/' );
		RemoveRedundantDividers( result );

		return result;
	}

#if defined(WIN32)
	std::string Filename::MakeAbsolutePath( const char* string )
	{
		char fullpath[ 2048] = { 0 };
		_fullpath( fullpath, string, 2048 );
		std::string result = Normalize( fullpath );
		return result;
	}
#endif


	//static
	void Filename::MakeAllDividers( std::string &str, char new_divider )
	{
		for( uint32_t ii = 0; ii < str.length(); ii++ )
		{
			if ( str[ ii] == '\\' )
			{
				str[ ii] = new_divider;
			}
			else if ( str[ ii] == '/' )
			{
				str[ ii] = new_divider;
			}
		}
	}

	//static
	void Filename::RemoveRedundantDividers( std::string &str )
	{
		//find every string "//", until index == npos (meaning "//" wasn't found)
		std::string::size_type index = str.find( "//" );
		for(; index != std::string::npos; index = str.find( "//" ))
		{
			str.erase( index, 1 );
		}
	}

	void Filename::getPathAndFilename( const std::string& string, std::string& path, std::string& filename )
	{
		size_t slash_pos = string.find_last_of( "/" );
		if ( slash_pos != std::string::npos )
		{
			path = string.substr( 0, slash_pos + 1 );
			filename = string.substr( slash_pos + 1, std::string::npos );
		}
		else
		{
			// in this case, if we didn't find "any" slash, we need to return the 'emtpy' string
			path.clear();
			filename = string;
		}
	}

	std::string Filename::GetPath( const std::string &str )
	{
		std::string result = str;
		MakeAllDividers( result, '/' );
		size_t slash_pos = result.find_last_of( "/" );
		if ( slash_pos != std::string::npos )
		{
			result = result.substr( 0, slash_pos + 1 );
		}
		else
		{
			// in this case, if we didn't find "any" slash, we need to return the 'emtpy' string
			result.clear();
		}

		return result;
	}

	std::string Filename::GetExtension( const std::string& str, bool includeDot )
	{
		std::string filename = GetFilename( str ); // first, we'll get the filename
		std::string result; // result is empty by default
		size_t dot_pos = filename.find_last_of( "." );
		if ( dot_pos != std::string::npos )
		{
			result = filename.substr( includeDot ? dot_pos : dot_pos + 1 );
		}

		return result;
	}

	std::string Filename::GetFilename( const std::string & str )
	{
		std::string result = str;
		MakeAllDividers( result, '/' );
		size_t slash_pos = result.find_last_of( "/" );
		if ( slash_pos != std::string::npos )
		{
			result = result.substr( slash_pos + 1 );
		}

		return result;
	}

	/**
	 * static
	 *
	 * Finds the specified directory and replaces it in the specified path
	 *
	 * @param origDir Find this in the specified path
	 * @param newDir  Replace origDir with this directory
	 * @param path    The path to do the operations on
	 */
	void Filename::ReplaceDirectory( const std::string &origDir, const std::string &newDir, std::string &path )
	{
		size_t dirPos = path.find( origDir );
		if ( dirPos != std::string::npos )
		{
			path.erase( dirPos, origDir.length());
			path.insert( dirPos, newDir );
		}
	}

	/**
	 * static
	 *
	 * Removes the specified directory from the specified path
	 *
	 * @param origDir The directory to remove from the path
	 * @param path    The path to remove from
	 */
	void Filename::RemoveDir( const std::string &origDir, std::string &path )
	{
		size_t dirPos = path.find( origDir );
		if ( dirPos != std::string::npos )
		{
			path.erase( dirPos, origDir.length());
		}
	}

	std::string Filename::ReplaceFilenameExtension( const std::string& filename, const std::string& newext )
	{
		std::string result;
		std::string ext;
		std::string::size_type pos;

		if ( newext[0] != '.' )
		{
			ext = "." + newext;
		}
		else
		{
			ext = newext;
		}

		result = filename;
		pos = result.find_last_of( "." );
		if ( pos != std::string::npos )
		{
			result.erase( pos );
		}

		result += ext;
		return result;
	}

	std::string Filename::StripFilenameExtension( const std::string& filename )
	{
		std::string result;
		std::string::size_type pos;

		result = filename;
		pos = result.find_last_of( "." );
		if ( pos != std::string::npos )
		{
			result.erase( pos );
		}

		return result;
	}

	void Filename::StripDrive( const std::string& path, std::string& outpath )
	{
		size_t offset;

		outpath = path;
		Filename::MakeAllDividers( outpath, '\\' ); // all forward slashes
		offset = outpath.find_first_of( ":" );
		outpath.erase( 0, offset+1 );
	}

	/**
	   pattern consists of "prefix_###_postfix.extension"
	   /**/
	bool Filename::Rename( StringList& old_list, StringList& new_list, const std::string& pattern, int starting_index )
	{
		bool result = false;

		if ( pattern.empty())
		{

			new_list = old_list;

		}
		else
		{
			size_t first_marker = pattern.find_first_of( "#" );
			size_t last_marker = pattern.find_last_of( "#" );
			new_list.clear(); // clear the new list!
			std::string pattern_extension = Filename::GetExtension( pattern, true );

			if ( first_marker != std::string::npos )
			{
				std::string pattern_prefix = pattern.substr( 0, first_marker );
				std::string pattern_postfix = pattern.substr( last_marker+1 );

				size_t pattern_length = (last_marker == std::string::npos) ? 1 : (last_marker - first_marker) + 1;
				size_t index = starting_index;
				StringList::iterator it = old_list.begin();
				for(; it != old_list.end(); it++, index++ )
				{
					std::string new_name = util::string::format( "%s%0*d%s", pattern_prefix.c_str(), pattern_length, index, pattern_postfix.c_str());
					if ( pattern_extension.empty())
					{
						new_name += Filename::GetExtension((*it), true );
					}

					new_list.push_back( new_name );
				}
			}
			else
			{
				// no pattern matching, we're just going to use the pattern as an absolute
				StringList::iterator it = old_list.begin();
				for(; it != old_list.end(); it++ )
				{
					std::string new_name = pattern;
					if ( pattern_extension.empty())
					{
						new_name += Filename::GetExtension((*it), true );
					}

					new_list.push_back( new_name );
				}
			}
		}

		return result;
	}


} // namespace util

/* ----------------------------------------------------------------------------
 *  $Change: 703 $
 * ---------------------------------------------------------------------------- */
