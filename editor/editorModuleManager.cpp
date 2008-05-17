// This file is part of the Noise++ Editor.
// Copyright (c) 2008, Urs C. Hanselmann
//
// The Noise++ Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Noise++ Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Noise++ Editor.  If not, see <http://www.gnu.org/licenses/>.
//

#include <cassert>
#include "editorModuleManager.h"
#include "modules/EditorPerlinModule.h"

EditorModuleManager::EditorModuleManager()
{
	registerFactory(new EditorModuleFactory<EditorPerlinModule>);
}

EditorModuleManager::~EditorModuleManager()
{
	clearModules ();
	for (FactoryMap::iterator it=mFactories.begin();it!=mFactories.end();++it)
	{
		delete it->second;
	}
	mFactories.clear ();
}

EditorModuleManager &EditorModuleManager::getInstance ()
{
	static EditorModuleManager instance;
	return instance;
}

void EditorModuleManager::registerFactory (EditorModuleFactoryBase *factory)
{
	std::string name = factory->getName();
	assert (mFactories.find(name) == mFactories.end());
	mFactories.insert (std::make_pair(name, factory));
}

bool EditorModuleManager::moduleExists (const std::string &name) const
{
	return (mModules.find(name) != mModules.end());
}

EditorModule *EditorModuleManager::getModule (const std::string &name) const
{
	ModuleMap::const_iterator it=mModules.find(name);
	if (it != mModules.end())
		return it->second;
	else
		return 0;
}

bool EditorModuleManager::createModule (const std::string &name, const std::string &type)
{
	if (mModules.find(name) != mModules.end())
		return false;

	FactoryMap::iterator fact=mFactories.find(type);
	if (fact == mFactories.end())
		return false;

	EditorModule *module = fact->second->createInstance ();
	mModules.insert(std::make_pair(name, module));

	return true;
}

void EditorModuleManager::fillFactoryArray (wxArrayString &arr)
{
	for (FactoryMap::iterator it=mFactories.begin();it!=mFactories.end();++it)
	{
		arr.Add(wxString(it->first.c_str(), wxConvUTF8));
	}
}

void EditorModuleManager::fillModuleArray (wxArrayString &arr)
{
	for (ModuleMap::iterator it=mModules.begin();it!=mModules.end();++it)
	{
		arr.Add(wxString(it->first.c_str(), wxConvUTF8));
	}
}

void EditorModuleManager::removeModule (const std::string &name)
{
	ModuleMap::iterator it=mModules.find(name);
	if (it != mModules.end())
	{
		delete it->second;
		mModules.erase (it);
	}
}

bool EditorModuleManager::renameModule (const std::string &oldName, const std::string &newName)
{
	ModuleMap::iterator it=mModules.find(oldName);
	if (it == mModules.end())
		return false;

	if (mModules.find(newName) != mModules.end())
		return false;

	EditorModule *module = it->second;
	mModules.erase (it);
	mModules.insert (std::make_pair(newName, module));
	return true;
}

void EditorModuleManager::clearModules ()
{
	for (ModuleMap::iterator it=mModules.begin();it!=mModules.end();++it)
	{
		delete it->second;
	}
	mModules.clear ();
}

void EditorModuleManager::writeToDoc (TiXmlElement *root)
{
	TiXmlElement *modules = new TiXmlElement ("Modules");
	for (ModuleMap::iterator it=mModules.begin();it!=mModules.end();++it)
	{
		TiXmlElement *module = new TiXmlElement ("Module");
		module->SetAttribute ("name", it->first);
		module->SetAttribute ("type", it->second->getFactoryName());
		it->second->writeProperties (module);
		modules->LinkEndChild(module);
	}
	root->LinkEndChild (modules);
}

bool EditorModuleManager::readDoc (TiXmlElement *root)
{
	assert (mModules.empty());
	TiXmlElement *modules = root->FirstChildElement("Modules");
	if (!modules)
		return false;

	TiXmlNode *child = 0;
	while( (child = modules->IterateChildren( "Module", child )) != NULL )
	{
		TiXmlElement *moduleElement = child->ToElement();
		if (moduleElement)
		{
			const char *name = moduleElement->Attribute ("name");
			const char *type = moduleElement->Attribute ("type");
			if (name == NULL || type == NULL)
				return false;
			if (!createModule (name, type))
				return false;
			EditorModule *module = getModule(name);
			assert (module);
			if (!module->readProperties (moduleElement))
				return false;
		}
	}

	return true;
}
