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

#include "EditorConstantModule.h"

std::string EditorConstantModule::FACTORY_NAME = "Constant";

EditorConstantModule::EditorConstantModule() : EditorModule(0)
{
}

void EditorConstantModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxFloatProperty(wxT("Value"), wxPG_LABEL, mModule.getValue()) );
}

void EditorConstantModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Value"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setValue (val);
	}
}

bool EditorConstantModule::validate (wxPropertyGrid *pg)
{
	return true;
}

void EditorConstantModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	prop = new TiXmlElement ("Value");
	prop->SetDoubleAttribute ("value", mModule.getValue());
	element->LinkEndChild (prop);
}

bool EditorConstantModule::readProperties (TiXmlElement *element)
{
	double dval;
	TiXmlElement *prop;

	prop = element->FirstChildElement ("Value");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setValue (dval);

	return true;
}
