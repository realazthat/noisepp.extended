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

#ifndef EDITORSIZEDLG_H
#define EDITORSIZEDLG_H

#include <wx/wx.h>

class editorSizeDlg : public wxDialog
{
	private:
		wxString mWidth;
		wxTextCtrl *mWidthCtrl;
		wxString mHeight;
		wxTextCtrl *mHeightCtrl;

	public:
		editorSizeDlg(wxWindow* parent, const wxString& title, int width, int height);
		int getWidth () const;
		int getHeight () const;
};

#endif // EDITORSIZEDLG_H
