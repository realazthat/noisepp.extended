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

#ifndef NOISEPP_RIDGEDMULTI_H
#define NOISEPP_RIDGEDMULTI_H

#include "NoisePrerequisites.h"
#include "NoiseModule.h"
#include "NoiseGenerator.h"
#include "NoisePipeline.h"

namespace noisepp
{
	/// Ridged-multifractal noise module base.
	class RidgedMultiModuleBase
	{
		protected:
			/// The frequency.
			Real mFrequency;
			/// The number of octaves.
			int mOctaveCount;
			/// The seed.
			int mSeed;
			/// The noise quality.
			int mQuality;
			/// The lacunarity.
			Real mLacunarity;
			/// The exponent.
			Real mExponent;
			/// The offset.
			Real mOffset;
			/// The gain factor.
			Real mGain;

		public:
			/// Constructor.
			RidgedMultiModuleBase () :
			mFrequency(1.0), mOctaveCount(6), mSeed(0), mQuality(NOISE_QUALITY_STD), mLacunarity(2.0), mExponent(1.0), mOffset(1.0), mGain(2.0) {}
			virtual ~RidgedMultiModuleBase () {}

			/// Sets the frequency.
			void setFrequency (Real v)
			{
				mFrequency = v;
			}
			/// Returns the frequency.
			Real getFrequency () const
			{
				return mFrequency;
			}
			/// Sets the number of octaves.
			void setOctaveCount (int v)
			{
				mOctaveCount = v;
			}
			/// Returns the number of octaves.
			int getOctaveCount () const
			{
				return mOctaveCount;
			}
			/// Sets the noise seed.
			void setSeed (int v)
			{
				mSeed = v;
			}
			/// Returns the noise seed.
			int getSeed () const
			{
				return mSeed;
			}
			/// Sets the noise quality.
			void setQuality (int v)
			{
				mQuality = v;
			}
			/// Returns the noise quality.
			int getQuality () const
			{
				return mQuality;
			}
			/// Sets the noise lacunarity.
			void setLacunarity (Real v)
			{
				mLacunarity = v;
			}
			/// Returns the noise lacunarity.
			Real getLacunarity () const
			{
				return mLacunarity;
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
			/// Sets the offset.
			void setOffset (Real v)
			{
				mOffset = v;
			}
			/// Returns the offset.
			Real getOffset () const
			{
				return mOffset;
			}
			/// Sets the gain factor.
			void setGain (Real v)
			{
				mGain = v;
			}
			/// Returns the gain factor.
			Real getGain () const
			{
				return mGain;
			}
	};

	class RidgedMultiElement1D : public PipelineElement1D
	{
		private:
			struct Octave
			{
				int seed;
				Real scale;
				Real spectralWeight;
			};
			Octave *mOctaves;
			size_t mOctaveCount;
			int mQuality;
			Real mOffset;
			Real mGain;

			NOISEPP_INLINE Real calculateGradient (Real x, int seed) const
			{
				if (mQuality == NOISE_QUALITY_STD)
					return Generator1D::calcGradientCoherentNoiseStd (x, seed);
				else if (mQuality == NOISE_QUALITY_HIGH)
					return Generator1D::calcGradientCoherentNoiseHigh (x, seed);
				else
					return Generator1D::calcGradientCoherentNoiseLow (x, seed);
			}
		public:
			RidgedMultiElement1D (size_t octaves, Real frequency, Real lacunarity, Real exponent, Real offset, Real gain, int mainSeed, int quality) : mOctaveCount(octaves), mQuality(quality), mOffset(offset), mGain(gain)
			{
				mOctaves = new Octave[mOctaveCount];
				int seed;
				Real scale = frequency;
				for (size_t o=0;o<mOctaveCount;++o)
				{
					seed = (mainSeed + int(o)) & 0x7fffffff;
					mOctaves[o].spectralWeight = pow(scale, -exponent);
					mOctaves[o].scale = scale;
					mOctaves[o].seed = seed;

					scale *= lacunarity;
				}
			}
			virtual ~RidgedMultiElement1D ()
			{
				delete mOctaves;
				mOctaves = NULL;
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real value = 0.0;
				Real signal = 0.0;
				Real weight = 1.0;

				for (size_t o=0;o<mOctaveCount;++o)
				{
					const Real nx = Math::MakeInt32Range (x * mOctaves[o].scale);
					signal = calculateGradient(nx, mOctaves[o].seed);
					signal = mOffset - std::fabs(signal);
					signal *= signal;
					signal *= weight;
					weight = signal * mGain;
					if (weight > Real(1.0))
						weight = Real(1.0);
					if (weight < Real(-1.0))
						weight = Real(-1.0);

					value += signal * mOctaves[o].spectralWeight;
				}

				return (value * Real(1.25)) - Real(1.0);
			}
	};

	/** 1D module for generating ridged-multifractal noise.
		Generates ridged-multifractal noise.
	*/
	class RidgedMultiModule1D : public Module1D, public RidgedMultiModuleBase
	{
		public:
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline1D *pipe) const
			{
				return pipe->addElement (this, new RidgedMultiElement1D(mOctaveCount, mFrequency, mLacunarity, mExponent, mOffset, mGain, mSeed, mQuality));
			}
	};

	class RidgedMultiElement2D : public PipelineElement2D
	{
		private:
			struct Octave
			{
				int seed;
				Real scale;
				Real spectralWeight;
			};
			Octave *mOctaves;
			size_t mOctaveCount;
			int mQuality;
			Real mOffset;
			Real mGain;

