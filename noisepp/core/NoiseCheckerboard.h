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

#ifndef NOISEPP_CHECKERBOARD_H
#define NOISEPP_CHECKERBOARD_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	class CheckerboardElement1D : public PipelineElement1D
	{
		public:
			virtual Real getValue (Real x, Cache *cache) const
			{
				const int ix = (int)(floor (Math::MakeInt32Range (x)));
				return (ix & 1)? Real(-1.0) : Real(1.0);
			}
	};

	/// 1D module for generating a checkerboard pattern.
	class CheckerboardModule1D : public Module1D
	{
		public:
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline1D *pipe) const
			{
				return pipe->addElement (this, new CheckerboardElement1D);
			}
	};

	class CheckerboardElement2D : public PipelineElement2D
	{
		public:
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				const int ix = (int)(floor (Math::MakeInt32Range (x)));
				const int iy = (int)(floor (Math::MakeInt32Range (y)));
				return (ix & 1 ^ iy & 1)? Real(-1.0) : Real(1.0);
			}
	};

	/// 2D module for generating a checkerboard pattern.
	class CheckerboardModule2D : public Module2D
	{
		public:
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline2D *pipe) const
			{
				return pipe->addElement (this, new CheckerboardElement2D);
			}
	};

	class CheckerboardElement3D : public PipelineElement3D
	{
		public:
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				const int ix = (int)(floor (Math::MakeInt32Range (x)));
				const int iy = (int)(floor (Math::MakeInt32Range (y)));
				const int iz = (int)(floor (Math::MakeInt32Range (z)));
				return (ix & 1 ^ iy & 1 ^ iz & 1)? Real(-1.0) : Real(1.0);
			}
	};

	/// 3D module for generating a checkerboard pattern.
	class CheckerboardModule3D : public Module3D
	{
		public:
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline3D *pipe) const
			{
				return pipe->addElement (this, new CheckerboardElement3D);
			}
	};
};

#endif // NOISEPP_CHECKERBOARD_H
