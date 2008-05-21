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

#include "EditorScaleBiasModule.h"

std::string EditorScaleBiasModule::FACTORY_NAME = "ScaleBias";

EditorScaleBiasModule::EditorScaleBiasModule() : EditorModule(1)
{
}

void EditorScaleBiasModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module"), mSourceModules[0]);
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxFloatProperty(wxT("Scale"), wxPG_LABEL, mModule3D.getScale()) );
	pg->Append( wxFloatProperty(wxT("Bias"), wxPG_LABEL, mModule3D.getBias()) );
}

void EditorScaleBiasModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Source module"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[0] = val;
	}
	else if (name == _("Scale"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setScale (val);
		mModule2D.setScale (val);
	}
	else if (name == _("Bias"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setBias (val);
		mModule2D.setBias (val);
	}
}

bool EditorScaleBiasModule::validate (wxPropertyGrid *pg)
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

	return valid;
}

void EditorScaleBiasModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	writeSourceModules (element);

	prop = new TiXmlElement ("Scale");
	prop->SetDoubleAttribute ("value", mModule3D.getScale());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Bias");
	prop->SetDoubleAttribute ("value", mModule3D.getBias());
	element->LinkEndChild (prop);
}

bool EditorScaleBiasModule::readProperties (TiXmlElement *element)
{
	TiXmlElement *prop;
	double dval;

	if (!readSourceModules (element))
		return false;

	prop = element->FirstChildElement ("Scale");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setScale (dval);
	mModule2D.setScale (dval);

	prop = element->FirstChildElement ("Bias");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setBias (dval);
	mModule2D.setBias (dval);

	return true;
}
