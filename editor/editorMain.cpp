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

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "editorMain.h"
#include "NoiseUtils.h"

BEGIN_EVENT_TABLE(editorFrame, wxFrame)
	EVT_CLOSE(editorFrame::OnClose)
	EVT_MENU(idMenuNew, editorFrame::OnNew)
	EVT_MENU(idMenuOpen, editorFrame::OnOpen)
	EVT_MENU(idMenuSave, editorFrame::OnSave)
	EVT_MENU(idMenuSaveAs, editorFrame::OnSaveAs)
	EVT_MENU(idMenuExportPipeline, editorFrame::OnExportPipeline)
	EVT_MENU(idMenuExportAsBMP, editorFrame::OnExportAsBMP)
	EVT_MENU(idMenuQuit, editorFrame::OnQuit)
	EVT_MENU(idMenuAbout, editorFrame::OnAbout)
	EVT_BUTTON(idModuleAddBtn, editorFrame::OnModuleAdd)
	EVT_BUTTON(idModuleRenBtn, editorFrame::OnModuleRen)
	EVT_BUTTON(idModuleDelBtn, editorFrame::OnModuleDel)
	EVT_BUTTON(idModuleGenBtn, editorFrame::OnModuleGen)
	EVT_LISTBOX(idModuleList, editorFrame::OnModuleSelect)
	EVT_PG_CHANGED(idModuleProperties, editorFrame::OnModuleProperyGridChange)
END_EVENT_TABLE()

