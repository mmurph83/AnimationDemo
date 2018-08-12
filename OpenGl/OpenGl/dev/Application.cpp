
#include <math.h>
#include <vmath.h>
#include <new>
#include <malloc.h>
#include <stdio.h>
#include "MathEngine.h"
#include "PCSNode.h"
#include "ShaderObject.h"
#include "CameraObject.h"
#include "WindowObject.h"
#include "Model.h"
#include "GraphicsObject.h"
#include "CameraManager.h"
#include "GameObject.h"
#include "Keyboard.h"
#include "Texture.h"
#include "TextureObject.h"
#include "LightObject.h"
#include "PCSTree.h"
#include "CameraController.h"
#include "PCSTreeForwardIterator.h"
#include "File.h"
#include "eat.h"
#include "BoundingSphere.h"
#include "AnimationHierarchy.h"
#include "AnimationData.h"
#include "StackData.h"
#include "LayerData.h"

bool privLoadTGATexture(const char *szFileName, GLint minFilter, GLint magFilter, GLint wrapMode);
GLbyte* gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);
bool VectFacingSameDir(const Vect& spherePos, const Vect& planeNormal, const Vect& planePoint);
bool SpherePlaneIntersectionTest(const Vect& spherePos, const float radius, const Vect& planeNormal, const Vect& planePoint);
void CullingTest(GameObject* g, const Sphere &s);

void CreateAnimationList(char* fileName, int total, GraphicsObject* pGrachics, int startIndex, AnimationHierarchy** pReturnVal)
{
	File::Handle file;
	File::Open(file, fileName, File::Mode::READ);
	unsigned int size = 0;
	File::Seek(file, File::Location::END, 0);
	File::Tell(file, size);
	File::Seek(file, File::Location::BEGIN, 0);
	int bones = 0;
	File::Read(file, &bones, sizeof(int));
	int* data = (int*)malloc(bones * sizeof(int));
	File::Read(file, data, sizeof(int)*bones);

	unsigned int currentSize = 0;
	int stacks;
	File::Read(file, &stacks, sizeof(int));
	char t[28];
	File::Seek(file, File::Location::CURRENT, sizeof(t));
	File::Tell(file, currentSize);
	void * d = malloc(size - currentSize);
	File::Read(file, d, size - currentSize);
	//AnimationHierarchy** pReturnVal = (AnimationHierarchy**)malloc(sizeof(AnimationHierarchy**)*total);
	for (int i = 0; i < total; i++)
	{
		AnimationData* pAnimData = new AnimationData(stacks, d);
		AnimationHierarchy* aH = new AnimationHierarchy(bones, data, pGrachics, pAnimData);
		pReturnVal[i + startIndex] = aH;
		pReturnVal[i + startIndex]->Interpolate();
	}
	free(data);
	//Fix Alignment Issue while reading data
	//d->SetPointer();
	//File::Read(file, d, sizeof(LayerData));
	//File::Read(file, &bones, sizeof(int));
	File::Close(file);
}
AnimationHierarchy* Test(GraphicsObject* pGrachics)
{
	File::Handle file;
	File::Open(file, "tf", File::Mode::READ);
	unsigned int size = 0;
	File::Seek(file, File::Location::END, 0);
	File::Tell(file, size);
	File::Seek(file, File::Location::BEGIN, 0);
	int bones = 0;
	File::Read(file, &bones, sizeof(int));
	int* data = (int*)malloc(bones * sizeof(int));
	File::Read(file, data, sizeof(int)*bones);
	
	unsigned int currentSize = 0;
	int stacks;
	File::Read(file, &stacks, sizeof(int));
	char t[28];
	File::Seek(file, File::Location::CURRENT, sizeof(t));
	File::Tell(file, currentSize);
	void * d = malloc(size - currentSize);
	File::Read(file, d, size - currentSize);
	AnimationData* pAnimData = new AnimationData(stacks, d);
	AnimationHierarchy* aH = new AnimationHierarchy(bones, data, pGrachics, pAnimData);
	free(data);
	aH->Interpolate();
	//Fix Alignment Issue while reading data
	//d->SetPointer();
	//File::Read(file, d, sizeof(LayerData));
	//File::Read(file, &bones, sizeof(int));
	File::Close(file);
	return aH;
}
void LoadLiuModel(Model *& m, char* fileName, Sphere &s)
{
	m = nullptr;
	File::Handle vertFile;
	File::Handle triFile;
	unsigned int vertSize;
	unsigned int triSize;
	unsigned char* vertData = nullptr;
	unsigned char* triData = nullptr;

	eat(fileName, ChunkType::VERTS_TYPE, "Vertex", vertData, vertSize);
	eat(fileName, ChunkType::TRI_TYPE, "Triangles", triData, triSize);
	float * t = (float*)vertData;
	Vect * v = new Vect[vertSize / (8 * sizeof(float))];
	for (unsigned int i = 0; i < vertSize / (8 * sizeof(float)); i++, t += 8)
	{
		v[i].set(t[0], t[1], t[2]);
	}
	RitterSphere(s, v, vertSize / (8 * sizeof(float)));
	/*if (File::Open(vertFile, "Vert.bin", File::Mode::READ) == File::Error::SUCCESS)
	{
	File::Seek(vertFile, File::Location::END, 0);
	File::Tell(vertFile, vertSize);
	File::Seek(vertFile, File::Location::BEGIN, 0);
	vertData = (char*)malloc(vertSize);
	File::Read(vertFile, vertData, vertSize);
	}
	if (File::Open(triFile, "Triangles.bin", File::Mode::READ) == File::Error::SUCCESS)
	{
	File::Seek(triFile, File::Location::END, 0);
	File::Tell(triFile, triSize);
	File::Seek(triFile, File::Location::BEGIN, 0);
	triData = (char*)malloc(triSize);
	File::Read(triFile, triData, triSize);
	}*/
	m = new Model(vertData, triData, vertSize, triSize);

}
void SetGameObjectData(GameObject& gObj, Vect * pos, ShaderObject* graphObj, Model* m, Texture::Name texName)
{
	gObj.setTexture(texName);
	gObj.GetGraphics().SetModel(m);
	gObj.GetGraphics().SetShader(graphObj);
	gObj.SetPos(pos);
}

