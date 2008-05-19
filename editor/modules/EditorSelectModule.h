#ifndef EDITORSELECTMODULE_H
#define EDITORSELECTMODULE_H

#include <editorModule.h>

class EditorSelectModule : public EditorModule
{
	public:
		EditorSelectModule();
		static std::string FACTORY_NAME;
		const std::string &getFactoryName () const
		{
			return FACTORY_NAME;
		}
		virtual noisepp::Module2D &get2DModule ()
		{
			return mModule2D;
		}
		virtual noisepp::Module3D &get3DModule ()
		{
			return mModule3D;
		}
		virtual void fillPropertyGrid (wxPropertyGrid *pg);
		virtual void onPropertyChange (wxPropertyGrid *pg, const wxString &name);
		virtual bool validate (wxPropertyGrid *pg);
		virtual void writeProperties (TiXmlElement *element);
		virtual bool readProperties (TiXmlElement *element);
	protected:
	private:
		noisepp::SelectModule2D mModule2D;
		noisepp::SelectModule3D mModule3D;
};

#endif // EDITORSELECTMODULE_H
