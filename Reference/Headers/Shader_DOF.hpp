#include "Shader_Base.hpp"

//GaussFilter = X =================
float4 gaussFilterX[7] =
{
	0.0f,	 -3.0f,	 0.0f,	 1.0f / 64.0f,
	0.0f,	 -2.0f,	 0.0f,	 6.0f / 64.0f,
	0.0f,	 -1.0f,	 0.0f,	 15.0f / 64.0f,
	0.0f,	 0.0f,	 0.0f,	 20.0f / 64.0f,
	0.0f,	 1.0f,	 0.0f,	 15.0f / 64.0f,
	0.0f,	 2.0f,	 0.0f,	 6.0f / 64.0f,
	0.0f,	 3.0f,	 0.0f,	 1.0f / 64.0f
};
//=================================

float		fRange = 0.0f;//0.0125f;
float		fFocus = 0.5f;

float		g_fFar;

texture				g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture				g_BlendBlurTexture;
sampler BlendBlurSampler = sampler_state
{
	texture = g_BlendBlurTexture;
};

vector vDofParams;
float fDofFocus;

//float ComputeDepth(float depth)
//{
//	float f = 0.0f;
//	
//	if (depth < vDofParams.y)
//	{
//		f = (depth - vDofParams.y) / (vDofParams.y - vDofParams.x);
//	}
//	else
//	{
//		f = (depth - vDofParams.y) / (vDofParams.z - vDofParams.y);
//		f = clamp(f, 0, vDofParams.w);
//	}
//
//	return f * 0.5f + 0.5f;
//}

bool ComputeDepth(float depth)
{	
	if (depth < fDofFocus - 30.0f || depth > fDofFocus + 30.0f)
	{
		return false;
	}
	
	return true;
}

struct PS_IN
{
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};

PS_OUT PS_BLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vBlur = 0.0f;
	for (int i = 0; i < 7; ++i)
	{
		vBlur += tex2D(BaseSampler,
			float2(In.vTexUV.x + gaussFilterX[i].x * (1 / 70.0f) + 0.0f,
				In.vTexUV.y + gaussFilterX[i].y * (1 / 70.0f) + 0.0f)) * gaussFilterX[i].w;
	}

	for (int i = 0; i < 7; ++i)
	{
		vBlur += tex2D(BaseSampler,
			float2(In.vTexUV.x + gaussFilterX[i].x * (1 / -70.0f) + 0.0f,
				In.vTexUV.y + gaussFilterX[i].y * (1 / -70.0f) + 0.0f)) * gaussFilterX[i].w;
	}

	Out.vColor = vBlur;

	return Out;
}

PS_OUT PS_DOF(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vOriginal = tex2D(BaseSampler, In.vTexUV);
	vector vBlur = tex2D(BlendBlurSampler, In.vTexUV);

	float fDepth = tex2D(DepthSampler, In.vTexUV).x;	
	fDepth *= g_fFar;

	Out.vColor = lerp(vOriginal, vBlur, saturate(fRange * abs(fFocus - fDepth)));
	//if (ComputeDepth(fDepth))
	//	Out.vColor = vOriginal;
	//else
	//	Out.vColor = vBlur;
	
	return Out;
}

technique Default_Device
{
	pass Blur
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR();
	}

	pass Final
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_DOF();
	}
}