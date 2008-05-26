#include <stdexcept>

#include "NoiseReader.h"
#include "Noise.h"
#include "NoiseUtils.h"

namespace noisepp
{
namespace utils
{

Reader::Reader (InStream &stream) : mStream(stream), mModuleCount(0)
{
	unsigned char ver;
	stream.read (ver);
	if (ver != NOISE_FILE_VERSION)
		throw std::runtime_error ("Input file has wrong version");
	stream.read (mModuleCount);
	for (unsigned short i=0;i<mModuleCount;++i)
	{
		readModule ();
	}
	for (ModuleVector::iterator it=mModules.begin();it!=mModules.end();++it)
	{
		readModuleRel (*it);
	}
}

void Reader::readModule ()
{
	unsigned short typeID;
	mStream.read (typeID);
	Module *module = NULL;
	switch (typeID)
	{
		case MODULE_PERLIN:
			module = new PerlinModule;
			break;
		case MODULE_BILLOW:
			module = new BillowModule;
			break;
		case MODULE_ADDITION:
			module = new AdditionModule;
			break;
		case MODULE_ABSOLUTE:
			module = new AbsoluteModule;
			break;
		case MODULE_BLEND:
			module = new BlendModule;
			break;
		case MODULE_CHECKERBOARD:
			module = new CheckerboardModule;
			break;
		case MODULE_CLAMP:
			module = new ClampModule;
			break;
		case MODULE_CONSTANT:
			module = new ConstantModule;
			break;
		case MODULE_CURVE:
			module = new CurveModule;
			break;
		case MODULE_EXPONENT:
			module = new ExponentModule;
			break;
		case MODULE_INVERT:
			module = new InvertModule;
			break;
		case MODULE_MAXIMUM:
			module = new MaximumModule;
			break;
		case MODULE_MINIMUM:
			module = new MinimumModule;
			break;
		case MODULE_MULTIPLY:
			module = new MultiplyModule;
			break;
		case MODULE_POWER:
			module = new PowerModule;
			break;
		case MODULE_RIDGEDMULTI:
			module = new RidgedMultiModule;
			break;
		case MODULE_SCALEBIAS:
			module = new ScaleBiasModule;
			break;
		case MODULE_SELECT:
			module = new SelectModule;
			break;
		case MODULE_SCALEPOINT:
			module = new ScalePointModule;
			break;
		case MODULE_TURBULENCE:
			module = new TurbulenceModule;
			break;
		case MODULE_TERRACE:
			module = new TerraceModule;
			break;
		case MODULE_TRANSLATEPOINT:
			module = new TranslatePointModule;
			break;
		case MODULE_VORONOI:
			module = new VoronoiModule;
			break;
	}
	if (!module)
		throw std::runtime_error ("Invalid module type ID");
	assert (module->getType() == typeID);
	module->read (mStream);
	mModules.push_back (module);
}

void Reader::readModuleRel (Module *module)
{
	assert (module);
	for (size_t i=0;i<module->getSourceModuleCount();++i)
	{
		unsigned short id;
		mStream.read (id);
		Module *child = getModule(id);
		if (!child)
			throw std::runtime_error ("Wrong child attribute");
		module->setSourceModule(i, child);
	}
}

Module *Reader::getModule (unsigned short id)
{
	if (id < mModules.size())
		return mModules[id];
	return NULL;
}

Reader::~Reader ()
{
	for (ModuleVector::iterator it=mModules.begin();it!=mModules.end();++it)
	{
		delete *it;
	}
	mModules.clear ();
}

};
};
