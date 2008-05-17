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
#include <wx/wx.h>
#include "editorModule.h"

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

EditorModule::EditorModule() : mWidth(0), mHeight(0), mImage(NULL), mBitmap(NULL)
{
}

EditorModule::~EditorModule()
{
	freeImage ();
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
	if (valid)
	{
		pg->SetPropertyColour(wxString(name, wxConvUTF8), *wxWHITE);
		pg->SetPropertyColourToDefault(wxString(name, wxConvUTF8));
	}
	else
		pg->SetPropertyColour(wxString(name, wxConvUTF8), *wxRED);
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
	noisepp::ElementID id = get2DModule().addToPipeline (pipeline2D);
	noisepp::PipelineElement2D *element = pipeline2D->getElement(id);

	double *buffer = new double[w*h];

	double xDelta = width / double(w-1);
	double yDelta = height / double(h-1);

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
