#include "pch.h"
#include "Configs.h"

#include "SRAC/Graphics/TextureManager.h"
#include "SRAC/Graphics/Texture.h"

static void ReadAnimationNode(XMLNode rootNode, XMLNode node, Texture* texture, Animation& out_animation)
{
	out_animation.startIndex = toInt(node.attribute("index")->value());
	out_animation.frameCount = toInt(node.value());
	out_animation.frameTime = toFloat(rootNode.child("FrameTime").value());

	if (XMLNode::Attribute attributeNode = node.attribute("minLoops"))
	{
		out_animation.minLoops = toInt(attributeNode->value());
	}

	if (XMLNode::Attribute attributeNode = node.attribute("looping"))
	{
		out_animation.looping = toBool(attributeNode->value());
	}

	if (XMLNode::Attribute attributeNode = node.attribute("frameTime"))
	{
		out_animation.frameTime = toFloat(attributeNode->value());
	}

	// extend the frame time so as to match an animation with targetFrames number of frames in it
	if (XMLNode::Attribute attributeNode = node.attribute("targetFrames"))
	{
		float target_frames = toFloat(attributeNode->value());
		float time_ratio = target_frames / (float)out_animation.frameCount;
		out_animation.frameTime *= time_ratio;
	}

	XMLNode frameSizeNode = rootNode.child("FrameSize");
	const float frame_x = toFloat(frameSizeNode.attribute("x")->value());
	const float frame_y = toFloat(frameSizeNode.attribute("y")->value());

	XMLNode objectSizeNode = rootNode.child("ObjectSize");
	const float object_sx = toFloat(objectSizeNode.attribute("x")->value());
	const float object_sy = toFloat(objectSizeNode.attribute("y")->value());

	XMLNode objectPosNode = rootNode.child("ObjectPosition");
	const float object_px = toFloat(objectPosNode.attribute("x")->value());
	const float object_py = toFloat(objectPosNode.attribute("y")->value());

	XMLNode directionNode = rootNode.child("Direction");
	const int dir_x = toInt(directionNode.attribute("x")->value());
	const int dir_y = toInt(directionNode.attribute("y")->value());

	out_animation.spriteSheet.sprite = texture;
	out_animation.spriteSheet.frameSize = VectorF(frame_x, frame_y);
	out_animation.spriteSheet.objectSize = VectorF(object_sx, object_sy);
	out_animation.spriteSheet.objectPos = VectorF(object_px, object_py);
	out_animation.spriteSheet.boundaries = (texture->originalDimentions / out_animation.spriteSheet.frameSize).toInt();
	out_animation.direction = VectorI(dir_x, dir_y);
	out_animation.canFlip = StringCompare(directionNode.value(), "CanFlip");
}

void AnimationConfig::Read(XMLParser& parser)
{
	const XMLNode node = parser.rootChild("Animator");

	// id
	const char* id = node.child("ID").value();

	const char* spriteSheet = node.child("SpriteSheet").value();
	Texture* texture = TextureManager::Get()->getTexture(spriteSheet, FileManager::Folder::Image_Animations);
	if (!texture)
	{
		DebugPrint(Error, "No Sprite sheet found for this animation");
	}

	XMLNode animationsParentNode = node.child("Animations");
	XMLNode animationNode = animationsParentNode.child();
	while (animationNode)
	{
		Animation animation;
		ReadAnimationNode(node, animationNode, texture, animation);

		const char* action = animationNode.name();
		ActionState state = stringToAction(action);
		animation.action = state;

		animations.push_back(animation);

		animationNode = animationNode.next();
	}
}
