matrix mvMatrix, projection;

struct VertexOut
{
	float4 position : POSITION;
	float2 depth : TEXCOORD0;
};

VertexOut VSFunc(float3 input : POSITION)
{
	VertexOut output;

	output.position.xyz = input;
	output.position.w = 1.0f;
	output.position = mul(output.position, mvMatrix);
	output.position = mul(output.position, projection);
	output.depth.xy = output.position.zw;

	return output;
}

float4 PSFunc(float2 depth : TEXCOORD0) : COLOR
{
	float4 output;
	output.x = depth.x / depth.y;
	output.y = output.x * output.x;
	output.z = output.w = 1.0f;
	return output;
}

technique Shadow
{
	pass Pass0
	{
		VertexShader = compile vs_2_0 VSFunc();
		PixelShader = compile ps_2_0 PSFunc();
	}
}