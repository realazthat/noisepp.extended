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

#ifndef EDITORTURBULENCEMODULE_H
#define EDITORTURBULENCEMODULE_H

#include <editorModule.h>


class EditorTurbulenceModule : public EditorModule
{
	public:
		EditorTurbulenceModule();
		static std::string FACTORY_NAME;
		const std::string &getFactoryName () const
		{
			return FACTORY_NAME;
		}
		virtual noisepp::Module &getModule ()
		{
			return mModule;
		}
		virtual void fillPropertyGrid (wxPropertyGrid *pg);
		virtual void onPropertyChange (wxPropertyGrid *pg, const wxString &name);
		virtual bool validate (wxPropertyGrid *pg);
		virtual void writeProperties (TiXmlElement *element);
		virtual bool readProperties (TiXmlElement *element);
	protected:
	private:
		noisepp::TurbulenceModule mModule;
};

#endif // EDITORTURBULENCEMODULE_H
