#include "pch.h"
#include "AnimationReader.h"

#include "Animations/Animator.h"
#include "Graphics/TextureManager.h"


void AnimationReader::buildAnimator(const XMLNode animationNode, Animator& out_animator)
{
	if (animationNode)
	{
		//// id
		//const char* id = animationNode.child("ID").value();

		//// frame time
		//float frameTime = toFloat(animationNode.child("FrameTime").value());
		//out_animator.setFrameTime(frameTime);

		//// frame size
		//XMLNode frameSizeNode = animationNode.child("FrameSize");
		//float x = toFloat(frameSizeNode.attribute("x")->value());
		//float y = toFloat(frameSizeNode.attribute("y")->value());
		//VectorF frameSize(x,y);

		//// animation tiles
		//const TextureManager* textures = TextureManager::Get();
		//const FileManager* fm = FileManager::Get();

		//BasicString folder_path = fm->folderPath(FileManager::Image_Animations);
		//const char* subFolderPath = animationNode.child("Folder").value();
		//const char* fileName = animationNode.child("File").value();
		//BasicString animationFolder = folder_path + subFolderPath + "\\" + fileName;
		//Texture* texture = textures->getTexture(animationFolder.c_str(), FileManager::Image_Animations);

		//// animations
		//XMLNode animations = animationNode.child("Animations");
		//XMLNode node = animations.child();
		//while (node)
		//{
		//	// startOffset
		//	VectorF startOffset(0.0f, 0.0f);
		//	if (XMLNode::Attribute rowAttribute = node.attribute("row"))
		//	{
		//		int row = toInt(rowAttribute->value());
		//		startOffset.y = row * frameSize.y;
		//	}
		//	if (XMLNode::Attribute colAttribute = node.attribute("col"))
		//	{
		//		int col = toInt(colAttribute->value());
		//		startOffset.x = col * frameSize.x;
		//	}

		//	u32 frameCount = toInt(node.value());
		//	Animation::Action action = stringToAction(node.name());

		//	Animation animation(texture, startOffset, frameSize, frameCount, action);
		//	out_animator.addAnimation(animation);

		//	node = node.next();
		//}
	}
	else
		DebugPrint(Warning, "No animation node provided, cannot build animator");
}
