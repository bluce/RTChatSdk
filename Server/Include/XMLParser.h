#ifndef _ST_XMLPARSER_H
#define _ST_XMLPARSER_H

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string>


class stXMLParser
{
 public:
	stXMLParser();

	~stXMLParser();

	bool InitFile(const char * file_name);

	bool InitStr(const char * str_content);

   	bool GetXMLContent(std::string & out_content);

	bool GetNodeContent(xmlNodePtr p_node, std::string & out_content, bool head=true);

	xmlNodePtr GetRootNode(const char * root_name);

	xmlNodePtr GetChildNode(const xmlNodePtr p_parent, const char *child_name , unsigned int nIdx = 1);

	xmlNodePtr GetNextNode(const xmlNodePtr p_node, const char *next_name = NULL);

	bool GetNodeNum(const xmlNodePtr p_node, const char *sub_name, void *ret_value, const int ret_size);

	bool GetNodeChar(const xmlNodePtr p_node, const char *sbu_name, void *ret_value, const int ret_size);

	bool GetNodeStr(const xmlNodePtr p_node, const char *sbu_name, std::string & ret_value);

	const char * GetFilename() const;

	static char * UTF8toANSI(const std::string & from);
	
private:

	void Final();

	xmlDocPtr m_doc;
	
	std::string m_strFilename;
};

#endif
