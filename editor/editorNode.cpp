// This file is part of the Noise++ Editor.
// Copyright (c) 2008, Urs C. Hanselmann
//
// The Noise++ Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Noise++ Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Noise++ Editor.  If not, see <http://www.gnu.org/licenses/>.
//

#include "editorNode.h"
#include "editorModule.h"
#include "editorModuleManager.h"

#include "NoiseUtils.h"

const int nodeMinRes = 32;
const int nodeDetailLevels = 5;

editorNode::editorNode(int level, int x, int y, noisepp::Module &module) :
	mLevel(level), mX(x), mY(y), mPipeline(0), mElement(0), mThread(0), mCurLevel(-1), mMaxLevel(-1), mData(0), mDone(false)
{
	mPipeline = noisepp::utils::System::createOptimalPipeline2D ();
	noisepp::ElementID id = module.addToPipeline (mPipeline);
	mElement = mPipeline->getElement(id);

	mThread = new threadpp::Thread (threadEntry, this);
}

void *editorNode::threadEntry (void *node)
{
	editorNode *n = static_cast<editorNode*>(node);
	assert (n);
	try
	{
		n->threadFunc();
	}
	catch (std::exception &e)
	{
		std::cout << "exception thrown: " << e.what() << std::endl;
	}
	return NULL;
}

void editorNode::threadFunc ()
{
	int res = nodeMinRes;

	double extent = (1<<mLevel);
	double lowerX = mX * extent;
	double lowerY = mY * extent;
	noisepp::utils::PlaneBuilder2D builder;
	builder.setBounds (lowerX, lowerY, lowerX+extent, lowerY+extent);

	for (int l=0;l<nodeDetailLevels;++l)
	{
		double *data = new double[res*res];
		builder.setDestination(data);
		builder.setSize(res, res);
		builder.build (mPipeline, mElement);

		threadpp::Mutex::Lock lk(mMutex);
		if (mData)
			delete[] mData;
		mData = data;
		mMaxLevel = l;
		lk.unlock ();
		res *= 2;
	}

	/*noisepp::utils::Image img;
	img.create (res, res);

	noisepp::utils::GradientRenderer gradients;
	gradients.addGradient (-1.0, noisepp::utils::ColourValue(0.0f, 0.0f, 0.2f));
	gradients.addGradient (-0.8, noisepp::utils::ColourValue(0.0f, 0.0f, 0.6f));
	gradients.addGradient ( 0.0, noisepp::utils::ColourValue(1.0f, 0.0f, 0.0f));
	gradients.addGradient ( 0.6, noisepp::utils::ColourValue(1.0f, 1.0f, 0.0f));
	gradients.addGradient ( 1.0, noisepp::utils::ColourValue(1.0f, 1.0f, 1.0f));
	gradients.renderImage (img, mData);

	glGenTextures (1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	if (GLEW_SGIS_generate_mipmap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	else
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);*/

	mDone = true;
}

editorNode::~editorNode()
{
	if (mThread)
	{
		mThread->join ();
		delete mThread;
		mThread = 0;
	}
	if (mElement)
		mElement = 0;
	if (mPipeline)
	{
		delete mPipeline;
		mPipeline = 0;
	}
	if (mData)
	{
		delete[] mData;
		mData = 0;
	}
}
