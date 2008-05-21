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

#include "EditorVoronoiModule.h"

std::string EditorVoronoiModule::FACTORY_NAME = "Voronoi";

EditorVoronoiModule::EditorVoronoiModule() : EditorModule(0)
{
}

void EditorVoronoiModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxFloatProperty(wxT("Frequency"), wxPG_LABEL, mModule.getFrequency()) );
	pg->Append( wxFloatProperty(wxT("Displacement"), wxPG_LABEL, mModule.getDisplacement()) );
	pg->Append( wxIntProperty(wxT("Seed"), wxPG_LABEL, mModule.getSeed()) );
	pg->Append( wxBoolProperty(wxT("Enable distance"), wxPG_LABEL, mModule.isDistanceEnabled()) );
}

void EditorVoronoiModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Frequency"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setFrequency (val);
	}
	else if (name == _("Displacement"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setDisplacement (val);
	}
	else if (name == _("Seed"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mModule.setSeed (val);
	}
	else if (name == _("Enable distance"))
	{
		bool val = pg->GetPropertyValueAsBool (name);
		mModule.setEnableDistance (val);
	}
}

bool EditorVoronoiModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	valid = setValid (pg, "Frequency", mModule.getFrequency() > 0) && valid;
	return valid;
}

void EditorVoronoiModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	prop = new TiXmlElement ("Frequency");
	prop->SetDoubleAttribute ("value", mModule.getFrequency());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Displacement");
	prop->SetDoubleAttribute ("value", mModule.getDisplacement());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Seed");
	prop->SetAttribute ("value", mModule.getSeed());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("EnableDistance");
	prop->SetAttribute ("value", mModule.isDistanceEnabled());
	element->LinkEndChild (prop);
}

bool EditorVoronoiModule::readProperties (TiXmlElement *element)
{
	double dval;
	int ival;
	TiXmlElement *prop;

	prop = element->FirstChildElement ("Frequency");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setFrequency (dval);

	prop = element->FirstChildElement ("Displacement");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setDisplacement (dval);

	prop = element->FirstChildElement ("Seed");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule.setSeed (ival);

	prop = element->FirstChildElement ("EnableDistance");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule.setEnableDistance (ival != 0);

	return true;
}
