/***************************************************************************
                             UiPluginManager.cpp
                             -------------------

    begin                : Tue June 15 2010
    copyright            : (C) 2010 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <libxml/parser.h>
#include <string.h>

#include "prefs.h"
#include "UiPluginManager.h"

UiPluginManager::UiPluginManager(void)
{
	_pluginList[PLUGINTYPE_VIDEO_ENCODER] = std::list<UiPlugInfo>();
	_loaded = false;
}

void UiPluginManager::addPlugin(PluginType type, const char* id, bool enabled, bool isDefault)
{
	UiPlugInfo info;

	info.id = id;
	info.enabled = enabled;
	info.isDefault = isDefault;

	_pluginList[type].push_back(info);
}

void UiPluginManager::clearAll(void)
{
	_pluginList[PLUGINTYPE_VIDEO_ENCODER].clear();
}

void UiPluginManager::load(void)
{
	char* xml;
	preferences prefs;

	clearAll();
	prefs.get(PLUGIN_ORDER, &xml);

	if (strlen(xml) > 0)
	{
		xmlDocPtr xmlDoc = xmlReadMemory(xml, strlen(xml), "pluginOrder.xml", NULL, 0);
		xmlNodePtr xmlNodeRoot = xmlDocGetRootElement(xmlDoc);

		for (xmlNodePtr xmlChild = xmlNodeRoot->children; xmlChild; xmlChild = xmlChild->next)
		{
			if (strcmp((char*)xmlChild->name, "videoEncoder") == 0)
			{
				for (xmlNodePtr xmlPlugin = xmlChild->children; xmlPlugin; xmlPlugin = xmlPlugin->next)
					addXmlToPluginInfoList(xmlPlugin, PLUGINTYPE_VIDEO_ENCODER);
			}
		}

		xmlFreeDoc(xmlDoc);
	}

	ADM_dealloc(xml);
	_loaded = true;
}

std::list<UiPluginManager::UiPlugInfo> UiPluginManager::getRankedList(PluginType type, std::set<std::string> availablePluginIds)
{
	if (!_loaded)
		load();

	std::set<std::string> idSet(availablePluginIds);
	std::list<UiPlugInfo> rankedList;
	std::list<UiPlugInfo> pluginList = _pluginList[type];

	for (std::list<UiPlugInfo>::const_iterator itPlugin = pluginList.begin(); itPlugin != pluginList.end(); itPlugin++)
	{
		std::string id = itPlugin->id;
		std::set<std::string>::const_iterator itId = idSet.find(id);

		if (itId != idSet.end())
		{
			UiPlugInfo info;

			info.id = id;
			info.enabled = itPlugin->enabled;
			info.isDefault = itPlugin->isDefault;

			rankedList.push_back(info);
			idSet.erase(itId);
		}
	}

	for (std::set<std::string>::const_iterator it = idSet.begin(); it != idSet.end(); it++)
	{
		UiPlugInfo info;

		info.id = *it;
		info.enabled = true;
		info.isDefault = false;

		rankedList.push_back(info);
	}

	return rankedList;
}

void UiPluginManager::save(void)
{
	xmlDocPtr xmlDoc = xmlNewDoc((const xmlChar*)"1.0");
	xmlNodePtr xmlNodeRoot = xmlNewDocNode(xmlDoc, NULL, (const xmlChar*)"pluginOrder", NULL);
	xmlNodePtr xmlNodeGroup;

	xmlDocSetRootElement(xmlDoc, xmlNodeRoot);

	xmlNodeGroup = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"videoEncoder", NULL);
	addPluginInfoListToXml(xmlNodeGroup, _pluginList[PLUGINTYPE_VIDEO_ENCODER]);

	xmlChar *tempBuffer;
	int tempBufferSize;

	xmlDocDumpMemory(xmlDoc, &tempBuffer, &tempBufferSize);

	preferences prefs;

	prefs.set(PLUGIN_ORDER, (char*)tempBuffer);

	xmlFree(tempBuffer);
	xmlFreeDoc(xmlDoc);
}

void UiPluginManager::addPluginInfoListToXml(void *xmlNode, std::list<UiPlugInfo> pluginList)
{
	for (std::list<UiPlugInfo>::const_iterator it = pluginList.begin(); it != pluginList.end(); it++)
	{
		xmlNodePtr xmlNodeChild = xmlNewChild((xmlNodePtr)xmlNode, NULL, (const xmlChar*)"plugin", NULL);

		xmlNewProp(xmlNodeChild, (const xmlChar*)"id", (const xmlChar*)it->id.c_str());
		xmlNewProp(xmlNodeChild, (const xmlChar*)"enabled", it->enabled ? (const xmlChar*)"true" : (const xmlChar*)"false");
		xmlNewProp(xmlNodeChild, (const xmlChar*)"default", it->isDefault ? (const xmlChar*)"true" : (const xmlChar*)"false");
	}
}

void UiPluginManager::addXmlToPluginInfoList(void *xmlNode, PluginType type)
{
	const char *id = NULL;
	bool enabled = true, isDefault = false;

	for (xmlAttrPtr xmlAttr = ((xmlNodePtr)xmlNode)->properties; xmlAttr; xmlAttr = xmlAttr->next)
	{
		xmlChar *attrContent = xmlNodeGetContent(xmlAttr->children);

		if (strcmp((const char*)xmlAttr->name, "id") == 0 && !id)
			id = ADM_strdup((const char*)attrContent);
		else if (strcmp((const char*)xmlAttr->name, "enabled") == 0)
			enabled = (strcmp((const char*)attrContent, "true") == 0);
		else if (strcmp((const char*)xmlAttr->name, "default") == 0)
			isDefault = (strcmp((const char*)attrContent, "true") == 0);

		xmlFree(attrContent);
	}

	this->addPlugin(type, id, enabled, isDefault);

	if (id)
		delete [] id;
}
