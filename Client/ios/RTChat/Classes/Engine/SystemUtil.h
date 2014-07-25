#pragma once

#include "public.h"
#include <vector>

class SystemUtil;

/** Information about a file/directory within the archive will be
returned using a FileInfo struct.
@see
Archive
*/
struct FileInfo 
{
    /// The file's fully qualified name
    String filename;
    /// Path name; separated by '/' and ending with '/'
    String path;
    /// Base filename
    String basename;
    size_t size;
	
	WORD ToIndex();
};

typedef std::vector<FileInfo> FileInfoList;

class SystemUtil
{

public:

	static const DWORD FilenameToIndex( const String &filename );  

	static const String CreateTempFile( const String &filename );

	static void MakeDir( const String &filename );


	static void DeleteTempFileByPrefix(const String &prefix);

	static const String GetShortName(const String &fullName);
	

	static String GetLastStringOf(const String &src,const char ch);

	static bool Is_Reserved_Dir (const char *fn);

	
    void FindFiles( const String &folder, FileInfoList &files, bool recursive );
	
	//找文件夹
	static void FindFolders(const String &pattern, StringVector* simpleList,bool recursive=false);

	//文件是否存在
	static bool IsFileExist(const String &pattern,bool resursive=false);

	//文件夹是否存在
	static bool IsFolderExist(const String &pattern);
	
	//循环冗余码生成
	static DWORD GeneratorCRC32(BYTE* InStr, DWORD len);
    
private:
    
    
    void InnerFileFiles( const String &folder, FileInfoList &files, bool recursive );

};


