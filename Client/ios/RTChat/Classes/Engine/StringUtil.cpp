#include "StringUtil.h"
#include <algorithm>


const String StringUtil::BLANK;
//-----------------------------------------------------------------------
void StringUtil::trim(String& str, bool left, bool right)
{
    /*
    size_t lspaces, rspaces, len = length(), i;

    lspaces = rspaces = 0;

    if( left )
    {
        // Find spaces / tabs on the left
        for( i = 0;
            i < len && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
            ++lspaces, ++i );
    }
    
    if( right && lspaces < len )
    {
        // Find spaces / tabs on the right
        for( i = len - 1;
            i >= 0 && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
            rspaces++, i-- );
    }

    *this = substr(lspaces, len-lspaces-rspaces);
    */
    static const String delims = " \t\r";
    if(right)
        str.erase(str.find_last_not_of(delims)+1); // trim right
    if(left)
        str.erase(0, str.find_first_not_of(delims)); // trim left
}

//-----------------------------------------------------------------------
std::vector<String> StringUtil::split( const String& str, const String& delims, unsigned int maxSplits)
{
    std::vector<String> ret;
    // Pre-allocate some space for performance
    ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

    unsigned int numSplits = 0;

    // Use STL methods 
    size_t start, pos;
    start = 0;
    do 
    {
        pos = str.find_first_of(delims, start);
        if (pos == start)
        {
            // Do nothing
            start = pos + 1;
        }
        else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
        {
            // Copy the rest of the string
            ret.push_back( str.substr(start) );
            break;
        }
        else
        {
            // Copy up to delimiter
            ret.push_back( str.substr(start, pos - start) );
            start = pos + 1;
        }
        // Parse up to next real data
        start = str.find_first_not_of(delims, start);
        ++numSplits;

    } while (pos != String::npos);



    return ret;
}

//-----------------------------------------------------------------------
void StringUtil::toLowerCase(String& str)
{
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
		tolower);
}

//-----------------------------------------------------------------------
void StringUtil::toUpperCase(String& str) 
{
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
		toupper);
}
//-----------------------------------------------------------------------
bool StringUtil::startsWith(const String& str, const String& pattern, bool lowerCase)
{
    size_t thisLen = str.length();
    size_t patternLen = pattern.length();
    if (thisLen < patternLen || patternLen == 0)
        return false;

    String startOfThis = str.substr(0, patternLen);
    if (lowerCase)
        StringUtil::toLowerCase(startOfThis);

    return (startOfThis == pattern);
}
//-----------------------------------------------------------------------
bool StringUtil::endsWith(const String& str, const String& pattern, bool lowerCase)
{
    size_t thisLen = str.length();
    size_t patternLen = pattern.length();
    if (thisLen < patternLen || patternLen == 0)
        return false;

    String endOfThis = str.substr(thisLen - patternLen, patternLen);
    if (lowerCase)
        StringUtil::toLowerCase(endOfThis);

    return (endOfThis == pattern);
}
//-----------------------------------------------------------------------
String StringUtil::standardisePath(const String& init)
{
    String path = init;

    std::replace( path.begin(), path.end(), '\\', '/' );
    if( path[path.length() - 1] != '/' )
        path += '/';

    return path;
}
//-----------------------------------------------------------------------
void StringUtil::splitFilename(const String& qualifiedName, 
    String& outBasename, String& outPath)
{
    String path = qualifiedName;
    // Replace \ with / first
    std::replace( path.begin(), path.end(), '\\', '/' );
    // split based on final /
    size_t i = path.find_last_of('/');

    if (i == String::npos)
    {
        outPath.clear();
		outBasename = qualifiedName;
    }
    else
    {
        outBasename = path.substr(i+1, path.size() - i - 1);
        outPath = path.substr(0, i+1);
    }

}
//-----------------------------------------------------------------------
void StringUtil::splitBaseFilename(const String& fullName, 
	String& outBasename, String& outExtention)
{
	size_t i = fullName.find_last_of(".");
	if (i == String::npos)
	{
		outExtention.clear();
		outBasename = fullName;
	}
	else
	{
		outExtention = fullName.substr(i+1);
		outBasename = fullName.substr(0, i);
	}
}
// ----------------------------------------------------------------------------------------------------------------------------------------------
void StringUtil::splitFullFilename(	const String& qualifiedName, 
	String& outBasename, String& outExtention, String& outPath )
{
	String fullName;
	splitFilename( qualifiedName, fullName, outPath );
	splitBaseFilename( fullName, outBasename, outExtention );
}
//-----------------------------------------------------------------------
bool StringUtil::match(const String& str, const String& pattern, bool caseSensitive)
{
    String tmpStr = str;
	String tmpPattern = pattern;
    if (!caseSensitive)
    {
        StringUtil::toLowerCase(tmpStr);
        StringUtil::toLowerCase(tmpPattern);
    }

    String::const_iterator strIt = tmpStr.begin();
    String::const_iterator patIt = tmpPattern.begin();
	String::const_iterator lastWildCardIt = tmpPattern.end();
    while (strIt != tmpStr.end() && patIt != tmpPattern.end())
    {
        if (*patIt == '*')
        {
			lastWildCardIt = patIt;
            // Skip over looking for next character
            ++patIt;
            if (patIt == tmpPattern.end())
			{
				// Skip right to the end since * matches the entire rest of the string
				strIt = tmpStr.end();
			}
			else
            {
				// scan until we find next pattern character
                while(strIt != tmpStr.end() && *strIt != *patIt)
                    ++strIt;
            }
        }
        else
        {
            if (*patIt != *strIt)
            {
				if (lastWildCardIt != tmpPattern.end())
				{
					// The last wildcard can match this incorrect sequence
					// rewind pattern to wildcard and keep searching
					patIt = lastWildCardIt;
					lastWildCardIt = tmpPattern.end();
				}
				else
				{
					// no wildwards left
					return false;
				}
            }
            else
            {
                ++patIt;
                ++strIt;
            }
        }

    }
	// If we reached the end of both the pattern and the string, we succeeded
	if (patIt == tmpPattern.end() && strIt == tmpStr.end())
	{
    	return true;
	}
	else
	{
		return false;
	}

}
//-----------------------------------------------------------------------
String StringUtil::toString(Real val, WORD precision, 
    WORD width, char fill, std::ios::fmtflags flags)
{
    StringUtil::StrStreamType stream;
    stream.precision(precision);
    stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}
