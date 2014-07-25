#pragma once
#include "public.h"
#include "Vector2.h"
#include "Rect.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
 /** Utility class for manipulating Strings.  */
#define D3DCOLOR DWORD
class StringUtil
{
public:
    typedef std::ostringstream StrStreamType;

    /** Removes any whitespace characters, be it standard space or
        TABs and so on.
        @remarks
            The user may specify wether they want to trim only the
            beginning or the end of the String ( the default action is
            to trim both).
    */
    static void trim( String& str, bool left = true, bool right = true );

    /** Returns a StringVector that contains all the substrings delimited
        by the characters in the passed <code>delims</code> argument.
        @param
            delims A list of delimiter characters to split by
        @param
            maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
            parameters is > 0, the splitting process will stop after this many splits, left to right.
    */
	static std::vector< String > split( const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);

    /** Upper-cases all the characters in the string.
    */
    static void toLowerCase( String& str );

    /** Lower-cases all the characters in the string.
    */
    static void toUpperCase( String& str );


    /** Returns whether the string begins with the pattern passed in.
    @param pattern The pattern to compare with.
    @param lowerCase If true, the end of the string will be lower cased before
        comparison, pattern should also be in lower case.
    */
    static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

    /** Returns whether the string ends with the pattern passed in.
    @param pattern The pattern to compare with.
    @param lowerCase If true, the end of the string will be lower cased before
        comparison, pattern should also be in lower case.
    */
    static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

    /** Method for standardising paths - use forward slashes only, end with slash.
    */
    static String standardisePath( const String &init);

    /** Method for splitting a fully qualified filename into the base name
        and path.
    @remarks
        Path is standardised as in standardisePath
    */
    static void splitFilename(const String& qualifiedName,
        String& outBasename, String& outPath);

	/** Method for splitting a fully qualified filename into the base name,
	extension and path.
	@remarks
	Path is standardised as in standardisePath
	*/
	static void splitFullFilename(const String& qualifiedName, 
		String& outBasename, String& outExtention, 
		String& outPath);

	/** Method for splitting a filename into the base name
	and extension.
	*/
	static void splitBaseFilename(const String& fullName, 
		String& outBasename, String& outExtention);


    /** Simple pattern-matching routine allowing a wildcard pattern.
    @param str String to test
    @param pattern Pattern to match against; can include simple '*' wildcards
    @param caseSensitive Whether the match is case sensitive or not
    */
    static bool match(const String& str, const String& pattern, bool caseSensitive = true);


    /// Constant blank string, useful for returning by ref where local does not exist
    static const String BLANK;

      /** Converts a Real to a String. */
    static String toString(Real val, WORD precision = 6, 
        WORD width = 0, char fill = ' ', 
        std::ios::fmtflags flags = std::ios::fmtflags(0) );
    /** Converts an int to a String. */
    static String toString(int val, WORD width = 0, 
        char fill = ' ', 
        std::ios::fmtflags flags = std::ios::fmtflags(0) );

    /** Converts a size_t to a String. */
    static String toString(size_t val, 
        WORD width = 0, char fill = ' ', 
        std::ios::fmtflags flags = std::ios::fmtflags(0) );
    /** Converts an unsigned long to a String. */


    static String toString(WORD val, 
        WORD width = 0, char fill = ' ', 
        std::ios::fmtflags flags = std::ios::fmtflags(0) );
	

    /** Converts a long to a String. */
    static String toString(long val, 
        WORD width = 0, char fill = ' ', 
        std::ios::fmtflags flags = std::ios::fmtflags(0) );
    /** Converts a boolean to a String. 
    @param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
    */
    static String toString(bool val, bool yesNo = false);
	/** Converts a Vector2 to a String. 
    @remarks
        Format is "x y" (i.e. 2x Real values, space delimited)
    */
	template <typename T>
    static String toString(const Vector2<T>& val)
	{
		StringUtil::StrStreamType stream;
		stream << val.x << " " << val.y;
		return stream.str();
	}

	template <typename T>
	static String toString(const T& vec,char delim = ' ')
	{
		StringUtil::StrStreamType stream;

		auto it = vec.begin();
		while(it != vec.end())
		{
			stream << *it;
			++it;
			if(it != vec.end())
			{
				stream << delim;
			}
		}
		
		return stream.str();
	}
	/** Converts a Rect to a String. 
    @remarks
        Format is "r g b a" (i.e. 4x Real values, space delimited). 
    */
    static String toString(const RectF& val);

    /** Converts a StringVector to a string.
    @remarks
        Strings must not contain spaces since space is used as a delimiter in
        the output.
    */
    static String toString(const StringVector& val);

    /** Converts a String to a Real. 
    @returns
        0.0 if the value could not be parsed, otherwise the Real version of the String.
    */
    static float parseFloat(const String& val);
    /** Converts a String to a whole number. 
    @returns
        0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
    static int parseInt(const String& val);
	/** Converts a String to rect	
    @returns
        0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
	static RectF parseRect(const String& val);

    /** Converts a String to a whole number. 
    @returns
        0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
    static unsigned int parseUnsignedInt(const String& val);
    /** Converts a String to a whole number. 
    @returns
        0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
    static long parseLong(const String& val);
    /** Converts a String to a whole number. 
    @returns
        0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
    static unsigned long parseUnsignedLong(const String& val);
    /** Converts a String to a boolean. 
    @remarks
        Returns true if case-insensitive match of the start of the string
		matches "true", "yes" or "1", false otherwise.
    */
    static bool parseBool(const String& val);
	/** Parses a Vector2 out of a String.
    @remarks
        Format is "x y" ie. 2 Real components, space delimited. Failure to parse returns
        Vector2::ZERO.
    */
	template <typename T>
    static Vector2<T> parseVector2(const String& val)
	{
		// Split on space
		std::vector<String> vec = StringUtil::split(val,", ");

		if (vec.size() != 2)
		{
			return Vector2<T>(0,0);
		}
		else
		{
			return Vector2<T>(parseInt(vec[0]),parseInt(vec[1]));
		}
	}

	template <typename T>
	static std::vector<T>& parseVec(const String& val,std::vector<T>& vecData)
	{
		vecData.clear();
		std::vector<String> vec = StringUtil::split(val,", ");
		for(size_t i=0;i<vec.size();++i)
		{
			std::istringstream str(vec[i]);
			T a;
			str>>a;
			vecData.push_back(a);
		}
		return vecData;
	}
    
    template <typename T>
	static std::vector<T>& parseStringToVec(const String& val,std::vector<T>& vecData, const String& delim)
	{
		vecData.clear();
		std::vector<String> vec = StringUtil::split(val, delim);
		for(size_t i=0;i<vec.size();++i)
		{
			std::istringstream str(vec[i]);
			T a;
			str>>a;
			vecData.push_back(a);
		}
		return vecData;
	}

    /** Pareses a StringVector from a string.
    @remarks
        Strings must not contain spaces since space is used as a delimiter in
        the output.
    */
    static StringVector parseStringVector(const String& val);
    
    /** Checks the String is a valid number value. */
    static bool isNumber(const String& val);

	static D3DCOLOR parseColor(const String& str);
	static String toColorString(DWORD dwColor);


	static void replace(String &strBase, const String &strSrc, const String &strDes);

};