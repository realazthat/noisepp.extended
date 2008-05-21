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
	pg->Append( wxFloatProperty(wxT("Frequency"), wxPG_LABEL, mModule3D.getFrequency()) );
	pg->Append( wxFloatProperty(wxT("Displacement"), wxPG_LABEL, mModule3D.getDisplacement()) );
	pg->Append( wxIntProperty(wxT("Seed"), wxPG_LABEL, mModule3D.getSeed()) );
	pg->Append( wxBoolProperty(wxT("Enable distance"), wxPG_LABEL, mModule3D.isDistanceEnabled()) );
}

void EditorVoronoiModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Frequency"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setFrequency (val);
		mModule2D.setFrequency (val);
	}
	else if (name == _("Displacement"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setDisplacement (val);
		mModule2D.setDisplacement (val);
	}
	else if (name == _("Seed"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mModule3D.setSeed (val);
		mModule2D.setSeed (val);
	}
	else if (name == _("Enable distance"))
	{
		bool val = pg->GetPropertyValueAsBool (name);
		mModule3D.setEnableDistance (val);
		mModule2D.setEnableDistance (val);
	}
}

bool EditorVoronoiModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	valid = setValid (pg, "Frequency", mModule3D.getFrequency() > 0) && valid;
	return valid;
}

void EditorVoronoiModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	prop = new TiXmlElement ("Frequency");
	prop->SetDoubleAttribute ("value", mModule3D.getFrequency());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Displacement");
	prop->SetDoubleAttribute ("value", mModule3D.getDisplacement());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Seed");
	prop->SetAttribute ("value", mModule3D.getSeed());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("EnableDistance");
	prop->SetAttribute ("value", mModule3D.isDistanceEnabled());
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
	mModule3D.setFrequency (dval);
	mModule2D.setFrequency (dval);

	prop = element->FirstChildElement ("Displacement");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setDisplacement (dval);
	mModule2D.setDisplacement (dval);

	prop = element->FirstChildElement ("Seed");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule3D.setSeed (ival);
	mModule2D.setSeed (ival);

	prop = element->FirstChildElement ("EnableDistance");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule3D.setEnableDistance (ival != 0);
	mModule2D.setEnableDistance (ival != 0);

	return true;
}
