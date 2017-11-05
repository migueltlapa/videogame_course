struct VIn
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

/*VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = position;
	output.color = color;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}*/

float4 PShader(VIn vIn) : SV_TARGET
{
	return vIn.color;
}
