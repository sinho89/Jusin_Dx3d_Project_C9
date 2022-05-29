#include "Shader_Base.hpp"

Matrix			g_matViewInv;
Matrix			g_matProjInv;
float			g_fColor;
float			g_fFar;
float			g_fReflect;

texture  g_DepthTexture;
texture  g_BlendTexture;
texture  g_NormalTexture;



sampler ParticleSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	ADDRESSU = CLAMP;
	ADDRESSV = CLAMP;
};

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

sampler BlendSampler = sampler_state
{
	texture = g_BlendTexture;

	minfilter = linear;
	magfilter = linear;

	ADDRESSU = CLAMP;
	ADDRESSV = CLAMP;
};

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


struct VS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV	  : TEXCOORD0; //ÅØ½ºÃÄ ÁÂÇ¥
	vector		vNormal   : NORMAL;
};

struct VS_OUT
{
	vector	vPosition	 : POSITION;
	float2	vTexUV		 : TEXCOORD0;
	vector	vEffectDepth : TEXCOORD1;
	vector  vNormal		 : NORMAL;

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
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	return Out;
}

struct PS_IN
{
	float2  vTexUV : TEXCOORD0;
	vector	vEffectDepth : TEXCOORD1;
	vector vNormal : NORMAL;
};

struct PS_OUT
{
	vector vColor : COLOR0;
	vector vNormal : COLOR1;
};

PS_OUT PS_Reflect(PS_IN In)
{
	//±¼Àý ºÎºÐ
	PS_OUT Out = (PS_OUT)0.0f;


	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	Out.vColor.a = 1.0f;

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//ÀÌÆåÆ®ÀÇ ±íÀÌ.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;


	////³ë¸» ºÎºÐ
	float3 Normal = tex2D(NormalSampler, In.vTexUV);

	Normal = normalize(Normal);

	Out.vColor.rgb = tex2D(BlendSampler, uv + Normal.xy * g_fReflect).rgb; //±¼Àý·ü


	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	//Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
		Out.vColor.a = 0.0f;



	return Out;
}



PS_OUT PS_ALPHA(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;


	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	Out.vColor.a = 1.0f;

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//ÀÌÆåÆ®ÀÇ ±íÀÌ.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;


	////³ë¸» ºÎºÐ
	float3 Normal = tex2D(NormalSampler, In.vTexUV);

	Normal = normalize(Normal);

	Out.vColor.rgb = tex2D(BlendSampler, uv + Normal.xy * g_fReflect).rgb; //±¼Àý·ü


	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	//Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
		Out.vColor.a = 0.0f;



	return Out;
}

technique Reflect_Technique
{
	pass Default
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		ALPHABLENDENABLE = TRUE;

		SRCBLEND = SRCALPHA;					//SRC : ±×¸®·Á°í ÇÏ´Â°ª
		DESTBLEND = INVSRCALPHA;				//Dst : ¹è°æ°ª

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_Reflect();

	}

}