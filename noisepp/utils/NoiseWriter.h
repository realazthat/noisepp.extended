#ifndef NOISEWRITER_H
#define NOISEWRITER_H

#include <vector>
#include <map>

#include "NoiseModule.h"
#include "NoiseOutStream.h"

namespace noisepp
{
namespace utils
{

class Writer
{
	private:
		OutStream &mStream;

		unsigned short mModuleCount;

		typedef std::map<const Module*, unsigned short> ModuleMap;
		typedef std::vector<const Module*> ModuleVector;
		ModuleMap mModules;
		ModuleVector mModuleVec;

		void writeModule (const Module *module);
		void writeModuleRel (const Module *module);
	public:
		Writer (OutStream &stream);
		unsigned short addModule (const Module *module);
		unsigned short addModule (const Module &module)
		{ return addModule (&module); }

		unsigned short getModuleID (const Module *module);

		void writePipeline ();
};

};
};

#endif // NOISEWRITER_H
