#ifndef NOISEREADER_H
#define NOISEREADER_H

#include <vector>

#include "NoiseModule.h"
#include "NoiseInStream.h"

namespace noisepp
{
namespace utils
{

class Reader
{
	private:
		InStream &mStream;

		unsigned short mModuleCount;

		typedef std::vector<Module*> ModuleVector;
		ModuleVector mModules;

		void readModule ();
		void readModuleRel (Module *module);
	public:
		Reader (InStream &stream);
		~Reader ();
		Module *getModule (unsigned short id=0);
};

};
};

#endif // NOISEREADER_H
