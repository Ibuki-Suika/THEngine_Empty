#include "lighting.fx"

#define SHADOW_EPSILON 0.0001f

struct Fog
{
	float4 color;
	float start;
	float end;
};

matrix worldMatrix, mvMatrix, normalMatrix, projection;
matrix lightVPNear, lightVPMid, lightVPFar;
texture tex;
texture shadowMapNear, shadowMapMid, shadowMapFar;
float4 argb;
bool fogEnable;
bool hasTexture;
int shadowMapWidth, shadowMapHeight;
float znear, zfar, blendBand;
Fog fog;

float4 posInWorld : TEXCOORD3;

sampler TextureSampler = sampler_state
{
	texture = <tex>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler ShadowSampler[3] =
{
	sampler_state
{
	texture = <shadowMapNear>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
},
sampler_state
{
	texture = <shadowMapMid>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
},
sampler_state
{
	texture = <shadowMapFar>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
}
};

VertexOut VSFunc(VertexIn input)
{
	VertexOut output;

	output.sv_position.xyz = input.position;
	output.sv_position.w = 1.0f;
	output.posInWorld = mul(output.sv_position, worldMatrix);
	output.sv_position = mul(output.sv_position, mvMatrix);

	output.positionInView = output.sv_position;
	output.sv_position = mul(output.sv_position, projection);
	output.position = output.sv_position;

	output.texCoord = input.texCoord;

	float4 normal4;
	normal4.xyz = input.normal.xyz;
	normal4.w = 0;

	float4 outputNormal4 = mul(normal4, normalMatrix);

	output.normal = outputNormal4.xyz;

	return output;
}

void fogShading(inout float4 color, in PixelIn input)
{
	if (fogEnable)
	{
		float3 posInView = (input.positionInView).xyz;
		float distToCamera = length(posInView);
		float fogLerp = saturate((distToCamera - fog.start) / (fog.end - fog.start)) * fog.color.w;
		color.xyz = lerp(color.xyz, fog.color.xyz, fogLerp);
	}
}

float4 getTexturedColor(in PixelIn input)
{
	float4 color = argb;
	if (hasTexture)
	{
		color *= tex2D(TextureSampler, input.texCoord.xy);
	}
	else
	{
		color *= material.diffuse;
	}
	return color;
}

float getPCF(in int cascadedLevel, in float4 posInShadowMap, in float shadowBias)
{
	matrix lightVP[3] = { lightVPNear, lightVPMid, lightVPFar };

	float2 shadowTex = 0.5 * posInShadowMap.xy / posInShadowMap.w + float2(0.5, 0.5);
	shadowTex.y = 1.0f - shadowTex.y;

	float depth = posInShadowMap.z / posInShadowMap.w;

	float2 texelPos;
	texelPos.x = shadowMapWidth * shadowTex.x;
	texelPos.y = shadowMapHeight * shadowTex.y;

	float2 lerps = frac(texelPos);
	float dx = 1.0 / shadowMapWidth;
	float dy = 1.0 / shadowMapHeight;

	float3 vShadowTexDDX = mul(ddx(posInWorld), lightVP[cascadedLevel]).xyz;
	float3 vShadowTexDDY = mul(ddy(posInWorld), lightVP[cascadedLevel]).xyz;
	float2x2 matScreentoShadow = float2x2(vShadowTexDDX.xy, vShadowTexDDY.xy);
	float fInvDeterminant = 1.0f / determinant(matScreentoShadow);

	float2x2 matShadowToScreen = float2x2 (
		matScreentoShadow._22 * fInvDeterminant,
		matScreentoShadow._12 * -fInvDeterminant,
		matScreentoShadow._21 * -fInvDeterminant,
		matScreentoShadow._11 * fInvDeterminant);

	float2 vRightShadowTexelLocation = float2(dx, 0.0f);
	float2 vUpShadowTexelLocation = float2(0.0f, dy);
	float2 vRightTexelDepthRatio = mul(vRightShadowTexelLocation, matShadowToScreen);
	float2 vUpTexelDepthRatio = mul(vUpShadowTexelLocation, matShadowToScreen);

	float fUpTexelDepthDelta = vUpTexelDepthRatio.x * vShadowTexDDX.z
		+ vUpTexelDepthRatio.y * vShadowTexDDY.z;
	float fRightTexelDepthDelta = vRightTexelDepthRatio.x * vShadowTexDDX.z
		+ vRightTexelDepthRatio.y * vShadowTexDDY.z;

	float sourcevals[4];
	int i = cascadedLevel;
	float d0 = tex2D(ShadowSampler[i], shadowTex).r;
	float d1 = tex2D(ShadowSampler[i], shadowTex + float2(dx, 0)).r + fRightTexelDepthDelta;
	float d2 = tex2D(ShadowSampler[i], shadowTex + float2(0, dy)).r + fUpTexelDepthDelta;
	float d3 = tex2D(ShadowSampler[i], shadowTex + float2(dx, dy)).r + fRightTexelDepthDelta + fUpTexelDepthDelta;
	sourcevals[0] = d0 < depth - shadowBias ? 0.0f : 1.0f;
	sourcevals[1] = d1 + shadowBias < depth ? 0.0f : 1.0f;
	sourcevals[2] = d2 + shadowBias < depth ? 0.0f : 1.0f;
	sourcevals[3] = d3 + shadowBias < depth ? 0.0f : 1.0f;

	float amount = saturate(lerp(lerp(sourcevals[0], sourcevals[1], lerps.x),
		lerp(sourcevals[2], sourcevals[3], lerps.x),
		lerps.y));

	return amount;
}

float chebyshev(float2 moments, float depth)
{
	float p = ( depth <= moments.x) ? 1.0f : 0.0f;
	float variance = moments.y - moments.x * moments.x;
	variance = min(1.0f, max(variance + 0.0001f, 0.0f));
	float d = depth - moments.x;
	float p_max = variance / (variance + d * d);
	return max(p, p_max);
}

float2 computeMoments(float depth)
{
	float2 moments;
	// First moment is the depth itself.  
	moments.x = depth;
	// Compute partial derivatives of depth.  
	float dx = ddx(depth);
	float dy = ddy(depth);
	// Compute second moment over the pixel extents.  
	moments.y = depth * depth + 0.25*(dx*dx + dy*dy);
	return moments;
}

float getVSM(in int cascadedLevel, in float4 posInShadowMap)
{
	matrix lightVP[3] = { lightVPNear, lightVPMid, lightVPFar };
	float2 shadowTex = 0.5 * posInShadowMap.xy / posInShadowMap.w + float2(0.5, 0.5);
	shadowTex.y = 1.0f - shadowTex.y;
	float depth = posInShadowMap.z / posInShadowMap.w;

	float2 moments = tex2D(ShadowSampler[cascadedLevel], shadowTex.xy).xy;
	return chebyshev(moments, depth);
}

float getLightDensity(in PixelIn input)
{
	float4 posInShadowMap;

	//compute bias
	float3 normal = normalize(input.normal);
	float3 lightDir = normalize(directionalLight.direction);
	float shadowBias = max(0.004 * (1.0 - dot(normal, lightDir)), SHADOW_EPSILON);

	matrix lightVP[3] = { lightVPNear, lightVPMid, lightVPFar };
	float zvalue = input.positionInView.z / input.positionInView.w;
	float dz = (zfar - znear) / 3;
	for (int i = 0; i < 3; i++)
	{
		float end = znear + dz * (i + 1);
		if (zvalue <= end)
		{
			posInShadowMap = mul(input.posInWorld, lightVP[i]);
			/*float margin1 = end - blendBand;
			float margin2 = end - dz + blendBand;
			if (zvalue >= margin1 && i < 2)
			{
				//return lerp(getPCF(i, posInShadowMap, shadowBias),
				//	getPCF(i + 1, posInShadowMap, shadowBias),
				//	lerp(1, 0.5, (zvalue - margin1) / blendBand));
				return lerp(getVSM(i, posInShadowMap),
					getVSM(i + 1, posInShadowMap),
					lerp(1, 0.5, (zvalue - margin1) / blendBand));
			}
			if (zvalue <= margin2 && i > 0)
			{
				//return lerp(getPCF(i, posInShadowMap, shadowBias),
				//	getPCF(i - 1, posInShadowMap, shadowBias),
				//	lerp(1, 0.5, (margin2 - zvalue) / blendBand));
				return lerp(getVSM(i, posInShadowMap),
					getVSM(i - 1, posInShadowMap),
					lerp(1, 0.5, (margin2 - zvalue) / blendBand));
			}*/
			//return getPCF(i, posInShadowMap, shadowBias);
			return getVSM(i, posInShadowMap);
		}
	}
	//the code should not go here
	return 0;
}

float4 PSFunc(PixelIn input) : COLOR
{
	float4 color;

color = getTexturedColor(input);
fogShading(color, input);

return color;
}

float4 PSFunc_Ambient(PixelIn input) : COLOR
{
	float4 color;

	color = getTexturedColor(input);
	color = ambientLighting(color);
	fogShading(color, input);

	return color;
}

float4 PSFunc_Directional(PixelIn input) : COLOR
{
	float4 color;

	color = getTexturedColor(input);
	color = shadeWithDirectional(color, input);
	color *= getLightDensity(input);
	fogShading(color, input);

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

technique Ambient
{
	pass Pass0
	{
		VertexShader = compile vs_2_0 VSFunc();
		PixelShader = compile ps_2_0 PSFunc_Ambient();
	}
}

technique Directional
{
	pass Pass0
	{
		VertexShader = compile vs_2_0 VSFunc();
		PixelShader = compile ps_3_0 PSFunc_Directional();
	}
}