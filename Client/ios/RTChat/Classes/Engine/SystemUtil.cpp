#include "SystemUtil.h"
#include<sys/types.h>
#include<dirent.h>
#include <sys/stat.h>

WORD FileInfo::ToIndex()
{
	return (WORD)SystemUtil::FilenameToIndex( filename );
}
const DWORD SystemUtil::FilenameToIndex( const String &filename )
{
	std::string temp = filename;
	temp = temp.substr(filename.find_last_of('\\')+1);
	temp = temp.substr(0,temp.length()-4);
	return (DWORD)atoi(temp.c_str());

}

//¥¥Ω®¡Ÿ ±Œƒº˛£¨…æ≥˝‘≠”–µƒ¡Ÿ ±Œƒº˛
const String SystemUtil::CreateTempFile( const String &filename )
{
	return "";
}

void SystemUtil::MakeDir( const String &filePath )
{
	DWORD dwBegin = 0;
	DWORD dwEnd = filePath.find_first_of('\\',dwBegin);
	while(dwEnd != std::string::npos)
	{
		//::CreateDirectory(filePath.substr(0,dwEnd).c_str(),NULL);

		dwBegin = dwEnd + 1;

		dwEnd = filePath.find_first_of('\\',dwBegin);
	}
}

const String SystemUtil::GetShortName(const String &fullName)
{
	String temp = fullName;
	temp = temp.substr(temp.find_last_of('\\')+1);
	return temp;
}
//…æ≥˝¡Ÿ ±ƒø¬ºœ¬√Êµƒ¥¯”–prefix«∞◊∫µƒŒƒº˛
void SystemUtil::DeleteTempFileByPrefix(const String &prefix)
{
	
}


bool SystemUtil::Is_Reserved_Dir (const char *fn)
{
    return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
}

String SystemUtil::GetLastStringOf(const String &src,const char ch)
{
	size_t pos = src.find_last_of(ch);
	return src.substr(pos+1,src.length());
}

void SystemUtil::InnerFileFiles( const String &folder, FileInfoList &files, bool recusive )
{
    DIR *dir = opendir( folder.c_str() );
    if ( dir == NULL )
        return;
    struct dirent *ptr;
    while( (ptr = readdir(dir)) != NULL )
    {
        if ( ptr->d_name[0] == '.' )
            continue;
        
        String filename = folder + "/" + ptr->d_name;
        if ( ptr->d_type == 4 )
        {
            if ( recusive )
                InnerFileFiles( filename, files, recusive );
            continue;
        }
        
        FileInfo info;
        info.filename = filename;
        info.basename = ptr->d_name;
        
        struct stat temp;
        stat(filename.c_str(), &temp);
        info.size = temp.st_size;
        files.push_back(info);
    }
    
    closedir(dir);
}

void SystemUtil::FindFiles( const String &folder, FileInfoList &files, bool recursive )
{
    InnerFileFiles( folder, files, recursive );
}
//Œƒº˛ «∑Ò¥Ê‘⁄
bool SystemUtil::IsFileExist(const String &pattern,bool recursive)
{
//	StringVector simpleList;
//	FindFiles(pattern,recursive,false,&simpleList,NULL);
//	if (simpleList.size())
//		return true;
	return false;
}

//Œƒº˛º– «∑Ò¥Ê‘⁄
bool SystemUtil::IsFolderExist(const String &pattern)
{
//	StringVector simpleList;
//	FindFolders(pattern,&simpleList,false);
//	if (simpleList.size())
//		return true;
	return false;
}
//—≠ª∑»ﬂ”‡¬Î…˙≥…
DWORD SystemUtil::GeneratorCRC32(BYTE* InStr, DWORD len)
{
	//…˙≥…Crc32µƒ≤È—Ø±Ì   
	DWORD Crc32Table[256];    
	int i,j;      
	DWORD Crc;      
	for (i = 0; i < 256; i++)
	{      
		Crc = i;      
		for (j = 0; j < 8; j++)
		{      
		  if (Crc & 1)      
			Crc = (Crc >> 1) ^ 0xEDB88320;      
		  else     
			Crc >>= 1;    
		}      
		Crc32Table[i] = Crc;      
	}      

	//ø™ ºº∆À„CRC32–£—È÷µ   
	Crc=0xffffffff;      
	for(DWORD i=0; i<len; i++)
	{
		Crc = (Crc >> 8) ^ Crc32Table[(Crc & 0xFF) ^ InStr[i]];      
	}   
	Crc ^= 0xFFFFFFFF;   
	return Crc;      

}