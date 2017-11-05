cbuffer PerApplication : register(b0)
{
	matrix projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
	matrix viewMatrix;
}

cbuffer PerObject : register(b2)
{
	matrix worldMatrix;
}

struct AppData
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
};


struct VOut
{
	float4 color		: COLOR;
	float4 position		: SV_POSITION;
	float4 positionWS	: POSITION;
	float3 normalWS		: NORMAL;
};

VOut VShader(AppData IN)
{
	VOut OUT;

	matrix wvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
	OUT.position = mul(wvp, float4(IN.position, 1.0f));
	OUT.color = float4(1.0f,0.0f,0.0f, 1.0f);
	OUT.positionWS = mul(float4(IN.position, 1.0f), worldMatrix);
	OUT.normalWS = normalize(mul(float4(IN.normal, 1.0f), worldMatrix).xyz);
	return OUT;
}