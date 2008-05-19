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

#ifndef EDITORCANVAS_H
#define EDITORCANVAS_H

#include <wx/wx.h>
#include <wx/window.h>

class editorCanvas : public wxScrolledWindow
{
	public:
		editorCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxHSCROLL | wxVSCROLL | wxFULL_REPAINT_ON_RESIZE);
		virtual ~editorCanvas();
		void setModule (const wxString &module = wxEmptyString);
	protected:
	private:
		wxString mModule;
		void OnPaint(wxPaintEvent& event);
		void drawModule(wxPaintDC &dc, int center_x, int center_y, wxCoord w, wxCoord h, const wxString &name, wxBitmap *bitmap);
};

#endif // EDITORCANVAS_H
