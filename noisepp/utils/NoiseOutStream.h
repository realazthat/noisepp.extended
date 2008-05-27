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

#ifndef NOISEOUTSTREAM_H
#define NOISEOUTSTREAM_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

#include "NoiseEndianUtils.h"

namespace noisepp
{
namespace utils
{

class OutStream
{
	public:
		OutStream();
		virtual void write (const void *buffer, size_t len) = 0;
		virtual size_t tell () = 0;
		virtual void seek (size_t pos) = 0;
		virtual ~OutStream();

		template <class T>
		void write (T t)
		{
			EndianUtils::flipEndian (&t, sizeof(T));
			write (&t, sizeof(T));
		}
		void writeInt (int t)
		{
			write (t);
		}
		void writeDouble (double t)
		{
			write (t);
		}
	protected:
	private:
};

class FileOutStream : public OutStream
{
	private:
		std::ofstream mFile;
	public:
		FileOutStream();
		FileOutStream(const std::string &filename);
		bool open (const std::string &filename);
		bool isOpen () const;
		void close ();
		template <class T>
		void write (T t)
		{
			EndianUtils::flipEndian (&t, sizeof(T));
			write (&t, sizeof(T));
		}
		virtual void write (const void *buffer, size_t len);
		virtual size_t tell ();
		virtual void seek (size_t pos);
};

class MemoryOutStream : public OutStream
{
	private:
		char *mBuffer;
		size_t mPosition, mSize, mRealSize;
	public:
		MemoryOutStream ();
		void clear ();
		char *getBuffer ()
		{
			return mBuffer;
		}
		size_t getBufferSize () const
		{
			return mSize;
		}
		template <class T>
		void write (T t)
		{
			EndianUtils::flipEndian (&t, sizeof(T));
			write (&t, sizeof(T));
		}
		virtual ~MemoryOutStream ();
		virtual void write (const void *buffer, size_t len);
		virtual size_t tell ();
		virtual void seek (size_t pos);
};

};
};

#endif // NOISEOUTSTREAM_H
