#include "Shader_Base.hpp"

float	g_fGage = 1.f;
float   g_fOriGage;
int		g_iIndex;
float2	g_vUvCut;
float	g_fNumColor;
int		g_iBossGage;
float	g_fGageEffect;
float	g_fWhiteAlpha;
float	g_fStartUV = 1.f;
float	g_fEndUV = 1.f;
float   g_fTransColor = 1.f;

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

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

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

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_MAIN_GAGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;

	if(g_fGage < In.vTexUV.x)
		Out.vColor.a *= 0.f;

	return Out;
}

PS_OUT PS_MAIN_GAGEINVERSE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;

	if(g_fGage < In.vTexUV.x)
		Out.vColor.a *= 0.f;

	return Out;
}
PS_OUT PS_MAIN_BLOCK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	if(g_iIndex == 0)
		Out.vColor = vector(1.f, 1.f, 1.f, 1.f);
	if(g_iIndex == 1)
		Out.vColor = vector(0.f, 1.f, 0.f, 1.f);
	if(g_iIndex == 2)
		Out.vColor = vector(0.f, 0.f, 1.f, 1.f);

	return Out;
}
PS_OUT PS_MAIN_COMBO(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.b *= g_fNumColor;
	Out.vColor.g *= g_fNumColor;

	return Out;
}

PS_OUT PS_MAIN_FADE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;
	Out.vColor.r = 0.6f;

	return Out;
}

PS_OUT PS_MAIN_BOSSGAGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;


	if (g_iBossGage == 1)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= 1.f;
		}

		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.6f;
			Out.vColor.g *= 0.2f;
			Out.vColor.b *= 0.6f;
			Out.vColor.a *= 0.5f;
		}


		if((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= 1.f;
		}
	}
	else if (g_iBossGage == 2)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.2f;
			Out.vColor.g *= 0.2f;
			Out.vColor.b *= 0.6f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= 1.f;
		}
	}
	else if (g_iBossGage == 3)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.2f;
			Out.vColor.g *= 0.6f;
			Out.vColor.b *= 0.2f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
	}
	else if (g_iBossGage == 4)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= g_fGageEffect;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.6f;
			Out.vColor.g *= 0.6f;
			Out.vColor.b *= 0.2f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
	}
	else if (g_iBossGage == 5)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.a *= 0.f;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.6f;
			Out.vColor.g *= 0.2f;
			Out.vColor.b *= 0.2f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= g_fGageEffect;
		}
	}
	
	//g_fOriGage
	if (g_iBossGage == 0)
		Out.vColor.a *= 0.f;

	return Out;
}
PS_OUT PS_MAIN_RANK_FADE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;
	Out.vColor.r *= 0.0f;
	Out.vColor.g *= 0.0f;
	Out.vColor.b *= 0.0f;

	return Out;
}

PS_OUT PS_MAIN_BLIGHT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;

	if (g_fStartUV >= 1.f)
		return Out;

	if (g_fStartUV <= In.vTexUV.x && g_fEndUV >= In.vTexUV.x)
	{
		if (g_fStartUV < 1.f)
		{
			Out.vColor.r = 1.f;
			Out.vColor.g = 1.f;
			Out.vColor.b = 1.f;
		}
		else
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= 1.f;
		}
	}
	return Out;
}

PS_OUT PS_MAIN_PLAYERNUM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;
	Out.vColor.r *= 1.f;
	Out.vColor.g *= 0.25f;
	Out.vColor.b *= 0.f;

	return Out;
}

