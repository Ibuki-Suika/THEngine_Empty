matrix world, projection, view;

texture tex;
float4 argb;
int texWidth, texHeight;

sampler TextureSampler = sampler_state
{ 
	texture = <tex>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
	AddressU = mirror;
 	AddressV = mirror;
};

struct VertexIn
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VertexOut
{
    float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

VertexOut VSFunc(VertexIn input)
{
	VertexOut output;
	matrix transform;

	transform = mul(world, view);

	output.position.xyz = input.position;
	output.position.w = 1.0f;
	output.position = mul(output.position, transform);

	output.position = mul(output.position, projection);

	output.texCoord = input.texCoord;
	return output;
}

float4 PSFunc(VertexOut input) : COLOR
{
	float4 color;
	float2 uv = input.texCoord;

	color = tex2D(TextureSampler, uv.xy);
	color.xyz *= argb.yzw;
	color.w *= argb.x;
	return color;
}

technique Sprite
{
	pass Pass0
	{
		VertexShader = compile vs_2_0 VSFunc();
		PixelShader = compile ps_2_0 PSFunc();
	}
}