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

#ifndef EDITORGLCANVAS_H
#define EDITORGLCANVAS_H

#include <GL/glew.h>
#include <wx/wx.h>
#include <wx/glcanvas.h>

#include "editorNode.h"

class editorGLCanvas : public wxGLCanvas
{
	public:
		editorGLCanvas(wxFrame *frame, wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
		virtual ~editorGLCanvas();
		void setModule (const wxString &module = wxEmptyString);
	protected:
	private:
		wxFrame *mFrame;
		wxString mModule;
		void OnPaint(wxPaintEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnMouseWheel(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnMouseClick(wxMouseEvent& event);
		void drawModule(wxPaintDC &dc, int center_x, int center_y, wxCoord w, wxCoord h, const wxString &name, wxBitmap *bitmap);
		bool mInit;

		void init ();

		double mScale;
		double mPosX, mPosY;
		long mDownX, mDownY;
};

#endif // editorGLCanvas_H
