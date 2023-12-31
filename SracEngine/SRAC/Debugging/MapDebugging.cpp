#include "pch.h"
#include "MapDebugging.h"

#if MAP_DEBUGGING
#include "Game/Camera/Camera.h"
#include "Debugging/DebugDraw.h"
#include "Map/Map.h"


void renderMapBoundaries(const Map* map)
{
	VectorF topLeft = map->getFirstRect().TopLeft();
	VectorF botLeft = map->tile(Index(0, map->yCount() - 1))->rect().BotLeft();
	VectorF topRight = map->getLastRect().TopRight();
	VectorF botRight = map->getBottomLastRect().BotRight();

	VectorF mid = map->midPoint();
	VectorF topMid = VectorF(mid.x, 0.0f);
	VectorF botMid = VectorF(mid.x, botLeft.y);

	debugDrawLine(topLeft, botLeft, RenderColour::Red);
	debugDrawLine(topMid, botMid, RenderColour::Red);
	debugDrawLine(topRight, botRight, RenderColour::Red);
}


void renderSurfaceTypes(const Grid<MapTile>& data)
{
	Camera* camera = Camera::Get();

	int fontSize = 16;
	VectorF offset = VectorF(0.0f, 20.0f);

	for (unsigned int y = 0; y < data.yCount(); y++)
	{
		for (unsigned int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);
			MapTile tile = data.get(index);
			RectF tileRect = tile.rect();

			if (camera->inView(tileRect))
			{
				debugDrawRectOutline(Camera::Get()->toCameraCoords(tileRect), RenderColour::Green);

#if LABEL_SURFACE_COLLISION_TYPES
				renderCollisionTypes(tile, tileRect, offset, fontSize);
#endif

#if LABEL_SURFACE_RENDER_TYPES
				renderRenderTypes(tile, tileRect, offset, fontSize);
#endif

#if LABEL_SURFACE_DECOR_TYPES
				renderDecorTypes(tile, tileRect, offset, fontSize);
#endif

#if LABEL_TILE_INDEX
				renderTileIndexes(data, index, 12);
#endif
			}
		}
	}
}


void renderTileIndexes(const Grid<MapTile>& data, Index index, int fontSize)
{
	RenderColour colour = RenderColour::White;
	BasicString indexString = BasicString("  ") + BasicString((float)index.x) + BasicString(", ") + BasicString((float)index.y);
	debugRenderText(indexString, fontSize, data.get(index).rect().TopLeft(), colour, "None");
}

void renderRenderTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize)
{
	VectorF startingPoint = tileRect.TopLeft();
	RenderColour colour = RenderColour::Red;

	if (tile.has(RenderTile::Wall))
	{
		debugRenderText("Wall", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Floor
	if (tile.has(RenderTile::Floor_1))
	{
		debugRenderText("Floor_1", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Floor_2))
	{
		debugRenderText("Floor_2", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Floor))
	{
		debugRenderText("Floor", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Floor - Sides
	if (tile.has(RenderTile::Floor_Left))
	{
		debugRenderText("Floor left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Floor_Right))
	{
		debugRenderText("Floor right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Floor_Top))
	{
		debugRenderText("Floor top", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Floor_Bottom))
	{
		debugRenderText("Floor bottom", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Floor - Corners
	if (tile.has(RenderTile::Floor_Top_Left))
	{
		debugRenderText("Floor top left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
	if (tile.has(RenderTile::Floor_Top_Right))
	{
		debugRenderText("Floor top right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
	if (tile.has(RenderTile::Floor_Bottom_Left))
	{
		debugRenderText("Floor bottom left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
	if (tile.has(RenderTile::Floor_Bottom_Right))
	{
		debugRenderText("Floor bottom right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Floor - Grating
	if (tile.has(RenderTile::Floor_Grating))
	{
		debugRenderText("Floor grating", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Bottom walls
	if (tile.has(RenderTile::Bottom_Lower))
	{
		debugRenderText("Bottom lower", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Bottom_Upper))
	{
		debugRenderText("Bottom upper", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Top walls
	if (tile.has(RenderTile::Top_Lower))
	{
		debugRenderText("Top lower", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Top_Upper))
	{
		debugRenderText("Top upper", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Side walls
	if (tile.has(RenderTile::Right))
	{
		debugRenderText("Right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Left))
	{
		debugRenderText("Left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Bottom))
	{
		debugRenderText("Bottom", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Corners
	if (tile.has(RenderTile::Bottom_Right))
	{
		debugRenderText("Bottom right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Bottom_Left))
	{
		debugRenderText("Bottom left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Top_Right))
	{
		debugRenderText("Top right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Top_Left))
	{
		debugRenderText("Top left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Points
	if (tile.has(RenderTile::Point_Bottom_Right))
	{
		debugRenderText("bot right point", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Point_Bottom_Left))
	{
		debugRenderText("bot left point", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Point_Top_Right))
	{
		debugRenderText("top right point", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Point_Top_Left))
	{
		debugRenderText("top left point", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Column parts
	if (tile.has(RenderTile::Column_Top))
	{
		debugRenderText("column top", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Column_Upper))
	{
		debugRenderText("column upper", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Column_Lower))
	{
		debugRenderText("column lower", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Floor_ColumnBase))
	{
		debugRenderText("floor column base", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Water
	if (tile.has(RenderTile::Water_Left))
	{
		debugRenderText("Water left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Water_Middle))
	{
		debugRenderText("Water middle", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Water_Top_Left))
	{
		debugRenderText("Water top left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Water_Top))
	{
		debugRenderText("Water top", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (startingPoint == tileRect.TopLeft())
	{
		debugRenderText("No render label", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
}
void renderCollisionTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize)
{
	VectorF startingPoint = tileRect.TopLeft();
	RenderColour colour = RenderColour::Blue;

	if (tile.is(CollisionTile::Floor))
	{
		debugRenderText("Floor", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.is(CollisionTile::Wall))
	{
		debugRenderText("Wall", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.is(CollisionTile::Water))
	{
		debugRenderText("Water", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}


	if (startingPoint == tileRect.TopLeft())
	{
		debugRenderText("No collision label", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
}


void renderDecorTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize)
{
	RenderColour colour = RenderColour::Green;

	if (tile.has(DecorType::Water))
	{
		debugRenderText("Water", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(DecorType::Column))
	{
		debugRenderText("Column", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(DecorType::Torch_Handle))
	{
		debugRenderText("Torch handle", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(DecorType::Torch_Bowl))
	{
		debugRenderText("Torch bowl", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(DecorType::Spikes))
	{
		debugRenderText("Spikes", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(DecorType::Grating))
	{
		debugRenderText("Grating", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(DecorType::GratingTrigger))
	{
		debugRenderText("Grating Trigger", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
}


#endif // DEBUG