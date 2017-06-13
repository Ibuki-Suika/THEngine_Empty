struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 emmisive;
	float power;
};

struct DirectionalLight
{
	float4 color;
	float3 direction;
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
	float4 position : TEXCOORD4;
	float4 posInWorld : TEXCOORD3;
	float4 positionInView : TEXCOORD2;
	float3 normal : TEXCOORD1;
	float2 texCoord : TEXCOORD0;
};

struct PixelIn
{
	float4 position : TEXCOORD4;
	float4 posInWorld : TEXCOORD3;
	float4 positionInView : TEXCOORD2;
	float3 normal : TEXCOORD1;
	float2 texCoord : TEXCOORD0;
};

Material material;
DirectionalLight directionalLight;
float4 ambientLight;

float4 ambientLighting(in float4 color)
{
	return color * material.ambient * ambientLight;
}

float4 directionalLighting(float4 diffuse, float4 specular, float power, float3 normal, float3 viewDir)
{
	normal = normalize(normal);
	viewDir = normalize(viewDir);
	float3 lightDir = directionalLight.direction;
	float cosine = -dot(lightDir, normal);
	if (cosine < 0)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	float3 halfAngle = normalize(lightDir + viewDir);
	return directionalLight.color * cosine * diffuse + pow(max(-dot(normal, halfAngle), 0.0f), power) * specular
		* directionalLight.color;
}

float4 shadeWithDirectional(float4 color, PixelIn input)
{
	return directionalLighting(color, material.specular, material.power, input.normal, input.positionInView);
}