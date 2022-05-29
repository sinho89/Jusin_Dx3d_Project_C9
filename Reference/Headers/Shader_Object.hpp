#include "Shader_Base.hpp"

vector			g_vLightDir = (vector)0.f;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;

float			g_fPower = 1.f;
vector			g_vCamPosition;

vector			g_vMtrlDiffuse;
vector			g_vMtrlAmbient;
vector			g_vMtrlSpecular;

struct VS_IN
{
	vector		vPosition	:	POSITION;
	vector		vNormal		:	NORMAL;
	float2		vTexUV		:	TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition	:	POSITION;
	vector		vShade		:	COLOR0;
	vector		vSpecular	:	COLOR1;
	float2		vTexUV		:	TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;

	matrix	matWV, matWVP;
	matWV	=	mul(g_matWorld, g_matView);
	matWVP	=	mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.0f), matWVP);

	//난반사
	vector	vWorldNormal = mul(vector(In.vNormal.xyz, 0.0f), g_matWorld);
	vector	vWorldLight = g_vLightDir * -1.0f;

	float	fIntensity = max(dot(normalize(vWorldNormal), normalize(vWorldLight)), 0.0f);

	Out.vShade = fIntensity;
	Out.vShade.a = 1.0f;

	//정반사
	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vWorldLight));

	vector	vWorldPos = mul(vector(In.vPosition.xyz, 1.0f), g_matWorld);
	vector	vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.0f, normalize(vReflect)), 0.0f), g_fPower);
	Out.vSpecular.a = 1.0f;

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector	vShader		:	COLOR0;
	vector	vSpecular	:	COLOR1;
	float2	vTexUV		:	TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor		:	COLOR0;
};

PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);

	//float fAlpha = vColor.a;

	Out.vColor = (vColor * (g_vLightDiffuse * g_vMtrlDiffuse)) * (In.vShader + (g_vLightAmbient * g_vMtrlAmbient)) + (g_vLightSpecular * g_vMtrlSpecular) * In.vSpecular;

	Out.vColor.a = g_fAlpha;

	return Out;
}

struct VS_OUT_ALPHA
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	float2		vTexUV : TEXCOORD0;
};

struct PS_IN_ALPHA
{
	vector		vShade : COLOR0;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT_ALPHA VS_MAIN_ALPHATEST(VS_IN In)
{
	VS_OUT_ALPHA		Out = (VS_OUT_ALPHA)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	//빛~
	vector		vLightDirInv = normalize(g_vLightDir) * -1.f;

	vector		vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	vWorldNormal = normalize(vWorldNormal);

	Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	return Out;
}

PS_OUT PS_MAIN_ALPHATEST(PS_IN_ALPHA In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = Out.vColor * In.vShade;

	return Out;
}


technique Default_Device
{
	pass Default
	{
		alphatestenable = false;

		//alphatestenable = false;
		alphafunc = greater;
		alpharef = 0x0000003f;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader	 = compile ps_3_0 PS_MAIN();
	}

	pass AlphaTree
	{
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0x0000003f;

		VertexShader = compile vs_3_0 VS_MAIN_ALPHATEST();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHATEST();
	}

	pass AlphaTest
	{	
		Alphablendenable = true;

		srcblend = SRCALPHA;
		destblend = ONE;

		VertexShader = compile vs_3_0 VS_MAIN_ALPHATEST();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHATEST();
	}

	pass AlphaOne
	{
		Alphablendenable = true;


		srcblend = SRCALPHA;
		destblend = ONE;
	}

	pass PickingRect
	{
		Alphablendenable = true;
		
		srcblend = SRCALPHA;
		destblend = INVSRCALPHA;

		cullmode= none;

		//셰이더 버전 3.0...
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();

	}
	
};