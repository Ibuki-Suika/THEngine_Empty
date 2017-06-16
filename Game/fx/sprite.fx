matrix world, projection, view;

float4 argb;
texture tex;
int screenWidth, screenHeight;
int viewportWidth, viewportHeight;

sampler TextureSampler = sampler_state
{
	texture = <tex>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VertexIn
{
	float3 position : POSITION;
	float4 color : COLOR0;
	float2 texCoord : TEXCOORD0;
};

struct VertexOut
{
	float4 position : POSITION;
	float4 color : COLOR;
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
	output.color = input.color;
	output.texCoord = input.texCoord;

	output.position.xy += float2(-output.position.w / viewportWidth, output.position.w / viewportHeight);

	return output;
}

float4 PSFunc(VertexOut input) : COLOR
{
	float4 color;
	float2 uv = input.texCoord;

	color = tex2D(TextureSampler, uv.xy);
	color *= input.color;
	color = saturate(color);
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