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

#include "EditorSelectModule.h"

std::string EditorSelectModule::FACTORY_NAME = "Select";

EditorSelectModule::EditorSelectModule() : EditorModule(3)
{
}

void EditorSelectModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module 1"), mSourceModules[0]);
	appendSourceModuleProperty (pg, wxT("Source module 2"), mSourceModules[1]);
	appendSourceModuleProperty (pg, wxT("Control module"), mSourceModules[2]);
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxFloatProperty(wxT("Lower bound"), wxPG_LABEL, mModule.getLowerBound()) );
	pg->Append( wxFloatProperty(wxT("Upper bound"), wxPG_LABEL, mModule.getUpperBound()) );
	pg->Append( wxFloatProperty(wxT("Edge falloff"), wxPG_LABEL, mModule.getEdgeFalloff()) );
}

void EditorSelectModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Source module 1"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[0] = val;
	}
	else if (name == _("Source module 2"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[1] = val;
	}
	else if (name == _("Control module"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[2] = val;
	}
	else if (name == _("Lower bound"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setLowerBound (val);
	}
	else if (name == _("Upper bound"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setUpperBound (val);
	}
	else if (name == _("Edge falloff"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setEdgeFalloff (val);
	}
}

bool EditorSelectModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	EditorModule *module = NULL;

	module = getSourceModule(0);
	if (module && module->validateTree(this))
	{
		mModule.setSourceModule(0, module->getModule());
	}
	valid = setValid (pg, "Source module 1", module != NULL && module->validateTree(this) && module->validate(NULL)) && valid;
	module = getSourceModule(1);
	if (module && module->validateTree(this))
	{
		mModule.setSourceModule(1, module->getModule());
	}
	valid = setValid (pg, "Source module 2", module != NULL && module->validateTree(this) && module->validate(NULL)) && valid;
	module = getSourceModule(2);
	if (module && module->validateTree(this))
	{
		mModule.setSourceModule(2, module->getModule());
	}
	valid = setValid (pg, "Control module", module != NULL && module->validateTree(this) && module->validate(NULL)) && valid;

	valid = setValid (pg, "Lower bound", mModule.getUpperBound() > mModule.getLowerBound()) && valid;
	valid = setValid (pg, "Upper bound", mModule.getLowerBound() < mModule.getUpperBound()) && valid;
	valid = setValid (pg, "Edge falloff", mModule.getEdgeFalloff() >= 0) && valid;

	return valid;
}

void EditorSelectModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	writeSourceModules (element);

	prop = new TiXmlElement ("LowerBound");
	prop->SetDoubleAttribute ("value", mModule.getLowerBound());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("UpperBound");
	prop->SetDoubleAttribute ("value", mModule.getUpperBound());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("EdgeFalloff");
	prop->SetDoubleAttribute ("value", mModule.getEdgeFalloff());
	element->LinkEndChild (prop);
}

bool EditorSelectModule::readProperties (TiXmlElement *element)
{
	TiXmlElement *prop;
	double dval;

	if (!readSourceModules (element))
		return false;

	prop = element->FirstChildElement ("LowerBound");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setLowerBound (dval);

	prop = element->FirstChildElement ("UpperBound");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setUpperBound (dval);

	prop = element->FirstChildElement ("EdgeFalloff");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setEdgeFalloff (dval);

	return true;
}