struct Vert_xyzuv
{
	float x;
	float y;
	float z;
	float u;
	float v;
};

#pragma pack(1)
typedef struct
{
	GLbyte	identsize;              // Size of ID field that follows header (0)
	GLbyte	colorMapType;           // 0 = None, 1 = paletted
	GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;          // First colour map entry
	unsigned short	colorMapLength;         // Number of colors
	unsigned char 	colorMapBits;   // bits per palette entry
	unsigned short	xstart;                 // image x origin
	unsigned short	ystart;                 // image y origin
	unsigned short	width;                  // width in pixels
	unsigned short	height;                 // height in pixels
	GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
	GLbyte	descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)

void TestFunction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	printf("hi");
}

void privLoadTexture(const char * const _assetName, GLuint *&textureID)
{
	// Get an ID, for textures (OpenGL poor man's handle)
	glGenTextures(1, textureID);

	// Bind it.
	glBindTexture(GL_TEXTURE_2D, *textureID);

	// Loat the texture
	privLoadTGATexture(_assetName, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

// Load a TGA as a 2D Texture. Completely initialize the state
bool privLoadTGATexture(const char *szFileName, GLint minFilter, GLint magFilter, GLint wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if (pBits == NULL)
		return false;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

	free(pBits);

	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}

GLbyte* gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	FILE *pFile;			// File pointer
	TGAHEADER tgaHeader;		// TGA file header
	unsigned long lImageSize;		// Size in bytes of image
	short sDepth;			// Pixel depth;
	GLbyte	*pBits = NULL;          // Pointer to bits

									// Default/Failed values
	*iWidth = 0;
	*iHeight = 0;
	*eFormat = GL_RGB;
	*iComponents = GL_RGB;

	// Attempt to open the file
	fopen_s(&pFile, szFileName, "rb");
	if (pFile == NULL)
		return NULL;

	// Read in header (binary)
	fread(&tgaHeader,  18, 1, pFile);


	// Get width, height, and depth of texture
	*iWidth = tgaHeader.width;
	*iHeight = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;

	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	unsigned int size = sizeof(TGAHEADER);
	size = size;
	if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return NULL;

	// Calculate size of image buffer
	lImageSize = (unsigned int)tgaHeader.width *  (unsigned int)tgaHeader.height *  (unsigned int)sDepth;

	// Allocate memory and check for success
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if (pBits == NULL)
		return NULL;

	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	if (fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return NULL;
	}

	// Set OpenGL format expected
	switch (sDepth)
	{

	case 3:     // Most likely case
		*eFormat = GL_BGR;
		*iComponents = GL_RGB;
		break;

	case 4:
		*eFormat = GL_BGRA;
		*iComponents = GL_RGBA;
		break;
	case 1:
		//assert(false);
		// bad case - keenan
		// i commented out
		//  *eFormat = GL_LUMINANCE;
		//  *iComponents = GL_LUMINANCE;
		break;
	default:        // RGB
					// If on the iPhone, TGA's are BGR, and the iPhone does not 
					// support BGR without alpha, but it does support RGB,
					// so a simple swizzle of the red and blue bytes will suffice.
					// For faster iPhone loads however, save your TGA's with an Alpha!

		break;
	}



	// Done with File
	fclose(pFile);

	// Return pointer to image data
	return pBits;
}

static unsigned int CompileShader(unsigned int type, const char* source)
{
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)alloca(length*sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		printf("Failed to compile");
		printf("%s\n", message);
	}

	return id;
}

