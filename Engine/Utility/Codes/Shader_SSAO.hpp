#include "Shader_Base.hpp"

float sampleRadius = 10.0f;		//주변값 샘플링을 위한 반경값
float distanceScale = 500.0f;	//Occlusion강도

vector cornerFrustum;

texture DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = DepthTexture;

	minfilter = linear;
	magfilter = linear;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture RandNoramlTexture;
sampler RandNoramlSampler = sampler_state
{
	texture = RandNoramlTexture;

	minfilter = linear;
	magfilter = linear;
	AddressU = WRAP;
	AddressV = WRAP;
};

vector samples[16] = // 16개의 샘플링을위한 값들 
{
	vector(0.355512, -0.709318, -0.102371, 0.0),
	vector(0.534186, 0.71511, -0.115167, 0.0),
	vector(-0.87866, 0.157139, -0.115167, 0.0),
	vector(0.140679, -0.475516, -0.0639818, 0.0),
	vector(-0.0796121, 0.158842, -0.677075, 0.0),
	vector(-0.0759516, -0.101676, -0.483625, 0.0),
	vector(0.12493, -0.0223423, -0.483625, 0.0),
	vector(-0.0720074, 0.243395, -0.967251, 0.0),
	vector(-0.207641, 0.414286, 0.187755, 0.0),
	vector(-0.277332, -0.371262, 0.187755, 0.0),
	vector(0.63864, -0.114214, 0.262857, 0.0),
	vector(-0.184051, 0.622119, 0.262857, 0.0),
	vector(0.110007, -0.219486, 0.435574, 0.0),
	vector(0.235085, 0.314707, 0.696918, 0.0),
	vector(-0.290012, 0.0518654, 0.522688, 0.0),
	vector(0.0975089, -0.329594, 0.609803, 0.0)
};

struct VS_IN
{
	vector	vPosition : POSITION;
	float2	TexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector	vPosition : POSITION;
	float2	TexUV : TEXCOORD0;
	float3	vViewDir : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Out.vPosition = In.vPosition;
	Out.TexUV = In.TexUV;

	float3 corner = float3(cornerFrustum.x * In.vPosition.x, -cornerFrustum.y * In.vPosition.y, cornerFrustum.z);

	Out.vViewDir = corner;

	return Out;
}

struct PS_IN
{
	float2	TexUV : TEXCOORD0;
	float3	vViewDir : TEXCOORD1;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vViewDir = vector(In.vViewDir, 0.0f);
	normalize(vViewDir);

	float fDepth = tex2D(DepthSampler, In.TexUV).y;
	float3 Se = fDepth * vViewDir.xyz;

	float3 randNormal = tex2D(RandNoramlSampler, In.TexUV * 200.0f).rgb;

	float FinalColor = 0.0f;

	for (int i = 0; i < 16; ++i)
	{
		float3 Ray = reflect(samples[i].xyz, randNormal) * sampleRadius;

		vector sample = vector(Se + Ray, 1.0f);
		vector ss = mul(sample, g_matProj);

		float2 sampleTexUV = 0.5f * ss.xy / ss.w + float2(0.5f, 0.5f);
		float SampleDepth = tex2D(DepthSampler, sampleTexUV).y;

		if (SampleDepth == 1.0f)
		{
			FinalColor++;
		}
		else
		{
			//Depth > SampleDepth 일 경우 occulsion 적용
			float occlusion = distanceScale * max(fDepth - SampleDepth, 0.0f);
			FinalColor += 1.0f / (1.0f + occlusion * occlusion * 0.1f);
		}
	}

	Out.vColor = vector(FinalColor / 16.0f, FinalColor / 16.0f, FinalColor / 16.0f, 1.0f);

	return Out;
}

technique Default_Device
{
	pass SSAO
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}