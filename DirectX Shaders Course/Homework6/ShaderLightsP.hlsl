#define SPECULAR_POWER 120.0

cbuffer Ojo : register(b0)
{
	float4 EyePos: POSITION0;
	//---16BYTES
	float4 LDir: POSITION1;
	//---16BYTES
}

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

struct Light
{
	float4 Position;
	//-----------16 bytes 
	float4 Direction;
	//-----------16 bytes 
	float4 Color;
	//-----------16 bytes 
	float  Kc;	//constant 
	float  Kl;	//linear
	float  Kq;	//quadratic
	float  SpotAngle; 
	//-----------16 bytes 
};

cbuffer LightDef : register(b1)
{
	Light gLight[3];
}

struct Material {
	float4 Ambient; //16
	float4 Diffuse; //16
	float4 Specular; //16
	//----------------boundary 16 bytes
	float  SpecularPower; //4
	bool   UseTexture; //4 bytes
	float2 Padding; 
	//----------------boundary 16 bytes
};

cbuffer MaterialInfo: register (b2)
{
	Material gMaterial;
}
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
	return light.Color * pow(NdotH, gMaterial.SpecularPower);
}


//--------------POINT LIGHT
float DoAttenuation(Light light, float d)
{
	float totalAttenuation = light.Kc + light.Kl*d + light.Kq * d * d;
	return 1.0f / totalAttenuation;
}

LightingResult DoPointLight(Light light, float3 V, float3 N, float4 P)
{
	LightingResult result;

	float3 L = (light.Position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);

	result.Diffuse	= attenuation*DoDiffuse(light, L, N);
	result.Specular = attenuation*DoSpecular(light, V, L, N);

	return result;
}


//------------------SPOTLIGHT
float DoCone(Light light, float3 L){
	float minCos = cos(light.SpotAngle);
	float maxCos = (1.0f + minCos) / 2.0f;
	float cosAngle = dot(light.Direction.xyz, -L);
	return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoSpotLight(Light light, float3 V, float3 N, float4 P)
{
	LightingResult result;

	float3 L = (light.Position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);
	float intensitySpot = DoCone(light, L);

	result.Diffuse	= intensitySpot*attenuation*DoDiffuse(light, L, N);
	result.Specular = intensitySpot*attenuation*DoSpecular(light, V, L, N);

	return result;
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
	float4 diffuse ;
	float4 specular;


	/*Light gLight;
	// Comentamos estas lineas porque ya estamos enviando la informacion desde la aplicacion

	gLight.Position		= EyePosition; // float4(0, 10.0f, 0, 1.0f);
	gLight.Direction	= LDir; //	 float4(0, -1.0f, 0, 1.0f);
	gLight.Color		= float4(1, 1, 1, 1);
	gLight.Kc = 1.0f;
	gLight.Kl = 0.03f;
	gLight.Kq = 0.02f;
	gLight.SpotAngle = radians(15.0f);*/

	//LightingResult lr = DoDirectionalLight(gLight, V, N);
	//LightingResult lr = DoPointLight(gLight, V, N, P);

	for (int i = 0; i < 3; i++)
	{
		LightingResult lr = DoSpotLight(gLight[i], V, N, P);

		diffuse += lr.Diffuse;
		specular += lr.Specular;
	}

	float4 finalColor = gMaterial.Ambient +
						gMaterial.Diffuse*diffuse +
						gMaterial.Specular*specular;

	return finalColor;
}