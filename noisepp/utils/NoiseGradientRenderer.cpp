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

#include "NoiseGradientRenderer.h"

namespace noisepp
{
namespace utils
{

GradientRenderer::GradientRenderer()
{
}

void GradientRenderer::addGradient (Real value, const ColourValue &color)
{
	Gradient gradient;
	gradient.value = value;
	gradient.color = color;
	mGradients.push_back (gradient);
	std::sort (mGradients.begin(), mGradients.end());
}

void GradientRenderer::renderImage (Image &image, const Real *data)
{
	NoiseAssert (mGradients.size() >= 2, mGradients);
	unsigned char *buffer = image.getPixelData ();
	int pixels = image.getWidth()*image.getHeight();
	for (int i=0;i<pixels;++i)
	{
		Real value = *data++;
		ColourValue color = mGradients.front().color;
		size_t n;
		for (n=0;n<mGradients.size();++n)
		{
			if (mGradients[n].value > value)
				break;
		}
		if (n < mGradients.size())
		{
			if (n > 0)
			{
				const Gradient &left = mGradients[n-1];
				const Gradient &right = mGradients[n];
				const float a = (float)((value - left.value) / (right.value - left.value));
				color = left.color * (1.0f-a) + right.color * a;
			}
		}
		else
			color = mGradients.back().color;
		color.writeRGB (buffer);
	}
}

GradientRenderer::~GradientRenderer()
{
}

};
};
