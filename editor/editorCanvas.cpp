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

const wxCoord titleHeight = 20;

editorCanvas::editorCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) : wxScrolledWindow(parent, id, pos, size, style)
{
	SetBackgroundColour (*wxWHITE);
	SetScrollRate( 10, 10 );
	Connect(wxEVT_PAINT, wxPaintEventHandler(editorCanvas::OnPaint));
}

editorCanvas::~editorCanvas()
{
}

void editorCanvas::drawModule(wxPaintDC &dc, int center_x, int center_y, wxCoord w, wxCoord h, const wxString &name, wxBitmap *bitmap)
{
	wxCoord width = w + 2;
	wxCoord height = h + 3;

	dc.DrawRectangle(center_x-width/2, center_y-(height+titleHeight)/2, width, height+titleHeight);
	wxCoord tLine = center_y-(height+titleHeight)/2+titleHeight+1;
	dc.DrawLine (center_x-width/2, tLine, center_x+width/2, tLine);

	wxCoord text_width, text_height;
	dc.GetTextExtent (name, &text_width, &text_height);

	dc.DrawText (name, center_x-text_width/2, center_y-(height+titleHeight)/2+(titleHeight-text_height)/2);

	if (bitmap)
	{
		/*wxCoord xo = center_x-w/2;
		wxCoord yo = tLine+1;

		EditorModule *module = EditorModuleManager::getInstance().getModule(name);
		if (module)
		{
			noisepp::Pipeline2D pipeline;
			noisepp::ElementID id = module->get2DModule().addToPipe (pipeline);
			noisepp::PipelineElement2D *element = pipeline.getElement(id);
			noisepp::Cache *cache = pipeline.createCache ();
			int y, lasty;
			for (int n=0;n<w;++n)
			{
				double d = (element->getValue((double)n/(double)(w-1), 0, cache) + 1.0) * 0.5 * (h-1);
				if (d < 0.0)
					d = 0.0;
				if (d > (h-1))
					d = h-1;
				lasty = y;
				y = (int)d;
				if (n > 0)
				{
					dc.DrawLine(xo+n-1, yo+lasty, xo+n, yo+y);
				}
			}
			pipeline.freeCache (cache);
		}*/

		assert (bitmap->GetWidth() == w);
		assert (bitmap->GetHeight() == h);
		dc.DrawBitmap (*bitmap, center_x-w/2, tLine+1, false);
	}
}

