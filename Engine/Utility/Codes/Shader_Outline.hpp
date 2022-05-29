#include "Shader_Base.hpp"

texture	g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture	g_MapDepthTexture;
sampler MapDepthSampler = sampler_state
{
	texture = g_MapDepthTexture;
};

texture	g_OutlineTexture;
sampler OutlineSampler = sampler_state
{
	texture = g_OutlineTexture;
	minfilter = linear;
	magfilter = linear;
};

texture	g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
	minfilter = linear;
	magfilter = linear;
};
float3x3 convX = {	-1, 0, 1,
					-2, 0, 2,
					-1, 0, 1 };

float3x3 convY = {	 1,  2,  1,
					 0,  0,  0,
					-1, -2, -1 };

float2 gPixelOffset = float2(1.0f, 1.0f);

matrix g_matViewInv;
matrix g_matProjInv;

vector g_vCamPosition;

float g_fFar;

struct PS_IN
{
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	/*float lx = 0.0f;
	float ly = 0.0f;

	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			float2 offset = float2(x, y) * gPixelOffset;
			float3 tex = tex2D(BaseSampler, In.vTexUV + offset).rgb;
			float luminance = dot(tex, float3(0.3f, 0.59f, 0.11f));

			lx += luminance * convX[y + 1][x + 1];
			ly += luminance * convY[y + 1][x + 1];
		}
	}

	float fl = sqrt((lx*lx) + (ly*ly));
	Out.vColor = vector(fl.xxx, 1.0f);*/

	//if (Out.vColor.r == 0.0f && Out.vColor.g == 0.0f && Out.vColor.b == 0.0f)
	//	Out.vColor.a = 0.0f;

	/*vector Outline = tex2D(BaseSampler, In.vTexUV);

	if (Outline.r != 0.0f && Outline.g != 0.0f && Outline.b != 0.0f)
	{
		Out.vColor = 1.0f;
	}
	else
		Out.vColor = 0.0f;*/

	vector vOutline = tex2D(BaseSampler, In.vTexUV);

	//float fMapDepth = tex2D(MapDepthSampler, In.vTexUV).y;
	//float fDepth = tex2D(BaseSampler, In.vTexUV).y;

	float aa = 4.0f * vOutline.r;
	aa -= tex2D(BaseSampler, In.vTexUV + float2(vOutline.b, 0.0f)).r;
	aa -= tex2D(BaseSampler, In.vTexUV + float2(-vOutline.b, 0.0f)).r;
	aa -= tex2D(BaseSampler, In.vTexUV + float2(0.0f, vOutline.b)).r;
	aa -= tex2D(BaseSampler, In.vTexUV + float2(0.0f, -vOutline.b)).r;

	vector col = 0.0f;
	if (aa == 0.0f)
	{
		col = 0.0f;
	}
	else
	{
		if (vOutline.g > 0.1f && vOutline.g < 0.2f) // Red
		{
			col = vector(1.0f, 0.0f, 0.0f, aa);
		}
		else if (vOutline.g > 0.2f && vOutline.g < 0.3f) //Violet
		{
			col = vector(0.9333f, 0.5098f, 0.9333f, aa);
		}
		else //White
		{
			col = vector(1.0f, 1.0f, 1.0f, aa);
		}
	}

	Out.vColor = col;

	return Out;
}

PS_OUT PS_OUTLINECULL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;
	
	vector vDepth = tex2D(BaseSampler, In.vTexUV);

	if (vDepth.y != 0.0f)
	{
		vector	worldNormal = tex2D(NormalSampler, In.vTexUV);
		worldNormal = vector(worldNormal.xyz * 2.0f - 1.0f, 0.0f);

		vector	vLightDir;
		vector	vPosition;

		float		fViewZ = vDepth.x * g_fFar;

		vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
		vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
		vPosition.z = vDepth.y * fViewZ;
		vPosition.w = fViewZ;

		vPosition = mul(vPosition, g_matProjInv);
		vPosition = mul(vPosition, g_matViewInv);

		float3 vLimLightDir = normalize(g_vCamPosition.xyz - vPosition.xyz);

		half rim = 1.0f - saturate(dot(vLimLightDir, worldNormal.xyz));
		float3 RimLightColor = float3(1.0f, 0.8431f, 0.0f) * pow(rim, 2.5f);

		Out.vColor.rgb = RimLightColor;
	}	

	return Out;
}

technique Default_Device
{
	pass Outline
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Outline_Cull
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_OUTLINECULL();
	}
}