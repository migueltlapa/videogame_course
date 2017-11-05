Texture2D Texture: register(t0); //max 128 textures
Texture2D Texture2: register(t1); //max 128 textures
TextureCube gCubeMap: register(t2);
//Texture2D gNormalMap: register(t1);

sampler	Sampler: register(s0); //max 16 samplers

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

cbuffer Ojo : register(b0)
{
	float4 EyePos: POSITION0;
	//---16BYTES
	float4 LDir: POSITION1;
	//---16BYTES
}

struct Fog
{
	float4 Color; //16 bytes
	//-----------------
	float  Start; //4 bytes
	float  Range; //4 bytes
	bool   Enable;//4 bytes
	float  pad;	  //4 bytes
	//-----------------
};

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
	bool   Enabled; // 4 bytes
	float  AnglePos;
	float2 Padding;
};

cbuffer LightDef : register(b1)
{
	Light gLight[6];
}

struct Material {
	float4 Ambient;		//16
	float4 Diffuse;		//16
	float4 Specular;	//16
	float4 Reflect;		//16
	float4 Refract;		//16
	//----------------boundary 16 bytes
	float	SpecularPower;		//4 bytes
	int  	UseTexture;			//4 bytes
	bool	EnableReflection;	//4 bytes
	bool	EnableRefraction;	//4 bytes
	//----------------boundary 16 bytes
	float   FusionAlpha;
	float3 padding;

};

cbuffer MaterialInfo: register (b2)
{
	Material gMaterial;
}

cbuffer FogInfo : register(b3)
{
	Fog gFog;
}

struct PInb
{
	float4 position		: SV_POSITION;
	float4 positionWS	: POSITION;
	float3 normalWS		: NORMAL;
	float2 texcoord		: TEXCOORD;
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

	result.Diffuse = attenuation*DoDiffuse(light, L, N);
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

	result.Diffuse = intensitySpot*attenuation*DoDiffuse(light, L, N);
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


// Transforms a normal map sample to world space.
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f*normalMapSample - 1.0f; //(-1,-1,-1)

		// Build orthonormal basis.
		float3 N = unitNormalW;
		float3 T = normalize(tangentW - dot(tangentW, N)*N);
		float3 B = cross(N, T);

		float3x3 TBN = float3x3(T, B, N);

		// Transform from tangent space to world space.
		float3 bumpedNormalW = mul(normalT, TBN);

		return bumpedNormalW;
}


float4 PShader(PInb IN) : SV_TARGET
{
	float4 EyePosition = EyePos; //float4(0, 0, -1, 1);

	float4 P = IN.positionWS;
	//float3 N = normalize(IN.normalWS);
	float3 N = IN.normalWS;
	float3 V = (EyePosition - P).xyz;
	float distToEye = length(V);
	V = V / distToEye;


	/*float3 bumpedNormalWS = N;
	if (gMaterial.UseTexture){
	float3 normalMapSample = gNormalMap.Sample(Sampler, IN.texcoord).rgb;
	bumpedNormalWS = NormalSampleToWorldSpace(normalMapSample,N,	IN.tangentWS);
	}*/
	//LightingResult lr = DoSpotLight(gLight, V, bumpedNormalWS, P);

	//LightingResult lr = DoSpotLight(gLight[0], V, N, P);

	float4 diffuse = { 0, 0, 0, 0 };
	float4 specular = { 0, 0, 0, 0 };

	float4 texColor = { 1, 1, 1, 1 };
	float4 texColor2 = { 1, 1, 1, 1 };

	float4 finalColor = gMaterial.Ambient;
		if (gLight[0].Enabled){
		
		for (int i = 0; i < 6; i++)
		{
			LightingResult lr = DoSpotLight(gLight[i], V, N, P);

			diffuse += lr.Diffuse;
			specular += lr.Specular;
		}


		finalColor += gMaterial.Diffuse * diffuse +
			gMaterial.Specular*specular;
		}


	

		if (gMaterial.UseTexture == 1){
			texColor = Texture.Sample(Sampler, IN.texcoord);
			finalColor *= texColor;
			}
		
	// Clipping COlor
		if (finalColor.r >= 0.7 && finalColor.g >= 0.7 && finalColor.b >= 0.7)
			{
			clip(-1);

			}
		

		


		// Cambiar el Alpha de 2 Texturas 
		if (gMaterial.UseTexture == 2){
			texColor	= Texture.Sample(Sampler, IN.texcoord);
			texColor2 = Texture2.Sample(Sampler, IN.texcoord);
			finalColor *= ((1 - gMaterial.FusionAlpha)*texColor) + (gMaterial.FusionAlpha * texColor2);
			
		}

//	finalColor *= texColor* texColor2;
	finalColor.a = gMaterial.Diffuse.a* texColor.a;


	if (gFog.Enable)
	{
		float fogLerp = saturate((distToEye - gFog.Start) / gFog.Range);
		finalColor = lerp(finalColor, gFog.Color, fogLerp);
	}

	if (gMaterial.EnableReflection){
		float3 incident = -V;
			float3 reflectionVec = reflect(incident, N);
			float4 reflectionColor = gCubeMap.Sample(Sampler, reflectionVec);
			finalColor += gMaterial.Reflect * reflectionColor;
	}

	if (gMaterial.EnableRefraction){
		float3 incident = -V;
			float3 refractionVec = refract(incident, N, gMaterial.Refract.a);
			float4 refractionColor = gCubeMap.Sample(Sampler, refractionVec);
			finalColor += float4(gMaterial.Refract.xyz, 1.0) * refractionColor;
	}

	return finalColor;
}