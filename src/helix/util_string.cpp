/**********************************************************************
*
* (c) Ridgeback Software 1996-2008
*
* $File: //depot/source/core/rbUtil/util_string.cpp $
* $Revision: #8 $
* $DateTime: 2008/09/07 22:21:46 $
* $Author: mikeg $
*
**********************************************************************/

#include <string>
#include "util_string.h"

//", ;"

namespace util
{

	namespace string
	{

		std::string wrap( const std::string& string, const char* _end )
		{
			std::string result;
			result.append( _end );
			result.append( string );
			result.append( _end );
			return result;
		}

		std::string camelCase( const char *string )
		{
			std::string result( string );

			size_t pos = result.find( ' ', 0 );
			while ( pos != std::string::npos )
			{
				result.erase( pos, 1 );
				pos = result.find( ' ', 0 );
			}
			return result;
		}

		std::string format( const char* format, ... )
		{
			const int max_char = 1024;
			char message[ max_char];
			//vsnprintf(message, max_char-1, format, marker);

			va_list marker;
			va_start( marker, format );     /* Initialize variable arguments. */
			vsprintf_s(message, max_char, format, marker);
			va_end( marker );              /* Reset variable arguments.      */

			return std::string( message );
		}


		int split( const char* _string, const char* key, StringList& list )
		{
			list.clear();
			int result = 0;
			if ( _string )
			{
				result = split( std::string( _string ), key, list );
			}

			return result;
		}

		int split( std::string _string, const char* key, StringList& list )
		{
			list.clear();
			if ( !_string.empty())
			{
				size_t pos = 0;
				do
				{
					size_t end = _string.find_first_of( key, pos );
					if ( pos != end )
					{
						std::string part = _string.substr( pos, (end - pos));
						if ( part.length() > 0 )
						{
							list.push_back( part );
						}
					}

					pos = end + 1;
				}
				while ( pos != 0 );
			}

			return (int)list.size();
		}

		int split( const char* _string, const char* key, IndexList& list )
		{
			list.clear();
			std::string whole = _string;
			if ( !whole.empty())
			{
				size_t pos = 0;
				do
				{
					size_t end = whole.find_first_of( key, pos );
					if ( pos != end )
					{
						std::string part = whole.substr( pos, (end - pos));
						if ( part.length() > 0 )
						{
							list.push_back( atoi( part.c_str()));
						}
					}

					pos = end + 1;
				}
				while ( pos != 0 );
			}

			return (int)list.size();
		}

		std::string join( const char* seperator, const StringList& list )
		{
			std::string result;
			if ( !list.empty())
			{
				StringList::const_iterator it = list.begin();
				for(; it != list.end(); it++, result += seperator )
				{
					result += (*it);
				}
				result.erase( result.length() - 1, 1 );
			}

			return result;
		}

		static void wrap( StringList& list, const char* prefix, const char* postfix )
		{
			std::string _pre( prefix );
			std::string _post( postfix );
			StringList::iterator it = list.begin();
			for(; it != list.end(); it++ )
			{
				(*it) = _pre + (*it) + _post;
			}
		}

	}

}
