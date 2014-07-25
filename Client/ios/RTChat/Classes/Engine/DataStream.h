#pragma once
#include "public.h"
#include <fstream>
#include <iostream>

using namespace std;


class DataStream
{
public:
	
	DataStream();
	virtual ~DataStream();

	virtual bool Open() = 0;

	virtual void Close(void) = 0;
    
	virtual size_t Read(void* buf, size_t count) = 0;

	template <typename T>
	DataStream* Read(T &val)
	{
		Read(static_cast<void*>(&val),sizeof(T));
		return this;
	}
    
	virtual void Skip(long count) = 0; 

    
	virtual void Seek(size_t pos) = 0;

    
	virtual size_t Tell(void)const = 0;

    
	virtual bool Eof(void)const = 0;

    
	size_t Size(void)const { return mDataSize; }


	virtual void Write(void* buf, size_t count){	};

	template <typename T>
	DataStream* Write(T val)
	{
		Write(static_cast<void*>(&val),sizeof(T));
		return this;
	}	

protected:

	DWORD mDataSize;//数据大小

};

//操作一块内存
class MemoryDataStream : public DataStream
{
public:

	//外部提供一块内存，由外部才操作内存的释放
	MemoryDataStream( BYTE *startAddress, size_t size );

	//内部提供一块内存区域，内部释放，expectSize为期望值，如果超出该大小，系统会自动扩充
	MemoryDataStream( size_t expectSize = 0 );

	virtual ~MemoryDataStream();

	
	virtual bool Open();

	//关闭数据流
	virtual void Close(void);

	//读取一定的字节到buf中
	virtual size_t Read(void* buf, size_t count);
	//跳过指定的字节数进行读取
	virtual void Skip(long count);
	//跳转到指定的字节
	virtual void Seek(size_t pos);
	//获取当前的字节偏移
	virtual size_t Tell(void)const;
	//是否已经到达末尾
	virtual bool Eof(void)const;

	//写流数据
	virtual void Write(void* buf, size_t count);

	std::vector<BYTE> &GetInnerMemory() { return mInnerMemory; }

private:

	//内部维护内存的才能使用该函数
	void Resize( size_t newSize );

private:

	BYTE *mStartAddress;

	size_t mCursor;

	std::vector<BYTE> mInnerMemory;

};

//
class FileDataStream : public DataStream
{
public:

	FileDataStream(const String &filename = "");

	~FileDataStream();

	void SetFile( const String &filename ) { mFilename = filename; }

	//打开文件流
	virtual bool Open();
	//读取一定的字节到buf中
	virtual size_t Read(void* buf, size_t count);
	//跳过指定的字节数进行读取
	virtual void Skip(long count); 
	//跳转到指定的字节
	virtual void Seek(size_t pos);
	//获取当前的字节偏移
	virtual size_t Tell(void)const;
	//是否已经到达末尾
	virtual bool Eof(void)const;
	//关闭数据流
	virtual void Close(void);

	virtual void Write(void* buf, size_t count);

	String GetFileName(){return mFilename;}

	void GetAllBuffer( std::vector<BYTE> &datas );

protected:

	FILE*  m_pFile;

	String mFilename;

};

DataStream *OpenDataStream( const String &file );

//关闭流
void CloseDataStream( DataStream *&pStream );