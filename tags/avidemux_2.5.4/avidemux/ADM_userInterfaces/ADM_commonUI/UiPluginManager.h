/***************************************************************************
                               UiPluginManager.h
                               -----------------

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

#ifndef UI_PLUGIN_MANAGER
#define UI_PLUGIN_MANAGER

#include <list>
#include <map>
#include <set>
#include <string>

class UiPluginManager
{
public:
	enum PluginType
    {
        PLUGINTYPE_VIDEO_ENCODER = 1
    };

	typedef struct
	{
		std::string id;
		bool enabled;
		bool isDefault;
	} UiPlugInfo;

	UiPluginManager(void);

	void addPlugin(PluginType type, const char* id, bool enabled, bool isDefault);
	void clearAll(void);
	void save(void);
	std::list<UiPlugInfo> getRankedList(PluginType type, std::set<std::string> availablePluginIds);

private:
	std::map< PluginType, std::list<UiPlugInfo> > _pluginList;
	bool _loaded;

	void addPluginInfoListToXml(void *xmlNode, std::list<UiPlugInfo> pluginList);
	void addXmlToPluginInfoList(void *xmlNode, PluginType type);
	void load(void);
};

#endif