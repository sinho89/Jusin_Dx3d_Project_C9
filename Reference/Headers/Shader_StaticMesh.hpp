#include "Shader_Base.hpp"

vector			g_vLightDir = (vector)0.f;

vector			g_vLightPosition;
vector			g_vCamPosition;

float			g_fPower = 1.f;
float			g_fFar;

matrix			g_matLightView;
matrix			g_matLightProj;

matrix			g_matLightProj_1;
matrix			g_matLightProj_2;

vector			g_vLightDiffuse;
vector			g_vLightAmbient;

vector			g_vMtrlDiffuse = vector(1.0f, 1.0f, 1.0f, 1.0f);
vector			g_vMtrlAmbient = vector(0.2f, 0.2f, 0.2f, 1.f);
vector			g_vMtrlSpecular = vector(1.0f, 1.0f, 1.0f, 1.0f);

float			g_fLightCamFar;

float			g_fBrightness = 0.0f;

bool			g_bSpecularCheck = false;
bool			g_bBlurCheck = false;

bool			g_bOutline = false;
float			g_fColorCheck = 0.0f;
float			g_fOutlineThickness = 0.0005f;


float			g_bOutlinePower;
vector			g_vColor;

bool			g_bLimLight = false;

float			rimWidth = 0.0f;

bool			m_gUVAnimation = false;
float			m_gTime;
float			m_gSpeed = 2.0f;
float			m_gWaveHeight = 3.0f;
float			m_gWaveFrequency = 10.0f;
float			m_gUVSpeed = 0.25f;

//For.Normal map Sampler=============
texture			g_Normal_Texture;
sampler NormalSampler = sampler_state
{
	texture = g_Normal_Texture;

	minfilter = linear;
	magfilter = linear;
};

//For.Specular map Sampler============
texture			g_Specular_Texture;
sampler SpecularSampler = sampler_state
{
	texture = g_Specular_Texture;

	minfilter = linear;
	magfilter = linear;
};

texture			g_Depth_Texture;
sampler DepthSampler = sampler_state
{
	texture = g_Depth_Texture;

	minfilter = linear;
	magfilter = linear;
};

texture			g_RandNormal_Texture;
sampler RandNormalSampler = sampler_state
{
	texture = g_RandNormal_Texture;

	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	vector		vPosition	: POSITION;
	vector		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;

	float3		mTangent	: TANGENT;
	float3		mBinormal	: BINORMAL;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;

	float3			T		: TEXCOORD1;
	float3			B		: TEXCOORD2;
	float3			N		: TEXCOORD3;

	float3		vLightDir	: TEXCOORD4;
	float3		vViewDir	: TEXCOORD5;

	vector		vDepthPos	: TEXCOORD6;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;

	Matrix	matWV, matVP, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matVP = mul(g_matView, g_matProj);
	matWVP = mul(matWV, g_matProj);
	
	vector	posWorld = mul(In.vPosition, g_matWorld);
	
	if (m_gUVAnimation)
	{
		float cosTime = m_gWaveHeight * cos(m_gTime * m_gSpeed + In.vTexUV.x * m_gWaveFrequency);
		In.vPosition.y += cosTime;
		Out.vTexUV = In.vTexUV + float2(m_gTime * m_gUVSpeed, 0);
	}
	else
		Out.vTexUV = In.vTexUV;

	Out.vPosition = mul(In.vPosition, matWVP);
	Out.vDepthPos = Out.vPosition;

	Out.N = normalize(mul(In.vNormal.xyz, (float3x3)g_matWorld));
	Out.T = normalize(mul(In.mTangent, (float3x3)g_matWorld));
	Out.B = normalize(mul(In.mBinormal, (float3x3)g_matWorld));

	Out.vLightDir = normalize(g_vLightDir.xyz);
	Out.vViewDir = posWorld;//normalize(posWorld.xyz - g_vCamPosition.xyz);

	return Out;
}

struct PS_IN
{
	float2	vTexUV		: TEXCOORD0;

	float3		T		: TEXCOORD1;
	float3		B		: TEXCOORD2;
	float3		N		: TEXCOORD3;

	float3	vLightDir	: TEXCOORD4;
	float3	vViewDir	: TEXCOORD5;

	vector	vDepthPos	: TEXCOORD6;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
	//vector	vNormal		: COLOR1;
	//vector	vSpecular	: COLOR2;
	vector	vDepthMap : COLOR1;
	vector	vCharSampler : COLOR2;
	vector	vOutlineChar : COLOR3;
};

PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	
	//Out.vColor = Out.vColor * g_vMtrlDiffuse + g_vMtrlAmbient;

	if (m_gUVAnimation)
	{
		Out.vColor.rgb *= 4.0f;
	}

	Out.vColor.rgb += float3(g_fBrightness, g_fBrightness, g_fBrightness);
	Out.vColor *= g_fAlpha;

	//if (true == g_bBlurCheck)
	//	Out.vCharSampler = tex2D(BaseSampler, In.vTexUV);

	Out.vOutlineChar = vector(0.0f, 0.0f, 0.0f, 1.0f);
	
	//Out.vColor = Out.vColor * g_vMtrlDiffuse + g_vMtrlAmbient;
	//Out.vColor.a = g_fAlpha * Out.vColor.a;

	float3	tangentNormal = tex2D(NormalSampler, In.vTexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);
	float3	worldNormal = mul(TBN, tangentNormal);

	//Out.vNormal = vector(worldNormal.xyz * 0.5f + 0.5f, 0.0f);

	//float3	LightDir = normalize(In.vLightDir);
	//float3	diffuse = saturate(dot(worldNormal, -LightDir));
	//diffuse = Out.vColor.rgb * diffuse;

	if (g_bOutline)
	{
		/*float3 vLimLightDir = normalize(g_vCamPosition.xyz - In.vViewDir.xyz);
		half rim = 1.0f - saturate(dot(vLimLightDir, worldNormal.xyz));
		float3 RimLightColor = g_vColor.rgb * pow(rim, g_bOutlinePower);

		Out.vOutlineChar = vector(RimLightColor, 1.0f);*/
		Out.vOutlineChar = vector(1.0f, g_fColorCheck, g_fOutlineThickness, 1.0f);
	}

	//float3 specular = 0.0f;
	//if (diffuse.x > 0.0f)
	//{
	//	/*float3 vReflect = reflect(LightDir, worldNormal);
	//	float3 vLook = normalize(In.vViewDir.xyz);

	//	specular = saturate(dot(vReflect, -vLook));
	//	specular = pow(specular, g_fPower);*/

	//	vector specularIntensity = tex2D(SpecularSampler, In.vTexUV);
	//	specular *= specularIntensity.xyz;
	//}

	//Out.vSpecular = vector(specular.xyz, 0.0f);// *(g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	//Out.vSpecular *= g_vMtrlSpecular;

	//float fDepth = In.vDepthPos.z / In.vDepthPos.w;

	//Out.vDepthMap = vector(fDepth.xxx, 1.0f);

	Out.vDepthMap = vector(In.vDepthPos.w / g_fFar,
		In.vDepthPos.z / In.vDepthPos.w,
		0.0f,
		1.0f);

	//In.vShadowPos.w / g_fLightCamFar, 
	//In.vShadowPos.z / In.vShadowPos.w);

	/*float curDepth = In.vShadowPos.z / In.vShadowPos.w;

	float2 uv = In.vShadowPos.xy / In.vShadowPos.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float ShadowDepth = tex2D(ShadowSampler, uv).r;

	if(curDepth > ShadowDepth + 0.0000125f)
	{
	Out.vColor.rgb *= 0.5f;
	}*/

	return Out;
}

//===============================================
//Shade=========================================
//===============================================

VS_OUT VS_SHADE(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Matrix	matWV, matVP, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matVP = mul(g_matView, g_matProj);
	matWVP = mul(matWV, g_matProj);

	vector	posWorld = mul(In.vPosition, g_matWorld);

	if (m_gUVAnimation)
	{
		float cosTime = m_gWaveHeight * cos(m_gTime * m_gSpeed + In.vTexUV.x * m_gWaveFrequency);
		In.vPosition.y += cosTime;
		Out.vTexUV = In.vTexUV + float2(m_gTime * m_gUVSpeed, 0);
	}
	else
		Out.vTexUV = In.vTexUV;

	Out.vPosition = mul(In.vPosition, matWVP);

	Out.N = normalize(mul(In.vNormal.xyz, (float3x3)g_matWorld));
	Out.T = normalize(mul(In.mTangent, (float3x3)g_matWorld));
	Out.B = normalize(mul(In.mBinormal, (float3x3)g_matWorld));

	Out.vLightDir = normalize(g_vLightDir.xyz);
	Out.vViewDir = normalize(posWorld.xyz - g_vCamPosition.xyz);

	Out.vDepthPos = posWorld;

	return Out;
}

