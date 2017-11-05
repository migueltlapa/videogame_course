


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

cbuffer PerWorld : register(b3){

	matrix worldViewProjectionMatrix;

}





struct AppData
{
	float3 position : POSITION;
	float3 normal: NORMAL;
	float3 color : COLOR;
	

};


struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut VShader(AppData IN)
{
	VOut OUT;
	
	//matrix wvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
	
	OUT.position = mul(worldViewProjectionMatrix, float4(IN.position, 1.0f));
	//IN.color = 0.1f;
	OUT.color = float4(IN.color,1.0f);
	//OUT.color = float4(0.0f,1.0f,0.0f, 1.0f);

	return OUT;
}