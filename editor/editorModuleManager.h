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

#ifndef EDITORMODULEMANAGER_H
#define EDITORMODULEMANAGER_H

#include <map>
#include <string>
#include <wx/arrstr.h>
#include <wx/string.h>
#include <xml/tinyxml.h>

#include "editorModuleFactory.h"
#include "editorModule.h"

class EditorModuleManager
{
	private:
		typedef std::map<std::string, EditorModuleFactoryBase*> FactoryMap;
		typedef std::map<std::string, EditorModule*> ModuleMap;

		FactoryMap mFactories;
		ModuleMap mModules;

	public:
		EditorModuleManager();
		~EditorModuleManager();
		static EditorModuleManager &getInstance ();

		void registerFactory (EditorModuleFactoryBase *factory);
		bool moduleExists (const std::string &name) const;
		EditorModule *getModule (const std::string &name) const;
		bool createModule (const std::string &name, const std::string &type);
		void removeModule (const std::string &name);
		bool renameModule (const std::string &oldName, const std::string &newName);
		void clearModules ();

		bool moduleExists (const wxString &name) const
		{ return moduleExists (std::string(name.mb_str())); }
		EditorModule *getModule (const wxString &name) const
		{ return getModule (std::string(name.mb_str())); }
		bool createModule (const wxString &name, const wxString &type)
		{ return createModule (std::string(name.mb_str()), std::string(type.mb_str())); }
		void removeModule (const wxString &name)
		{ removeModule (std::string(name.mb_str())); }
		bool renameModule (const wxString &oldName, const wxString &newName)
		{ return renameModule (std::string(oldName.mb_str()), std::string(newName.mb_str())); }

		void fillFactoryArray (wxArrayString &arr);
		void fillModuleArray (wxArrayString &arr);
		int fillModuleArrayWithException (wxArrayString &arr, EditorModule *exception, const wxString &defaultValue = wxEmptyString);

		void writeToDoc (TiXmlElement *root);
		bool readDoc (TiXmlElement *root);

		void freeChildrenData (EditorModule *module);
};

#endif // EDITORMODULEMANAGER_H
