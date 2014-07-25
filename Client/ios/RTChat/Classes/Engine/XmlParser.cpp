#include "XmlParser.h"
#include "DataStream.h"
#include <istream>
#include <vector>


XmlDocument& XmlSerializer::Parse(const String& xmlFileName)
{
    
    DataStream *stream = OpenDataStream( xmlFileName );

	Parse(stream);
    
    CloseDataStream( stream );
	return mDoc;
}

XmlDocument& XmlSerializer::Parse(DataStream* stream)
{
	if (stream && stream->Size())
	{
		std::vector<BYTE> heap;
		heap.resize(stream->Size());
		stream->Read(&heap[0], heap.size());
		
		mDoc.Parse((const char*)&heap[0]);
	}
	return mDoc;
}

bool XmlSerializer::Write(const String& xmlFileName)
{
	return mDoc.SaveFile(xmlFileName.c_str());
}

bool XmlSerializer::Write(XmlDocument* xmlDoc, const String& xmlFileName)
{
	if (xmlDoc)
	{
		xmlDoc->SaveFile(xmlFileName.c_str());
		return true;
	}
	return false;
}

XmlDocument& XmlSerializer::ParseOrCreateXmlDocument(const String& xmlFileName)
{
	Parse(xmlFileName);

	mDoc.InsertEndChild(TiXmlDeclaration("1.0", "gb3212", ""));

	return mDoc;
}	


bool XmlQueryContent(const XmlElement* node, String& dest)
{
	if (node)
	{
		const XmlNode* child = node->FirstChild();
		while (child)
		{
			const XmlText* textNode = node->ToText();
			if (textNode)
			{
				const char* str = textNode->Value();
				if (str)
				{
					dest = str;
					return true;
				}
				return false;
			}

			child = child->NextSibling();
		}
	}
	return false;
}

bool XmlHasAttrib(const XmlElement* node,const String& attrib)
{
	return node && node->Attribute(attrib.c_str());
}
bool XmlQueryAttrib(const XmlElement* node, const String& attrib, String& val)
{
	const char* str = 0;
	if (node && (str = node->Attribute(attrib.c_str())))
	{
		val = str;
		return true;
	}
	return false;
}
bool XmlQueryAttrib(const XmlElement* node, const String& attrib, DWORD& val)
{
	const char* str = 0;
	if (node && (str = node->Attribute(attrib.c_str())))
	{
		val = atoi(str);
		return true;
	}
	return false;
}
bool XmlQueryAttrib(const XmlElement* node, const String& attrib, WORD& val)
{
	const char* str = 0;
	if (node && (str = node->Attribute(attrib.c_str())))
	{
		val = atoi(str);
		return true;
	}
	return false;
}
bool XmlQueryColorAttrib(const XmlElement *node, const String &attrib, DWORD &color)
{
	String strVal;
	if (XmlQueryAttrib(node, attrib, strVal))
	{
		std::vector<String> colors = StringUtil::split(strVal,", ");
		color = ARGB(StringUtil::parseUnsignedInt(colors[0]),
			StringUtil::parseUnsignedInt(colors[1]),
			StringUtil::parseUnsignedInt(colors[2]),
			StringUtil::parseUnsignedInt(colors[3]));
		return true;
	}
	return false;
}
bool XmlQueryAttrib(const XmlElement *node, const String &attrib, RectF &rect)
{
	String strVal;
	if (XmlQueryAttrib(node, attrib, strVal))
	{
	    std::vector<String> vec = StringUtil::split(strVal, ", ");
		rect.left = StringUtil::parseFloat(vec[0]);
		rect.top = StringUtil::parseFloat(vec[1]);
		rect.right = rect.left + StringUtil::parseFloat(vec[2]);
		rect.bottom = rect.top + StringUtil::parseFloat(vec[3]);
		return true;
	}
	return false;
}
bool XmlQueryAttrib(const XmlElement *node, const String &attrib, PointF &position)
{
	String strVal;
	if (XmlQueryAttrib(node, attrib, strVal))
	{
	    std::vector<String> vec = StringUtil::split(strVal, ", ");
		position.x = StringUtil::parseInt(vec[0]);
		position.y = StringUtil::parseInt(vec[1]);
		return true;
	}
	return false;
}

//2012-12-30
bool XmlQueryAttrib(const XmlElement *node, const String &attrib, tm &date,BYTE num)
{
	String strVal;
	if (XmlQueryAttrib(node, attrib, strVal))
	{
		std::vector<String> vec = StringUtil::split(strVal, "- ");
		if (vec.size() == num)
		{
			date.tm_year = StringUtil::parseInt(vec[0]) - 1900;
			date.tm_mon = StringUtil::parseInt(vec[1]) - 1;
			date.tm_mday = StringUtil::parseInt(vec[2]);
			return true;
		}		
	}
	return false;
}
TiXmlElement* XmlSetAttrib(XmlElement* node, const String& attrib, const char* value)
{
	assert(node);
	node->SetAttribute(attrib.c_str(), value);
	return node;
}

TiXmlElement* XmlFindOrAddChildElement(XmlNode* parent, const String& nodeName)
{
	assert(parent);
	TiXmlElement* node = parent->FirstChildElement(nodeName.c_str());
	if (!node)
	{
		node = parent->InsertEndChild(TiXmlElement(nodeName.c_str()))->ToElement();
	}
	return node;
}
TiXmlElement* XmlFindOrAddChildElementMatched(XmlNode* parent, const String& nodeName,
	const String& attribName, const String& attribValue)
{
	assert(parent);

	TiXmlElement* child = parent->FirstChildElement(nodeName.c_str());
	String value;
	while (child)
	{
		if (XmlQueryAttrib(child, attribName, value)
			&& value == attribValue)
		{
			return child;
		}

		child = child->NextSiblingElement(nodeName.c_str());
	}
	child = parent->InsertEndChild(TiXmlElement(nodeName.c_str()))->ToElement();
	if (child)
	{
		child->SetAttribute(attribName.c_str(), attribValue.c_str());
	}
	return child;
}

TiXmlElement* XmlGetMatchChildElement(XmlNode* parent, const String& nodeName,
	const String& attribName, const String& attribValue)
{
	assert(parent);

	TiXmlElement* child = parent->FirstChildElement(nodeName.c_str());
	String value;
	while (child)
	{
		if (XmlQueryAttrib(child, attribName, value)
			&& value == attribValue)
		{
			return child;
		}

		child = child->NextSiblingElement(nodeName.c_str());
	}

	return NULL;
}