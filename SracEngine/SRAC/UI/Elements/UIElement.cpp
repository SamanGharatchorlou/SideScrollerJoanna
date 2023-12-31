#include "pch.h"
#include "UIElement.h"

UIElement::UIElement(const StringMap32& attributes) 
{
	fill(attributes);
}

void UIElement::fill(const StringMap32& attributes)
{
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");

	// If one dimention is not set, auto size keeping the texture ratio
	float width = attributes.contains("width") ? attributes.getFloat("width") : -1.0f;
	float height = attributes.contains("height") ? attributes.getFloat("height") : -1.0f;
	VectorF size(width, height);

#if DEBUG_CHECK
	if (size.x < 0.0f && size.y < 0.0f)
		DebugPrint(Warning, "Either the height OR the width must at least be set. Neither has been");
#endif

	mRect = RectF(VectorF(x, y), VectorF(width, height));

	if(attributes.contains("id"))
		mId = attributes.at("id");
}


void UIElement::setRect(RectF rect)
{
	mRect = rect;
}