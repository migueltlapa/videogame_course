#pragma once
#include "IElement2D.h"

class Sprite : public IElement2D
{
public:
	Sprite();
	Sprite(const Vector2& position);
	~Sprite();


	void Load(ID3D11Device * device, const wchar_t* file);


	//getters
	virtual const Vector2 & getPosition() const;
	virtual const Vector2 & getOrigin() const;
	virtual const Vector2 & getScale() const;
	virtual const Color & getTint() const;
	virtual const float & getAlpha() const;
	virtual const float & getRotation() const;

	//setters

	virtual void setPosition(const Vector2& position);
	virtual void setOrigin(const Vector2 & origin);
	virtual void setScale(const Vector2 & scale);
	virtual void setTint(const Color & color);
	virtual void setAlpha(const float alpha);
	virtual void setRotation(const float rotation);

	virtual void Draw(DirectX::SpriteBatch* spriteBatch);

protected:

	ID3D11Resource*				m_pResource;
	ID3D11ShaderResourceView*	m_pTexture;

	UINT						m_Width;
	UINT						m_Height;
	RECT						m_SourceRect;

	Vector2						m_Position;
	Vector2						m_Origin;
	Vector2						m_Scale;
	Color						m_Tint;
	float						m_Alpha;
	float						m_Rotation;




};

