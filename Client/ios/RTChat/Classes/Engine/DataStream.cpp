#include "DataStream.h"
#include "SystemUtil.h"

DataStream::DataStream()
:mDataSize(0)
{

}
DataStream::~DataStream()
{

}

MemoryDataStream::MemoryDataStream( BYTE *startAddress, size_t size )
{
	mStartAddress = startAddress;
	mDataSize = size;
	mCursor = 0;
}
MemoryDataStream::MemoryDataStream( size_t expectSize )
{
	mStartAddress = NULL;
	mDataSize = expectSize;
	mCursor = 0;
	Resize( expectSize );

}
MemoryDataStream::~MemoryDataStream()
{
	Close();
}
bool MemoryDataStream::Open()
{
	return true;
}

void MemoryDataStream::Resize( size_t newSize )
{
	if ( newSize <= mInnerMemory.size() )
		return;

	size_t oldSize = mInnerMemory.size();
	mInnerMemory.resize( newSize );
	//清空内存
	memset( &mInnerMemory[oldSize], '\0', newSize - oldSize );	

	mDataSize = newSize;
}
//读取一定的字节到buf中
size_t MemoryDataStream::Read(void* buf, size_t count)
{
	if ( mStartAddress )
		memcpy( buf, (void*)(mStartAddress+mCursor), count );
	else
		memcpy( buf, (void*)&mInnerMemory[mCursor], count );
	mCursor += count;
	return count;
}
//跳过指定的字节数进行读取
void MemoryDataStream::Skip(long count)
{
	mCursor += count;
}
//跳转到指定的字节
void MemoryDataStream::Seek(size_t pos)
{
	mCursor = pos;
}
//获取当前的字节偏移
size_t MemoryDataStream::Tell(void)const
{
	return mCursor;
}
//是否已经到达末尾
bool MemoryDataStream::Eof(void)const 
{
	return mCursor == mDataSize;
}
//写流数据
void MemoryDataStream::Write(void* buf, size_t count)
{
	if ( mStartAddress )
		memcpy( mStartAddress + mCursor, buf, count );
	else
	{
		//自增
		if ( (mCursor + count ) > Size() )
			Resize( mCursor + count );

		memcpy( &mInnerMemory[mCursor], buf, count );
	}
	mCursor += count;
}
//关闭数据流
void MemoryDataStream::Close(void)
{
	mStartAddress = NULL;

	mDataSize = 0;

	mCursor = 0;

	mInnerMemory.clear();
}



FileDataStream::FileDataStream(const String &filename)
:m_pFile(NULL)
{
	mFilename = filename;

	if(!mFilename.empty())
	{
		Open();
	}
}


bool FileDataStream::Open()
{
	if(m_pFile != NULL)
	{
		Close();
	}

	//setlocale(LC_CTYPE, "chs" );
	m_pFile = fopen(mFilename.c_str(),"rb+");
    
	if(m_pFile == NULL)
    {
		m_pFile = fopen(mFilename.c_str(),"rb");
    }
	if(m_pFile == NULL)
	{
		m_pFile = fopen(mFilename.c_str(),"wb+");
	}

	if(m_pFile)
	{
		fseek(m_pFile,0,SEEK_END);
		mDataSize = ftell(m_pFile);
		fseek(m_pFile,0,SEEK_SET);
	}
    else {
        return false;
    }
//	setlocale(LC_CTYPE, "");

	return true;

}
FileDataStream::~FileDataStream()
{
	Close();
}

//读取一定的字节到buf中
size_t FileDataStream::Read(void* buf, size_t count)
{
	if(m_pFile)
	{
		return fread(buf,1,count,m_pFile);
	}

	return 0;
}

void FileDataStream::GetAllBuffer( std::vector<BYTE> &datas )
{
	if ( Size() == 0 )
		return;

	size_t curPos = Tell();
	Seek( 0 );
	datas.resize( Size() );
	Read( &datas[0], datas.size() );
	Seek( curPos );
}

//跳过指定的字节数进行读取
void FileDataStream::Skip(long count)
{
	if(m_pFile)
	{
		fseek(m_pFile,count,SEEK_CUR);
	}
} 
//跳转到指定的字节
void FileDataStream::Seek(size_t pos)
{
	if(m_pFile)
	{
		fseek(m_pFile,pos,SEEK_SET);
	}
}
//获取当前的字节偏移
size_t FileDataStream::Tell(void)const
{
	if(m_pFile)
	{
		return ftell(m_pFile);
	}
	return 0;
}
//是否已经到达末尾
bool FileDataStream::Eof(void)const
{
	if(m_pFile)
	{
		return feof(m_pFile);
	}

	return true;
}
//关闭数据流
void FileDataStream::Close(void)
{
	if (m_pFile)
    {
		fclose(m_pFile);
		m_pFile = NULL;
    }
}
void FileDataStream::Write(void* buf, size_t count)
{
	if(m_pFile)
	{
		fwrite(buf,1,count,m_pFile);
		//fflush(m_pFile);
	}
}
DataStream *OpenDataStream( const String &file )
{
//	DataStream *pStream = NULL;
//	
//    String filename = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename( file.c_str() );
//    
//    pStream = new FileDataStream( filename );
//	
//	return pStream;
    return NULL;
}

//关闭流
void CloseDataStream( DataStream *&pStream )
{
	pStream->Close();

	SAFE_DELETE( pStream );
}