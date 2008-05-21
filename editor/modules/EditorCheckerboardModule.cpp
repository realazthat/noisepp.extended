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

#include "EditorCheckerboardModule.h"

std::string EditorCheckerboardModule::FACTORY_NAME = "Checkerboard";

EditorCheckerboardModule::EditorCheckerboardModule() : EditorModule(0)
{
}

void EditorCheckerboardModule::fillPropertyGrid (wxPropertyGrid *pg)
{
}

void EditorCheckerboardModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
}

bool EditorCheckerboardModule::validate (wxPropertyGrid *pg)
{
	return true;
}

void EditorCheckerboardModule::writeProperties (TiXmlElement *element)
{
}

bool EditorCheckerboardModule::readProperties (TiXmlElement *element)
{
	return true;
}