struct PS_SHADE_OUT
{
	vector	vNormal : COLOR0;
	vector	vSpecular : COLOR1;
	vector	vD_Specular : COLOR2;
	vector	vLimLight : COLOR3;
};

PS_SHADE_OUT PS_SHADE(PS_IN In)
{
	PS_SHADE_OUT Out = (PS_SHADE_OUT)0.0f;
	
	float3	tangentNormal = tex2D(NormalSampler, In.vTexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);
	float3	worldNormal = mul(TBN, tangentNormal);

	Out.vNormal = vector(worldNormal.xyz * 0.5f + 0.5f, 0.0f);

	float3	LightDir = normalize(In.vLightDir);
	//float3	diffuse = saturate(dot(worldNormal, -LightDir));
	//diffuse = Out.vColor.rgb * diffuse;
	
	float3 specular = 0.0f;
	//if (diffuse.x > 0.0f)
	//{
		float3 vReflect = reflect(LightDir, worldNormal);
		float3 vLook = normalize(In.vViewDir.xyz);

		specular = saturate(dot(vReflect, -vLook));
		specular = pow(specular, g_fPower);

		vector specularIntensity = tex2D(SpecularSampler, In.vTexUV);
		//specular = specularIntensity.xyz;
		specular *= specularIntensity.xyz;
	//}
	
	Out.vSpecular = vector(specular.xyz, 0.0f);// *(g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
											   //Out.vSpecular *= g_vMtrlSpecular;

											   //float fDepth = In.vDepthPos.z / In.vDepthPos.w;

											   //Out.vDepthMap = vector(fDepth.xxx, 1.0f);

	if (g_bLimLight)
	{
		float3 vLimLightDir = normalize(g_vCamPosition.xyz - In.vDepthPos.xyz);
		half rim = 1.0f - saturate(dot(vLimLightDir, worldNormal.xyz));
		float3 RimLightColor = g_vColor.rgb * pow(rim, g_bOutlinePower);

		Out.vLimLight = vector(RimLightColor, 0.0f);
	}

	if(g_bSpecularCheck)
		Out.vD_Specular = vector(specular.xyz, 0.0f);


	return Out;
}

//===============================================
//Shadow=========================================
//===============================================
struct VS_SHADOW_IN
{
	vector		vPosition	: POSITION;
};

struct VS_SHADOW_OUT
{
	vector		vPosition	: POSITION;
	vector		vDepthPos	: TEXCOORD0;
};

VS_SHADOW_OUT VS_SHADOW_MAIN(VS_SHADOW_IN In)
{
	VS_SHADOW_OUT	Out = (VS_SHADOW_OUT)0;

	Matrix	matWV, matWVP;
	matWV = mul(g_matWorld, g_matLightView);
	matWVP = mul(matWV, g_matLightProj);

	Out.vPosition = mul(In.vPosition, matWVP);
	Out.vDepthPos = Out.vPosition;

	return Out;
}

struct PS_SHADOW_IN
{
	vector		vDepthPos	: TEXCOORD0;
};

struct PS_SHADOW_OUT
{
	vector		vColor		: COLOR0;
};

PS_SHADOW_OUT PS_SHADOW_MAIN(PS_SHADOW_IN In)
{
	PS_SHADOW_OUT Out = (PS_SHADOW_OUT)0.0f;

	float fDepth = In.vDepthPos.z / In.vDepthPos.w;

	Out.vColor = vector(fDepth.xxx, 1.0f);

	return Out;
}

//===============================================
//MiniMap=========================================
//===============================================
struct VS_MINIMAP_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_MINIMAP_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;	
};

