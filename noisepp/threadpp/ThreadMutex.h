// Thread++ Library
// Copyright (c) 2008 Urs C. Hanselmann
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef THREADPP_MUTEX_H
#define THREADPP_MUTEX_H

#include "ThreadPrerequisites.h"

namespace threadpp
{
	/// A mutual exclusion object
	class Mutex
	{
		friend class Condition;
		private:
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
			pthread_mutex_t mMutex;
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
			CRITICAL_SECTION mMutex;
#endif
			THREADPP_INLINE void lock ()
			{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_mutex_lock(&mMutex);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				EnterCriticalSection (&mMutex);
#endif
			}
			THREADPP_INLINE void unlock ()
			{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_mutex_unlock(&mMutex);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				LeaveCriticalSection (&mMutex);
#endif
			}
		public:
			class Lock
			{
				friend class Condition;
				private:
					Mutex &mMutex;
					bool mLocked;

				public:
					/// Scoped lock object.
					/// @param mutex The mutex.
					/// @param doLock If set to true the mutex is locked after creating.
					THREADPP_INLINE Lock (Mutex &mutex, bool doLock=true) : mMutex(mutex), mLocked(false)
					{
						if (doLock)
							lock ();
					}
					/// Locks the mutex.
					THREADPP_INLINE void lock ()
					{
						assert (!mLocked);
						mMutex.lock ();
						mLocked = true;
					}
					/// Unlocks the mutex.
					THREADPP_INLINE void unlock ()
					{
						assert (mLocked);
						mMutex.unlock ();
						mLocked = false;
					}
					/// Destructor. If mutex is locked it is unlocked here.
					THREADPP_INLINE ~Lock ()
					{
						if (mLocked)
							unlock ();
					}
			};

			/// Constructor.
			THREADPP_INLINE Mutex ()
			{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_mutex_init(&mMutex, NULL);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				InitializeCriticalSection (&mMutex);
#endif
			}
			/// Destructor.
			THREADPP_INLINE ~Mutex ()
			{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_mutex_destroy(&mMutex);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				DeleteCriticalSection (&mMutex);
#endif
			}
	};
};

#endif
