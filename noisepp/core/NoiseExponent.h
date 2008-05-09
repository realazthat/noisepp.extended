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

#ifndef NOISEPP_EXPONENT_H
#define NOISEPP_EXPONENT_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	/// Exponent module base class
	template <class Pipeline, class Element>
	class ExponentModuleBase : public Module<Pipeline>
	{
		private:
			Real mExponent;

		public:
			/// Constructor.
			ExponentModuleBase() : Module<Pipeline>(1), mExponent(1.0)
			{
			}
			/// Sets the exponent.
			void setExponent (Real v)
			{
				mExponent = v;
			}
			/// Returns the exponent.
			Real getExponent () const
			{
				return mExponent;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline *pipe) const
			{
				assert (Module<Pipeline>::getSourceModule (0));
				ElementID first = Module<Pipeline>::getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new Element(pipe, first, mExponent));
			}
	};

	class ExponentElement1D : public PipelineElement1D
	{
		private:
			ElementID mElement;
			Real mExponent;
			const PipelineElement1D *mElementPtr;

		public:
			ExponentElement1D (const Pipeline1D *pipe, ElementID element, Real exponent) : mElement(element), mExponent(exponent)
			{
				mElementPtr = pipe->getElement(element);
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, cache);
				return (std::pow (std::fabs ((value + Real(1.0)) / Real(2.0)), mExponent) * Real(2.0) - Real(1.0));
			}
	};

	class ExponentElement2D : public PipelineElement2D
	{
		private:
			ElementID mElement;
			Real mExponent;
			const PipelineElement2D *mElementPtr;

		public:
			ExponentElement2D (const Pipeline2D *pipe, ElementID element, Real exponent) : mElement(element), mExponent(exponent)
			{
				mElementPtr = pipe->getElement(element);
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, cache);
				return (std::pow (std::fabs ((value + Real(1.0)) / Real(2.0)), mExponent) * Real(2.0) - Real(1.0));
			}
	};

	class ExponentElement3D : public PipelineElement3D
	{
		private:
			ElementID mElement;
			Real mExponent;
			const PipelineElement3D *mElementPtr;

		public:
			ExponentElement3D (const Pipeline3D *pipe, ElementID element, Real exponent) : mElement(element), mExponent(exponent)
			{
				mElementPtr = pipe->getElement(element);
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, z, cache);
				return (std::pow (std::fabs ((value + Real(1.0)) / Real(2.0)), mExponent) * Real(2.0) - Real(1.0));
			}
	};

	/** 1D exponent module.
		Normalizes the output of the source module and exponentiates it
	*/
	class ExponentModule1D : public ExponentModuleBase<Pipeline1D, ExponentElement1D>
	{ };
	/** 2D exponent module.
		Normalizes the output of the source module and exponentiates it
	*/
	class ExponentModule2D : public ExponentModuleBase<Pipeline2D, ExponentElement2D>
	{ };
	/** 3D exponent module.
		Normalizes the output of the source module and exponentiates it
	*/
	class ExponentModule3D : public ExponentModuleBase<Pipeline3D, ExponentElement3D>
	{ };
};

#endif
