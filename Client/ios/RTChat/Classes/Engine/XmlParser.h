#pragma once
#include "public.h"
#include "Rect.h"
#include "Vector2.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "StringUtil.h"


typedef TiXmlBase         XmlBase;
typedef TiXmlNode         XmlNode;
typedef TiXmlElement      XmlElement;
typedef TiXmlComment      XmlComment;
typedef TiXmlDocument     XmlDocument;
typedef TiXmlDeclaration  XmlDeclaration;
typedef TiXmlText         XmlText;

/**xml文件的解析与输入输出
*/
class DataStream;
class XmlSerializer
{
public:
		XmlDocument& Parse(const String& xmlFileName);
		XmlDocument& Parse(DataStream* stream);
		XmlDocument& ParseOrCreateXmlDocument(const String& xmlFileName);
		bool Write(const String& xmlFileName = "");

static bool Write(XmlDocument* xmlDoc, const String& xmlFileName = "");

private:
	XmlDocument mDoc;
};//XmlSerializer

///获取node的content
bool XmlQueryContent(const XmlElement* node, String& dest);

///判断一个element节点是否有某个属性
bool XmlHasAttrib(const XmlElement* node,const String& attrib);
///获取node的某个属性
bool XmlQueryAttrib(const XmlElement* node, const String& attrib, String& val);
bool XmlQueryAttrib(const XmlElement* node, const String& attrib, DWORD& val);
bool XmlQueryAttrib(const XmlElement* node, const String& attrib, WORD& val);
bool XmlQueryAttrib(const XmlElement *node, const String &attrib, RectF &rect);
bool XmlQueryAttrib(const XmlElement *node, const String &attrib, PointF &position);
bool XmlQueryAttrib(const XmlElement *node, const String &attrib, tm &date,BYTE num = 3);
bool XmlQueryColorAttrib(const XmlElement *node, const String &attrib, DWORD &color);
///获取node的某个属性
template<typename tValue>
inline bool XmlQueryAttrib(const XmlElement* node, const String& attrib, tValue& val)
{
	String strVal;
	if (XmlQueryAttrib(node, attrib, strVal))
	{
		val = (tValue)StringUtil::parseInt( strVal );

		return true;
	}
	return false;
}
///获取node的某进制性属性
template<typename tValue,unsigned int radix>
inline bool XmlQueryAttribHex(const XmlElement* node, const String& attrib, tValue& val)
{
	String strVal;
	if (XmlQueryAttrib(node, attrib, strVal))
	{
		std::istringstream istr(strVal);
		istr.setf(std::ios::basefield);
		istr >> val;
		istr.setf(std::ios::dec,std::ios::basefield);
		return true;
	}
	return false;
}

///设置某节点的属性值（没有相关属性则加入新属性）
TiXmlElement* XmlSetAttrib(XmlElement* node, const String& attrib, const char* value);
///设置某节点的属性值（没有相关属性则加入新属性）
inline TiXmlElement* XmlSetAttrib(XmlElement* node, const String& attrib, const String& value)
{
	return XmlSetAttrib(node, attrib, value.c_str());
}
///设置某节点的属性值（没有相关属性则加入新属性）
inline TiXmlElement* XmlSetAttrib(XmlElement* node, const String& attrib, DWORD value)
{
	return XmlSetAttrib(node, attrib, avar("%u", value) );
}
inline TiXmlElement* XmlSetAttrib(XmlElement* node, const String& attrib, WORD value)
{
	return XmlSetAttrib(node, attrib, avar("%u", value)  );
}
inline TiXmlElement* XmlSetColorAttrib(XmlElement* node, const String& attrib, DWORD &value)
{
	return XmlSetAttrib(node, attrib,avar("%u %u %u %u",GETA(value),GETR(value),GETG(value),GETB(value)) );
}
template <typename T>
inline TiXmlElement* XmlSetAttrib(XmlElement* node, const String& attrib, Vector2<T> &value)
{
	return XmlSetAttrib(node, attrib,StringUtil::toString(value) );
}
inline TiXmlElement* XmlSetAttrib(XmlElement* node, const String& attrib, RectF &value)
{
	return XmlSetAttrib(node, attrib,StringUtil::toString(value) );
}

///获取或者加入一个element节点
TiXmlElement* XmlFindOrAddChildElement(XmlNode* parent, const String& nodeName);

///获取或者加入一个匹配指定属性的element节点
TiXmlElement* XmlFindOrAddChildElementMatched(XmlNode* parent, const String& nodeName, const String& attribName, const String& attribValue);

TiXmlElement* XmlGetMatchChildElement(XmlNode* parent, const String& nodeName, const String& attribName, const String& attribValue);