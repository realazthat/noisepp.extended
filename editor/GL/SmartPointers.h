#ifndef SMARTPOINTERS_H
#define SMARTPOINTERS_H

#include <GL/glew.h>
#include <ThreadMutex.h>

#include <cassert>
#include <algorithm>

namespace GL
{

template <class T>
class SharedPtr
{
	protected:
		threadpp::Mutex *mMutex;
		GLuint mPtr;
		unsigned int *mUseCount;
	public:
		SharedPtr () : mMutex(0), mPtr(0), mUseCount(0)
		{
		}
		explicit SharedPtr(GLuint ptr) : mMutex(0), mPtr(ptr), mUseCount(new unsigned int(1))
		{
			mMutex = new threadpp::Mutex;
		}
		SharedPtr (const SharedPtr &r) : mMutex(0), mPtr(0), mUseCount(0)
		{
			if (r.mMutex)
			{
				threadpp::Mutex::Lock lk(*r.mMutex);
				mMutex = r.mMutex;
				mPtr = r.mPtr;
				mUseCount = r.mUseCount;
				if(mUseCount)
					++(*mUseCount);
			}
		}
		SharedPtr& operator=(const SharedPtr& r)
		{
			if (mPtr == r.mPtr)
				return *this;
			T tmp(r);
			swap(tmp);
			return *this;
		}
		virtual ~SharedPtr ()
		{
			release ();
		}
		inline GLuint operator*() const { return mPtr; }
		inline GLuint get() const { return mPtr; }
		inline bool isNull(void) const { return mPtr == 0; }
		inline void setNull(void)
		{
			if (mPtr)
			{
				release();
				mPtr = 0;
				mUseCount = 0;
			}
        }

	protected:
		virtual void swap(SharedPtr &other)
		{
			std::swap(mPtr, other.mPtr);
			std::swap(mUseCount, other.mUseCount);
			std::swap(mMutex, other.mMutex);
		}
		inline void release(void)
        {
			bool destroyThis = false;
            if (mMutex)
			{
				threadpp::Mutex::Lock lk(*mMutex);
				if (mUseCount)
				{
					if (--(*mUseCount) == 0)
					{
						destroyThis = true;
	                }
				}
            }
			if (destroyThis)
				destroy();
            mMutex = 0;
        }
        virtual void free () = 0;
        void destroy(void)
        {
            free ();
            delete mUseCount;
			delete mMutex;
        }
};

class TexturePtr : public SharedPtr<TexturePtr>
{
	public:
		static GLuint create ()
		{
			GLuint tex = 0;
			glGenTextures (1, &tex);
			return tex;
		}
		virtual void free ()
		{
			glDeleteTextures (1, &mPtr);
		}
};

};

#endif // SMARTPOINTERS_H
