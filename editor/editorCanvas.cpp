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

#include "editorCanvas.h"
#include "editorModule.h"
#include "editorModuleManager.h"

editorCanvas::editorCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) : wxScrolledWindow(parent, id, pos, size, style)
{
	SetBackgroundColour (*wxWHITE);
	SetScrollRate( 10, 10 );
	Connect(wxEVT_PAINT, wxPaintEventHandler(editorCanvas::OnPaint));
}

editorCanvas::~editorCanvas()
{
}

void editorCanvas::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC( dc );

	wxCoord w, h;
	dc.GetSize (&w, &h);

	if (!mModule.IsEmpty())
	{
		wxCoord width = 200;
		wxCoord height = 203;
		wxCoord titleHeight = 20;

		dc.DrawRectangle((w-width)/2, (h-(height+titleHeight))/2, width, (height+titleHeight));
		wxCoord tLine = (h-(height+titleHeight))/2+titleHeight;
		dc.DrawLine ((w-width)/2, tLine, (w-width)/2+width, tLine);

		wxCoord text_width, text_height;
		dc.GetTextExtent (mModule, &text_width, &text_height);

		dc.DrawText (mModule, (w-text_width)/2, (h-(height+titleHeight))/2+titleHeight/2-text_height/2);

		EditorModule *module = EditorModuleManager::getInstance().getModule(mModule);
		if (module && module->getBitmap())
		{
			dc.DrawBitmap (*module->getBitmap(), (w-width)/2+1, tLine+1, false);
		}
	}
}

void editorCanvas::setModule (const wxString &module)
{
	mModule = module;
	Refresh ();
}
