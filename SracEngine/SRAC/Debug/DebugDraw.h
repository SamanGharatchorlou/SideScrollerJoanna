#pragma once


void debugDrawPoint(VectorF point, float size, Colour colour);
void debugDrawLine(VectorF pointA, VectorF pointB, Colour colour);
void debugDrawRectOutline(RectF rect, Colour colour, bool translateToCameraCoords = false);
void debugDrawRect(RectF rect, Colour colour);
void debugDrawRects(std::vector<RectF> rects, Colour colour);
void debugDrawQuad(Quad2D<float> quad, Colour colour);
void debugRenderText(const BasicString text, int ptSize, VectorF position, Colour colour, const char* alignment = "Center");