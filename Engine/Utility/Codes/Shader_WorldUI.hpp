#include "Shader_Base.hpp"

float			g_fGage = 1.f;
int				g_iIndex;
float			g_fFar;
float			g_fDepth;
bool			g_bCritical = false;
bool			g_bBackBlur = false;

texture  g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

struct VS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vUIDepth : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vUIDepth = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
	vector		vUIDepth : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;
	
	float2 uv = In.vUIDepth.xy / In.vUIDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fUIDepth = In.vUIDepth.z / In.vUIDepth.w;
	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vUIDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fUIDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}

PS_OUT PS_MAIN_GAGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;

	float fEffectDepth = In.vUIDepth.z / In.vUIDepth.w;	

	float2 uv = In.vUIDepth.xy / In.vUIDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vUIDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	if (g_fGage < In.vTexUV.x)
		Out.vColor.a *= 0.f;

	Out.vColor.r /= g_fGage;
	Out.vColor.g *= g_fGage;


	return Out;
}
PS_OUT PS_MAIN_DAM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;

	float2 uv = In.vUIDepth.xy / In.vUIDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fUIDepth = In.vUIDepth.z / In.vUIDepth.w;
	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vUIDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fUIDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	if (g_bCritical)
		Out.vColor.r = 1.f;

	if (g_bBackBlur)
	{
		Out.vColor.r = 0.f;
		Out.vColor.g = 0.f;
		Out.vColor.b = 0.f;
	}

	if (g_bBackBlur && g_bCritical)
	{
		Out.vColor.r = 1.f;
		Out.vColor.g = 0.25f;
		Out.vColor.b = 0.f;

	}
	
	return Out;
}
PS_OUT PS_MAIN_PLAYERDAM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;

	float2 uv = In.vUIDepth.xy / In.vUIDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fUIDepth = In.vUIDepth.z / In.vUIDepth.w;
	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vUIDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fUIDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}
	Out.vColor.r *= 1.f;
	Out.vColor.g *= 0.2f;
	Out.vColor.b *= 0.f;

	return Out;
}
technique WorldUI_Technique
{
	pass WorldUI_Bar
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;
	
		ZWRITEENABLE = FALSE;

		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass WorldUI_Gage
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;

		ZWRITEENABLE = FALSE;

		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_GAGE();
	}

	pass WorldUI_DamNumber
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;

		ZWRITEENABLE = FALSE;

		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_DAM();
	}

	pass WorldUI_PlayerDamNumber
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;

		ZWRITEENABLE = FALSE;

		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_PLAYERDAM();
	}
}