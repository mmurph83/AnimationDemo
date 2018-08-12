#include "AnimationHierarchy.h"
#include <new>
#include "QuatApp.h"
#include "VectApp.h"


AnimationHierarchy::AnimationHierarchy(int maxCount, int* data, GraphicsObject* pGraphics, AnimationData* pAnim) : totalBones(maxCount), pData(pAnim), time(0)
{
	gObjectList = (GameObject*)malloc(sizeof(GameObject)*(totalBones + 1));
	pTree = new PCSTree();
	for (unsigned int i = 0; i < totalBones + 1; i++)
	{
		 new (gObjectList + i) GameObject(pGraphics);
	}
	pTree->Insert(gObjectList, nullptr);
	gObjectList[0].SetPos(&Vect(0, 0, 0.0f));
	gObjectList[0].SetScale(&Vect(1.0f, 1.0f, 1.0f));
	Matrix m = Matrix(MatrixSpecialType::IDENTITY);
	gObjectList->index = -1;
	gObjectList[0].setOrient(&m);
	gObjectList++;
	for (int i = 0; i < totalBones; i++)
	{
		printf("Index: %d Parent: %d\n", i, data[i]);
		(gObjectList + i)->index = i;
		pTree->Insert(gObjectList + i, gObjectList + data[i]);
	}
	gObjectList--;
	pLayer = pData->GetStackData(1)->GetLayer(0);
	currentFrame = 0;
	nextFrame = 1;
	time = 0;
}

void AnimationHierarchy::Update(int dTime)
{
	time += dTime;
	int a = pLayer->GetTimeAtFrame(currentFrame);
	int b = pLayer->GetTimeAtFrame(nextFrame);
	if (pLayer->GetTimeAtFrame(currentFrame) > time)
	{
		while (currentFrame >= 0 && pLayer->GetTimeAtFrame(currentFrame) > time)
		{
			currentFrame--;
		}
		nextFrame = currentFrame + 1;
	}
	else if (pLayer->GetTimeAtFrame(nextFrame) < time)
	{
		while (nextFrame < pLayer->GetTotalFrames() && pLayer->GetTimeAtFrame(nextFrame) <= time)
		{
			nextFrame++;
		}
		currentFrame = nextFrame - 1;
	}
	if (currentFrame >= pLayer->GetTotalFrames() -1)
	{
		currentFrame = 0;
		nextFrame = 1;
		time = 0;
	}
	else if (currentFrame < 0)
	{
		currentFrame = pLayer->GetTotalFrames() - 1;
		nextFrame = pLayer->GetTotalFrames();
		time = pLayer->GetTimeAtFrame(nextFrame);
	}
	Interpolate();
}
AnimationHierarchy::~AnimationHierarchy()
{
	free(gObjectList);
}

void AnimationHierarchy::SetCurrentFrame(int frame)
{
	if (frame >= pLayer->GetTotalFrames())
	{
		//pFrameCurrent = pLayer->GetDataFromFrame(pLayer->GetTotalFrames() - 1);
	//	pFrameNext = pLayer->GetDataFromFrame(0);
	}
	else
	{
		//pFrameCurrent = pLayer->GetDataFromFrame(frame);
		//pFrameNext = pLayer->GetDataFromFrame(frame + 1);
	}
}
void AnimationHierarchy::SetAnimation(int stack, int layer)
{
	pLayer = pData->GetStackData(stack)->GetLayer(layer);
	currentFrame = 0;
	nextFrame = 1;
	time = 0;
}
void AnimationHierarchy::Interpolate()
{
	int i = 0;
	float tx, ty, tz;
	float rx, ry, rz;
	float sx, sy, sz;

	Vect posA;
	Vect posB;
	Vect scaleA;
	Vect scaleB;
	Quat qA;
	Quat qB;
	Vect scale;
	Vect pos;
	Quat rot;
	Matrix m;
	int timeA;
	int timeB;
	timeA = pLayer->GetTimeAtFrame(currentFrame);
	timeB = pLayer->GetTimeAtFrame(nextFrame);
	float ratio = (float)(time - timeA) / (float)(timeB - timeA);
	printf("Time: %d, Frame A: %d, Frame B: %d, Ratio: %f\n", time, timeA, timeB, ratio);
	for (i = 0; i < totalBones; i++)
	{
		pLayer->GetFrameData(currentFrame, i, posA, qA, scaleA);
		pLayer->GetFrameData(nextFrame, i, posB, qB, scaleA);
		
		float ratio = 0;
		QuatApp::Slerp(rot, qA, qB, ratio);
		VectApp::Lerp(pos, posA, posB, ratio);
		VectApp::Lerp(scale, scaleA, scaleB, ratio);
		gObjectList[i + 1].SetPos(&pos);
		gObjectList[i + 1].SetRotation(&rot);
		gObjectList[i + 1].SetScale(&Vect(1,1,1));
		printf("Index: %d    Pos: (%f, %f, %f)\n",i, pos[x],pos[y],pos[z]);
		printf("Index: %d    Rot: (%f, %f, %f)\n", i, rot[x], rot[y], rot[z]);
		printf("Index: %d    Scale: (%f, %f, %f)\n", i, scale[x], scale[y], scale[z]);
	}
	
}

PCSTree* AnimationHierarchy::getTree()
{
	return pTree;
}