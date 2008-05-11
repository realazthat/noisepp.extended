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

#ifndef THREADPP_CONDITION_H
#define THREADPP_CONDITION_H

#include "ThreadMutex.h"

namespace threadpp
{
	/// Condition class.
	class Condition
	{
		private:
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
			pthread_cond_t mCondition;
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
			HANDLE mGate;
			HANDLE mQueue;
			HANDLE mMutex;
			unsigned mGone;
			unsigned long mBlocked;
			unsigned mWaiting;
#endif
		public:
			/// Constructor.
			THREADPP_INLINE Condition ()
#if THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
			 : mGone(0), mBlocked(0), mWaiting(0)
#endif
			{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_cond_init (&mCondition, NULL);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				mGate = CreateSemaphore(0, 1, 1, 0);
				mQueue = CreateSemaphore(0, 0, (std::numeric_limits<long>::max)(), 0);
				mMutex = CreateMutex(0, 0, 0);
#endif
			}
			/// Destructor.
			THREADPP_INLINE ~Condition ()
			{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_cond_destroy (&mCondition);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				CloseHandle (mGate);
				CloseHandle (mQueue);
				CloseHandle (mMutex);
#endif
			}
			/// Notify one waiting thread.
			THREADPP_INLINE void notifyOne ()
			{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_cond_signal (&mCondition);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				unsigned signals = 0;
				int res = 0;
				res = WaitForSingleObject(mMutex, INFINITE);
				assert(res == WAIT_OBJECT_0);
				if (mWaiting != 0) // the m_gate is already closed
				{
					if (mBlocked == 0)
					{
						res = ReleaseMutex(mMutex);
						assert(res);
						return;
					}
					++mWaiting;
					--mBlocked;
					signals = 1;
				}
				else
				{
					res = WaitForSingleObject(mGate, INFINITE);
					assert(res == WAIT_OBJECT_0);
					if (mBlocked > mGone)
					{
						if (mGone != 0)
						{
							mBlocked -= mGone;
							mGone = 0;
						}
						signals = mWaiting = 1;
						--mBlocked;
					}
					else
					{
						res = ReleaseSemaphore(mGate, 1, 0);
						assert(res);
					}
				}
				res = ReleaseMutex(mMutex);
				assert(res);

				if (signals)
				{
					res = ReleaseSemaphore(mQueue, signals, 0);
					assert(res);
				}
#endif
			}
			/// Notify all waiting threads.
			THREADPP_INLINE void notifyAll ()
			{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_cond_broadcast (&mCondition);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				unsigned signals = 0;

				int res = 0;
				res = WaitForSingleObject(mMutex, INFINITE);
				assert(res == WAIT_OBJECT_0);

				if (mWaiting != 0) // the mGate is already closed
				{
					if (mBlocked == 0)
					{
						res = ReleaseMutex(mMutex);
						assert(res);
						return;
					}

					mWaiting += (signals = mBlocked);
					mBlocked = 0;
				}
				else
				{
					res = WaitForSingleObject(mGate, INFINITE);
					assert(res == WAIT_OBJECT_0);
					if (mBlocked > mGone)
					{
						if (mGone != 0)
						{
							mBlocked -= mGone;
							mGone = 0;
						}
						signals = mWaiting = mBlocked;
						mBlocked = 0;
					}
					else
					{
						res = ReleaseSemaphore(mGate, 1, 0);
						assert(res);
					}
				}

				res = ReleaseMutex(mMutex);
				assert(res);

				if (signals)
				{
					res = ReleaseSemaphore(mQueue, signals, 0);
					assert(res);
				}
#endif
			}
			/// Wait for a signal.
			THREADPP_INLINE void wait (Mutex::Lock &mutex)
			{
#if THREADPP_PLATFORM == THREADPP_PLATFORM_UNIX
				pthread_cond_wait (&mCondition, &mutex.mMutex.mMutex);
#elif THREADPP_PLATFORM == THREADPP_PLATFORM_WINDOWS
				int res = 0;
				res = WaitForSingleObject(mGate, INFINITE);
				assert(res == WAIT_OBJECT_0);
				++mBlocked;
				res = ReleaseSemaphore(mGate, 1, 0);
				assert(res);

				mutex.unlock ();

				res = WaitForSingleObject(mQueue, INFINITE);
				assert(res == WAIT_OBJECT_0);

				unsigned was_waiting=0;
				unsigned was_gone=0;

				res = WaitForSingleObject(mMutex, INFINITE);
				assert(res == WAIT_OBJECT_0);
				was_waiting = mWaiting;
				was_gone = mGone;
				if (was_waiting != 0)
				{
					if (--mWaiting == 0)
					{
						if (mBlocked != 0)
						{
							res = ReleaseSemaphore(mGate, 1, 0); // open mGate
							assert(res);
							was_waiting = 0;
						}
						else if (mGone != 0)
							mGone = 0;
					}
				}
				else if (++mGone == ((std::numeric_limits<unsigned>::max)() / 2))
				{
					// timeout occured, normalize the m_gone count
					// this may occur if many calls to wait with a timeout are made and
					// no call to notify_* is made
					res = WaitForSingleObject(mGate, INFINITE);
					assert(res == WAIT_OBJECT_0);
					mBlocked -= mGone;
					res = ReleaseSemaphore(mGate, 1, 0);
					assert(res);
					mGone = 0;
				}
				res = ReleaseMutex(mMutex);
				assert(res);

				if (was_waiting == 1)
				{
					for (; was_gone; --was_gone)
					{
						// better now than spurious later
						res = WaitForSingleObject(mQueue, INFINITE);
						assert(res == WAIT_OBJECT_0);
					}
					res = ReleaseSemaphore(mGate, 1, 0);
					assert(res);
				}

				mutex.lock ();
#endif
			}
	};
};

#endif
