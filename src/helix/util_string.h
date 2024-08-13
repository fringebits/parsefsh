/**********************************************************************
*
* Copyright (c) Ridgeback Software 1996-2004
*
* $File: //depot/source/core/rbUtil/util_string.h $
* $Revision: #10 $
* $DateTime: 2008/09/07 22:21:46 $
* $Author: mikeg $
* $Change: 735 $
*
**********************************************************************/
#pragma once

#include <string>
#include <list>
#include <algorithm>
#include <stdarg.h>
#include <assert.h>

namespace util
{

	class StringList :
		public std::list< std::string >
	{
	public:
		bool contains( const char* string ) const;
		bool contains( const std::string& string ) const;
		std::string& getAt( unsigned int index )
		{
			assert( index < size());
			iterator it = begin();
			while( index-- > 0 )
			{
				++it;
			}
			return *it;
		}
	};

	inline bool StringList::contains( const char* string ) const
	{
		const_iterator it = std::find( begin(), end(), string );
		return it != end();
	}

	inline bool StringList::contains( const std::string& string ) const
	{
		const_iterator it = std::find( begin(), end(), string );
		return it != end();
	}


	class IndexList :
		public std::list< int >
	{
	};

} // end namespace util

namespace util
{

	namespace string
	{

		inline bool isValid( const std::string& string )
		{
			// with GPS / OSX, seems like we've got some problems related to the string length -- not clear what is
			// causing this -- runtime tests look good -- this function is supposeed to validate the strings
			bool result = true;

			size_t len = string.length(); // get the length of the string
			size_t ii = 0;
			for(; ii < len; ii++ )
			{
				if ( 0 == string.at( ii ))
				{
					result = false;
					break;
				}
			}

			assert( result );

			return result;
		}

		inline bool validate( std::string& string )
		{
			// with GPS / OSX, seems like we've got some problems related to the string length -- not clear what is
			// causing this -- runtime tests look good -- this function is supposeed to validate the strings
			bool result = true;

			size_t len = (uint32_t)string.length(); // get the length of the string
			size_t ii = 0;
			for(; ii < len; ii++ )
			{
				if ( 0 == string.at( ii ))
				{
					// we found a terminator *before* the end of the string -- this is a problem
					string = string.substr( 0, ii );
					result = false;
					break;
				}
			}

			assert( string.length() == ii );
			//assert( result );

			return result;
		}

		inline void tolower_inplace( std::string& string )
		{
			std::transform( string.begin(), string.end(), string.begin(), ::tolower );
		}

		inline std::string tolower( const std::string& string )
		{
			std::string result( string );
			tolower_inplace( result );
			return result;
		}

		std::string camelCase( const char* string );

		std::string wrap( const std::string& string, const char* _end = "'" );

		std::string format( const char* format, ... );

		int split( const char* string, const char* seperator, IndexList& list ); // split a list of integers
		int split( const char* string, const char* seperator, StringList& list ); // split a list of strings
		int split( const std::string string, const char* seperator, StringList& list ); // split a list of strings
		std::string join( const char* seperator, IndexList& list );
		std::string join( const char* seperator, const StringList& list );


#if defined(EC_TARGET_IOS) || defined(EC_TARGET_OSX)

		inline CFStringRef convert( const std::string& value )
		{
			return ::CFStringCreateWithCString( CFAllocatorGetDefault(), value.c_str(), kCFStringEncodingMacRoman );
		}

		inline std::string convert( CFStringRef& value )
		{
			std::string result = ::CFStringGetCStringPtr( value, kCFStringEncodingMacRoman );
			return result;
		}

		inline std::string convert( CFURLRef& url )
		{
			std::string result;
			uint8 buffer[ 4096];
			bool ret = CFURLGetFileSystemRepresentation( url, true, buffer, 4096 );
			if ( ret )
			{
				result.assign((char*)buffer );
			}

			return result;
		}

#endif

	} // end namespace string

} // end namespace util
