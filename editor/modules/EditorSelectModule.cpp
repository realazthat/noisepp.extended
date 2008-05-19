#include "EditorSelectModule.h"

std::string EditorSelectModule::FACTORY_NAME = "Select";

EditorSelectModule::EditorSelectModule() : EditorModule(3)
{
}

void EditorSelectModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	appendSourceModuleProperty (pg, wxT("Source module 1"), mSourceModules[0]);
	appendSourceModuleProperty (pg, wxT("Source module 2"), mSourceModules[1]);
	appendSourceModuleProperty (pg, wxT("Control module"), mSourceModules[2]);
	pg->Append( wxFloatProperty(wxT("Lower bound"), wxPG_LABEL, mModule3D.getLowerBound()) );
	pg->Append( wxFloatProperty(wxT("Upper bound"), wxPG_LABEL, mModule3D.getUpperBound()) );
	pg->Append( wxFloatProperty(wxT("Edge falloff"), wxPG_LABEL, mModule3D.getEdgeFalloff()) );
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
		mModule3D.setLowerBound (val);
		mModule2D.setLowerBound (val);
	}
	else if (name == _("Upper bound"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setUpperBound (val);
		mModule2D.setUpperBound (val);
	}
	else if (name == _("Edge falloff"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule3D.setEdgeFalloff (val);
		mModule2D.setEdgeFalloff (val);
	}
}

bool EditorSelectModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	EditorModule *module = NULL;

	module = getSourceModule(0);
	if (module)
	{
		mModule2D.setSourceModule(0, module->get2DModule());
		mModule3D.setSourceModule(0, module->get3DModule());
	}
	valid = setValid (pg, "Source module 1", module != NULL && module->validate(NULL)) && valid;
	module = getSourceModule(1);
	if (module)
	{
		mModule2D.setSourceModule(1, module->get2DModule());
		mModule3D.setSourceModule(1, module->get3DModule());
	}
	valid = setValid (pg, "Source module 2", module != NULL && module->validate(NULL)) && valid;
	module = getSourceModule(2);
	if (module)
	{
		mModule2D.setSourceModule(2, module->get2DModule());
		mModule3D.setSourceModule(2, module->get3DModule());
	}
	valid = setValid (pg, "Control module", module != NULL && module->validate(NULL)) && valid;

	valid = setValid (pg, "Lower bound", mModule3D.getUpperBound() > mModule3D.getLowerBound()) && valid;
	valid = setValid (pg, "Upper bound", mModule3D.getLowerBound() < mModule3D.getUpperBound()) && valid;
	valid = setValid (pg, "Edge falloff", mModule3D.getEdgeFalloff() >= 0) && valid;

	return valid;
}

void EditorSelectModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	writeSourceModules (element);

	prop = new TiXmlElement ("LowerBound");
	prop->SetDoubleAttribute ("value", mModule3D.getLowerBound());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("UpperBound");
	prop->SetDoubleAttribute ("value", mModule3D.getUpperBound());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("EdgeFalloff");
	prop->SetDoubleAttribute ("value", mModule3D.getEdgeFalloff());
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
	mModule3D.setLowerBound (dval);
	mModule2D.setLowerBound (dval);

	prop = element->FirstChildElement ("UpperBound");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setUpperBound (dval);
	mModule2D.setUpperBound (dval);

	prop = element->FirstChildElement ("EdgeFalloff");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule3D.setEdgeFalloff (dval);
	mModule2D.setEdgeFalloff (dval);

	return true;
}
