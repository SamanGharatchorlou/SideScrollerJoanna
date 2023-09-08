#pragma once

class Texture;
struct Renderable;

constexpr u32 c_RenderLayers = 10;

struct RenderPack
{
	RenderPack(Texture* tex, RectF box, u32 renderLayer) : texture(tex), rect(box), layer(renderLayer) { }
	Texture* texture = nullptr;
	RectF rect;
	RectF subRect = InvalidRectF;
	u32 layer = c_RenderLayers;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
};

struct DebugRenderPack
{
	enum DrawType
	{
		Point,
		Line,
		RectOutline,
		RectFill,
		Quad
	};
	RectF rect;
	Colour colour;
	DrawType type;
};

class RenderManager //: public Observer
{
public:
	RenderManager();

	void render();

	void AddRenderPacket(RenderPack renderPacket) { mRenderPackets[renderPacket.layer].push_back(renderPacket); }
	//void AddRenderPacketList(RenderPack renderPacket) { mRenderPackets[renderPacket.layer].push_back(renderPacket); }

	void AddDebugRenderPacker(const DebugRenderPack& renderPack);

	//void handleEvent(EventData& data) override;
	void addRenderable(Renderable* renderable);
	void removeRenderable(Renderable* renderable);

private:
	void renderPacketRange(int start, int end);
	//void renderPackets(RenderLayer layer);


private:
	std::vector<Renderable*> mRenderables;
	//std::vector<RenderPack> mRenderPackets;

	// renderlayers + the lowest
	std::vector<RenderPack> mRenderPackets[c_RenderLayers];
	std::vector<DebugRenderPack> mDebugRenders;
};
