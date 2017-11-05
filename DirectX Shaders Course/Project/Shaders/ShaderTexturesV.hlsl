cbuffer PerObject : register(b0)
{
	matrix wvp;
	matrix worldMatrix;
	matrix worldIT; //inverse transpose world matrix
	matrix texTransform;
}


struct AppData
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float2 texcoord	: TEXCOORD;
};


struct VOut
{
	float4 color		: COLOR;
	float4 position		: SV_POSITION;
	float4 positionWS	: POSITION;
	float3 normalWS		: NORMAL;
	float3 tangentWS	: TANGENT;
	float2 texcoord		: TEXCOORD;
};

VOut VShader(AppData IN)
{
	VOut OUT;

	OUT.position = mul(wvp, float4(IN.position, 1.0f));
	OUT.color = float4(1.0f,0.0f,0.0f, 1.0f);
	OUT.positionWS = mul(float4(IN.position, 1.0f), worldMatrix);
	OUT.normalWS = normalize(mul(worldIT,float4(IN.normal, 1.0f)).xyz);
	OUT.texcoord = mul(texTransform, float4(IN.texcoord, 0.0f, 1.0f)).xy;
	OUT.tangentWS = mul((float3x3)worldMatrix, IN.tangent);

	return OUT;
}