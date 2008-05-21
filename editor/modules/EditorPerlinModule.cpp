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

#include "EditorPerlinModule.h"

std::string EditorPerlinModule::FACTORY_NAME = "Perlin";

void EditorPerlinModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxFloatProperty(wxT("Frequency"), wxPG_LABEL, mModule.getFrequency()) );
	pg->Append( wxFloatProperty(wxT("Lacunarity"), wxPG_LABEL, mModule.getLacunarity()) );
	pg->Append( wxFloatProperty(wxT("Persistence"), wxPG_LABEL, mModule.getPersistence()) );
	pg->Append( wxFloatProperty(wxT("Scale"), wxPG_LABEL, mModule.getScale()) );
	pg->Append( wxIntProperty(wxT("Octaves"), wxPG_LABEL, mModule.getOctaveCount()) );
	pg->Append( wxIntProperty(wxT("Seed"), wxPG_LABEL, mModule.getSeed()) );
	appendQualityProperty (pg, mModule.getQuality());
}

void EditorPerlinModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	prop = new TiXmlElement ("Frequency");
	prop->SetDoubleAttribute ("value", mModule.getFrequency());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Lacunarity");
	prop->SetDoubleAttribute ("value", mModule.getLacunarity());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Persistence");
	prop->SetDoubleAttribute ("value", mModule.getPersistence());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Scale");
	prop->SetDoubleAttribute ("value", mModule.getScale());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Seed");
	prop->SetAttribute ("value", mModule.getSeed());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Octaves");
	prop->SetAttribute ("value", mModule.getOctaveCount());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Quality");
	prop->SetAttribute ("value", mModule.getQuality());
	element->LinkEndChild (prop);
}

bool EditorPerlinModule::readProperties (TiXmlElement *element)
{
	double dval;
	int ival;
	TiXmlElement *prop;

	prop = element->FirstChildElement ("Frequency");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setFrequency (dval);

	prop = element->FirstChildElement ("Lacunarity");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setLacunarity (dval);

	prop = element->FirstChildElement ("Persistence");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setPersistence (dval);

	prop = element->FirstChildElement ("Seed");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule.setSeed (ival);

	prop = element->FirstChildElement ("Octaves");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule.setOctaveCount (ival);

	prop = element->FirstChildElement ("Quality");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule.setQuality (ival);

	prop = element->FirstChildElement ("Scale");
	if (prop != NULL)
	{
		if (prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
			return false;
		mModule.setScale (dval);
	}

	return true;
}

void EditorPerlinModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Frequency"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setFrequency (val);
	}
	else if (name == _("Lacunarity"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setLacunarity (val);
	}
	else if (name == _("Persistence"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setPersistence (val);
	}
	else if (name == _("Scale"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setScale (val);
	}
	else if (name == _("Octaves"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mModule.setOctaveCount (val);
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

bool EditorPerlinModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	valid = setValid (pg, "Octaves", mModule.getOctaveCount() > 0 && mModule.getOctaveCount() <= 30) && valid;
	valid = setValid (pg, "Frequency", mModule.getFrequency() > 0) && valid;
	valid = setValid (pg, "Lacunarity", mModule.getLacunarity() > 0) && valid;
	valid = setValid (pg, "Persistence", mModule.getPersistence() > 0) && valid;
	return valid;
}
