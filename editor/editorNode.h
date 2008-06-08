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

#ifndef EDITORNODE_H
#define EDITORNODE_H

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include "Noise.h"

class editorNode
{
	public:
		editorNode(int level, int x, int y, noisepp::Module &module);
		virtual ~editorNode();
	protected:
	private:
		static void *threadEntry (void *node);
		void threadFunc ();

		int mLevel;
		int mX, mY;

		noisepp::Pipeline2D *mPipeline;
		noisepp::PipelineElement2D *mElement;

		threadpp::Thread *mThread;
		threadpp::Mutex mMutex;

		int mCurLevel;
		int mMaxLevel;
		double *mData;
};

#endif // EDITORNODE_H
