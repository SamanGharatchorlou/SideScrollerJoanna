#include "pch.h"
#include "Animation.h"

//#include "Graphics/Texture.h"

//void Animation::nextFrame()
//{
//	//Index bounaries = (mTexture->originalDimentions / mTileDimentions).toInt();
//	//mIndex += Index(1, 0);
//
//	//Index yIncrement(0, 0);
//	//if (mIndex.x >= bounaries.x)
//	//{
//	//	mIndex.x = 0;
//	//	yIncrement += Index(0, 1);
//	//}
//
//	//mIndex += yIncrement;
//
//	//if (mIndex.y >= bounaries.y)
//	//{
//	//	mIndex.y = 0;
//	//	mIndex.x = 0;
//	//	mLoops++;
//	//}
//}


//int Animation::currentFrame() const
//{
//	Index bounaries = (mTexture->originalDimentions / mTileDimentions).toInt();
//	//return (mIndex.y * bounaries.x) + mIndex.x;
//}


// -- Private Functions -- //

// Resize and reposition texture to keep fixed ratio and center within rect
//RectF Animation::autoSize(const RectF& rect) const
//{
//	VectorF theSize = mFrameSize.realiseSize(std::max(rect.Width(), rect.Height()));
//	VectorF offset = (rect.Size() - theSize) / 2.0f;
//	return RectF(rect.TopLeft() + offset, theSize);
//}
//
//
//RectF Animation::subTileRect(const RectF& rect) const
//{
////#if DEBUG_CHECK
////	VectorF requestSize = (mIndex + 1).toFloat() * mFrameSize;
////	VectorF objectSize = mTexture->originalDimentions;
////
////	if (requestSize.x > objectSize.x || requestSize.y > objectSize.y)
////		DebugPrint(Error, "Index(%d,%d) out of bounds", mIndex.x, mIndex.y);
////#endif
//
//	//VectorF position = mFrameSize * mIndex.toFloat();
//	//return RectF(position, mFrameSize);
//	return RectF();
//}