VS_MINIMAP_OUT VS_MINIMAP(VS_MINIMAP_IN In)
{
	VS_MINIMAP_OUT	Out = (VS_MINIMAP_OUT)0;

	Matrix	matWV, matVP, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(In.vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_MINIMAP_IN
{
	float2	vTexUV : TEXCOORD0;
};

struct PS_MINIMAP_OUT
{
	vector	vColor : COLOR0;
};

PS_MINIMAP_OUT	PS_MINIMAP(PS_MINIMAP_IN In)
{
	PS_MINIMAP_OUT	Out = (PS_MINIMAP_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	//Out.vColor.a = g_fAlpha;

	return Out;
}

struct VS_STAGEMAP_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_STAGEMAP_OUT
{
	vector		vPosition : POSITION;
	vector		vDepthPos : TEXCOORD0;
};

VS_STAGEMAP_OUT VS_STAGEMAP(VS_STAGEMAP_IN In)
{
	VS_STAGEMAP_OUT	Out = (VS_STAGEMAP_OUT)0.0f;

	Matrix	matWV, matVP, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	
	Out.vPosition = mul(In.vPosition, matWVP);
	Out.vDepthPos = Out.vPosition;

	return Out;
}

struct PS_STAGEMAP_IN
{
	vector		vDepthPos : TEXCOORD0;
};

struct PS_STAGEMAP_OUT
{
	vector		vColor : COLOR0;
};

PS_STAGEMAP_OUT PS_STAGEMAP(PS_STAGEMAP_IN In)
{
	PS_STAGEMAP_OUT Out = (PS_STAGEMAP_OUT)0.0f;

	Out.vColor = vector(In.vDepthPos.w / g_fFar,
		In.vDepthPos.z / In.vDepthPos.w,
		0.0f,
		1.0f);

	return Out;
}

PS_SHADOW_OUT PS_CACD_SHADOW_MAIN(PS_SHADOW_IN In)
{
	PS_SHADOW_OUT Out = (PS_SHADOW_OUT)0.0f;

	float fDepth = In.vDepthPos.z;
	Out.vColor = vector(fDepth.xxx, 1.0f);

	return Out;
}

struct VS_OUTLINE_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUTLINE_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vDepthPos : TEXCOORD1;
};

VS_OUTLINE_OUT VS_OUTLINE(VS_OUTLINE_IN In)
{
	VS_OUTLINE_OUT Out = (VS_OUTLINE_OUT)0.0f;

	Matrix	matWV, matVP, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matVP = mul(g_matView, g_matProj);
	matWVP = mul(matWV, g_matProj);

	vector	posWorld = mul(In.vPosition, g_matWorld);

	Out.vPosition = mul(In.vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	Out.vDepthPos = Out.vPosition;

	return Out;
}

struct PS_OUTLINE_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vDepthPos : TEXCOORD1;
};

struct PS_OUTLINE_OUT
{
	vector		vColor : COLOR0;
};

PS_OUTLINE_OUT PS_OUTLINE(PS_OUTLINE_IN In)
{
	PS_OUTLINE_OUT Out = (PS_OUTLINE_OUT)0.0f;

	float fMapDepth = tex2D(DepthSampler, In.vTexUV).y;
	float fDepth = In.vDepthPos.z / In.vDepthPos.w;

	if (fMapDepth > fDepth)
		Out.vColor.rgb = vector(0.0f, fDepth, 0.0f, 0.0f);

	//Out.vColor.rgb = vector(0.0f, In.vDepthPos.z / In.vDepthPos.w, 0.0f, 0.0f);

	return Out;
}

technique Default_Device
{
	//0
	pass Default
	{
		//ALPHABLENDENABLE = TRUE;
		//SRCBLEND = SRCALPHA;
		//DESTBLEND = INVSRCALPHA;
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0x0000003f;

		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}

	//1
	pass Shade
	{
		VertexShader	= compile vs_3_0 VS_SHADE();
		PixelShader		= compile ps_3_0 PS_SHADE();
	}

	//2
	pass Shadow
	{
		VertexShader	= compile vs_3_0 VS_SHADOW_MAIN();
		PixelShader		= compile ps_3_0 PS_SHADOW_MAIN();
	}
	//3
	pass MiniMap
	{
		VertexShader	= compile vs_3_0 VS_MINIMAP();
		PixelShader		= compile ps_3_0 PS_MINIMAP();
	}

	//4
	pass StageMapDepth
	{
		VertexShader	= compile vs_3_0 VS_STAGEMAP();
		PixelShader		= compile ps_3_0 PS_STAGEMAP();
	}

	//5
	pass CACD_Shadow
	{
		VertexShader = compile vs_3_0 VS_SHADOW_MAIN();
		PixelShader = compile ps_3_0 PS_CACD_SHADOW_MAIN();
	}

	//6
	pass Outline
	{
		VertexShader = compile vs_3_0 VS_OUTLINE();
		PixelShader  = compile ps_3_0 PS_OUTLINE();
	}
}