			NOISEPP_INLINE Real calculateGradient (Real x, Real y, int seed) const
			{
				if (mQuality == NOISE_QUALITY_STD)
					return Generator2D::calcGradientCoherentNoiseStd (x, y, seed);
				else if (mQuality == NOISE_QUALITY_HIGH)
					return Generator2D::calcGradientCoherentNoiseHigh (x, y, seed);
				else
					return Generator2D::calcGradientCoherentNoiseLow (x, y, seed);
			}
		public:
			RidgedMultiElement2D (size_t octaves, Real frequency, Real lacunarity, Real exponent, Real offset, Real gain, int mainSeed, int quality) : mOctaveCount(octaves), mQuality(quality), mOffset(offset), mGain(gain)
			{
				mOctaves = new Octave[mOctaveCount];
				int seed;
				Real scale = frequency;
				for (size_t o=0;o<mOctaveCount;++o)
				{
					seed = (mainSeed + int(o)) & 0x7fffffff;
					mOctaves[o].spectralWeight = pow(scale, -exponent);
					mOctaves[o].scale = scale;
					mOctaves[o].seed = seed;

					scale *= lacunarity;
				}
			}
			virtual ~RidgedMultiElement2D ()
			{
				delete mOctaves;
				mOctaves = NULL;
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real value = 0.0;
				Real signal = 0.0;
				Real weight = 1.0;

				for (size_t o=0;o<mOctaveCount;++o)
				{
					const Real nx = Math::MakeInt32Range (x * mOctaves[o].scale);
					const Real ny = Math::MakeInt32Range (y * mOctaves[o].scale);
					signal = calculateGradient(nx, ny, mOctaves[o].seed);
					signal = mOffset - std::fabs(signal);
					signal *= signal;
					signal *= weight;
					weight = signal * mGain;
					if (weight > Real(1.0))
						weight = Real(1.0);
					if (weight < Real(-1.0))
						weight = Real(-1.0);

					value += signal * mOctaves[o].spectralWeight;
				}

				return (value * Real(1.25)) - Real(1.0);
			}
	};

	/** 2D module for generating ridged-multifractal noise.
		Generates ridged-multifractal noise.
	*/
	class RidgedMultiModule2D : public Module2D, public RidgedMultiModuleBase
	{
		public:
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline2D *pipe) const
			{
				return pipe->addElement (this, new RidgedMultiElement2D(mOctaveCount, mFrequency, mLacunarity, mExponent, mOffset, mGain, mSeed, mQuality));
			}
	};

	class RidgedMultiElement3D : public PipelineElement3D
	{
		private:
			struct Octave
			{
				int seed;
				Real scale;
				Real spectralWeight;
			};
			Octave *mOctaves;
			size_t mOctaveCount;
			int mQuality;
			Real mOffset;
			Real mGain;

			NOISEPP_INLINE Real calculateGradient (Real x, Real y, Real z, int seed) const
			{
				if (mQuality == NOISE_QUALITY_STD)
					return Generator3D::calcGradientCoherentNoiseStd (x, y, z, seed);
				else if (mQuality == NOISE_QUALITY_HIGH)
					return Generator3D::calcGradientCoherentNoiseHigh (x, y, z, seed);
				else
					return Generator3D::calcGradientCoherentNoiseLow (x, y, z, seed);
			}
		public:
			RidgedMultiElement3D (size_t octaves, Real frequency, Real lacunarity, Real exponent, Real offset, Real gain, int mainSeed, int quality) : mOctaveCount(octaves), mQuality(quality), mOffset(offset), mGain(gain)
			{
				mOctaves = new Octave[mOctaveCount];
				int seed;
				Real scale = frequency;
				for (size_t o=0;o<mOctaveCount;++o)
				{
					seed = (mainSeed + int(o)) & 0x7fffffff;
					mOctaves[o].spectralWeight = pow(scale, -exponent);
					mOctaves[o].scale = scale;
					mOctaves[o].seed = seed;

					scale *= lacunarity;
				}
			}
			virtual ~RidgedMultiElement3D ()
			{
				delete mOctaves;
				mOctaves = NULL;
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real value = 0.0;
				Real signal = 0.0;
				Real weight = 1.0;

				for (size_t o=0;o<mOctaveCount;++o)
				{
					const Real nx = Math::MakeInt32Range (x * mOctaves[o].scale);
					const Real ny = Math::MakeInt32Range (y * mOctaves[o].scale);
					const Real nz = Math::MakeInt32Range (z * mOctaves[o].scale);
					signal = calculateGradient(nx, ny, nz, mOctaves[o].seed);
					signal = mOffset - std::fabs(signal);
					signal *= signal;
					signal *= weight;
					weight = signal * mGain;
					if (weight > Real(1.0))
						weight = Real(1.0);
					if (weight < Real(-1.0))
						weight = Real(-1.0);

					value += signal * mOctaves[o].spectralWeight;
				}

				return (value * Real(1.25)) - Real(1.0);
			}
	};

	/** 3D module for generating ridged-multifractal noise.
		Generates ridged-multifractal noise.
	*/
	class RidgedMultiModule3D : public Module3D, public RidgedMultiModuleBase
	{
		public:
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline3D *pipe) const
			{
				return pipe->addElement (this, new RidgedMultiElement3D(mOctaveCount, mFrequency, mLacunarity, mExponent, mOffset, mGain, mSeed, mQuality));
			}
	};
};

#endif
