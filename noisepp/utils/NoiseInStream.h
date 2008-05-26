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

#ifndef NOISEINSTREAM_H
#define NOISEINSTREAM_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

#include "NoiseEndianUtils.h"

namespace noisepp
{
namespace utils
{

class InStream
{
	public:
		InStream();
		virtual void read (void *buffer, size_t len) = 0;
		virtual size_t tell () = 0;
		virtual void seek (size_t pos) = 0;
		virtual ~InStream();

		template <class T>
		void read (T &t)
		{
			read (&t, sizeof(T));
			EndianUtils::flipEndian (&t, sizeof(T));
		}

		int readInt ()
		{
			int i;
			read (i);
			return i;
		}

		double readDouble ()
		{
			double d;
			read (d);
			return d;
		}
	protected:
	private:
};

class FileInStream : public InStream
{
	private:
		std::ifstream mFile;
	public:
		FileInStream();
		FileInStream(const std::string &filename);
		bool open (const std::string &filename);
		bool isOpen () const;
		void close ();
		virtual void read (void *buffer, size_t len);
		virtual size_t tell ();
		virtual void seek (size_t pos);
};

class MemoryInStream : public InStream
{
	private:
		char *mBuffer;
		size_t mPosition, mSize;
	public:
		MemoryInStream ();
		void open (char *buffer, size_t size);
		char *getBuffer ()
		{
			return mBuffer;
		}
		size_t getBufferSize () const
		{
			return mSize;
		}
		virtual void read (void *buffer, size_t len);
		virtual size_t tell ();
		virtual void seek (size_t pos);
};

};
};

#endif // NOISEINSTREAM_H
