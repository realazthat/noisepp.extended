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

#ifndef EDITORMAIN_H
#define EDITORMAIN_H

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/bookctrl.h>
#include <wx/listbox.h>
#include <wx/propgrid/propgrid.h>
#include <wx/aboutdlg.h>
#include <wx/choicdlg.h>
#include <wx/filedlg.h>

#include <xml/tinyxml.h>

#include "editorApp.h"
#include "editorModuleManager.h"
#include "editorCanvas.h"
#include "editorGLCanvas.h"
#include "editorSizeDlg.h"

class editorFrame: public wxFrame
{
	public:
		editorFrame(wxFrame *frame, const wxString& title);
		~editorFrame();
	private:
		wxListBox *mModuleList;
		wxPropertyGrid *mModuleProps;
		wxMenuItem *mSaveMenuItem;
		wxMenuItem *mExportMenuItem;

		wxMenu *mExportMenu;

		editorCanvas *mCanvas;
		//editorGLCanvas *mCanvas;
		wxButton *mAddBtn;
		wxButton *mRenameBtn;
		wxButton *mRemoveBtn;
		wxButton *mGenerateBtn;
		wxString mFileName;
		wxString mSelectedModule;
		bool mChanged;

		void setChanged (bool changed=true);

		void doSave ();
		void doSaveAs ();
		void newDoc ();
		void saveDoc ();
		bool loadDoc ();
		bool checkForUnsavedChanges ();

		enum
		{
			idMenuNew = wxID_NEW,
			idMenuOpen = wxID_OPEN,
			idMenuSave = wxID_SAVE,
			idMenuSaveAs = wxID_SAVEAS,
			idMenuQuit = wxID_CLOSE,
			idMenuAbout = wxID_ABOUT,
			idModuleAddBtn = 1000,
			idModuleRenBtn,
			idModuleDelBtn,
			idModuleList,
			idModuleProperties,
			idModuleGenBtn,
			idMenuExportPipeline,
			idMenuExportAsBMP
		};
		void OnClose(wxCloseEvent& event);
		void OnNew(wxCommandEvent& event);
		void OnOpen(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);
		void OnSaveAs(wxCommandEvent& event);
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnModuleAdd(wxCommandEvent& event);
		void OnModuleRen(wxCommandEvent& event);
		void OnModuleDel(wxCommandEvent& event);
		void OnModuleGen(wxCommandEvent& event);
		void OnExportPipeline(wxCommandEvent& event);
		void OnExportAsBMP(wxCommandEvent& event);
		void updateModuleSelection();
		void OnModuleSelect(wxCommandEvent& event)
		{
			updateModuleSelection();
		}
		void OnModuleProperyGridChange(wxPropertyGridEvent& event);
		EditorModule *forceModuleSelected ();
		DECLARE_EVENT_TABLE()
};


#endif // EDITORMAIN_H