editorFrame::editorFrame(wxFrame *frame, const wxString& title)
		: wxFrame(frame, -1, title, wxDefaultPosition, wxSize(1024, 768)), mModuleList(NULL)
{
	// create a menu bar
	wxMenuBar* mbar = new wxMenuBar();
	wxMenu* fileMenu = new wxMenu(_T(""));
	fileMenu->Append(idMenuNew, _("&New"), _("Create a new project"));
	fileMenu->AppendSeparator();
	fileMenu->Append(idMenuOpen, _("&Open"), _("Open an existing project"));
	fileMenu->AppendSeparator();
	mSaveMenuItem = fileMenu->Append(idMenuSave, _("&Save"), _("Save the project"));
	fileMenu->Append(idMenuSaveAs, _("Save &as..."), _("Save the project under different name"));
	fileMenu->AppendSeparator();
	fileMenu->Append(idMenuExportPipeline, _("Export &pipeline"), _("Export the pipeline"));

	mExportMenu = new wxMenu;
	mExportMenu->Append(idMenuExportAsBMP, _("Bitmap"), _("Export the output of the selected module as Bitmap"));
	fileMenu->AppendSubMenu(mExportMenu, wxT("&Export as ..."));

	fileMenu->AppendSeparator();
	fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the Noise++ Editor"));
	mbar->Append(fileMenu, _("&File"));

	wxMenu* helpMenu = new wxMenu(_T(""));
	helpMenu->Append(idMenuAbout, _("&About\tF1"), _("About the Noise++ Editor"));
	mbar->Append(helpMenu, _("&Help"));

	wxSplitterWindow *spl1 = new wxSplitterWindow(this, -1, wxDefaultPosition, wxDefaultSize, wxSP_BORDER | wxCLIP_CHILDREN);
	spl1->SetMinimumPaneSize (250);
	wxPanel *leftWin = new wxPanel(spl1, wxID_ANY);
	mCanvas = new editorCanvas(spl1, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	//mCanvas = new editorGLCanvas(this, spl1, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	wxBookCtrl *leftBook = new wxBookCtrl(leftWin, wxID_ANY);
	wxPanel *modulePage = new wxPanel(leftBook, wxID_ANY);
	leftBook->AddPage(modulePage, _("Modules"));

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(leftBook, 1, wxEXPAND);
    leftWin->SetSizer(sizer);

	mAddBtn = new wxButton(modulePage, idModuleAddBtn, _("Add"));
	mRenameBtn = new wxButton(modulePage, idModuleRenBtn, _("Rename"));
	mRemoveBtn = new wxButton(modulePage, idModuleDelBtn, _("Remove"));
	mGenerateBtn = new wxButton(modulePage, idModuleGenBtn, _("Generate"));
	mRenameBtn->Disable ();
	mRemoveBtn->Disable ();
	mGenerateBtn->Disable ();

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	hbox->Add(mAddBtn, 2, wxEXPAND | wxLEFT, 5);
	hbox->Add(mRenameBtn, 3, wxEXPAND | wxLEFT | wxRIGHT, 5);
	hbox->Add(mRemoveBtn, 3, wxEXPAND | wxRIGHT, 5);

	mModuleList = new wxListBox(modulePage, idModuleList, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE | wxLB_SORT);
	mModuleProps = new wxPropertyGrid(modulePage, idModuleProperties, wxDefaultPosition, wxDefaultSize, wxPG_AUTO_SORT | wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);
	mModuleProps->SetExtraStyle( wxPG_EX_HELP_AS_TOOLTIPS | wxPG_EX_PROCESS_EVENTS_IMMEDIATELY );

	vbox->Add(mModuleList, 1, wxEXPAND | wxTOP, 10);
	vbox->Add(hbox, 0, wxEXPAND | wxTOP, 10);
	vbox->Add(mModuleProps, 2, wxEXPAND | wxTOP, 10);
	vbox->Add(mGenerateBtn, 0, wxEXPAND | wxTOP | wxBOTTOM | wxALIGN_RIGHT | wxRIGHT, 5);

	modulePage->SetSizer (vbox);

	spl1->SplitVertically (leftWin, mCanvas);
	spl1->SetSashPosition (300);

	SetMenuBar(mbar);
	SetMinSize(wxSize(1024,768));

	// create a status bar
	CreateStatusBar(3);
	wxString cores;
	int nCores = noisepp::utils::System::getNumberOfCPUs();
	if (nCores > 1)
	{
		cores << nCores << wxT(" CPUs");
	}
	else
		cores = wxT("1 CPU");
	SetStatusText(cores,2);

	Centre ();

	setChanged (false);
}


editorFrame::~editorFrame()
{
}

void editorFrame::OnClose(wxCloseEvent &event)
{
	if (checkForUnsavedChanges())
		Destroy();
}

void editorFrame::OnNew(wxCommandEvent &event)
{
	if (!checkForUnsavedChanges())
		return;

	newDoc ();
}

void editorFrame::OnOpen(wxCommandEvent &event)
{
	if (!checkForUnsavedChanges())
		return;

	wxFileDialog dialog (this, _("Open file"), _(""), _(""), _("Noise++ Editor project file (*.xml)|*.xml"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_OK)
	{
		newDoc ();
		mFileName = dialog.GetPath ();
		if (!loadDoc ())
		{
			wxMessageDialog dial (this, wxT("File is in unknown format or corrupted"), wxT("Error"), wxOK | wxICON_ERROR);
			dial.ShowModal();
			newDoc ();
		}
		setChanged (false);
	}
}

void editorFrame::doSave ()
{
	if (mFileName.IsEmpty())
	{
		doSaveAs ();
	}
	else
		saveDoc ();
}

void editorFrame::doSaveAs ()
{
	wxFileDialog dialog (this, _("Save file as ..."), _(""), _(""), _("Noise++ Editor project file (*.xml)|*.xml"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (dialog.ShowModal() == wxID_OK)
	{
		mFileName = dialog.GetPath ();
		saveDoc ();
	}
}

void editorFrame::OnSave(wxCommandEvent &event)
{
	doSave ();
}

void editorFrame::OnSaveAs(wxCommandEvent &event)
{
	doSaveAs ();
}

void editorFrame::newDoc ()
{
	mFileName.Clear();
	mModuleProps->Clear();
	mModuleList->Clear();
	EditorModuleManager::getInstance().clearModules ();
	updateModuleSelection ();
	setChanged (false);
}

bool editorFrame::loadDoc ()
{
	TiXmlDocument doc(std::string(mFileName.mb_str()));
	if (!doc.LoadFile())
		return false;

	TiXmlElement *root = doc.RootElement ();
	if (root->ValueStr() != "Document")
		return false;

	if (!EditorModuleManager::getInstance().readDoc (root))
		return false;
	wxArrayString modules;
	EditorModuleManager::getInstance().fillModuleArray(modules);
	mModuleList->Append (modules);

	return true;
}

void editorFrame::saveDoc ()
{
	setChanged (false);
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	TiXmlElement *root = new TiXmlElement ("Document");
	EditorModuleManager::getInstance().writeToDoc (root);
	doc.LinkEndChild(root);

	doc.SaveFile (std::string(mFileName.mb_str()));
}

void editorFrame::setChanged (bool changed)
{
	mChanged = changed;
	bool enabled = mChanged || mFileName.IsEmpty();
	mSaveMenuItem->Enable (enabled);
}

bool editorFrame::checkForUnsavedChanges ()
{
	if (mChanged)
	{
		wxMessageDialog dialog (this, _("There are unsaved changes. Do you want to save them?"), _("Save changes?"), wxYES_NO | wxCANCEL);
		int rt = dialog.ShowModal ();
		if (rt == wxID_YES)
			doSave ();
		else if (rt == wxID_CANCEL)
			return false;
	}
	return true;
}

void editorFrame::OnModuleAdd(wxCommandEvent& event)
{
	wxArrayString moduleTypes;
	EditorModuleManager::getInstance().fillFactoryArray(moduleTypes);

	wxSingleChoiceDialog moduleTypeDialog(this, wxT("Please select the type of the new module"), wxT("Type of new module"), moduleTypes);
	if (moduleTypeDialog.ShowModal() != wxID_OK)
		return;
	wxString moduleType = moduleTypeDialog.GetStringSelection();

	wxString defaultName = moduleType;
	for (int n=1;EditorModuleManager::getInstance().moduleExists(defaultName);++n)
	{
		defaultName = moduleType;
		defaultName << n;
	}

	wxTextEntryDialog dialog (this, wxT("Please enter the name of the new module"), wxT("Name of new module"), defaultName);
	if (dialog.ShowModal() == wxID_OK)
	{
		wxString moduleName = dialog.GetValue();
		moduleName.Trim();
		if (moduleName.IsEmpty())
		{
			wxMessageDialog dial (this, wxT("Module name is empty"), wxT("Error"), wxOK | wxICON_ERROR);
			dial.ShowModal();
		}
		else
		{
			// create the module
			if (!EditorModuleManager::getInstance().createModule(moduleName, moduleType))
			{
				wxMessageDialog dial (this, wxT("Module with this name already exists"), wxT("Error"), wxOK | wxICON_ERROR);
				dial.ShowModal();
			}
			else
			{
				setChanged ();
				// append it to the list
				int id = mModuleList->Append (moduleName);
				mModuleList->Select (id);
				// update selection
				updateModuleSelection();
			}
		}
	}
}

void editorFrame::OnModuleRen(wxCommandEvent& event)
{
	int id = mModuleList->GetSelection();
	if (id != wxNOT_FOUND)
	{
		wxTextEntryDialog dialog (this, wxT("Please enter the new name"), wxT("Rename module"), mModuleList->GetString(id));
		if (dialog.ShowModal() == wxID_OK)
		{
			wxString moduleName = dialog.GetValue();
			moduleName.Trim();
			if (moduleName.IsEmpty())
			{
				wxMessageDialog dial (this, wxT("Module name is empty"), wxT("Error"), wxOK | wxICON_ERROR);
				dial.ShowModal();
			}
			else if (moduleName != mModuleList->GetString(id))
			{
				// rename the module
				if (!EditorModuleManager::getInstance().renameModule(mModuleList->GetString(id), moduleName))
				{
					wxMessageDialog dial (this, wxT("Module with this name already exists"), wxT("Error"), wxOK | wxICON_ERROR);
					dial.ShowModal();
				}
				else
				{
					setChanged ();
					// remove it from list
					mModuleList->Delete (id);
					// append it again
					id = mModuleList->Append (moduleName);
					mModuleList->Select (id);
					// update selection
					updateModuleSelection();
				}
			}
		}
	}
	else
	{
		wxMessageDialog dial (this, wxT("No module selected"), wxT("Error"), wxOK | wxICON_ERROR);
		dial.ShowModal();
	}
}

void editorFrame::OnModuleDel(wxCommandEvent& event)
{
	int id = mModuleList->GetSelection();
	if (id != wxNOT_FOUND)
	{
		wxMessageDialog dialog (this, wxT("Are you sure to remove the module \"") + mModuleList->GetString(id) + wxT("\""), wxT("Remove?"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
		if (dialog.ShowModal() == wxID_YES)
		{
			setChanged ();
			// remove the module
			EditorModuleManager::getInstance().removeModule(mModuleList->GetString(id));
			// remove it from list
			mModuleList->Delete (id);
		}
	}
	else
	{
		wxMessageDialog dial (this, wxT("No module selected"), wxT("Error"), wxOK | wxICON_ERROR);
		dial.ShowModal();
	}
}

void editorFrame::OnModuleGen(wxCommandEvent& event)
{
	int id = mModuleList->GetSelection();
	if (id != wxNOT_FOUND)
	{
		EditorModule *module = EditorModuleManager::getInstance().getModule(mModuleList->GetString(id));
		if (module)
		{
			module->validate(NULL);
			assert (module->validate(mModuleProps));
			for (int i=0;i<module->getNumberOfSourceModules();++i)
			{
				module->getSourceModule(i)->generate (0, 0, 1, 1, 200, 200);
			}
			module->generate (0, 0, 1, 1, 200, 200);
			mCanvas->Refresh ();
		}
	}
}

void editorFrame::updateModuleSelection()
{
	mModuleProps->Clear();
	int id = mModuleList->GetSelection();
	if (id != wxNOT_FOUND)
	{
		// selection changed
		EditorModule *module = EditorModuleManager::getInstance().getModule(mModuleList->GetString(id));
		if (module)
		{
			mCanvas->setModule(mModuleList->GetStringSelection());
			module->fillPropertyGrid(mModuleProps);
			bool valid = module->validate (mModuleProps);

			mRenameBtn->Enable ();
			mRemoveBtn->Enable ();
			mGenerateBtn->Enable (valid);
		}
	}
	else
	{
		// module killed
		mCanvas->setModule();
		mRenameBtn->Disable ();
		mRemoveBtn->Disable ();
		mGenerateBtn->Disable ();
	}
}

void editorFrame::OnQuit(wxCommandEvent &event)
{
	if (checkForUnsavedChanges())
		Destroy();
}

void editorFrame::OnAbout(wxCommandEvent &event)
{
	wxAboutDialogInfo info;
    info.SetName(_("The Noise++ Editor"));
    info.SetCopyright(_T("Copyright (C) 2008 by Urs C. Hanselmann"));

    wxAboutBox(info);
}

void editorFrame::OnModuleProperyGridChange(wxPropertyGridEvent& event)
{
	EditorModule *module = EditorModuleManager::getInstance().getModule(mModuleList->GetStringSelection());
	if (module)
	{
		setChanged ();
		module->changeProperty(mModuleProps, event);
		bool valid = module->validate (mModuleProps);
		mGenerateBtn->Enable (valid);
		if (valid)
		{
			module->generate (0, 0, 1, 1, 200, 200);
			mCanvas->Refresh ();
		}
	}
}

EditorModule *editorFrame::forceModuleSelected ()
{
	EditorModule *module = EditorModuleManager::getInstance().getModule(mModuleList->GetStringSelection());
	if (!module)
	{
		wxMessageDialog dial (this, wxT("No module selected"), wxT("Error"), wxOK | wxICON_ERROR);
		dial.ShowModal();
	}
	return module;
}

void editorFrame::OnExportPipeline(wxCommandEvent& event)
{
	EditorModule *module = forceModuleSelected();
	if (module)
	{
		wxFileDialog dialog (this, _("Export pipeline as ..."), _(""), _(""), _("Noise++ pipeline (*.pipeline)|*.pipeline"), wxFD_SAVE);
		if (dialog.ShowModal() == wxID_OK)
		{
			wxString path = dialog.GetPath ();
			if (!module->exportToFile(path.mb_str()))
			{
				wxMessageDialog dial (this, wxT("Can't open file for writing"), wxT("Error"), wxOK | wxICON_ERROR);
				dial.ShowModal();
			}
		}
	}
}

void editorFrame::OnExportAsBMP(wxCommandEvent& event)
{
	EditorModule *module = forceModuleSelected();
	if (module)
	{
		editorSizeDlg dlg (this, _("Bitmap resolution"), 512, 512);
		if (dlg.ShowModal() == wxID_OK)
		{
			const int maxres = (1<<15);
			int width = dlg.getWidth();
			int height = dlg.getHeight();
			if (width > 0 && width <= maxres && height > 0 && height <= maxres)
			{
				wxFileDialog dialog (this, _("Export as Bitmap ..."), _(""), _(""), _("Bitmap (*.bmp)|*.bmp"), wxFD_SAVE);
				if (dialog.ShowModal() == wxID_OK)
				{
					wxString path = dialog.GetPath ();
					if (!module->exportToBMP(path.mb_str(), width, height))
					{
						wxMessageDialog dial (this, wxT("Can't open file for writing"), wxT("Error"), wxOK | wxICON_ERROR);
						dial.ShowModal();
					}
				}
			}
			else
			{
				wxMessageDialog dial (this, wxT("Invalid size parameters"), wxT("Error"), wxOK | wxICON_ERROR);
				dial.ShowModal();
			}
		}
	}
}
