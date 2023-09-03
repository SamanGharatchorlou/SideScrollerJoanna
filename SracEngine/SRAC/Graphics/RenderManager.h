#pragma once

class Texture;
struct Renderable;

struct RenderPack
{
	enum Layer
	{
		Lowest,
		Floor,
		LowerTiles,
		Actors,
		UpperTiles,
		UI,
		Highest,
		Count
	};

	RenderPack(Texture* tex, RectF box, Layer renderLayer) : texture(tex), rect(box), layer(renderLayer) { }
	Texture* texture = nullptr;
	RectF rect;
	RectF subRect = InvalidRectF;
	Layer layer = Layer::Highest;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
};

class RenderManager //: public Observer
{
public:
	RenderManager();

	void Init();

	void render();

	void AddRenderPacket(RenderPack renderPacket) { mRenderPackets.push_back(renderPacket); }

	//void handleEvent(EventData& data) override;
	void addRenderable(Renderable* renderable);
	void removeRenderable(Renderable* renderable);

private:
	void renderPacketRange(int start, int end);
	//void renderPackets(RenderLayer layer);


private:
	std::vector<Renderable*> mRenderables;
	std::vector<RenderPack> mRenderPackets;


};