PS_OUT PS_MAIN_BARDGAGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;


	if (g_iBossGage == 1)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= 1.f;
		}

		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 1.1f;
			Out.vColor.g *= 1.1f;
			Out.vColor.b *= 1.1f;
			Out.vColor.a *= 0.5f;
		}


		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 1.5f;
			Out.vColor.g *= 1.5f;
			Out.vColor.b *= 1.5f;
		}
	}
	else if (g_iBossGage == 2)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= 1.f;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.6f;
			Out.vColor.g *= 0.2f;
			Out.vColor.b *= 0.6f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= 1.f;
		}
	}
	else if (g_iBossGage == 3)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.2f;
			Out.vColor.g *= 0.2f;
			Out.vColor.b *= 0.6f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= 1.f;
		}
	}
	else if (g_iBossGage == 4)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.2f;
			Out.vColor.g *= 0.6f;
			Out.vColor.b *= 0.2f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
	}
	else if (g_iBossGage == 5)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= g_fGageEffect;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.6f;
			Out.vColor.g *= 0.6f;
			Out.vColor.b *= 0.2f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
	}
	else if (g_iBossGage == 6)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= 1.f;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.6f;
			Out.vColor.g *= 0.2f;
			Out.vColor.b *= 0.2f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= g_fGageEffect;
		}
	}
	else if (g_iBossGage == 7)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.2f;
			Out.vColor.g *= 0.2f;
			Out.vColor.b *= 0.6f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= 1.f;
		}
	}
	else if (g_iBossGage == 8)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.2f;
			Out.vColor.g *= 0.6f;
			Out.vColor.b *= 0.2f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= g_fGageEffect;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
	}
	else if (g_iBossGage == 9)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= g_fGageEffect;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.6f;
			Out.vColor.g *= 0.2f;
			Out.vColor.b *= 0.2f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= 1.f;
			Out.vColor.b *= g_fGageEffect;
		}
	}
	else if (g_iBossGage == 10)
	{
		if (g_fOriGage < In.vTexUV.x)
		{
			Out.vColor.a *= 0.f;
		}
		if ((g_fGage < In.vTexUV.x) && (g_fOriGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 0.6f;
			Out.vColor.g *= 0.2f;
			Out.vColor.b *= 0.2f;
			Out.vColor.a *= 0.5f;
		}
		if ((g_fGage >= In.vTexUV.x))
		{
			Out.vColor.r *= 1.f;
			Out.vColor.g *= g_fGageEffect;
			Out.vColor.b *= g_fGageEffect;
		}
	}

	//g_fOriGage
	if (g_iBossGage == 0)
		Out.vColor.a *= 0.f;

	return Out;
}
PS_OUT PS_MAIN_HANUMAN_NAME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;
	Out.vColor.r *= g_fTransColor;
	Out.vColor.g *= g_fTransColor;

	return Out;
}
PS_OUT PS_MAIN_BARDIEL_NAME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;
	Out.vColor.g *= g_fTransColor;
	Out.vColor.b *= g_fTransColor;

	return Out;
}
technique UI_Technique
{
	pass UI_Alphablend //0
	{
		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass UI_AlphaTest //1
	{
		Alphatestenable = true;

		Alpharef = 0x0000003f;
		Alphafunc = greater;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass UI_AlphablendTest //2
	{
		Alphablendenable = true;
		Alphatestenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;
		Alpharef = 0x0000003f;
		Alphafunc = greater;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass UI_Gage //3
	{
		Alphatestenable = true;

		Alpharef = 0x0000003f;
		Alphafunc = greater;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_GAGE();
	}
	pass UI_GageInverse //4
	{
		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_GAGEINVERSE();
	}

	pass UI_Block //5
	{
		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_BLOCK();
	}

	pass UI_COMBO //6
	{
		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_COMBO();
	}

	pass UI_FADE // 7
	{
		Alphablendenable = true;
		Alphatestenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;
		Alpharef = 0x0000003f;
		Alphafunc = greater;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_FADE();
	}
	pass UI_Boss_Gage //8
	{
		Alphatestenable = true;

		Alpharef = 0x0000003f;
		Alphafunc = greater;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_BOSSGAGE();
	}
	pass UI_RANK_FADE // 9
	{
		Alphablendenable = true;
		Alphatestenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;
		Alpharef = 0x0000003f;
		Alphafunc = greater;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_RANK_FADE();
	}

	pass UI_Alphablend //10
	{
		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_BLIGHT();
	}
	pass UI_Alphablend //11
	{
		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_PLAYERNUM();
	}
		pass UI_Bard_Gage //12
	{
		Alphatestenable = true;

	Alpharef = 0x0000003f;
	Alphafunc = greater;

	VertexShader = compile vs_3_0 VS_MAIN();
	PixelShader = compile ps_3_0 PS_MAIN_BARDGAGE();
	}

	pass UI_HanumanName //13
	{
		Alphablendenable = true;
		Alphatestenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;
		Alpharef = 0x0000003f;
		Alphafunc = greater;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_HANUMAN_NAME();
	}
	pass UI_BardielName //14
	{
		Alphablendenable = true;
		Alphatestenable = true;

		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;
		Alpharef = 0x0000003f;
		Alphafunc = greater;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_BARDIEL_NAME();
	}
}