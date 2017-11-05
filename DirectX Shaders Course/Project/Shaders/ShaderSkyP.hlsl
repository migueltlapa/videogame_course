TextureCube gCubeMap: register(t0);
SamplerState SimpleSampler: register(s0);

struct PIn
{
	float4 position		: SV_POSITION;
	float4 positionWS	: POSITION;
	float3 normalWS		: NORMAL;
	float2 texcoord		: TEXCOORD;
};

float4 PShader(PIn IN) : SV_TARGET
{
	return gCubeMap.Sample(SimpleSampler, IN.positionWS.xyz);
}

