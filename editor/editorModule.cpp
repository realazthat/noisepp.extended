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

#include <cstdlib>
#include <sstream>
#include <wx/wx.h>
#include "editorModule.h"
#include "editorModuleManager.h"
#include "NoiseUtils.h"

class LineJob2D : public noisepp::PipelineJob
{
	private:
		noisepp::Pipeline2D *mPipe;
		noisepp::PipelineElement2D *mElement;
		double x, y;
		int n;
		double delta;
		double *buffer;

	public:
		LineJob2D (noisepp::Pipeline2D *pipe, noisepp::PipelineElement2D *element, double x, double y, int n, double delta, double *buffer) :
			mPipe(pipe), mElement(element), x(x), y(y), n(n), delta(delta), buffer(buffer)
		{
		}
		void execute (noisepp::Cache *cache)
		{
			for (int i=0;i<n;++i)
			{
				// cleans the cache
				mPipe->cleanCache (cache);
				// calculates the value
				buffer[i] = mElement->getValue(x, y, cache);
				// move on
				x += delta;
			}
		}
};

EditorModule::EditorModule(int sourceModules) : mSourceModules(NULL), mSourceModuleCount(sourceModules), mWidth(0), mHeight(0), mImage(NULL), mBitmap(NULL)
{
	if (mSourceModuleCount > 0)
	{
		mSourceModules = new wxString[mSourceModuleCount];
	}
}

EditorModule::~EditorModule()
{
	freeImage ();
	if (mSourceModules)
	{
		delete[] mSourceModules;
		mSourceModules = NULL;
	}
}

void EditorModule::freeImage ()
{
	if (mImage)
	{
		delete mImage;
		mImage = NULL;
	}
	if (mBitmap)
	{
		delete mBitmap;
		mBitmap = NULL;
	}
}

bool EditorModule::setValid (wxPropertyGrid *pg, const char *name, bool valid)
{
	if (pg == NULL)
		return valid;
	if (valid)
	{
		pg->SetPropertyColour(wxString(name, wxConvUTF8), *wxWHITE);
		pg->SetPropertyColourToDefault(wxString(name, wxConvUTF8));
	}
	else
	{
		pg->SetPropertyColour(wxString(name, wxConvUTF8), *wxRED);
		pg->SetPropertyBackgroundColour(wxString(name, wxConvUTF8), *wxRED);
	}
	return valid;
}

void EditorModule::generate (double x, double y, double width, double height, int w, int h, int threads)
{
	freeImage ();
	noisepp::Pipeline2D *pipeline2D = NULL;
	if (threads > 1)
		pipeline2D = new noisepp::ThreadedPipeline2D (threads);
	else
		pipeline2D = new noisepp::Pipeline2D;

	noisepp::ElementID id = getModule().addToPipeline (pipeline2D);
	noisepp::PipelineElement2D *element = pipeline2D->getElement(id);

	double *buffer = new double[w*h];

	double xDelta = width / double(w);
	double yDelta = height / double(h);

	for (int yi=0;yi<h;++yi)
	{
		pipeline2D->addJob (new LineJob2D (pipeline2D, element, x, y, w, xDelta, buffer+yi*w));
		y += yDelta;
	}

	pipeline2D->executeJobs ();

	unsigned char *pixels = (unsigned char *)malloc(w*h*3);
	unsigned char *dest = pixels;
	double d;
	unsigned char p;
	for (int n=0;n<w*h;++n)
	{
		d = (buffer[n] + 1.0) * 0.5 * 255.0;
		if (d < 0.0)
			d = 0.0;
		if (d > 255.0)
			d = 255.0;
		p = (unsigned char)(d);
		*dest++ = p;
		*dest++ = p;
		*dest++ = p;
	}

	mImage = new wxImage (w, h, pixels);
	mBitmap = new wxBitmap (*mImage);

	delete[] buffer;
	delete pipeline2D;
}

void EditorModule::appendQualityProperty (wxPropertyGrid *pg, int quality)
{
	wxArrayString qualityArr;
	qualityArr.Add(wxT("Low"));
	qualityArr.Add(wxT("Default"));
	qualityArr.Add(wxT("High"));

	wxArrayInt arrIds;
	arrIds.Add(noisepp::NOISE_QUALITY_LOW);
	arrIds.Add(noisepp::NOISE_QUALITY_STD);
	arrIds.Add(noisepp::NOISE_QUALITY_HIGH);

	pg->Append( wxEnumProperty(wxT("Quality"), wxPG_LABEL, qualityArr, arrIds, quality) );
}

EditorModule *EditorModule::getSourceModule (int i) const
{
	assert (i < mSourceModuleCount);
	return EditorModuleManager::getInstance().getModule (mSourceModules[i]);
}

void EditorModule::appendSourceModuleProperty (wxPropertyGrid *pg, const wxString &name, const wxString &defaultValue)
{
	wxArrayString moduleArr;
	moduleArr.Add(wxT("(Please choose)"));
	int id = EditorModuleManager::getInstance().fillModuleArrayWithException(moduleArr, this, defaultValue);
	pg->Append( wxEnumProperty(name, wxPG_LABEL, moduleArr, id) );
}

void EditorModule::writeSourceModules (TiXmlElement *element)
{
	TiXmlElement *prop;

	for (int i=0;i<mSourceModuleCount;++i)
	{
		std::ostringstream s;
		s << "Module" << i;
		prop = new TiXmlElement (s.str());
		prop->SetAttribute ("value", mSourceModules[i].mb_str());
		element->LinkEndChild (prop);
	}
}

bool EditorModule::readSourceModules (TiXmlElement *element)
{
	TiXmlElement *prop;
	const char *name;

	for (int i=0;i<mSourceModuleCount;++i)
	{
		std::ostringstream s;
		s << "Module" << i;
		prop = element->FirstChildElement (s.str());
		if (prop == NULL || (name = prop->Attribute ("value")) == NULL)
			return false;
		mSourceModules[i] = wxString(name, wxConvUTF8);
	}

	return true;
}

void EditorModule::onPropertyChange (wxPropertyGrid *pg, wxPropertyGridEvent& event)
{
	onPropertyChange (pg, event.GetPropertyName());
}

bool EditorModule::exportToFile (const char *name)
{
	noisepp::utils::FileOutStream f;
	if (!f.open (name))
		return false;

	noisepp::utils::Writer writer(f);
	writer.addModule (getModule());
	writer.writePipeline ();

	f.close ();

	return true;
}
