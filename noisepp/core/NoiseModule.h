// Noise++ Library
// Copyright (c) 2008, Urs C. Hanselmann
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright notice,
//      this list of conditions and the following disclaimer in the documentation
//      and/or other materials provided with the distribution.
//    * Neither the name of the Noise++ Library's copyright owner nor the names
//      of its contributors may be used to endorse or promote products derived
//      from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef NOISEPP_MODULE_H
#define NOISEPP_MODULE_H

#include "NoisePrerequisites.h"
#include "NoisePipeline.h"

namespace noisepp
{
	/// Module base class.
	template <class Pipeline>
	class Module
	{
		protected:
			/// Array of source modules.
			const Module<Pipeline> **mSourceModules;
			/// Number of source modules.
			size_t mSourceModuleCount;

		public:
			/// @param sourceModuleCount The number of source modules.
			Module (size_t sourceModuleCount=0) : mSourceModules(NULL), mSourceModuleCount(sourceModuleCount)
			{
				if (mSourceModuleCount)
				{
					mSourceModules = new const Module<Pipeline> *[mSourceModuleCount];
					memset (mSourceModules, 0, mSourceModuleCount*sizeof(Module<Pipeline>*));
				}
			}
			/// Returns the number of source modules.
			size_t getSourceModuleCount () const
			{
				return mSourceModuleCount;
			}
			/// Recursively looks for the specified module in all source modules.
			/// @param module The module to look for.
			bool walkTree (const Module<Pipeline> *module) const
			{
				for (size_t i=0;i<mSourceModuleCount;++i)
				{
					if (mSourceModules[i] == module)
						return true;
				}
				for (size_t i=0;i<mSourceModuleCount;++i)
				{
					if (mSourceModules[i] && mSourceModules[i]->walkTree(module))
						return true;
				}
				return false;
			}
			/// Sets a source module
			/// @param id The place to put the module.
			/// @param module The source module.
			void setSourceModule (size_t id, const Module<Pipeline> *module)
			{
				assert (id < mSourceModuleCount);
				assert (module);
				assert (module != this);
				assert (!module->walkTree(this));
				mSourceModules[id] = module;
			}
			/// @copydoc setSourceModule(size_t, const Module<Pipeline>*).
			void setSourceModule (size_t id, const Module<Pipeline> &module)
			{
				setSourceModule (id, &module);
			}
			/// Returns the specified source module.
			const Module<Pipeline> *getSourceModule (size_t id) const
			{
				return mSourceModules[id];
			}
			/// Adds the module to the specified pipeline.
			virtual ElementID addToPipeline (Pipeline *pipe) const = 0;
			/// Adds the module to the specified pipeline.
			ElementID addToPipe (Pipeline &pipe) const
			{
				return addToPipeline(&pipe);
			}
			virtual ~Module ()
			{
				if (mSourceModules)
				{
					delete[] mSourceModules;
					mSourceModules = NULL;
				}
			}
	};

	/// Template class for a module with one source module
	template <class Pipeline, class Element>
	class SingleSourceModule : public Module<Pipeline>
	{
		public:
			/// Constructor.
			SingleSourceModule() : Module<Pipeline>(1)
			{
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline *pipe) const
			{
				assert (Module<Pipeline>::getSourceModule (0));
				ElementID first = Module<Pipeline>::getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new Element(pipe, first));
			}
	};

	/// Template class for a module with two source modules
	template <class Pipeline, class Element>
	class DualSourceModule : public Module<Pipeline>
	{
		public:
			/// Constructor.
			DualSourceModule() : Module<Pipeline>(2)
			{
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline *pipe) const
			{
				assert (Module<Pipeline>::getSourceModule (0));
				assert (Module<Pipeline>::getSourceModule (1));
				ElementID first = Module<Pipeline>::getSourceModule(0)->addToPipeline(pipe);
				ElementID second = Module<Pipeline>::getSourceModule(1)->addToPipeline(pipe);
				return pipe->addElement (this, new Element(pipe, first, second));
			}
	};

	/// Basic template class for a module with two source modules and a control module
	template <class Pipeline>
	class TripleSourceModuleBase : public Module<Pipeline>
	{
		public:
			/// Constructor.
			TripleSourceModuleBase() : Module<Pipeline>(3)
			{
			}
			/// Sets the control module.
			void setControlModule (const Module<Pipeline> &module)
			{
				Module<Pipeline>::setSourceModule (2, module);
			}
			/// Sets the control module.
			void setControlModule (const Module<Pipeline> *module)
			{
				Module<Pipeline>::setSourceModule (2, module);
			}
			/// Returns the control module.
			const Module<Pipeline> *getControlModule () const
			{
				return Module<Pipeline>::getSourceModule(2);
			}
	};

	/// Template class for a module with two source modules and a control module
	template <class Pipeline, class Element>
	class TripleSourceModule : public TripleSourceModuleBase<Pipeline>
	{
		public:
			/// for a module with two source modules and a control module
			virtual ElementID addToPipeline (Pipeline *pipe) const
			{
				assert (Module<Pipeline>::getSourceModule (0));
				assert (Module<Pipeline>::getSourceModule (1));
				assert (Module<Pipeline>::getSourceModule (2));
				ElementID first = Module<Pipeline>::getSourceModule(0)->addToPipeline(pipe);
				ElementID second = Module<Pipeline>::getSourceModule(1)->addToPipeline(pipe);
				ElementID third = Module<Pipeline>::getSourceModule(2)->addToPipeline(pipe);
				return pipe->addElement (this, new Element(pipe, first, second, third));
			}
	};

	/// Basic type for 1D-modules
	typedef Module<Pipeline1D> Module1D;
	/// Basic type for 2D-modules
	typedef Module<Pipeline2D> Module2D;
	/// Basic type for 3D-modules
	typedef Module<Pipeline3D> Module3D;
};

#endif
