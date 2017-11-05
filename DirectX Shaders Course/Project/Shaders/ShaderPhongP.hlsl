#define SPECULAR_POWER 120.0

cbuffer Ojo : register(b0)
{
	float4 EyePos: POSITION;
}

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

struct Light
{
	float4 Position;
	float4 Direction;
	float4 Color;
};

struct PIn
{
	float4 color		: COLOR;
	float4 position		: SV_POSITION;
	float4 positionWS	: POSITION;
	float3 normalWS		: NORMAL;	
};

//L and N must be normalized
float4 DoDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L)); //clamp (dot(N,L),0,1);
	return light.Color*NdotL;
}

float4 DoSpecular(Light light, float3 V, float3 L, float3 N)
{
	// Phong lighting.
	float3 R = normalize(reflect(-L, N));
	//R = 2 *dot(L, N)*N - L; // Phong original
	float RdotV = max(0, dot(R, V));

	// Blinn-Phong lighting
	float3 H = normalize(L + V);
	float NdotH = max(0, dot(N, H));

							 //NdotH
	return light.Color * pow(NdotH, SPECULAR_POWER);
}

LightingResult DoDirectionalLight(Light light, float3 V, float3 N)
{
	LightingResult result;

	float3 L = -light.Direction.xyz;

	result.Diffuse = DoDiffuse(light, L, N);
	result.Specular = DoSpecular(light, V, L, N);
	
	return result;

}

float4 PShader(PIn IN) : SV_TARGET
{
	float4 EyePosition = EyePos; //float4(0, 0, -1, 1);

	float4 P = IN.positionWS;
	//float3 N = normalize(IN.normalWS);
	float3 N = IN.normalWS;
	float3 V = normalize(EyePosition - P).xyz;

	float4 emissive = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 ambient = float4(0.07568f, 0.21424f, 0.07568f, 1.0f);
	float4 diffuse = 1.0f;
	float4 specular = 1.0f; 


	Light gLight;

	gLight.Position		= float4(0, 10.0f, 0, 1.0f);
	gLight.Direction	= float4(0, -1.0f, 0, 1.0f);
	gLight.Color		= float4(1, 1, 1, 1);
	
	LightingResult lr = DoDirectionalLight(gLight, V, N);
	
	float4 finalColor = emissive +ambient +lr.Diffuse + lr.Specular;

	return finalColor;
}