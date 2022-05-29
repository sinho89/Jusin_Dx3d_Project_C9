#include "Shader_Base.hpp"

Matrix			g_matViewInv;
Matrix			g_matProjInv;
vector			g_vColor;
float			g_fFar;
texture			g_DepthTexture;
float			g_fGage;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;

};

struct VS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV	  : TEXCOORD0; //ÅØ½ºÃÄ ÁÂÇ¥
};


struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV	  : TEXCOORD0;
	vector	vEffectDepth  : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Matrix matMV, matMVP;

	matMV = mul(g_matWorld, g_matView);
	matMVP = mul(matMV, g_matProj);

	Out.vPosition = mul(In.vPosition, matMVP);
	Out.vEffectDepth = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}


struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
	vector	vEffectDepth : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_vColor.a;

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//ÀÌÆåÆ®ÀÇ ±íÀÌ.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}


PS_OUT PS_COLORTRAIL(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.r += g_vColor.r;
	Out.vColor.g += g_vColor.g;
	Out.vColor.b += g_vColor.b;
	Out.vColor.a *= g_vColor.a;


	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//ÀÌÆåÆ®ÀÇ ±íÀÌ.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;


	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ	= In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}

PS_OUT PS_COLORTRAILTWO(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.r = g_vColor.r;
	Out.vColor.g = g_vColor.g;
	Out.vColor.b = g_vColor.b;
	Out.vColor.a *= g_vColor.a;


	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//ÀÌÆåÆ®ÀÇ ±íÀÌ.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;


	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}

PS_OUT PS_ALPHATRAIL(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_vColor.a;

	if (g_fGage > In.vTexUV.y)
		Out.vColor.a *= 0.f;

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//ÀÌÆåÆ®ÀÇ ±íÀÌ.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;


	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}



technique Trail_Technique
{
	//0
	pass Default
	{
		LIGHTING = FALSE;
		CullMode = NONE;

		ALPHABLENDENABLE = TRUE;
		ZWRITEENABLE = FALSE;

		BLENDOP = ADD;

		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	//1
	pass ColorTrail
	{
		LIGHTING = FALSE;
		CullMode = NONE;

		ALPHABLENDENABLE = TRUE;
		ZWRITEENABLE = FALSE;

		BLENDOP = ADD;

		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_COLORTRAIL();
	}

	//2 RGB°ª ¹Ù·Î Àû¿ë(=)
	pass ColorTrailTwo
	{
		LIGHTING = FALSE;
		CullMode = NONE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		ALPHABLENDENABLE = TRUE;

		BLENDOP = ADD;

		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_COLORTRAILTWO();
	}

	//3
	pass AlphaTrail
	{
		LIGHTING = FALSE;
		CullMode = NONE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;
		ALPHABLENDENABLE = TRUE;

		BLENDOP = ADD;

		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;


		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_ALPHATRAIL();
	}


}