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

#ifndef EDITORMODULEFACTORY_H
#define EDITORMODULEFACTORY_H

#include <string>

class EditorModule;

class EditorModuleFactoryBase
{
	public:
		EditorModuleFactoryBase();
		virtual ~EditorModuleFactoryBase();
		virtual EditorModule *createInstance() = 0;
		virtual const std::string &getName() = 0;
};

template <class T>
class EditorModuleFactory : public EditorModuleFactoryBase
{
	public:
		virtual EditorModule *createInstance ()
		{
			return new T;
		}
		virtual const std::string &getName ()
		{
			return T::FACTORY_NAME;
		}
};

#endif // EDITORMODULEFACTORY_H
