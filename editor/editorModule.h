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

#ifndef EDITORMODULE_H
#define EDITORMODULE_H

#include <GL/glew.h>
#include <string>
#include <Noise.h>
#include <wx/propgrid/propgrid.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/glcanvas.h>
#include <xml/tinyxml.h>

class EditorModuleManager;

class EditorModule
{
	friend class EditorModuleManager;

	public:
		EditorModule(int sourceModules=0);
		virtual const std::string &getFactoryName () const = 0;
		virtual noisepp::Module &getModule () = 0;
		virtual void fillPropertyGrid (wxPropertyGrid *pg) = 0;
		virtual bool validate (wxPropertyGrid *pg) = 0;
		virtual void writeProperties (TiXmlElement *element) = 0;
		virtual bool readProperties (TiXmlElement *element) = 0;
		virtual ~EditorModule();

		void generate (double x, double y, double width, double height, int w, int h);
		wxImage *getImage () const
		{
			return mImage;
		}
		wxBitmap *getBitmap () const
		{
			return mBitmap;
		}
		GLuint getTexture () const
		{
			return mTexture;
		}

		int getNumberOfSourceModules () const
		{
			return mSourceModuleCount;
		}

		const wxString &getSourceModuleName (int i) const
		{
			assert (i < mSourceModuleCount);
			return mSourceModules[i];
		}

		EditorModule *getSourceModule (int i) const;

		bool exportToFile (const char *name);
		bool exportToBMP (const char *name, int width, int height);

		void changeProperty (wxPropertyGrid *pg, wxPropertyGridEvent& event);

		bool validateTree (EditorModule *child);
	protected:
		bool setValid (wxPropertyGrid *pg, const char *name, bool valid);
		void appendQualityProperty (wxPropertyGrid *pg, int quality=noisepp::NOISE_QUALITY_STD);
		void appendSourceModuleProperty (wxPropertyGrid *pg, const wxString &name, const wxString &defaultValue = wxEmptyString);
		bool validateSourceModules ();

		void writeSourceModules (TiXmlElement *element);
		bool readSourceModules (TiXmlElement *element);

		virtual void onPropertyChange (wxPropertyGrid *pg, const wxString &name) {}
		virtual void onPropertyChange (wxPropertyGrid *pg, wxPropertyGridEvent& event);

		wxString *mSourceModules;
		int mSourceModuleCount;
	private:
		int mWidth, mHeight;
		wxImage *mImage;
		wxBitmap *mBitmap;
		GLuint mTexture;

		double *mData;

		void freeData ();
		void freeImage ();
};

#endif // EDITORMODULE_H
