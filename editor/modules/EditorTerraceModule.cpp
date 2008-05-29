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

#include "EditorTerraceModule.h"

#define MIN_POINTS 2
#define MAX_POINTS 30

std::string EditorTerraceModule::FACTORY_NAME = "Terrace";

EditorTerraceModule::EditorTerraceModule() : EditorModule(1), mPointCount(MIN_POINTS)
{
}

void EditorTerraceModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	mControlPointIDs.clear ();
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module"), mSourceModules[0]);
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxIntProperty(wxT("Control points"), wxPG_LABEL, mPointCount) );
	pg->Append( wxBoolProperty(wxT("Invert"), wxPG_LABEL, mModule.isInverted()) );
	mPointCategoryID = pg->Append( wxPropertyCategory(wxT("Points")) );
	changeNumberOfControlPoints(pg);
}

void EditorTerraceModule::changeNumberOfControlPoints (wxPropertyGrid *pg)
{
	if (mControlPointIDs.size() > mPointCount)
	{
		while (mControlPointIDs.size() > MIN_POINTS && mControlPointIDs.size() > mPointCount)
		{
			pg->Delete (mControlPointIDs.back());
			mControlPointIDs.pop_back ();
		}
	}
	else if (mControlPointIDs.size() < mPointCount)
	{
		noisepp::TerraceControlPointVector &points = mModule.getControlPoints ();
		while (mControlPointIDs.size() < MAX_POINTS && mControlPointIDs.size() < mPointCount)
		{
			wxString name = wxT("Point ");
			name << mControlPointIDs.size() + 1;
			double val = 0.0;
			if (mControlPointIDs.size() < points.size())
			{
				val = points[mControlPointIDs.size()];
			}
			wxPGId id = pg->AppendIn( mPointCategoryID, wxFloatProperty(name,wxPG_LABEL,val) );
			mControlPointIDs.push_back (id);
		}
	}
}

void EditorTerraceModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Control points"))
	{
		int val = pg->GetPropertyValueAsInt (name);
		mPointCount = val;
		changeNumberOfControlPoints (pg);
		if (mPointCount < MIN_POINTS || mPointCount > MAX_POINTS)
		{
			if (mPointCount < MIN_POINTS)
				mPointCount = MIN_POINTS;
			if (mPointCount > MAX_POINTS)
				mPointCount = MAX_POINTS;
			pg->SetPropertyValue (name, mPointCount);
		}
	}
	else if (name == _("Source module"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[0] = val;
	}
	else if (name == _("Invert"))
	{
		bool val = pg->GetPropertyValueAsBool (name);
		mModule.invert (val);
	}
}

bool EditorTerraceModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	EditorModule *module = NULL;

	module = getSourceModule(0);
	if (module && module->validateTree(this))
	{
		mModule.setSourceModule(0, module->getModule());
	}
	valid = setValid (pg, "Source module", module != NULL && module->validateTree(this) && module->validate(NULL)) && valid;

	if (pg)
	{
		mModule.clearControlPoints ();
		for (ControlPointList::iterator it=mControlPointIDs.begin();it!=mControlPointIDs.end();++it)
		{
			mModule.addControlPoint (pg->GetPropertyValueAsDouble(*it));
		}

		for (ControlPointList::iterator it=mControlPointIDs.begin();it!=mControlPointIDs.end();++it)
		{
			valid = setValid (pg, pg->GetPropertyName(*it).mb_str(), getNumberOfMatches(pg->GetPropertyValueAsDouble(*it)) == 1) && valid;
		}
		if (!valid)
			mModule.clearControlPoints ();
	}
	else
	{
		valid = !mModule.getControlPoints().empty() && valid;
	}

	return valid;
}

void EditorTerraceModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;
	writeSourceModules (element);

	prop = new TiXmlElement ("Invert");
	prop->SetAttribute ("value", mModule.isInverted());
	element->LinkEndChild (prop);

	noisepp::TerraceControlPointVector &points = mModule.getControlPoints ();

	for (noisepp::TerraceControlPointVector::iterator it=points.begin();it!=points.end();++it)
	{
		prop = new TiXmlElement ("Point");
		prop->SetDoubleAttribute ("value", *it);
		element->LinkEndChild (prop);
	}
}

bool EditorTerraceModule::readProperties (TiXmlElement *element)
{
	int ival;
	TiXmlElement *prop = NULL;
	TiXmlNode *child = NULL;

	if (!readSourceModules (element))
		return false;

	prop = element->FirstChildElement ("Invert");
	if (prop == NULL || prop->QueryIntAttribute ("value", &ival) != TIXML_SUCCESS)
		return false;
	mModule.invert (ival != 0);

	mModule.clearControlPoints ();

	while( (child = element->IterateChildren( "Point", child )) != NULL )
	{
		prop = child->ToElement();
		if (prop)
		{
			double value;
			if (prop->QueryDoubleAttribute ("value", &value) != TIXML_SUCCESS)
				return false;
			mModule.addControlPoint (value);
		}
	}

	mPointCount = mModule.getControlPoints().size();

	return true;
}

int EditorTerraceModule::getNumberOfMatches (const noisepp::Real &v)
{
	int matches = 0;
	noisepp::TerraceControlPointVector &points = mModule.getControlPoints ();

	for (noisepp::TerraceControlPointVector::iterator it=points.begin();it!=points.end();++it)
	{
		if (fabs(*it - v) < 1.0e-7)
			++matches;
	}
	return matches;
}
