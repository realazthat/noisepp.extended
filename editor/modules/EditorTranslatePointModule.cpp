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

#include "EditorTranslatePointModule.h"

std::string EditorTranslatePointModule::FACTORY_NAME = "TranslatePoint";

EditorTranslatePointModule::EditorTranslatePointModule() : EditorModule(1)
{
}

void EditorTranslatePointModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module"), mSourceModules[0]);
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxFloatProperty(wxT("Translate X"), wxPG_LABEL, mModule.getTranslationX()) );
	pg->Append( wxFloatProperty(wxT("Translate Y"), wxPG_LABEL, mModule.getTranslationY()) );
	pg->Append( wxFloatProperty(wxT("Translate Z"), wxPG_LABEL, mModule.getTranslationZ()) );
}

void EditorTranslatePointModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Source module"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[0] = val;
	}
	else if (name == _("Translate X"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setTranslationX (val);
	}
	else if (name == _("Translate Y"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setTranslationY (val);
	}
	else if (name == _("Translate Z"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setTranslationZ (val);
	}
}

bool EditorTranslatePointModule::validate (wxPropertyGrid *pg)
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

void EditorTranslatePointModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	writeSourceModules (element);

	prop = new TiXmlElement ("TranslateX");
	prop->SetDoubleAttribute ("value", mModule.getTranslationX());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("TranslateY");
	prop->SetDoubleAttribute ("value", mModule.getTranslationY());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("TranslateZ");
	prop->SetDoubleAttribute ("value", mModule.getTranslationZ());
	element->LinkEndChild (prop);
}

bool EditorTranslatePointModule::readProperties (TiXmlElement *element)
{
	TiXmlElement *prop;
	double dval;

	if (!readSourceModules (element))
		return false;

	prop = element->FirstChildElement ("TranslateX");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setTranslationX (dval);

	prop = element->FirstChildElement ("TranslateY");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setTranslationY (dval);

	prop = element->FirstChildElement ("TranslateZ");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setTranslationZ (dval);

	return true;
}
