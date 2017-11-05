#include "Sprite.h"
#include "DDSTextureLoader.h"



Sprite::Sprite()
{
}

Sprite::Sprite(const Vector2& position)
{
	m_Position = position;
	m_Alpha = 1.0f;
	m_Rotation = 0.0f;
	m_Scale = Vector2(1, 1);
	m_Tint = DirectX::Colors::White.v;
}


Sprite::~Sprite()
{
	Memory::SafeRelease(m_pTexture);
	Memory::SafeRelease(m_pResource);
}

void Sprite::Load(ID3D11Device * device, const wchar_t* file)
{
	HR(DirectX::CreateDDSTextureFromFile(device, file, &m_pResource, &m_pTexture));
	Utility::GetTextureDim(m_pResource, &m_Width, &m_Height);
	m_Origin = Vector2(m_Width / 2.0f, m_Height / 2.0f);
	m_SourceRect.left = 0; //top left
	m_SourceRect.top = 0;	
	m_SourceRect.right = m_Width;
	m_SourceRect.bottom = m_Height;
	
}

const Vector2 & Sprite::getPosition() const
{
	return m_Position;
}
const Vector2 & Sprite::getOrigin() const
{
	return m_Origin;
}
const Vector2 & Sprite::getScale() const
{
	return m_Scale;
}

const Color &   Sprite::getTint() const
{
	return m_Tint;
}
const float &   Sprite::getAlpha() const
{
	return m_Alpha;
}
const float &   Sprite::getRotation() const
{
	return m_Rotation;
}

//setters

void Sprite::setPosition(const Vector2& position)
{
	m_Position = position;
}
void Sprite::setOrigin(const Vector2 & origin)
{
	m_Origin = origin;
}
void Sprite::setScale(const Vector2 & scale)
{
	m_Scale = scale;
}
void Sprite::setTint(const Color& color)
{
	m_Tint = color;
}
void Sprite::setAlpha(const float alpha)
{
	m_Alpha = alpha;
}
void Sprite::setRotation(const float rotation)
{
	m_Rotation = rotation;
}

void Sprite::Draw(DirectX::SpriteBatch* spriteBatch)
{
	spriteBatch->Draw(m_pTexture, m_Position, &m_SourceRect, m_Tint, m_Rotation, m_Origin, m_Scale,
		DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
}