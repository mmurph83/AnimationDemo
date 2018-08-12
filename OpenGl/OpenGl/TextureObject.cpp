#include "TextureObject.h"



TextureObject::TextureObject(Texture::Name name): minParameter(Min_Filter_Parameter::Linear_Min), magParameter(Mag_Filter_Parameter::Linear_Mag), textureWrapS(Texture_Wrap::CLAMP_TO_EDGE), textureWrapT(Texture_Wrap::CLAMP_TO_EDGE), textureName(name)
{
}

TextureObject::TextureObject() : minParameter(Min_Filter_Parameter::Linear_Min), magParameter(Mag_Filter_Parameter::Linear_Mag), textureWrapS(Texture_Wrap::CLAMP_TO_EDGE), textureWrapT(Texture_Wrap::CLAMP_TO_EDGE), textureName(Texture::Name::NOT_INITIALIZED)
{
}


TextureObject::~TextureObject()
{
}

void TextureObject::SetMinFilter(Min_Filter_Parameter filter)
{
	minParameter = filter;
}

void TextureObject::SetMagFilter(Mag_Filter_Parameter filter)
{
	magParameter = filter;
}

void TextureObject::SetTextureWrapeS(Texture_Wrap wrap)
{
	textureWrapS = wrap;
}
void TextureObject::SetTextureWrapeT(Texture_Wrap wrap)
{
	textureWrapT = wrap;
}
void TextureObject::SetTexture(Texture::Name name)
{
	textureName = name;
}


void TextureObject::SendTexture()
{
	glActiveTexture(GL_TEXTURE0);
	GLuint textureID = Texture::Find(textureName);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTextureParameteri(TargetTexture::TEXTURE_2D, Name::TEXTURE_MIN_FILTER, minParameter);
	glTextureParameteri(TargetTexture::TEXTURE_2D, Name::TEXTURE_MAG_FILTER, magParameter);

	glTextureParameteri(TargetTexture::TEXTURE_2D, Name::TEXTURE_WRAP_S, textureWrapS);
	glTextureParameteri(TargetTexture::TEXTURE_2D, Name::TEXTURE_WRAP_T, textureWrapT);
}