#include "lighting.fx"

struct Fog
{
	float4 color;
	float start;
	float end;
};

struct Material
{
    float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 emissive;
	float power;
};

matrix mvMatrix, normalMatrix, projection;
texture tex;
float4 argb;
bool fogEnable;
bool hasTexture;
Fog fog;
Material material;

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
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

struct VertexOut
{
    float4 sv_position : POSITION;
	float4 positionInView : TEXCOORD2;
	float3 normal : TEXCOORD1;
	float2 texCoord : TEXCOORD0;
};

VertexOut VSFunc(VertexIn input)
{
	VertexOut output;

	output.sv_position.xyz = input.position;
	output.sv_position.w = 1.0f;
	output.sv_position = mul(output.sv_position, mvMatrix);

	output.positionInView = output.sv_position;
	output.sv_position = mul(output.sv_position, projection);

	output.texCoord = input.texCoord;

	float4 normal4;
	normal4.xyz = input.normal.xyz;
	normal4.w = 0;

	float4 outputNormal4 = mul(normal4, normalMatrix);
	outputNormal4 = mul(outputNormal4, projection);

	output.normal = outputNormal4.xyz;

	return output;
}

float4 PSFunc(VertexOut input) : COLOR
{
	float4 color;
	float2 uv = input.texCoord;

	if(hasTexture)
	{
	    color = tex2D(TextureSampler, uv.xy);
	}
	color.xyz *= argb.yzw;
	color.w *= argb.x;

	if (fogEnable)
	{
		float3 posInView = (input.positionInView).xyz;
		float distToCamera = length(posInView);
		float fogLerp = saturate((distToCamera - fog.start) / (fog.end - fog.start));
		color = lerp(color, fog.color, fogLerp);
	}

	return color;
}

technique Mesh
{
	pass Pass0
	{
		VertexShader = compile vs_2_0 VSFunc();
		PixelShader = compile ps_2_0 PSFunc();
	}
}