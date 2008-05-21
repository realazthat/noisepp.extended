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

#include "EditorClampModule.h"

std::string EditorClampModule::FACTORY_NAME = "Clamp";

EditorClampModule::EditorClampModule() : EditorModule(1)
{
}

void EditorClampModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module"), mSourceModules[0]);
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxFloatProperty(wxT("Lower bound"), wxPG_LABEL, mModule3D.getLowerBound()) );
	pg->Append( wxFloatProperty(wxT("Upper bound"), wxPG_LABEL, mModule3D.getUpperBound()) );
}

void EditorClampModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Source module"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[0] = val;
	}
	else if (name == _("Lower bound"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setLowerBound (val);
		mModule2D.setLowerBound (val);
	}
	else if (name == _("Upper bound"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setUpperBound (val);
		mModule2D.setUpperBound (val);
	}
}

bool EditorClampModule::validate (wxPropertyGrid *pg)
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

	valid = setValid (pg, "Lower bound", mModule3D.getUpperBound() > mModule3D.getLowerBound()) && valid;
	valid = setValid (pg, "Upper bound", mModule3D.getLowerBound() < mModule3D.getUpperBound()) && valid;

	return valid;
}

void EditorClampModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	writeSourceModules (element);

	prop = new TiXmlElement ("LowerBound");
	prop->SetDoubleAttribute ("value", mModule3D.getLowerBound());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("UpperBound");
	prop->SetDoubleAttribute ("value", mModule3D.getUpperBound());
	element->LinkEndChild (prop);
}

bool EditorClampModule::readProperties (TiXmlElement *element)
{
	TiXmlElement *prop;
	double dval;

	if (!readSourceModules (element))
		return false;

	prop = element->FirstChildElement ("LowerBound");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setLowerBound (dval);
	mModule2D.setLowerBound (dval);

	prop = element->FirstChildElement ("UpperBound");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setUpperBound (dval);
	mModule2D.setUpperBound (dval);

	return true;
}
