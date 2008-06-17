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

#include "editorGLCanvas.h"
#include "editorModule.h"
#include "editorModuleManager.h"

const wxCoord titleHeight = 20;
int attribList [] = {WX_GL_RGBA , WX_GL_DOUBLEBUFFER};

editorGLCanvas::editorGLCanvas(wxFrame *frame, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) : wxGLCanvas(parent, (wxGLCanvas*)NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , wxEmptyString, attribList),
		mFrame(frame), mInit(false), mScale(0.0), mPosX(0.5), mPosY(0.5)
{
	SetBackgroundColour (*wxWHITE);
	Connect(wxEVT_PAINT, wxPaintEventHandler(editorGLCanvas::OnPaint));
	Connect(wxEVT_SIZE, wxSizeEventHandler(editorGLCanvas::OnSize));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(editorGLCanvas::OnMouseWheel));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(editorGLCanvas::OnMouseClick));
	Connect(wxEVT_MOTION, wxMouseEventHandler(editorGLCanvas::OnMouseMove));
}

editorGLCanvas::~editorGLCanvas()
{
}

void editorGLCanvas::drawModule(wxPaintDC &dc, int center_x, int center_y, wxCoord w, wxCoord h, const wxString &name, wxBitmap *bitmap)
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
		assert (bitmap->GetWidth() == w);
		assert (bitmap->GetHeight() == h);
		dc.DrawBitmap (*bitmap, center_x-w/2, tLine+1, false);
	}
}

void editorGLCanvas::OnMouseWheel(wxMouseEvent& event)
{
	int w, h;
	GetClientSize(&w, &h);

	double scale_add = event.GetWheelRotation()/event.GetWheelDelta() * -0.1 * mScale;
	mScale += scale_add;
	if (mScale < 0.0001)
		mScale = 0.0001;
	/*mTranslateX += mTranslateX * scale_add;
	mTranslateY += mTranslateY * scale_add;*/
	Refresh(false);
}

void editorGLCanvas::OnMouseMove(wxMouseEvent& event)
{
	if (event.LeftIsDown())
	{
		mPosX -= (event.GetX() - mDownX) * mScale;
		mPosY -= (event.GetY() - mDownY) * mScale;
		mDownX = event.GetX();
		mDownY = event.GetY();
		Refresh(false);
	}
}

void editorGLCanvas::OnMouseClick(wxMouseEvent& event)
{
	if (event.GetButton() == wxMOUSE_BTN_LEFT)
	{
		mDownX = event.GetX();
		mDownY = event.GetY();
	}
}

void editorGLCanvas::OnSize(wxSizeEvent& event)
{
	// this is also necessary to update the context on some platforms
	wxGLCanvas::OnSize(event);

	// set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	int w, h;
	GetClientSize(&w, &h);
#ifndef __WXMOTIF__
	if (GetContext())
#endif
	{
		SetCurrent();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, (GLint) w, (GLint) h);
		glOrtho(0,w,0,h,0,128);
	}
}

void editorGLCanvas::init ()
{
	int w, h;
	GetClientSize(&w, &h);

	SetCurrent();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLint) w, (GLint) h);
	glOrtho(0,w,0,h,0,128);

	glewInit();

	glEnable(GL_TEXTURE_2D);

	mInit = true;
}

void editorGLCanvas::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

#ifndef __WXMOTIF__
	if (!GetContext()) return;
#endif
	SetCurrent();
	if (!mInit)
		init ();

	int w, h;
	GetClientSize(&w, &h);

	if (mScale == 0.0)
	{
		if (w > h)
			mScale = 1.0 / w;
		else
			mScale = 1.0 / h;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLint) w, (GLint) h);
	glOrtho(0,w,0,h,0,128);

	/* clear color and depth buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslated (w/2, h/2, 0);

	wxString status = _("(");
	status << mPosX - w * mScale * 0.5 << _("|") << mPosY - h * mScale * 0.5;
	status << _(") - (");
	status << mPosX + w * mScale * 0.5 << _("|") << mPosY + h * mScale * 0.5;
	status << _(")");
	mFrame->SetStatusText(status,1);

	glScaled (1.0/mScale, -1.0/mScale, 1);
	glTranslated (-mPosX, -mPosY, 0);

	wxCoord image_width = 200;
	wxCoord image_height = 200;
	if (!mModule.IsEmpty())
	{
		EditorModule *module = EditorModuleManager::getInstance().getModule(mModule);
		if (module)
		{
			GLuint texture = module->getTexture();
			glColor3f (1, 1, 1);
			glBindTexture(GL_TEXTURE_2D, texture);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 1); glVertex2d(0, 0);
				glTexCoord2f(1, 1); glVertex2d(1, 0);
				glTexCoord2f(1, 0); glVertex2d(1, 1);
				glTexCoord2f(0, 0); glVertex2d(0, 1);
			}
			glEnd();
		}
	}

	glFlush();
	SwapBuffers();
}

void editorGLCanvas::setModule (const wxString &module)
{
	mModule = module;
	Refresh ();
}

