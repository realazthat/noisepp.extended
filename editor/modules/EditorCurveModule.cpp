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

#include "EditorCurveModule.h"

std::string EditorCurveModule::FACTORY_NAME = "Curve";

EditorCurveModule::EditorCurveModule() : EditorModule(1)
{
}

void EditorCurveModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module"), mSourceModules[0]);
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
}

void EditorCurveModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
}

bool EditorCurveModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;



	return valid;
}

void EditorCurveModule::writeProperties (TiXmlElement *element)
{
}

bool EditorCurveModule::readProperties (TiXmlElement *element)
{
	return true;
}
