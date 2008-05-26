#include "NoiseWriter.h"
#include "NoiseUtils.h"

namespace noisepp
{
namespace utils
{

Writer::Writer (OutStream &stream) : mStream(stream), mModuleCount(0)
{
}

unsigned short Writer::getModuleID (const Module *module)
{
	assert (module);
	return mModules[module];
}

unsigned short Writer::addModule (const Module *module)
{
	assert (module);
	ModuleMap::iterator it = mModules.find (module);
	if (it == mModules.end())
	{
		unsigned short id = mModuleCount++;
		mModules.insert(std::make_pair(module, id));
		mModuleVec.push_back (module);

		// add child modules
		for (size_t i=0;i<module->getSourceModuleCount();++i)
		{
			const Module *child = module->getSourceModule(i);
			assert (child);
			addModule (child);
		}

		return id;
	}
	return it->second;
}

void Writer::writeModule (const Module *module)
{
	assert (module);
	unsigned short typeID = module->getType();
	mStream.write (typeID);
	module->write (mStream);
}

void Writer::writeModuleRel (const Module *module)
{
	assert (module);
	for (size_t i=0;i<module->getSourceModuleCount();++i)
	{
		const Module *child = module->getSourceModule(i);
		unsigned short id = getModuleID(child);
		mStream.write (id);
	}
}

void Writer::writePipeline ()
{
	assert (!mModuleVec.empty());
	assert (mModuleCount == mModules.size() && mModuleCount == mModuleVec.size());

	unsigned char ver = NOISE_FILE_VERSION;
	mStream.write (ver);
	mStream.write (mModuleCount);
	// writing module properties
	for (ModuleVector::iterator it=mModuleVec.begin();it!=mModuleVec.end();++it)
		writeModule(*it);
	// writing module relations
	for (ModuleVector::iterator it=mModuleVec.begin();it!=mModuleVec.end();++it)
		writeModuleRel(*it);
}

};
};
