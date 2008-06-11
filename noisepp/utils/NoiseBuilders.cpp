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

#include "NoiseBuilders.h"
#include "NoiseSystem.h"
#include "NoisePipelineJobs.h"
#include "NoiseMath.h"

namespace noisepp
{
namespace utils
{

Builder::Builder () : mDest(0), mWidth(0), mHeight(0), mModule(0)
{
}

void Builder::setSize (int width, int height)
{
	mWidth = width;
	mHeight = height;
}

void Builder::setDestination (Real *dest)
{
	mDest = dest;
}

void Builder::setModule (Module *module)
{
	mModule = module;
}

void Builder::checkParameters ()
{
	NoiseAssert(mDest != NULL, mDest);
	NoiseAssert(mWidth > 0, mWidth);
	NoiseAssert(mHeight > 0, mHeight);
	NoiseAssert(mModule != NULL, mModule);
}

Builder::~Builder ()
{
}

class SeamlessPlaneLineJob2D : public PipelineJob
{
	private:
		Pipeline2D *mPipe;
		PipelineElement2D *mElement;
		Real x, y;
		int n;
		Real xDelta;
		Real xExtent, yExtent;
		Real yBlend;
		Real *buffer;

	public:
		SeamlessPlaneLineJob2D (Pipeline2D *pipe, PipelineElement2D *element, Real x, Real y, int n, Real xDelta, Real xExtent, Real yExtent, Real yBlend, Real *buffer) :
			mPipe(pipe), mElement(element), x(x), y(y), n(n), xDelta(xDelta), xExtent(xExtent), yExtent(yExtent), yBlend(yBlend), buffer(buffer)
		{
		}
		void execute (Cache *cache)
		{
			Real blValue, brValue, tlValue, trValue;
			Real xBlend;
			Real yBlendM = Real(1) - yBlend;
			Real lowerX = x;
			Real y0, y1;
			for (int i=0;i<n;++i)
			{
				// cleans the cache
				mPipe->cleanCache (cache);
				// calculates the values
				blValue = mElement->getValue(x, y, cache);
				brValue = mElement->getValue(x+xExtent, y, cache);
				tlValue = mElement->getValue(x, y+yExtent, cache);
				trValue = mElement->getValue(x+xExtent, y+yExtent, cache);
				xBlend = Real(1) - ((x-lowerX) / xExtent);
				y0 = Math::InterpLinear(blValue, brValue, xBlend);
				y1 = Math::InterpLinear(tlValue, trValue, xBlend);
				buffer[i] = yBlendM * y0 + yBlend * y1;

				// move on
				x += xDelta;
			}
		}
};

PlaneBuilder2D::PlaneBuilder2D () : mLowerBoundX(0), mLowerBoundY(0), mUpperBoundX(0), mUpperBoundY(0), mSeamless(false)
{
}

void PlaneBuilder2D::build ()
{
	checkParameters ();
	NoiseAssert(mLowerBoundX < mUpperBoundX, (mLowerBoundX, mUpperBoundX));
	NoiseAssert(mLowerBoundY < mUpperBoundY, (mLowerBoundY, mUpperBoundY));

	PipelinePtr2D pipeline(System::createOptimalPipeline2D());
	ElementID id = mModule->addToPipe(*pipeline);
	PipelineElement2D *element = pipeline->getElement(id);

	Real xExtent = (mUpperBoundX - mLowerBoundX);
	Real yExtent = (mUpperBoundY - mLowerBoundY);
	Real xDelta = xExtent / (Real)mWidth;
	Real yDelta = yExtent / (Real)mHeight;
	Real yp = mLowerBoundY;
	if (!mSeamless)
	{
		for (int y=0;y<mHeight;++y)
		{
			pipeline->addJob (new LineJob2D(pipeline.get(), element, mLowerBoundX, yp, mWidth, xDelta, mDest+(y*mWidth)));
			yp += yDelta;
		}
	}
	else
	{
		for (int y=0;y<mHeight;++y)
		{
			Real yBlend = Real(1) - ((yp-mLowerBoundY) / yExtent);
			pipeline->addJob (new SeamlessPlaneLineJob2D(pipeline.get(), element, mLowerBoundX, yp, mWidth, xDelta, xExtent, yExtent, yBlend, mDest+(y*mWidth)));
			yp += yDelta;
		}
	}
	pipeline->executeJobs ();
}

void PlaneBuilder2D::setBounds (Real lowerBoundX, Real lowerBoundY, Real upperBoundX, Real upperBoundY)
{
	mLowerBoundX = lowerBoundX;
	mLowerBoundY = lowerBoundY;
	mUpperBoundX = upperBoundX;
	mUpperBoundY = upperBoundY;
}

Real PlaneBuilder2D::getLowerBoundX () const
{
	return mLowerBoundX;
}

Real PlaneBuilder2D::getLowerBoundY () const
{
	return mLowerBoundY;
}

Real PlaneBuilder2D::getUpperBoundX () const
{
	return mUpperBoundX;
}

Real PlaneBuilder2D::getUpperBoundY () const
{
	return mUpperBoundY;
}

void PlaneBuilder2D::setSeamless (bool v)
{
	mSeamless = v;
}

bool PlaneBuilder2D::isSeamless () const
{
	return mSeamless;
}

};
};
