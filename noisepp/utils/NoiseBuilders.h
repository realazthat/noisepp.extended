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

#ifndef NOISEBUILDERS_H
#define NOISEBUILDERS_H

#include "NoisePrerequisites.h"
#include "NoisePipelineJobs.h"
#include "NoiseModule.h"

namespace noisepp
{
namespace utils
{

class Builder
{
	protected:
		Real *mDest;
		int mWidth, mHeight;
		Module *mModule;

		void checkParameters ();

	public:
		Builder ();
		void setSize (int width, int height);
		void setDestination (Real *dest);
		void setModule (Module *module);
		virtual void build () = 0;
		virtual ~Builder ();
};

class PlaneBuilder2D : public Builder
{
	private:
		Real mLowerBoundX, mLowerBoundY;
		Real mUpperBoundX, mUpperBoundY;
		bool mSeamless;

	public:
		PlaneBuilder2D ();
		void build (Pipeline2D *pipeline, PipelineElement2D *element);
		virtual void build ();

		void setBounds (Real lowerBoundX, Real lowerBoundY, Real upperBoundX, Real upperBoundY);
		Real getLowerBoundX () const;
		Real getLowerBoundY () const;
		Real getUpperBoundX () const;
		Real getUpperBoundY () const;
		void setSeamless (bool v=true);
		bool isSeamless () const;
};

};
};

#endif // NOISEBUILDERS_H