void editorCanvas::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC( dc );

	wxCoord w, h;
	dc.GetSize (&w, &h);

	wxCoord image_width = 200;
	wxCoord image_height = 200;

	if (!mModule.IsEmpty())
	{
		EditorModule *module = EditorModuleManager::getInstance().getModule(mModule);
		if (module)
		{
			wxBitmap *bitmap = module->getBitmap();
			if (module->getNumberOfSourceModules() == 0)
			{
				drawModule (dc, w/2, h/2, image_width, image_height, mModule, bitmap);
			}
			else if (module->getNumberOfSourceModules() == 1)
			{
				drawModule (dc, w/2, h/2+(image_height+titleHeight)/2+10, image_width, image_height, mModule, bitmap);
				EditorModule *smodule0 = module->getSourceModule(0);
				wxBitmap *sbitmap0 = NULL;
				wxString sname0 = _("Source");
				if (smodule0)
				{
					sname0 += wxT(": ") + module->getSourceModuleName(0);
					sbitmap0 = smodule0->getBitmap();
				}
				drawModule (dc, w/2, h/2-((image_height+titleHeight)/2+10), image_width, image_height, sname0, sbitmap0);
				dc.DrawLine (w/2, h/2-(10-2), w/2, h/2+10);
				wxPoint arrowPoints[] = { wxPoint(w/2-5, h/2), wxPoint(w/2+5, h/2), wxPoint(w/2, h/2+8) };
				dc.SetBrush (*wxBLACK_BRUSH);
				dc.DrawPolygon (3, arrowPoints);
			}
			else if (module->getNumberOfSourceModules() == 2)
			{
				drawModule (dc, w/2, h/2+(image_height+titleHeight)/2+10, image_width, image_height, mModule, bitmap);
				EditorModule *smodule0 = module->getSourceModule(0);
				wxBitmap *sbitmap0 = NULL;
				wxString sname0 = _("Source 1");
				if (smodule0)
				{
					sname0 += wxT(": ") + module->getSourceModuleName(0);
					sbitmap0 = smodule0->getBitmap();
				}
				drawModule (dc, w/2-image_width/2-image_width/3, h/2-((image_height+titleHeight)/2+10), image_width, image_height, sname0, sbitmap0);
				dc.DrawLine (w/2, h/2-(10-2)-image_height/2, w/2, h/2+10);
				dc.DrawLine (w/2-image_width/3, h/2-(10-2)-image_height/2, w/2+image_width/3, h/2-(10-2)-image_height/2);

				EditorModule *smodule1 = module->getSourceModule(1);
				wxBitmap *sbitmap1 = NULL;
				wxString sname1 = _("Source 2");
				if (smodule1)
				{
					sname1 += wxT(": ") + module->getSourceModuleName(1);
					sbitmap1 = smodule1->getBitmap();
				}
				drawModule (dc, w/2+image_width/2+image_width/3, h/2-((image_height+titleHeight)/2+10), image_width, image_height, sname1, sbitmap1);

				wxPoint arrowPoints[] = { wxPoint(w/2-5, h/2), wxPoint(w/2+5, h/2), wxPoint(w/2, h/2+8) };
				dc.DrawPolygon (3, arrowPoints);
				dc.SetBrush (*wxBLACK_BRUSH);
			}
			else if (module->getNumberOfSourceModules() == 3)
			{
				drawModule (dc, w/2, h/2+(image_height+titleHeight)/2+10, image_width, image_height, mModule, bitmap);
				EditorModule *smodule0 = module->getSourceModule(2);
				wxBitmap *sbitmap0 = NULL;
				wxString sname0 = _("Control");
				if (smodule0)
				{
					sname0 += wxT(": ") + module->getSourceModuleName(2);
					sbitmap0 = smodule0->getBitmap();
				}
				drawModule (dc, w/2, h/2-((image_height+titleHeight)/2+10), image_width, image_height, sname0, sbitmap0);
				dc.DrawLine (w/2, h/2-(10-2), w/2, h/2+10);

				EditorModule *smodule1 = module->getSourceModule(0);
				wxBitmap *sbitmap1 = NULL;
				wxString sname1 = _("Source 1");
				if (smodule1)
				{
					sname1 += wxT(": ") + module->getSourceModuleName(0);
					sbitmap1 = smodule1->getBitmap();
				}
				drawModule (dc, w/2-(image_width+10), h/2-((image_height+titleHeight)/2+10), image_width, image_height, sname1, sbitmap1);

				EditorModule *smodule2 = module->getSourceModule(1);
				wxBitmap *sbitmap2 = NULL;
				wxString sname2 = _("Source 2");
				if (smodule2)
				{
					sname2 += wxT(": ") + module->getSourceModuleName(1);
					sbitmap2 = smodule2->getBitmap();
				}
				drawModule (dc, w/2+(image_width+10), h/2-((image_height+titleHeight)/2+10), image_width, image_height, sname2, sbitmap2);

				wxPoint arrowPoints[] = { wxPoint(w/2-5, h/2), wxPoint(w/2+5, h/2), wxPoint(w/2, h/2+8) };
				dc.DrawPolygon (3, arrowPoints);
				dc.SetBrush (*wxBLACK_BRUSH);
			}
		}
		else
			drawModule (dc, w/2, h/2, 200, 200, mModule, NULL);
	}
}

void editorCanvas::setModule (const wxString &module)
{
	mModule = module;
	Refresh ();
}
