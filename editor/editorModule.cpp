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

#include <GL/glew.h>
#include <cstdlib>
#include <sstream>
#include <wx/wx.h>
#include <wx/progdlg.h>
#include "editorModule.h"
#include "editorModuleManager.h"
#include "NoiseUtils.h"

EditorModule::EditorModule(int sourceModules) : mSourceModules(NULL), mSourceModuleCount(sourceModules), mWidth(0), mHeight(0), mImage(NULL), mBitmap(NULL), mTexture(0), mData(NULL)
{
	if (mSourceModuleCount > 0)
	{
		mSourceModules = new wxString[mSourceModuleCount];
	}
}

EditorModule::~EditorModule()
{
	freeImage ();
	freeData ();
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
	if (mTexture)
	{
		glDeleteTextures (1, &mTexture);
		mTexture = 0;
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

void EditorModule::generate (double x, double y, double width, double height, int w, int h)
{
	freeImage ();
	if (!mData)
	{
		mData = new double[w*h];
		try
		{
			noisepp::utils::PlaneBuilder2D builder;
			builder.setDestination(mData);
			builder.setModule (&getModule());
			builder.setSize (w, h);
			builder.setBounds(0, 0, width, height);
			builder.build ();
		}
		catch (std::exception &e)
		{
			std::cout << "exception: " << e.what() << std::endl;
		}
	}

	unsigned char *pixels = (unsigned char *)malloc(w*h*3);

	noisepp::utils::Image img;
	img.create (w, h);

	noisepp::utils::GradientRenderer gradients;
	gradients.addGradient (-1.0, noisepp::utils::ColourValue(0.0f, 0.0f, 0.0f));
	gradients.addGradient ( 1.0, noisepp::utils::ColourValue(1.0f, 1.0f, 1.0f));
	/*gradients.addGradient (-1.0, noisepp::utils::ColourValue(0.0f, 0.0f, 0.2f));
	gradients.addGradient (-0.8, noisepp::utils::ColourValue(0.0f, 0.0f, 0.6f));
	gradients.addGradient ( 0.0, noisepp::utils::ColourValue(1.0f, 0.0f, 0.0f));
	gradients.addGradient ( 0.6, noisepp::utils::ColourValue(1.0f, 1.0f, 0.0f));
	gradients.addGradient ( 1.0, noisepp::utils::ColourValue(1.0f, 1.0f, 1.0f));*/
	/*gradients.addGradient (-1.0000, noisepp::utils::ColourValue (  0,   0, 128)/255.f); // deeps
	gradients.addGradient (-0.2500, noisepp::utils::ColourValue (  0,   0, 255)/255.f); // shallow
	gradients.addGradient ( 0.0000, noisepp::utils::ColourValue (  0, 128, 255)/255.f); // shore
	gradients.addGradient ( 0.0625, noisepp::utils::ColourValue (240, 240,  64)/255.f); // sand
	gradients.addGradient ( 0.1250, noisepp::utils::ColourValue ( 32, 160,   0)/255.f); // grass
	gradients.addGradient ( 0.3750, noisepp::utils::ColourValue (224, 224,   0)/255.f); // dirt
	gradients.addGradient ( 0.7500, noisepp::utils::ColourValue (128, 128, 128)/255.f); // rock
	gradients.addGradient ( 1.0000, noisepp::utils::ColourValue (255, 255, 255)/255.f); // snow*/
	gradients.renderImage (img, mData);

	std::memcpy (pixels, img.getPixelData(), w*h*3);

	mImage = new wxImage (w, h, pixels);
	mBitmap = new wxBitmap (*mImage);
	/*glGenTextures (1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	if (GLEW_SGIS_generate_mipmap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	else
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);*/
}

void EditorModule::appendQualityProperty (wxPropertyGrid *pg, int quality)
{
	wxArrayString qualityArr;
	qualityArr.Add(wxT("Low"));
	qualityArr.Add(wxT("Default"));
	qualityArr.Add(wxT("High"));
	qualityArr.Add(wxT("Fast - Low"));
	qualityArr.Add(wxT("Fast - Default"));
	qualityArr.Add(wxT("Fast - High"));

	wxArrayInt arrIds;
	arrIds.Add(noisepp::NOISE_QUALITY_LOW);
	arrIds.Add(noisepp::NOISE_QUALITY_STD);
	arrIds.Add(noisepp::NOISE_QUALITY_HIGH);
	arrIds.Add(noisepp::NOISE_QUALITY_FAST_LOW);
	arrIds.Add(noisepp::NOISE_QUALITY_FAST_STD);
	arrIds.Add(noisepp::NOISE_QUALITY_FAST_HIGH);

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

void EditorModule::changeProperty (wxPropertyGrid *pg, wxPropertyGridEvent& event)
{
	freeData ();
	freeImage ();
	EditorModuleManager::getInstance().freeChildrenData (this);
	onPropertyChange (pg, event);
}

void EditorModule::freeData ()
{
	if (mData)
	{
		delete[] mData;
		mData = NULL;
	}
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

class BuilderProgressDlgCallback : public noisepp::utils::BuilderCallback
{
	private:
		wxProgressDialog &dlg;
		int max;
		int mod;

	public:
		BuilderProgressDlgCallback (wxProgressDialog &dlg, int max) : dlg(dlg), max(max)
		{
			mod = 1000;
		}
		void progress (int cur)
		{
			if (cur % mod == 0 || cur == max)
				dlg.Update(cur);
		}
};

bool EditorModule::exportToBMP (const char *name, int width, int height)
{
	double *data = new double[width*height];
	try
	{
		noisepp::utils::PlaneBuilder2D builder;
		builder.setDestination(data);
		builder.setModule (&getModule());
		builder.setSize (width, height);
		builder.setBounds(0, 0, 1.0, 1.0);
		wxProgressDialog progressDlg(wxT("Building ..."), wxT("Please wait while building ..."), builder.getProgressMaximum());
		progressDlg.Show (true);
		builder.setCallback(new BuilderProgressDlgCallback(progressDlg, builder.getProgressMaximum()));
		builder.build ();
	}
	catch (std::exception &e)
	{
		std::cout << "exception: " << e.what() << std::endl;
		delete[] data;
		return false;
	}

	noisepp::utils::Image img;
	img.create (width, height);

	noisepp::utils::GradientRenderer gradients;
	gradients.addGradient (-1.0, noisepp::utils::ColourValue(0.0f, 0.0f, 0.0f));
	gradients.addGradient ( 1.0, noisepp::utils::ColourValue(1.0f, 1.0f, 1.0f));
	wxProgressDialog progressDlg(wxT("Coloring ..."), wxT("Please wait while coloring ..."), height);
	progressDlg.Show (true);
	gradients.setCallback(new BuilderProgressDlgCallback(progressDlg, height));
	gradients.renderImage (img, data);

	if (!img.saveBMP (name))
	{
		delete[] data;
		return false;
	}

	delete[] data;
	return true;
}

bool EditorModule::validateTree (EditorModule *child)
{
	assert (child);
	return !getModule().walkTree(&child->getModule());
}