//-----------------------------------------------------------------------
String StringUtil::toString(int val, 
    WORD width, char fill, std::ios::fmtflags flags)
{
    StringUtil::StrStreamType stream;
	stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}
//-----------------------------------------------------------------------
String StringUtil::toString(size_t val, 
    WORD width, char fill, std::ios::fmtflags flags)
{
    StringUtil::StrStreamType stream;
	stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}

String StringUtil::toString(WORD val, 
    WORD width, char fill, std::ios::fmtflags flags)
{
    StringUtil::StrStreamType stream;
	stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}
String StringUtil::toString(long val, 
    WORD width, char fill, std::ios::fmtflags flags)
{
    StringUtil::StrStreamType stream;
	stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}
//-----------------------------------------------------------------------
 String StringUtil::toString(const RectF& val)
 {
	StringUtil::StrStreamType stream;
	stream << val.left << " " << val.top << " " << val.right << " " << val.bottom;
    return stream.str();
 }
//-----------------------------------------------------------------------
String StringUtil::toString(bool val, bool yesNo)
{
    if (val)
    {
        if (yesNo)
        {
            return "yes";
        }
        else
        {
            return "true";
        }
    }
    else
        if (yesNo)
        {
            return "no";
        }
        else
        {
            return "false";
        }
}

//-----------------------------------------------------------------------
String StringUtil::toString(const StringVector& val)
{
	StringUtil::StrStreamType stream;
    StringVector::const_iterator i, iend, ibegin;
    ibegin = val.begin();
    iend = val.end();
    for (i = ibegin; i != iend; ++i)
    {
        if (i != ibegin)
            stream << " ";

        stream << *i; 
    }
    return stream.str();
}
//-----------------------------------------------------------------------
float StringUtil::parseFloat(const String& val)
{
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	float ret = 0;
	str >> ret;

    return ret;
}
//-----------------------------------------------------------------------
int StringUtil::parseInt(const String& val)
{
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	int ret = 0;
	str >> ret;

    return ret;
}
RectF StringUtil::parseRect(const String& val)
{
	// Split on space
    std::vector<String> vec = StringUtil::split(val, ", ");
	vec.resize(4);
	return RectF(parseInt(vec[0]),parseInt(vec[1]),parseInt(vec[2]),parseInt(vec[3]));
}
//-----------------------------------------------------------------------
unsigned int StringUtil::parseUnsignedInt(const String& val)
{
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	unsigned int ret = 0;
	str >> ret;

	return ret;
}
//-----------------------------------------------------------------------
long StringUtil::parseLong(const String& val)
{
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	long ret = 0;
	str >> ret;

	return ret;
}
//-----------------------------------------------------------------------
unsigned long StringUtil::parseUnsignedLong(const String& val)
{
	// Use istringstream for direct correspondence with toString
	std::istringstream str(val);
	unsigned long ret = 0;
	str >> ret;

	return ret;
}
//-----------------------------------------------------------------------
bool StringUtil::parseBool(const String& val)
{
	return (StringUtil::startsWith(val, "true") || StringUtil::startsWith(val, "yes")
		|| StringUtil::startsWith(val, "1"));
}
//-----------------------------------------------------------------------
StringVector StringUtil::parseStringVector(const String& val)
{
    return StringUtil::split(val, ", ");
}
//-----------------------------------------------------------------------
bool StringUtil::isNumber(const String& val)
{
	std::istringstream str(val);
	float tst;
	str >> tst;
	return !str.fail() && str.eof();
}

//提取颜色
DWORD StringUtil::parseColor(const String& str)
{
	RectF rect = StringUtil::parseRect(str);
    return ARGB((int)rect.left,(int)rect.top,(int)rect.right,(int)rect.bottom);
}

String StringUtil::toColorString(DWORD dwColor)
{
	RectF rect((float)GETA(dwColor),(float)GETR(dwColor),(float)GETG(dwColor),(float)GETB(dwColor));
	return StringUtil::toString(rect);
}

void StringUtil::replace( String &strBase, const String &strSrc, const String &strDes)  
{  
	std::string::size_type pos = 0;  
	std::string::size_type srcLen = strSrc.size();  
	std::string::size_type desLen = strDes.size();  
	pos=strBase.find(strSrc, pos);   
	while ((pos != std::string::npos))  
	{  
		strBase.replace(pos, srcLen, strDes);  
		pos=strBase.find(strSrc, (pos+desLen));  
	}  
}  