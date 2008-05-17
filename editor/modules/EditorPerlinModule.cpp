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
	pg->Append( wxFloatProperty(wxT("Frequency"), wxPG_LABEL, mModule3D.getFrequency()) );
	pg->Append( wxFloatProperty(wxT("Lacunarity"), wxPG_LABEL, mModule3D.getLacunarity()) );
	pg->Append( wxFloatProperty(wxT("Persistence"), wxPG_LABEL, mModule3D.getPersistence()) );
	pg->Append( wxIntProperty(wxT("Octaves"), wxPG_LABEL, mModule3D.getOctaveCount()) );
	pg->Append( wxIntProperty(wxT("Seed"), wxPG_LABEL, mModule3D.getSeed()) );

	wxArrayString qualityArr;
	qualityArr.Add(wxT("Low"));
	qualityArr.Add(wxT("Default"));
	qualityArr.Add(wxT("High"));

	wxArrayInt arrIds;
	arrIds.Add(noisepp::NOISE_QUALITY_LOW);
	arrIds.Add(noisepp::NOISE_QUALITY_STD);
	arrIds.Add(noisepp::NOISE_QUALITY_HIGH);

	pg->Append( wxEnumProperty(wxT("Quality"), wxPG_LABEL, qualityArr, arrIds, mModule3D.getQuality()) );
}

void EditorPerlinModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	prop = new TiXmlElement ("Frequency");
	prop->SetDoubleAttribute ("value", mModule3D.getFrequency());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Lacunarity");
	prop->SetDoubleAttribute ("value", mModule3D.getLacunarity());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Persistence");
	prop->SetDoubleAttribute ("value", mModule3D.getPersistence());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Seed");
	prop->SetAttribute ("value", mModule3D.getSeed());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Octaves");
	prop->SetAttribute ("value", mModule3D.getOctaveCount());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Quality");
	prop->SetAttribute ("value", mModule3D.getQuality());
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
	mModule3D.setFrequency (dval);
	mModule2D.setFrequency (dval);

	prop = element->FirstChildElement ("Lacunarity");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setLacunarity (dval);
	mModule2D.setLacunarity (dval);

	prop = element->FirstChildElement ("Persistence");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setPersistence (dval);
	mModule2D.setPersistence (dval);

	prop = element->FirstChildElement ("Seed");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule3D.setSeed (ival);
	mModule2D.setSeed (ival);

	prop = element->FirstChildElement ("Octaves");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule3D.setOctaveCount (ival);
	mModule2D.setOctaveCount (ival);

	prop = element->FirstChildElement ("Quality");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule3D.setQuality (ival);
	mModule2D.setQuality (ival);

	return true;
}

void EditorPerlinModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Frequency"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setFrequency (val);
		mModule2D.setFrequency (val);
	}
	else if (name == _("Lacunarity"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setLacunarity (val);
		mModule2D.setLacunarity (val);
	}
	else if (name == _("Persistence"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setPersistence (val);
		mModule2D.setPersistence (val);
	}
	else if (name == _("Octaves"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mModule3D.setOctaveCount (val);
		mModule2D.setOctaveCount (val);
	}
	else if (name == _("Seed"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mModule3D.setSeed (val);
		mModule2D.setSeed (val);
	}
	else if (name == _("Quality"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mModule3D.setQuality (val);
		mModule2D.setQuality (val);
	}
}

bool EditorPerlinModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	valid = setValid (pg, "Octaves", mModule3D.getOctaveCount() > 0) && valid;
	valid = setValid (pg, "Frequency", mModule3D.getFrequency() > 0) && valid;
	valid = setValid (pg, "Lacunarity", mModule3D.getLacunarity() > 0) && valid;
	valid = setValid (pg, "Persistence", mModule3D.getPersistence() > 0) && valid;
	return valid;
}
