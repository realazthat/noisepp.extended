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

#include <string>
#include <Noise.h>
#include <wx/propgrid/propgrid.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <xml/tinyxml.h>

class EditorModule
{
	public:
		EditorModule();
		virtual const std::string &getFactoryName () const = 0;
		virtual noisepp::Module2D &get2DModule () = 0;
		virtual noisepp::Module3D &get3DModule () = 0;
		virtual void fillPropertyGrid (wxPropertyGrid *pg) = 0;
		virtual void onPropertyChange (wxPropertyGrid *pg, const wxString &name) = 0;
		virtual bool validate (wxPropertyGrid *pg) = 0;
		virtual void writeProperties (TiXmlElement *element) = 0;
		virtual bool readProperties (TiXmlElement *element) = 0;
		virtual ~EditorModule();

		void generate (double x, double y, double width, double height, int w, int h, int threads=1);
		wxImage *getImage () const
		{
			return mImage;
		}
		wxBitmap *getBitmap () const
		{
			return mBitmap;
		}
	protected:
		bool setValid (wxPropertyGrid *pg, const char *name, bool valid);
	private:
		int mWidth, mHeight;
		wxImage *mImage;
		wxBitmap *mBitmap;

		void freeImage ();
};

#endif // EDITORMODULE_H
