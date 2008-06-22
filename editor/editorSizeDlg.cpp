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

#include "editorSizeDlg.h"

editorSizeDlg::editorSizeDlg(wxWindow* parent, const wxString& title, int w, int h) : wxDialog(parent, -1, title, wxDefaultPosition, wxSize(180, 150))
{
	mWidth << w;
	mHeight << h;

	wxPanel *panel = new wxPanel(this, -1);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	wxArrayString excludeList;
	excludeList.Add (wxT("-"));
	wxTextValidator width_validator(wxFILTER_NUMERIC | wxFILTER_EXCLUDE_CHAR_LIST, &mWidth);
	wxTextValidator height_validator(wxFILTER_NUMERIC | wxFILTER_EXCLUDE_CHAR_LIST, &mWidth);
	width_validator.SetExcludes(excludeList);
	height_validator.SetExcludes(excludeList);

	wxStaticBox *stw = new wxStaticBox(panel, -1, wxT("Width"), wxPoint(5, 5), wxSize(80, 52));
	mWidthCtrl = new wxTextCtrl(panel, -1, mWidth, wxPoint(15, 23), wxSize(60, 25), 0, width_validator);
	wxStaticBox *sth = new wxStaticBox(panel, -1, wxT("Height"), wxPoint(90, 5), wxSize(80, 52));
	mHeightCtrl = new wxTextCtrl(panel, -1, mHeight, wxPoint(100, 23), wxSize(60, 25), 0, height_validator);

	wxButton *okButton = new wxButton(this, wxID_OK, wxT("OK"),
	                                  wxDefaultPosition, wxSize(82, 30));
	wxButton *closeButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"),
	                                     wxDefaultPosition, wxSize(82, 30));

	hbox->Add(okButton, 1);
	hbox->Add(closeButton, 1, wxLEFT, 5);

	vbox->Add(panel, 1);
	vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizer(vbox);

	Centre();
}

int editorSizeDlg::getWidth () const
{
	long value = 0;
	mWidthCtrl->GetValue().ToLong(&value);
	return (int)value;
}

int editorSizeDlg::getHeight () const
{
	long value = 0;
	mHeightCtrl->GetValue().ToLong(&value);
	return (int)value;
}
