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

#include "EditorTurbulenceModule.h"

std::string EditorTurbulenceModule::FACTORY_NAME = "Turbulence";

EditorTurbulenceModule::EditorTurbulenceModule() : EditorModule(1)
{
}

void EditorTurbulenceModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module"), mSourceModules[0]);
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxFloatProperty(wxT("Power"), wxPG_LABEL, mModule.getPower()) );
	pg->Append( wxFloatProperty(wxT("Frequency"), wxPG_LABEL, mModule.getFrequency()) );
	pg->Append( wxIntProperty(wxT("Roughness"), wxPG_LABEL, mModule.getRoughness()) );
	pg->Append( wxIntProperty(wxT("Seed"), wxPG_LABEL, mModule.getSeed()) );
	appendQualityProperty (pg, mModule.getQuality());
}

void EditorTurbulenceModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Source module"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[0] = val;
	}
	else if (name == _("Power"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setPower (val);
	}
	else if (name == _("Frequency"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setFrequency (val);
	}
	else if (name == _("Roughness"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mModule.setRoughness (val);
	}
	else if (name == _("Seed"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mModule.setSeed (val);
	}
	else if (name == _("Quality"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mModule.setQuality (val);
	}
}

bool EditorTurbulenceModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	EditorModule *module = NULL;

	module = getSourceModule(0);
	if (module && module->validateTree(this))
	{
		mModule.setSourceModule(0, module->getModule());
	}
	valid = setValid (pg, "Source module", module != NULL && module->validateTree(this) && module->validate(NULL)) && valid;

	valid = setValid (pg, "Power", mModule.getPower() >= 0) && valid;
	valid = setValid (pg, "Roughness", mModule.getRoughness() >= 0) && valid;
	valid = setValid (pg, "Frequency", mModule.getFrequency() > 0) && valid;

	return valid;
}

void EditorTurbulenceModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	writeSourceModules (element);

	prop = new TiXmlElement ("Power");
	prop->SetDoubleAttribute ("value", mModule.getPower());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Roughness");
	prop->SetDoubleAttribute ("value", mModule.getRoughness());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Frequency");
	prop->SetDoubleAttribute ("value", mModule.getFrequency());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Seed");
	prop->SetAttribute ("value", mModule.getSeed());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Quality");
	prop->SetAttribute ("value", mModule.getQuality());
	element->LinkEndChild (prop);
}

bool EditorTurbulenceModule::readProperties (TiXmlElement *element)
{
	TiXmlElement *prop;
	double dval;
	int ival;

	if (!readSourceModules (element))
		return false;

	prop = element->FirstChildElement ("Power");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setPower (dval);

	prop = element->FirstChildElement ("Frequency");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setFrequency (dval);

	prop = element->FirstChildElement ("Roughness");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule.setRoughness (ival);

	prop = element->FirstChildElement ("Seed");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule.setSeed (ival);

	prop = element->FirstChildElement ("Quality");
	if (prop != NULL)
	{
		if (prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
			return false;
		mModule.setQuality (ival);
	}

	return true;
}