static unsigned int CreateShader(const char* vertexShader, const char* fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}
void setBonePose(GameObject *node)
{
	// Now get the world matrices
	// getting the parent from current node
	GameObject *childNode = (GameObject *)node;
	GameObject *parentNode = (GameObject *)node->GetParent();

	if (parentNode == nullptr || parentNode->index == -1 )
		return;

	if (parentNode != 0 && childNode != 0)
	{
		// starting point
		Vect start(0.0f, 0.0f, 0.0f);

		//  At this point, Find the two bones initial positions in world space
		//  Now get the length and directions

		Vect ptA = start * *parentNode->GetWorld();
		Vect ptB = start * *childNode->GetWorld();

		// direction between the anchor points of the respective bones
		Vect dir = (ptA - ptB);

		// length of the bone 0
		float mag = dir.mag();
		mag *= 2.0f;
		Matrix S(SCALE, 8.0f, 8.0f, mag);
		Matrix m(ROT_ORIENT, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f));
		Quat Q(ROT_ORIENT, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f));
		Matrix T(TRANS, ptB);

		Matrix BoneOrient = S * Q * T;

		childNode->setOrient(&BoneOrient);
	}
}
void SetAnimationPose(GameObject *pRoot)
{
	PCSTreeForwardIterator pIter(pRoot);
	PCSNode *pNode = pIter.Next();
	GameObject *pGameObj = 0;

	// walks the anim node does the pose for everything that
	while (!pIter.IsDone())
	{
		assert(pNode);
		// Update the game object
		pGameObj = (GameObject *)pNode;
		setBonePose(pGameObj);

		pNode = pIter.Next();
	}
}


