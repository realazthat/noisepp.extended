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

#include "EditorCurveModule.h"

#define MIN_POINTS 4
#define MAX_POINTS 30

std::string EditorCurveModule::FACTORY_NAME = "Curve";

EditorCurveModule::EditorCurveModule() : EditorModule(1), mPointCount(MIN_POINTS)
{
}

void EditorCurveModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	mControlPointIDs.clear ();
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module"), mSourceModules[0]);
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxIntProperty(wxT("Control points"), wxPG_LABEL, mPointCount) );
	mPointCategoryID = pg->Append( wxPropertyCategory(wxT("Points")) );
	changeNumberOfControlPoints(pg);
}

void EditorCurveModule::changeNumberOfControlPoints (wxPropertyGrid *pg)
{
	if (mControlPointIDs.size() > mPointCount)
	{
		while (mControlPointIDs.size() > MIN_POINTS && mControlPointIDs.size() > mPointCount)
		{
			pg->Delete (mControlPointIDs.back().parent);
			mControlPointIDs.pop_back ();
		}
	}
	else if (mControlPointIDs.size() < mPointCount)
	{
		noisepp::CurveControlPointVector &points = mModule.getControlPoints ();
		while (mControlPointIDs.size() < MAX_POINTS && mControlPointIDs.size() < mPointCount)
		{
			wxString name = wxT("Point ");
			name << mControlPointIDs.size() + 1;
			ControlPoint point;
			point.parent = pg->AppendIn( mPointCategoryID, wxParentProperty(name,wxPG_LABEL) );
			double inval = 0.0;
			double outval = 0.0;
			if (mControlPointIDs.size() < points.size())
			{
				inval = points[mControlPointIDs.size()].inValue;
				outval = points[mControlPointIDs.size()].outValue;
			}
			point.in = pg->AppendIn( point.parent, wxFloatProperty(wxT("In value"),wxPG_LABEL,inval) );
			point.out = pg->AppendIn( point.parent, wxFloatProperty(wxT("Out value"),wxPG_LABEL,outval) );
			mControlPointIDs.push_back (point);
		}
	}
}

void EditorCurveModule::onPropertyChange (wxPropertyGrid *pg, wxPropertyGridEvent& event)
{
	const wxString &name = event.GetPropertyName();
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
}

bool EditorCurveModule::validate (wxPropertyGrid *pg)
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
			mModule.addControlPoint (pg->GetPropertyValueAsDouble(it->in), pg->GetPropertyValueAsDouble(it->out));
		}
		for (ControlPointList::iterator it=mControlPointIDs.begin();it!=mControlPointIDs.end();++it)
		{
			bool v = getNumberOfMatches(pg->GetPropertyValueAsDouble(it->in)) == 1;
			wxString name = pg->GetPropertyName(it->parent);
			wxString outname = name+wxT(".")+pg->GetPropertyName(it->out);
			valid = setValid (pg, name.mb_str(), v) && valid;
			setValid (pg, outname.mb_str(), true);
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

void EditorCurveModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;
	writeSourceModules (element);

	noisepp::CurveControlPointVector &points = mModule.getControlPoints ();

	for (noisepp::CurveControlPointVector::iterator it=points.begin();it!=points.end();++it)
	{
		prop = new TiXmlElement ("Point");
		prop->SetDoubleAttribute ("in", it->inValue);
		prop->SetDoubleAttribute ("out", it->outValue);
		element->LinkEndChild (prop);
	}
}

bool EditorCurveModule::readProperties (TiXmlElement *element)
{
	TiXmlElement *prop = NULL;
	TiXmlNode *child = NULL;

	if (!readSourceModules (element))
		return false;

	mModule.clearControlPoints ();

	while( (child = element->IterateChildren( "Point", child )) != NULL )
	{
		prop = child->ToElement();
		if (prop)
		{
			double in, out;
			if (prop->QueryDoubleAttribute ("in", &in) != TIXML_SUCCESS || prop->QueryDoubleAttribute ("out", &out) != TIXML_SUCCESS)
				return false;
			mModule.addControlPoint (in, out);
		}
	}

	mPointCount = mModule.getControlPoints().size();

	return true;
}

int EditorCurveModule::getNumberOfMatches (const noisepp::Real &v)
{
	int matches = 0;
	noisepp::CurveControlPointVector &points = mModule.getControlPoints ();

	for (noisepp::CurveControlPointVector::iterator it=points.begin();it!=points.end();++it)
	{
		if (fabs(it->inValue - v) < 1.0e-7)
			++matches;
	}
	return matches;
}
