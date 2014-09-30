#ifndef _STENCRYPT_H
#define _STENCRYPT_H

class stEncrypt
{
public:
	enum enEncryptType
	{
		ENCRYPT_TYPE_NONE,
		ENCRYPT_TYPE_RC5,
		ENCRYPT_TYPE_UNKNOW
	};
	
	stEncrypt();

	~stEncrypt(){}
	
	void SetEncryptType(enEncryptType type);

	enEncryptType GetEncryptType() const;
	
	void RC5SetKey(const unsigned char * _key, int key_len);
	
	void Encode(unsigned char  * data, unsigned int data_len);
	
	void Decode(unsigned char  * data, unsigned int data_len);

private:
	enEncryptType m_stEncryptType;
};

#endif
