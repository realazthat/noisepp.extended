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
	appendSourceModuleProperty (pg, wxT("Source module"), mSourceModules[0]);
	pg->Append( wxFloatProperty(wxT("Power"), wxPG_LABEL, mModule3D.getPower()) );
	pg->Append( wxFloatProperty(wxT("Roughness"), wxPG_LABEL, mModule3D.getRoughness()) );
	pg->Append( wxFloatProperty(wxT("Frequency"), wxPG_LABEL, mModule3D.getFrequency()) );
	pg->Append( wxIntProperty(wxT("Seed"), wxPG_LABEL, mModule3D.getSeed()) );
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
		mModule3D.setPower (val);
		mModule2D.setPower (val);
	}
	else if (name == _("Roughness"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setRoughness (val);
		mModule2D.setRoughness (val);
	}
	else if (name == _("Frequency"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setFrequency (val);
		mModule2D.setFrequency (val);
	}
	else if (name == _("Seed"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mModule3D.setSeed (val);
		mModule2D.setSeed (val);
	}
}

bool EditorTurbulenceModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	EditorModule *module = NULL;

	module = getSourceModule(0);
	if (module)
	{
		mModule2D.setSourceModule(0, module->get2DModule());
		mModule3D.setSourceModule(0, module->get3DModule());
	}
	valid = setValid (pg, "Source module", module != NULL && module->validate(NULL)) && valid;

	valid = setValid (pg, "Power", mModule3D.getPower() >= 0) && valid;
	valid = setValid (pg, "Roughness", mModule3D.getRoughness() >= 0) && valid;
	valid = setValid (pg, "Frequency", mModule3D.getFrequency() > 0) && valid;

	return valid;
}

void EditorTurbulenceModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	writeSourceModules (element);

	prop = new TiXmlElement ("Power");
	prop->SetDoubleAttribute ("value", mModule3D.getPower());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Roughness");
	prop->SetDoubleAttribute ("value", mModule3D.getRoughness());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Frequency");
	prop->SetDoubleAttribute ("value", mModule3D.getFrequency());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Seed");
	prop->SetAttribute ("value", mModule3D.getSeed());
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
	mModule3D.setPower (dval);
	mModule2D.setPower (dval);

	prop = element->FirstChildElement ("Roughness");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setRoughness (dval);
	mModule2D.setRoughness (dval);

	prop = element->FirstChildElement ("Frequency");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setFrequency (dval);
	mModule2D.setFrequency (dval);

	prop = element->FirstChildElement ("Seed");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule3D.setSeed (ival);
	mModule2D.setSeed (ival);

	return true;
}
