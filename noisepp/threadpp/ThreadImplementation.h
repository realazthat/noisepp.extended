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

#ifndef THREADPP_IMPLEMENTATION_H
#define THREADPP_IMPLEMENTATION_H

#include "ThreadPrerequisites.h"

namespace threadpp
{
	/// Thread object.
	class Thread
	{
		private:
			bool mJoinable;
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
			pthread_t mThread;
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
			HANDLE mThreadHandle;
			DWORD mThreadId;
#endif
		public:
			/// Creates a new thread that executes the specified routine.
			THREADPP_INLINE Thread (void *(*start_routine)(void *), void *arg=NULL) : mJoinable(true)
			{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_create(&mThread, NULL, start_routine, arg);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				mThreadHandle = CreateThread (0, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, &mThreadId);
#endif
			}
			THREADPP_INLINE ~Thread ()
			{
				if (mJoinable)
				{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
					pthread_detach(mThread);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
					CloseHandle (mThreadHandle);
#endif
				}
			}
			/// Wait for thread to exit.
			THREADPP_INLINE void join ()
			{
				assert (mJoinable);
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_join (mThread, NULL);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				WaitForSingleObject (mThreadHandle, INFINITE);
				CloseHandle (mThreadHandle);
#endif
				mJoinable = false;
			}
	};
	/// Thread group
	class ThreadGroup
	{
		private:
			typedef std::vector<Thread*> ThreadVector;
			ThreadVector mThreads;

		public:
			/// Creates a new thread.
			THREADPP_INLINE void createThread (void *(*start_routine)(void *), void *arg=NULL)
			{
				Thread *thread = new Thread (start_routine, arg);
				mThreads.push_back (thread);
			}
			/// Join all threads.
			THREADPP_INLINE void join ()
			{
				ThreadVector::iterator itEnd = mThreads.end ();
				for (ThreadVector::iterator it=mThreads.begin();it!=itEnd;++it)
				{
					(*it)->join ();
				}
			}
			/// Delete all threads.
			THREADPP_INLINE void clear ()
			{
				ThreadVector::iterator itEnd = mThreads.end ();
				for (ThreadVector::iterator it=mThreads.begin();it!=itEnd;++it)
				{
					delete *it;
				}
				mThreads.clear ();
			}
			THREADPP_INLINE ~ThreadGroup ()
			{
				clear ();
			}
	};
};

#endif
