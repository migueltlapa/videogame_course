#include "DXUtil.h"

struct Light
{
	XMFLOAT4 Position;
	//-----------16 bytes 
	XMFLOAT4 Direction;
	//-----------16 bytes 
	XMFLOAT4 Color;
	//-----------16 bytes 
	float  Kc;	//constant 
	float  Kl;	//linear
	float  Kq;	//quadratic
	float  SpotAngle;
	//-----------16 bytes
	int    Enabled;	//4 bytes
	float  AnglePos;	//4 bytes
	float  Padding[2];
};

struct Material
{
	XMFLOAT4 Ambient; //16
	XMFLOAT4 Diffuse; //16
	XMFLOAT4 Specular; //16
	//----------------boundary 16 bytes
	float	SpecularPower; //4
	int		UseTexture; //4 bytes
	int		FuseTexture;
	float	Padding;
	//----------------boundary 16 bytes
};

struct ComplexMaterial
{
	ComplexMaterial() : Ambient(0.0f, 0.0f, 0.0f, 0.0f),
		Diffuse(0.0f, 0.0f, 0.0f, 0.0f),
		Specular(0.0f, 0.0f, 0.0f, 0.0f),
		Reflect(0.0f, 0.0f, 0.0f, 0.0f),
		Refract(0.0f, 0.0f, 0.0f, 0.0f),
		SpecularPower(0.0),
		UseTexture(0),
		EnableReflection(0),
		EnableRefraction(0),
		FusionAlpha(0),
		Padding(0.0f, 0.0f, 0.0f){}
	XMFLOAT4 Ambient; //16
	XMFLOAT4 Diffuse; //16
	XMFLOAT4 Specular; //16
	XMFLOAT4 Reflect; //16
	XMFLOAT4 Refract; //16
	//----------------boundary 16 bytes
	float	SpecularPower;		//4 bytes
	int		UseTexture;			//4 bytes
	int		EnableReflection;	//4 bytes
	int		EnableRefraction;	//4 bytes
	//----------------boundary 16 bytes
	float   FusionAlpha;
	XMFLOAT3 Padding;
};


struct Fog
{
	XMFLOAT4 Color; //16 bytes
	//-----------------
	float   Start; //4 bytes
	float   Range; //4 bytes
	int     Enable;//4 bytes
	float   pad;   //4 bytes
	//-----------------
};

struct EYE
{
	XMFLOAT4 EyePos;
	XMFLOAT4 EyeDir;
};
