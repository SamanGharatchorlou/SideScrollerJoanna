#include "pch.h"
#include "Configs.h"

#include "SRAC/Graphics/TextureManager.h"
#include "SRAC/Graphics/Texture.h"

static void ReadAnimationNode(XMLNode rootNode, XMLNode node, Texture* texture, Animation& out_animation)
{
	out_animation.startIndex = toInt(node.attribute("index")->value());
	out_animation.frameCount = toInt(node.value());

	out_animation.frameTime = toFloat(rootNode.child("FrameTime").value());
	if (XMLNode::Attribute attributeNode = node.attribute("speedfactor"))
	{
		out_animation.frameTime /= toFloat(attributeNode->value());
	}

	if (XMLNode::Attribute attributeNode = node.attribute("minLoops"))
	{
		out_animation.minLoops = toInt(attributeNode->value());
	}

	XMLNode frameSizeNode = rootNode.child("FrameSize");
	float x = toFloat(frameSizeNode.attribute("x")->value());
	float y = toFloat(frameSizeNode.attribute("y")->value());

	out_animation.spriteSheet.sprite = texture;
	out_animation.spriteSheet.frameSize = VectorF(x, y);
	out_animation.spriteSheet.boundaries = (texture->originalDimentions / out_animation.spriteSheet.frameSize).toInt();
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

	if (XMLNode baseSizeNode = node.child("BaseSize"))
	{
		baseSize = true;
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
