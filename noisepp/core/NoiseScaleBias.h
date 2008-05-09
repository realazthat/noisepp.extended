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

#ifndef NOISEPP_SCALEBIAS_H
#define NOISEPP_SCALEBIAS_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	/** Module for scaling with bias.
		Scales the output value from the source module by scaling with bias.
	*/
	template <class Pipeline, class Element>
	class ScaleBiasModuleBase : public Module<Pipeline>
	{
		private:
			Real mScale, mBias;

		public:
			/// Constructor.
			ScaleBiasModuleBase() : Module<Pipeline>(1), mScale(1.0), mBias(0.0)
			{
			}
			/// Sets the scaling factor.
			void setScale (Real v)
			{
				mScale = v;
			}
			/// Returns the scaling factor.
			Real getScale () const
			{
				return mScale;
			}
			/// Sets the bias value.
			void setBias (Real v)
			{
				mBias = v;
			}
			/// Returns the bias value.
			Real getBias () const
			{
				return mBias;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline *pipe) const
			{
				assert (Module<Pipeline>::getSourceModule (0));
				ElementID first = Module<Pipeline>::getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new Element(pipe, first, mScale, mBias));
			}
	};

	class ScaleBiasElement1D : public PipelineElement1D
	{
		private:
			ElementID mElement;
			const PipelineElement1D *mElementPtr;
			Real mScale, mBias;

		public:
			ScaleBiasElement1D (const Pipeline1D *pipe, ElementID element, Real scale, Real bias) : mElement(element), mScale(scale), mBias(bias)
			{
				mElementPtr = pipe->getElement(mElement);
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, cache);
				return value * mScale + mBias;
			}
	};

	class ScaleBiasElement2D : public PipelineElement2D
	{
		private:
			ElementID mElement;
			const PipelineElement2D *mElementPtr;
			Real mScale, mBias;

		public:
			ScaleBiasElement2D (const Pipeline2D *pipe, ElementID element, Real scale, Real bias) : mElement(element), mScale(scale), mBias(bias)
			{
				mElementPtr = pipe->getElement(mElement);
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, cache);
				return value * mScale + mBias;
			}
	};

	class ScaleBiasElement3D : public PipelineElement3D
	{
		private:
			ElementID mElement;
			const PipelineElement3D *mElementPtr;
			Real mScale, mBias;

		public:
			ScaleBiasElement3D (const Pipeline3D *pipe, ElementID element, Real scale, Real bias) : mElement(element), mScale(scale), mBias(bias)
			{
				mElementPtr = pipe->getElement(mElement);
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, z, cache);
				return value * mScale + mBias;
			}
	};

	/** 1D module for scaling with bias.
		Scales the output value of the source module by scaling with bias.
	*/
	class ScaleBiasModule1D : public ScaleBiasModuleBase<Pipeline1D, ScaleBiasElement1D>
	{ };
	/** 2D module for scaling with bias.
		Scales the output value of the source module by scaling with bias.
	*/
	class ScaleBiasModule2D : public ScaleBiasModuleBase<Pipeline2D, ScaleBiasElement2D>
	{ };
	/** 3D module for scaling with bias.
		Scales the output value of the source module by scaling with bias.
	*/
	class ScaleBiasModule3D : public ScaleBiasModuleBase<Pipeline3D, ScaleBiasElement3D>
	{ };
};

#endif
