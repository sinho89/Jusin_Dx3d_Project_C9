#include "Shader_Base.hpp"

texture			g_AlphaTexture;

sampler AlphaSampler = sampler_state
{
	texture = g_AlphaTexture;

	minfilter = linear;
	magfilter = linear;
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
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix		matWV, matWVP;

	matWV	=	mul(g_matWorld, g_matView);
	matWVP	=	mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	vector vBase = tex2D(BaseSampler, In.vTexUV);
	vector vAlpha = tex2D(AlphaSampler, In.vTexUV);

	vector vResult = vBase * vAlpha;
	//lerp(vResult, vBase, vAlpha);

	Out.vColor = vResult;
	//Out.vColor.a = 1.0f;

	return Out;
}

PS_OUT PS_ONE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;

	return Out;
}

technique MiniMap_Technique
{
	pass Default
	{
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0x0000003f;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass ONE
	{
		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_ONE();
	}
}