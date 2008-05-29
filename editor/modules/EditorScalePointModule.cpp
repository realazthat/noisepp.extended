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

#include "EditorScalePointModule.h"

std::string EditorScalePointModule::FACTORY_NAME = "ScalePoint";

EditorScalePointModule::EditorScalePointModule() : EditorModule(1)
{
}

void EditorScalePointModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module"), mSourceModules[0]);
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxFloatProperty(wxT("Scale X"), wxPG_LABEL, mModule.getScaleX()) );
	pg->Append( wxFloatProperty(wxT("Scale Y"), wxPG_LABEL, mModule.getScaleY()) );
	pg->Append( wxFloatProperty(wxT("Scale Z"), wxPG_LABEL, mModule.getScaleZ()) );
}

void EditorScalePointModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Source module"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[0] = val;
	}
	else if (name == _("Scale X"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setScaleX (val);
	}
	else if (name == _("Scale Y"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setScaleY (val);
	}
	else if (name == _("Scale Z"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setScaleZ (val);
	}
}

bool EditorScalePointModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	EditorModule *module = NULL;

	module = getSourceModule(0);
	if (module && module->validateTree(this))
	{
		mModule.setSourceModule(0, module->getModule());
	}
	valid = setValid (pg, "Source module", module != NULL && module->validateTree(this) && module->validate(NULL)) && valid;

	return valid;
}

void EditorScalePointModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	writeSourceModules (element);

	prop = new TiXmlElement ("ScaleX");
	prop->SetDoubleAttribute ("value", mModule.getScaleX());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("ScaleY");
	prop->SetDoubleAttribute ("value", mModule.getScaleY());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("ScaleZ");
	prop->SetDoubleAttribute ("value", mModule.getScaleZ());
	element->LinkEndChild (prop);
}

bool EditorScalePointModule::readProperties (TiXmlElement *element)
{
	TiXmlElement *prop;
	double dval;

	if (!readSourceModules (element))
		return false;

	prop = element->FirstChildElement ("ScaleX");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setScaleX (dval);

	prop = element->FirstChildElement ("ScaleY");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setScaleY (dval);

	prop = element->FirstChildElement ("ScaleZ");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setScaleZ (dval);

	return true;
}
