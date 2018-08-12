#pragma once
#include "PCSTree.h"
#include "GameObject.h"
#include "GraphicsObject.h"
#include "AnimationData.h"
#include "LayerData.h"
class GameObject;
class PCSTree;
class GraphicsObject;
class AnimationData;
class LayerData;
class AnimationHierarchy
{
public:
	AnimationHierarchy(int maxCount, int* data, GraphicsObject* pGraphics, AnimationData* pAnim);
	~AnimationHierarchy();
	void SetCurrentFrame(int frame);
	void Update(int time);
	void Interpolate();
	void SetAnimation(int stack, int layer);
	PCSTree* getTree();
private:
	AnimationHierarchy() = delete;
	int totalBones;
	int time;
	int currentFrame;
	int nextFrame;
	GameObject* gObjectList;
	PCSTree* pTree;
	AnimationData* pData;
	LayerData* pLayer;
};

