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

#ifndef NOISEPIPELINEJOBS_H
#define NOISEPIPELINEJOBS_H

#include "NoisePipeline.h"

namespace noisepp
{

class LineJob2D : public PipelineJob
{
	private:
		Pipeline2D *mPipe;
		PipelineElement2D *mElement;
		Real x, y;
		int n;
		Real xDelta;
		Real *buffer;

	public:
		LineJob2D (Pipeline2D *pipe, PipelineElement2D *element, Real x, Real y, int n, Real xDelta, Real *buffer) :
			mPipe(pipe), mElement(element), x(x), y(y), n(n), xDelta(xDelta), buffer(buffer)
		{
		}
		void execute (Cache *cache)
		{
			for (int i=0;i<n;++i)
			{
				// cleans the cache
				mPipe->cleanCache (cache);
				// calculates the value
				buffer[i] = mElement->getValue(x, y, cache);
				// move on
				x += xDelta;
			}
		}
};

class LineJob3D : public PipelineJob
{
	private:
		Pipeline3D *mPipe;
		PipelineElement3D *mElement;
		Real x, y, z;
		int n;
		Real xDelta;
		Real *buffer;

	public:
		LineJob3D (noisepp::Pipeline3D *pipe, noisepp::PipelineElement3D *element, Real x, Real y, Real z, int n, Real xDelta, Real *buffer) :
			mPipe(pipe), mElement(element), x(x), y(y), z(z), n(n), xDelta(xDelta), buffer(buffer)
		{}
		void execute (noisepp::Cache *cache)
		{
			for (int i=0;i<n;++i)
			{
				// cleans the cache
				mPipe->cleanCache (cache);
				// calculates the value
				buffer[i] = mElement->getValue(x, y, z, cache);
				// move on
				x += xDelta;
			}
		}
};

};

#endif // NOISEPIPELINEJOBS_H
