#pragma once
#include "DXUtil.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"

using namespace DirectX::SimpleMath;

class IElement2D
{
public: 
	virtual ~IElement2D(){	}

	//getters
	virtual const Vector2 & getPosition() const = 0;
	virtual const Vector2 & getOrigin() const = 0;
	virtual const Vector2 & getScale() const = 0;
	virtual const Color & getTint() const = 0;
	virtual const float & getAlpha() const = 0;
	virtual const float & getRotation() const = 0;

	//setters

	virtual void setPosition(const Vector2& position) = 0;
	virtual void setOrigin(const Vector2 & origin) = 0;
	virtual void setScale(const Vector2 & scale) = 0;
	virtual void setTint(const Color & color) = 0;
	virtual void setAlpha(const float alpha) = 0;
	virtual void setRotation(const float rotation) = 0;

	//load function
	virtual void Load(ID3D11Device * device, const wchar_t* file)=0;

	//render functions
	virtual void Draw(DirectX::SpriteBatch* spriteBatch) = 0;



};