int main(void)
{
	
	
	char n[] = "hi";
	
	WindowObject::instance()->getWindow();
	
	ShaderObject *pShaderObject_texture = new ShaderObject(ShaderObject::Name::TEXTURE_SIMPLE, "textureRender");
	ShaderObject *pShaderObject_color = new ShaderObject(ShaderObject::Name::COLOR_LIGHT, "colorRender");
	ShaderObject *pShaderObject_lightTexture = new ShaderObject(ShaderObject::Name::COLOR_LIGHT, "texturePointLightDiff");
	ShaderObject *pShaderObject_lightColor = new ShaderObject(ShaderObject::Name::COLOR_LIGHT, "ColorPointLight");
	Model *cube = new Model(Model::BaseModelType::Cube);
	Model *sphere = new Model(Model::BaseModelType::Cube);
	Model *diamond = new Model(Model::BaseModelType::Cube);
	Model *pyramid = new Model(Model::BaseModelType::Cube);
	Sphere s;
	Sphere s0;
	Sphere s1;
	Sphere s2;
	Sphere s3;
	Model *buggy;
	Model *tower;
	Model *frigate;
	Model *astro;
	Model *bear;
	Model *CullDrawing;
	{
		float * vertData = new float[64];
		int * triList = new int[6 * 6];
		CameraObject* c = CameraManager::instance()->getCurrentCamera();
		c->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 10.0f), Vect(180.0f, 0.0f, 3090.0f));
		
		Vect v;
		unsigned int rate = 0;
		c->getFarTopLeft(v);
		vertData[0 + rate] = v.x();
		vertData[1 + rate] = v.y();
		vertData[2 + rate] = v.z();
		rate += 8;
		c->getFarTopRight(v);
		vertData[0 + rate] = v.x();
		vertData[1 + rate] = v.y();
		vertData[2 + rate] = v.z();
		rate += 8;
		c->getFarBottomRight(v);
		vertData[0 + rate] = v.x();
		vertData[1 + rate] = v.y();
		vertData[2 + rate] = v.z();
		rate += 8;
		c->getFarBottomLeft(v);
		vertData[0 + rate] = v.x();
		vertData[1 + rate] = v.y();
		vertData[2 + rate] = v.z();
		rate += 8;
		c->getNearTopLeft(v);
		vertData[0 + rate] = v.x();
		vertData[1 + rate] = v.y();
		vertData[2 + rate] = v.z();
		rate += 8;
		c->getNearTopRight(v);
		vertData[0 + rate] = v.x();
		vertData[1 + rate] = v.y();
		vertData[2 + rate] = v.z();
		rate += 8;
		c->getNearBottomRight(v);
		vertData[0 + rate] = v.x();
		vertData[1 + rate] = v.y();
		vertData[2 + rate] = v.z();
		rate += 8;
		c->getNearBottomLeft(v);
		vertData[0 + rate] = v.x();
		vertData[1 + rate] = v.y();
		vertData[2 + rate] = v.z();
		rate = 0;
		//far
		triList[0 + rate] = 0;
		triList[1 + rate] = 2;
		triList[2 + rate] = 1;
		rate += 3;
		triList[0 + rate] = 1;
		triList[1 + rate] = 3;
		triList[2 + rate] = 2;
		rate += 3;
		//right
		triList[0 + rate] = 2;
		triList[1 + rate] = 5;
		triList[2 + rate] = 1;
		rate += 3;
		triList[0 + rate] = 2;
		triList[1 + rate] = 7;
		triList[2 + rate] = 5;
		rate += 3;

		triList[0 + rate] = 4;
		triList[1 + rate] = 6;
		triList[2 + rate] = 0;
		rate += 3;
		triList[0 + rate] = 6;
		triList[1 + rate] = 3;
		triList[2 + rate] = 0;
		rate += 3;
		triList[0 + rate] = 6;
		triList[1 + rate] = 2;
		triList[2 + rate] = 3;
		rate += 3;
		triList[0 + rate] = 6;
		triList[1 + rate] = 7;
		triList[2 + rate] = 2;
		rate += 3;
		triList[0 + rate] = 6;
		triList[1 + rate] = 4;
		triList[2 + rate] = 5;
		rate += 3;
		triList[0 + rate] = 6;
		triList[1 + rate] = 5;
		triList[2 + rate] = 7;
		rate += 3;
		triList[0 + rate] = 0;
		triList[1 + rate] = 5;
		triList[2 + rate] = 4;
		rate += 3;
		triList[0 + rate] = 0;
		triList[1 + rate] = 1;
		triList[2 + rate] = 5;
		rate += 3;
		unsigned int vertSize = sizeof(float) * 64;
		unsigned int triSize = (unsigned int)sizeof(int) * 36;
		CullDrawing = new Model((unsigned char*)vertData, (unsigned char*)triList, vertSize, triSize);
	}
	LightObject light(Vect(0.0f, 0.0f, -3.0f), Vect(1.0f, 1.0f, 1.0f));
	GameObject gObj(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject gObj0(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject gObj1(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject gObj2(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject gObj3(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject gObj4(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject sgObj(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject sgObj0(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject sgObj1(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject sgObj2(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject sgObj3(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject sgObj4(new GraphicsObject(pShaderObject_texture, sphere, &light));
	GameObject sgObj5(new GraphicsObject(pShaderObject_texture, sphere, &light));
	PCSTree tree;
	PCSTree stree;
	Texture::Add("astroboy.tga", Texture::Name::Astro);
	Texture::Add("buggy_diff.tga", Texture::Name::Buggy_Diff);
	Texture::Add("space_frigate.tga", Texture::Name::Space_Frigate);
	Texture::Add("warbear.tga", Texture::Name::WarBear_01);
	Texture::Add("polarbear.tga", Texture::Name::WarBear_02);
	tree.Insert(&gObj, nullptr);
	tree.Insert(&gObj0, &gObj);
	tree.Insert(&gObj1, &gObj);
	tree.Insert(&gObj2, &gObj);
	tree.Insert(&gObj3, &gObj);
	tree.Insert(&gObj4, &gObj);
	//tree.Insert(&sgObj5, &gObj);
	stree.Insert(&sgObj, nullptr);
	stree.Insert(&sgObj0, &sgObj);
	stree.Insert(&sgObj1, &sgObj);
	stree.Insert(&sgObj2, &sgObj);
	stree.Insert(&sgObj3, &sgObj);
	stree.Insert(&sgObj4, &sgObj);
	stree.Insert(&sgObj5, &sgObj);
	LoadLiuModel(buggy, "buggy.liu", s);
	sgObj.SetScale(&(s.rad * 2.0f *Vect(1.0f, 1.0f, 1.0f)));
	LoadLiuModel(frigate, "space_frigate.liu", s0);
	sgObj0.SetScale(&(s0.rad * 2.0f *Vect(1.0f, 1.0f, 1.0f)));
	LoadLiuModel(astro, "astro.liu", s1);
	sgObj1.SetScale(&(s1.rad * 2.0f *Vect(1.0f, 1.0f, 1.0f)));
	LoadLiuModel(bear, "warbear.liu", s2);
	sgObj2.SetScale(&(s2.rad * 2.0f *Vect(1.0f, 1.0f, 1.0f)));
	LoadLiuModel(tower, "tower.liu", s3);
	sgObj3.SetScale(&(s3.rad * 2.0f *Vect(1.0f, 1.0f, 1.0f)));
	sgObj4.SetScale(&(s2.rad * 2.0f *Vect(1.0f, 1.0f, 1.0f)));
	SetGameObjectData(gObj, new Vect(680.0f, 0, -680.0f), pShaderObject_lightColor, buggy, Texture::Name::Buggy_Diff);
	SetGameObjectData(gObj0, new Vect(30.0f, 0, -80.0f), pShaderObject_lightColor, frigate, Texture::Name::Space_Frigate);
	SetGameObjectData(gObj1, new Vect(-330.0f, 0, -360.0f), pShaderObject_lightColor, astro, Texture::Name::Astro);
	SetGameObjectData(gObj2, new Vect(-200.0f, 0, -380.0f), pShaderObject_lightColor, bear, Texture::Name::WarBear_02);
	SetGameObjectData(gObj3, new Vect(0.0f, 0, -20.0f), pShaderObject_lightColor, tower, Texture::Name::Buggy_Diff);
	SetGameObjectData(gObj4, new Vect(-200.0f, 100.0f, -280.0f), pShaderObject_lightColor, bear, Texture::Name::WarBear_01);

	SetGameObjectData(sgObj, &(gObj.GetPos() + s.cntr), pShaderObject_color, sphere, Texture::Name::Buggy_Diff);
	SetGameObjectData(sgObj0, &(gObj0.GetPos() + s0.cntr), pShaderObject_color, sphere, Texture::Name::Space_Frigate);
	SetGameObjectData(sgObj1, &(gObj1.GetPos() + s1.cntr), pShaderObject_color, sphere, Texture::Name::Astro);
	SetGameObjectData(sgObj2, &(gObj2.GetPos() + s2.cntr), pShaderObject_color, sphere, Texture::Name::WarBear_02);
	SetGameObjectData(sgObj3, &(gObj3.GetPos() + s3.cntr), pShaderObject_color, sphere, Texture::Name::Buggy_Diff);
	SetGameObjectData(sgObj4, &(gObj4.GetPos() + s2.cntr), pShaderObject_color, sphere, Texture::Name::WarBear_01);
	SetGameObjectData(sgObj5, &Vect(), pShaderObject_color, CullDrawing, Texture::Name::WarBear_01);
	sgObj5.SetScale(&Vect(0.1f, 0.1f, 0.1f));
	sgObj5.GetGraphics().SetColor(Vect(0, 0, 0));
	s.rad *= 2.0f;
	s0.rad *= 2.0f;
	s1.rad *= 2.0f;
	s2.rad *= 2.0f;
	s3.rad *= 2.0f;
	//LoadLiuModel(tower, "tower.blo", s);
	//LoadLiuModel(frigate, "frigate.blo", s);
	//LoadLiuModel(astro, "astro.blo", s);
	//GLuint program = pShaderObject_texture->;
	/*GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);*/
	

	
	glfwSetKeyCallback(WindowObject::instance()->getWindow(), TestFunction);
	//glViewport(0, 0, 1024, 768);

	
	// Get the instance to the manager
	

	// Load the texture and get the textureID
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;


	Matrix TransA(TRANS, -2.0f, 0.0f, -4.0f);
	Matrix TransB(TRANS, 2.0f, 0.0f, -4.0f);
	//Matrix TransB(TRANS, sinf(2.1f * f) * 0.5f, cosf(1.7f * f) * 0.5f, sinf(1.3f * f) * cosf(1.5f * f) * 2.0f);
	Matrix RotY(ROT_Y, 90.0f);
	//Matrix RotX(ROT_X, ((float) 81.0f) * MATH_PI_180);
	//Matrix RotY(IDENTITY);
	Matrix RotX(IDENTITY);
	Matrix worldA = RotX * RotY  * TransA;
	//TransA.set(TRANS, Vect(0.0f, 0.0f, -4.0f));
	Matrix worldB = RotX * RotY  * TransB;

	
	
	CameraObject* cam = CameraManager::instance()->getCurrentCamera();
	cam->UpdateCamera();

	//cam.SetOrientation(Vect(2.0f, 0.0f, -4.0f), Vect(0.0f, 1.0f, 0.0f));
	

	
	
	GraphicsObject graphics(pShaderObject_texture, sphere, &light);
	graphics.SetTexture(Texture::Name::ROCKS);
	//GameObject gObj(&graphics);
	//gObj.SetPos(&Vect(2.0f, 0.0f, -3.0f));
	GraphicsObject graphics2(pShaderObject_lightTexture, sphere, &light);
	
	graphics2.SetTexture(Texture::Name::ROCKS);
	graphics2.SetWorld(&worldA);
	//graphics.SetWorld(&worldB);
	//g.SetPos(&Vect(2.0f, 0.0f, -3.0f));
	
	Keyboard k;
	float y = 0.0f;
	
	
	/*for (int i = 0; i < 0; i++)
	{
		tree.Insert(new GameObject(new GraphicsObject(pShaderObject_color,cube,&light)), &gObj);
	}
	{
		PCSTreeForwardIterator rIterator(&gObj);
		GameObject* obj;
		Vect pos(0,0,-680.0f);
		int i = 0;
		while (!rIterator.IsDone())
		{
			obj = (GameObject*)rIterator.CurrentItem();
			obj->SetPos(&pos);
			//obj->SetRotY(-88.489952);
			//
			obj->setTexture(Texture::Name::Buggy_Diff);
			obj->GetGraphics().SetShader(pShaderObject_texture);
			if (i > 12)
			{
				obj->GetGraphics().SetModel(sphere);
			}
			else if (i > 8)
			{
				obj->GetGraphics().SetModel(sphere);
			}
			else if (i > 0)
			{
				obj->GetGraphics().SetModel(dynamic);
				s.rad *= 2.0f;
				obj->SetScale(new Vect(s.rad, s.rad, s.rad));
				//pos += s.cntr;
				//obj->SetPos(&pos);
			}
			else
			{
				obj->GetGraphics().SetModel(dynamic);
				obj->SetScale(new Vect(1.0f, 1.0f, 1.0f));
				obj->SetRotY(-90.0f);
			}
			pos += Vect(1, 0, 0);
			rIterator.Next();
			i++;
		}
		i = i;
	}*/
	
	float currentTime;
	float prevTime = 0.0f;
	float timeDif = 0.0f;
	double time = 0.0;
	double pTime = 0.0;
	double cTime = 0.0;
	double tDif = 0.0;
	CameraController camController;
	bool renderSphere = false;
	const int totalAnims = 6;
	AnimationHierarchy** ah = (AnimationHierarchy**)malloc(sizeof(AnimationHierarchy**)*totalAnims);
	CreateAnimationList("ted", 3, &graphics, 0, ah);
	CreateAnimationList("human.human", 3, &graphics, 3, ah);
	ah[0]->SetAnimation(1, 0);
	ah[1]->SetAnimation(2, 0);
	ah[2]->SetAnimation(0, 0);
	ah[3]->SetAnimation(1, 0);
	ah[4]->SetAnimation(2, 0);
	ah[5]->SetAnimation(0, 0);
	{
		GameObject* g = (GameObject*)ah[0]->getTree()->GetRoot();
		g->SetPos(&Vect(180.0f, 0.0f, 2690.0f));
		g = (GameObject*)ah[1]->getTree()->GetRoot();
		g->SetPos(&Vect(180.0f, 80.0f, 2690.0f));
		g = (GameObject*)ah[2]->getTree()->GetRoot();
		g->SetPos(&Vect(180.0f, -80.0f, 2690.0f));
	}
	/*for (int i = 0; i < totalAnims; i++)
	{
		ah[i] = Test(&graphics);
	}
	for (int i = 0; i < totalAnims; i++)
	{
		//((GameObject*)ah[i]->getTree()->GetRoot())->SetPos(&Vect(0, -0 + (i*50), 0));
	}*/
	glfwSetTime(0.0);
	//ah->SetAnimation(0, 0);
	double timeScale = 1.0;
	while (!glfwWindowShouldClose(WindowObject::instance()->getWindow()))
	{
		if (Keyboard::GetKeyState(AZUL_KEY::KEY_1))
		{
			ah[0]->SetAnimation(0, 0);
			ah[1]->SetAnimation(0, 0);
			ah[2]->SetAnimation(0, 0);
		}
		else if (Keyboard::GetKeyState(AZUL_KEY::KEY_2))
		{
			ah[0]->SetAnimation(1, 0);
			ah[1]->SetAnimation(1, 0);
			ah[2]->SetAnimation(1, 0);

		}
		else if (Keyboard::GetKeyState(AZUL_KEY::KEY_3))
		{
			ah[0]->SetAnimation(2, 0);
			ah[1]->SetAnimation(2, 0);
			ah[2]->SetAnimation(2, 0);
		}
		else if (Keyboard::GetKeyState(AZUL_KEY::KEY_4))
		{
			timeScale = 0.5;
		}
		else if (Keyboard::GetKeyState(AZUL_KEY::KEY_5))
		{
			timeScale = 1.0;
		}
		else if (Keyboard::GetKeyState(AZUL_KEY::KEY_6))
		{
			timeScale = 2.0;
		}
		else if (Keyboard::GetKeyState(AZUL_KEY::KEY_7))
		{
			if (timeScale < 0.0)
			{
				timeScale = -timeScale;
			}
		}
		else if (Keyboard::GetKeyState(AZUL_KEY::KEY_8))
		{
			if (timeScale > 0.0)
			{
				timeScale = -timeScale;
			}
		}
		cTime = glfwGetTime();

		tDif = cTime - pTime;
		pTime = cTime;
		currentTime = (float)glfwGetTime();
		timeDif = currentTime - prevTime;
		
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glClearBufferfv(GL_COLOR, 0, green);
		glClearBufferfv(GL_DEPTH, 0, &one);

		if (Keyboard::GetKeyState(AZUL_KEY::KEY_1))
		{
			renderSphere = !renderSphere;
		}
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GameObject* obj;
		
		camController.Update(timeDif);
		for (int i = 0; i < totalAnims; i++)
		{
			ah[i]->Update((int)(tDif*1000.0*timeScale));
		}
		
		for (int i = 0; i < totalAnims; i++)
		{
			PCSTreeForwardIterator rIterator(ah[i]->getTree()->GetRoot());
			while (!rIterator.IsDone())
			{
				obj = (GameObject*)rIterator.CurrentItem();
				//CullingTest(obj);
				obj->Update(timeDif);
				//obj->Draw();
				rIterator.Next();
			}
		}
		
		for (int i = 0; i < totalAnims; i++)
		{
			SetAnimationPose((GameObject*)ah[i]->getTree()->GetRoot());
			PCSTreeForwardIterator srIterator(ah[i]->getTree()->GetRoot());
			while (!srIterator.IsDone())
			{
				obj = (GameObject*)srIterator.CurrentItem();
				//CullingTest(obj);
				//obj->Update(timeDif);
				obj->Draw();
				srIterator.Next();
			}
		}
		
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		CullingTest(&sgObj, s);
		CullingTest(&sgObj0, s0);
		CullingTest(&sgObj1, s1);
		CullingTest(&sgObj2, s2);
		CullingTest(&sgObj3, s3);
		CullingTest(&sgObj4, s2);
		
		if (renderSphere)
		{
			PCSTreeForwardIterator srIterator(stree.GetRoot());
			while (!srIterator.IsDone())
			{
				obj = (GameObject*)srIterator.CurrentItem();
				//CullingTest(obj);
				obj->Update(timeDif);
				obj->Draw();
				srIterator.Next();
			}
		}
		//g.Update();
		//cam->getPos() += Vect(0.0f, 0.001f, 0.0f);
		cam->UpdateCamera();
		//g.Draw();

		//graphics.SetWorld(&worldA);
		//graphics.Render();
		//glViewport(0, 0, 1024, 768);
		
		//glUseProgram(program);
		
		
		
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		//m.bindVAO();
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CW);
		//glDrawArrays(GL_TRIANGLES, 0, m.getNumVerts());
		
		glfwSwapBuffers(WindowObject::instance()->getWindow());/*
		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3*drawSize);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		glfwSwapBuffers(window);
		glfwPollEvents();*/
		glfwPollEvents();
		prevTime = currentTime;
	}

	glfwTerminate();
	return 0;
}
void CullingTest(GameObject* g, const Sphere &s)
{
	CameraObject* c = CameraManager::instance()->getCurrentCamera();
	Vect pos;
	c->getFarBottomRight(pos);
	if (SpherePlaneIntersectionTest(g->GetPos(), s.rad, c->GetBackNorm(), pos) || !VectFacingSameDir(g->GetPos(), c->GetBackNorm(), pos))
	{
		g->GetGraphics().SetColor(Vect(1.0f, 0.0f, 0.0f));
		return;
	}
	if (SpherePlaneIntersectionTest(g->GetPos(), s.rad, c->GetBottomNorm(), pos) || !VectFacingSameDir(g->GetPos(), c->GetBottomNorm(), pos))
	{
		g->GetGraphics().SetColor(Vect(1.0f, 0.0f, 0.0f));
		return;
	}
	if (SpherePlaneIntersectionTest(g->GetPos(), s.rad, c->GetRightNorm(), pos) || !VectFacingSameDir(g->GetPos(), c->GetRightNorm(), pos))
	{
		g->GetGraphics().SetColor(Vect(1.0f, 0.0f, 0.0f));
		return;
	}
	c->getNearTopLeft(pos);
	if (SpherePlaneIntersectionTest(g->GetPos(), s.rad, c->GetFrontNorm(), pos) || !VectFacingSameDir(g->GetPos(), c->GetFrontNorm(), pos))
	{
		g->GetGraphics().SetColor(Vect(1.0f, 0.0f, 0.0f));
		return;
	}
	if (SpherePlaneIntersectionTest(g->GetPos(), s.rad, c->GetTopNorm(), pos) || !VectFacingSameDir(g->GetPos(), c->GetTopNorm(), pos))
	{
		g->GetGraphics().SetColor(Vect(1.0f, 0.0f, 0.0f));
		return;
	}
	if (SpherePlaneIntersectionTest(g->GetPos(), s.rad, c->GetLeftNorm(), pos) || !VectFacingSameDir(g->GetPos(), c->GetLeftNorm(), pos))
	{
		g->GetGraphics().SetColor(Vect(1.0f, 0.0f, 0.0f));
		return;
	}
	g->GetGraphics().SetColor(Vect(0.0f, 0.0f, 0.0f));
	
}

bool SpherePlaneIntersectionTest(const Vect& spherePos, const float radius, const Vect& planeNormal, const Vect& planePoint)
{
	Vect temp = spherePos;
	temp -= planePoint;
	float length = temp.dot(planeNormal);
	length /= planeNormal.mag();
	return fabsf(length) <= radius;
}

bool VectFacingSameDir(const Vect& spherePos, const Vect& planeNormal, const Vect& planePoint)
{
	Vect v = spherePos - planePoint;
	v.norm();
	float val = v.dot(planeNormal);
	return val < 0.0f;
}
