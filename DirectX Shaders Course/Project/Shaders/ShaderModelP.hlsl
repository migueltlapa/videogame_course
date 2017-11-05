struct PIn
{
	float4 color : COLOR;
};

float4 PShader(PIn IN) : SV_TARGET
{
	return IN.color;
}