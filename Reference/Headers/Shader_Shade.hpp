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

//GaussFilter = Y =================
float4 gaussFilterY[7] =
{
	-3.0f,   0.0f,	0.0f, 1.0f / 64.0f,
	-2.0f,   0.0f,	0.0f, 6.0f / 64.0f,
	-1.0f,   0.0f,	0.0f, 15.0f / 64.0f,
	0.0f,	 0.0f,	0.0f, 20.0f / 64.0f,
	1.0f,	 0.0f,	0.0f, 15.0f / 64.0f,
	2.0f,	 0.0f,	0.0f, 6.0f / 64.0f,
	3.0f,	 0.0f,	0.0f, 1.0f / 64.0f
};
//=================================

//GaussFilter = X Y =================
float4 gaussFilterXY[7] =
{
	-3.0f,	 -3.0f,	 0.0f,	 1.0f / 64.0f,
	-2.0f,	 -2.0f,	 0.0f,	 6.0f / 64.0f,
	-1.0f,	 -1.0f,	 0.0f,	 15.0f / 64.0f,
	 0.0f,	  0.0f,	 0.0f,	 20.0f / 64.0f,
	 1.0f,	  1.0f,	 0.0f,	 15.0f / 64.0f,
	 2.0f,	  2.0f,	 0.0f,	 6.0f / 64.0f,
	 3.0f,	  3.0f,	 0.0f,	 1.0f / 64.0f
};
//=================================

//GaussFilter = Y X =================
float4 gaussFilterYX[7] =
{
	3.0f,   -3.0f,	0.0f, 1.0f / 64.0f,
	2.0f,   -2.0f,	0.0f, 6.0f / 64.0f,
	1.0f,   -1.0f,	0.0f, 15.0f / 64.0f,
	0.0f,	 0.0f,	0.0f, 20.0f / 64.0f,
	-1.0f,	 1.0f,	0.0f, 15.0f / 64.0f,
	-2.0f,	 2.0f,	0.0f, 6.0f / 64.0f,
	-3.0f,	 3.0f,	0.0f, 1.0f / 64.0f
};
//=================================

float3	Luminance = float3(0.212671f, 0.715160f, 0.072169f);

float3	R_to_X = float3(0.4674162f, 0.2944512f, 0.1886026f);
float3	G_to_Y = float3(0.2410115f, 0.6835475f, 0.0754410f);
float3	B_to_Z = float3(0.0219101f, 0.0736128f, 0.9933071f);

float3	X_to_R = float3(2.7454669f, -1.1358136f, -0.4350269f);
float3	Y_to_G = float3(-0.9692660f, 1.8760108f, 0.0415560f);
float3	Z_to_B = float3(0.0112723f, -0.1139754f, 1.0093968f);


float				g_fBlurValue;
float				g_fBlurPower = 1.0f;
float				texScaler = 1.0f / 160.0f;//1.0f / 128.0f;
float				texOffset = 0.0f;

//Bloom 효과 부가 결정 bool
bool				m_bGaussFilter = false;


vector				g_vLightDir;
vector				g_vLightDiffuse;
vector				g_vLightAmbient;

vector				g_vMtrlDiffuse = (1.0f, 1.0f, 1.0f, 1.0f);
vector				g_vMtrlAmbient = (1.0f, 1.0f, 1.0f, 1.0f);

float				g_fFar;
float				g_LightfFar;

Matrix				g_matViewInv;
Matrix				g_matProjInv;

Matrix				g_matLightView;
Matrix				g_matLightProj;

vector				g_vLightPos;
float				g_fRange;

bool				g_bShadow = true;

vector				g_vCamPosition;
vector				g_vViewDir;

float				rimWidth = 0.75f;

texture				g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture				g_DirectionalTexture;
sampler DirectionalSampler = sampler_state
{
	texture = g_DirectionalTexture;
};

texture				g_PointTexture;
sampler PointSampler = sampler_state
{
	texture = g_PointTexture;
};

texture				g_ShadowTexture;
sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;

	minfilter = linear;
	magfilter = linear;
	//mipfilter	= linear;

	//AddressU	= Border;
	//AddressV	= Border;
	//BorderColor = 0xFFFFFFFF;
};

texture				g_SpecularTexture;
sampler g_SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_Specular_Direc_Texture;
sampler Specular_Direc_Sampler = sampler_state
{
	texture = g_Specular_Direc_Texture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_Specular_Point_Texture;
sampler Specular_Point_Sampler = sampler_state
{
	texture = g_Specular_Point_Texture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_Blur_Texture;
sampler Blur_Sampler = sampler_state
{
	texture = g_Blur_Texture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_BlurDepth_Texture;
sampler BlurDepth_Sampler = sampler_state
{
	texture = g_BlurDepth_Texture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_BlurDepth_All_Texture;
sampler BlurDepth_All_Sampler = sampler_state
{
	texture = g_BlurDepth_All_Texture;

	minfilter = linear;
	magfilter = linear;
};

struct PS_IN
{
	float2 vTexUV : TEXCOORD0;
};

//struct PS_OUT
//{
//	vector vShade : COLOR0;
//	vector vShadow : COLOR1;
//};

struct PS_OUT
{
	vector vShade : COLOR0;
	vector vSpecular : COLOR1;
};

struct PS_NORMAL_OUT
{
	vector vShade : COLOR0;
};

//=================================================
//Light Directional================================
//=================================================
PS_OUT PS_DIRECTIONAL(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.0f;

	vector	vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz * 2.0f - 1.0f, 0.0f);

	vector	vShade = max(dot(normalize(g_vLightDir) * -1.0f, vNormal), 0.0f);
	//vShade.a = 1.0f;

	Out.vShade = vShade * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	vector	vDepth = tex2D(DepthSampler, In.vTexUV);

	float	fViewZ = vDepth.x * g_fFar;

	vector vPosition = 0.0f;

	//뷰 영역상 위치에서 투영행렬을 곱해놓은 상태에서의 위치.
	vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewZ;
	vPosition.z = vDepth.y * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);	//뷰 영역상 위치.
	vPosition = mul(vPosition, g_matViewInv);	//월드 영역상 위치.

	//그림자.
	/*vector vLightPos = 0.0f;
	vLightPos = mul(vPosition, g_matLightView);
	vLightPos = mul(vLightPos, g_matLightProj);

	float CurDepth = vLightPos.z / vLightPos.w;

	float2 uv = vLightPos.xy / vLightPos.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fShadowDepth = tex2D(ShadowSampler, uv).r;

	Out.vShadow = 1.0f;

	if (uv.x >= 0.0f && uv.x <= 1.0f && uv.y >= 0.0f && uv.y <= 1.0f)
	{
		if (CurDepth > fShadowDepth + 0.002f)
		{
			Out.vShadow.rgb *= 0.5f;
		}
	}*/
	
	return Out;
}

//=================================================
//Light Directional================================ NO Shadow
//=================================================
PS_NORMAL_OUT PS_NO_SHADOW_DIRECTIONAL(PS_IN In)
{
	PS_NORMAL_OUT Out = (PS_NORMAL_OUT)0.0f;

	vector	vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz * 2.0f - 1.0f, 0.0f);

	vector vLightDir = normalize(g_vLightDir);

	vector	vShade = max(dot(vLightDir * -1.0f, vNormal), 0.0f);
	//vShade.a = 1.0f;

	Out.vShade = vShade * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	//float3 vLook = normalize(g_vViewDir.xyz);

	//float3 specular = 0.0f;

	//specular = saturate(dot(vShade, -vLook));
	////specular = pow(specular, 20.0f);

	//vector vSpecular = tex2D(g_SpecularSampler, In.vTexUV);
	//specular *= vSpecular.xyz;

	//specular /= 2.0f;

	//Out.vSpecular = vector(specular.xyz, 0.0f);

	return Out;
}

//=================================================
//Light Point======================================
//=================================================
PS_OUT PS_POINT(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.0f;

	vector	vOriNormal = tex2D(NormalSampler, In.vTexUV);

	vector vNormal = vector(vOriNormal.xyz * 2.0f - 1.0f, 0.0f);

	vector		vLightDir;
	vector		vPosition;

	vector		vDepth = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vDepth.x * g_fFar;

	// 뷰영역상의 위치에 투영행렬을 곱해놓은 상태에서의 위치
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.y * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vLightDir = vPosition - g_vLightPos;

	float	fLength = length(vLightDir);

	vector	vShade = max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f);

	float	fAtt = max((g_fRange - fLength) / g_fRange, 0.f);

	vShade = vShade * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	Out.vShade = vShade;
	Out.vShade *= fAtt;
	Out.vShade.a = 1.f;

	//Out.vShade = 0.0f;

	//vector vLook	=	vPosition - g_vCamPosition;
	//vector vReflect	=	reflect(normalize(vLightDir), vNormal);

	//float3 vReflect = reflect(vLightDir.xyz, vNormal);
	//float3 vLook = normalize(g_vViewDir.xyz);

	//specular = saturate(dot(vReflect, -vLook));

	//specular = specular * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	
	//float3 specular = vShade;//0.0f;
	//	
	//specular = pow(vShade, 2.0f);

	//vector vSpecular = tex2D(g_SpecularSampler, In.vTexUV);
	//specular *= vSpecular.xyz;
	//
	//Out.vSpecular = vector(specular.xyz, 0.0f);
	//Out.vSpecular *= fAtt;


	//===================================

	//vector vLightPos = g_vLightPos;
	//vLightPos = mul(vLightPos, g_matView);
	//vLightPos = mul(vLightPos, g_matProj);

	//float fLightDepth = vLightPos.z; // vLightPos.w;
	//float CurDepth = vDepth.y;

	//if (fLightDepth > fViewZ)
	//{
	
	if (vOriNormal.r > 0.11f && vOriNormal.g > 0.11f && vOriNormal.b > 0.11f)
	{
		vector vLook = vPosition - g_vCamPosition;
		vector vReflect = reflect(normalize(vLightDir), vNormal);

		Out.vSpecular = pow(max(dot(normalize(vLook) * -1.0f, vReflect), 0.0f), 20.0f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
		Out.vSpecular *= fAtt;
	}
		
	//}	
	//Out.vSpecular = 1.0f;

	return Out;
}

PS_OUT PS_LIGHT_ACC(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.0f;

	vector vLight[2];
	
	vLight[0] = tex2D(DirectionalSampler, In.vTexUV);
	vLight[1] = tex2D(PointSampler, In.vTexUV);

	Out.vShade = vLight[0] + vLight[1];

	vector vSpecular[2];

	vSpecular[0] = tex2D(Specular_Direc_Sampler, In.vTexUV);
	vSpecular[1] = tex2D(Specular_Point_Sampler, In.vTexUV);

	Out.vSpecular = vSpecular[0] + vSpecular[1];

	return Out;
}

PS_NORMAL_OUT PS_BLUR_X(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	vector vColor = 0.0f;

	for (int i = 0; i < 7; ++i)
	{
		/*vColor += tex2D(g_SpecularSampler,
					float2(In.vTexUV.x + gaussFilterX[i].x * texScaler + texOffset,
							In.vTexUV.y + gaussFilterX[i].y * texScaler + texOffset)) * gaussFilterX[i].w;*/
		vColor += tex2D(BaseSampler,
			float2(In.vTexUV.x + gaussFilterX[i].x * (1 / g_fBlurValue) + texOffset,
				In.vTexUV.y + gaussFilterX[i].y * (1 / g_fBlurValue) + texOffset)) * gaussFilterX[i].w;
	}

	Out.vShade = vColor * g_fBlurPower;

	return Out;
}

PS_NORMAL_OUT PS_BLUR_Y(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	vector vColor = 0.0f;

	for (int i = 0; i < 7; ++i)
	{
		//vColor += tex2D(g_SpecularSampler,
		//	float2(In.vTexUV.x + gaussFilterY[i].x * texScaler + texOffset,
		//			In.vTexUV.y + gaussFilterY[i].y * texScaler + texOffset)) * gaussFilterY[i].w;
		vColor += tex2D(BaseSampler,
			float2(In.vTexUV.x + gaussFilterY[i].x * (1 / g_fBlurValue) + texOffset,
				In.vTexUV.y + gaussFilterY[i].y * (1 / g_fBlurValue) + texOffset)) * gaussFilterY[i].w;
	}

	Out.vShade = vColor * g_fBlurPower;

	return Out;
}

PS_NORMAL_OUT PS_BLUR_XY(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	vector vColor = 0.0f;

	for (int i = 0; i < 7; ++i)
	{
		vColor += tex2D(BaseSampler,
			float2(In.vTexUV.x + gaussFilterXY[i].x * (1 / g_fBlurValue) + texOffset,
				In.vTexUV.y + gaussFilterXY[i].y * (1 / g_fBlurValue) + texOffset)) * gaussFilterXY[i].w;
	}

	Out.vShade = vColor * g_fBlurPower;

	return Out;
}

PS_NORMAL_OUT PS_BLUR_YX(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	vector vColor = 0.0f;

	for (int i = 0; i < 7; ++i)
	{
		vColor += tex2D(BaseSampler,
			float2(In.vTexUV.x + gaussFilterYX[i].x * (1 / g_fBlurValue) + texOffset,
				In.vTexUV.y + gaussFilterYX[i].y * (1 / g_fBlurValue) + texOffset)) * gaussFilterYX[i].w;
	}

	Out.vShade = vColor * g_fBlurPower;

	return Out;
}

PS_NORMAL_OUT PS_BLUR_X_DEPTH(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	vector vColor = 0.0f;

	float fCurDepth = tex2D(DepthSampler, In.vTexUV).y;
	//float fBlurDepth = tex2D(BaseSampler, In.vTexUV).a;
	if (fCurDepth == 0.0f)
	{
		for (int i = 0; i < 7; ++i)
		{
			vColor += tex2D(BaseSampler,
				float2(In.vTexUV.x + gaussFilterX[i].x * (1 / g_fBlurValue) + texOffset,
					In.vTexUV.y + gaussFilterX[i].y * (1 / g_fBlurValue) + texOffset)) * gaussFilterX[i].w;
		}
	}	

	Out.vShade = vColor * g_fBlurPower;

	return Out;
}

PS_NORMAL_OUT PS_BLUR_Y_DEPTH(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	vector vColor = 0.0f;
	
	float fCurDepth = tex2D(DepthSampler, In.vTexUV).y;
	float fBlurDepth = tex2D(BaseSampler, In.vTexUV).a;
	if (fCurDepth == 0.0f)
	{
		for (int i = 0; i < 7; ++i)
		{
			vColor += tex2D(BaseSampler,
				float2(In.vTexUV.x + gaussFilterY[i].x * (1 / g_fBlurValue) + texOffset,
					In.vTexUV.y + gaussFilterY[i].y * (1 / g_fBlurValue) + texOffset)) * gaussFilterY[i].w;
		}
	}

	Out.vShade = vColor * g_fBlurPower;

	return Out;
}

PS_NORMAL_OUT PS_BLUR_XY_DEPTH(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	vector vColor = 0.0f;

	float fBlurDepth = tex2D(BaseSampler, In.vTexUV).a;
	if (fBlurDepth != 0.0f)
	{
		for (int i = 0; i < 7; ++i)
		{
			vColor += tex2D(BaseSampler,
				float2(In.vTexUV.x + gaussFilterXY[i].x * (1 / g_fBlurValue) + texOffset,
					In.vTexUV.y + gaussFilterXY[i].y * (1 / g_fBlurValue) + texOffset)) * gaussFilterXY[i].w;
		}
	}

	Out.vShade = vColor * g_fBlurPower;

	return Out;
}

PS_NORMAL_OUT PS_BLUR_YX_DEPTH(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	vector vColor = 0.0f;

	float fBlurDepth = tex2D(BaseSampler, In.vTexUV).a;
	if (fBlurDepth != 0.0f)
	{
		for (int i = 0; i < 7; ++i)
		{
			vColor += tex2D(BaseSampler,
				float2(In.vTexUV.x + gaussFilterYX[i].x * (1 / g_fBlurValue) + texOffset,
					In.vTexUV.y + gaussFilterYX[i].y * (1 / g_fBlurValue) + texOffset)) * gaussFilterYX[i].w;
		}
	}

	Out.vShade = vColor * g_fBlurPower;

	return Out;
}

PS_NORMAL_OUT PS_BLUR(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	/*vector vBlend = pow(abs(tex2D(BaseSampler, In.vTexUV)), 2.2f);
	float fLum = dot(vBlend.rgb, Luminance);

	if(fLum >= 0.1f)
		Out.vShade = tex2D(BaseSampler, In.vTexUV);*/
	Out.vShade = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

PS_NORMAL_OUT PS_POSTEFFECT(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	//float fCurDepth = tex2D(DepthSampler, In.vTexUV).y;
	//float fEffectDepth = tex2D(BlurDepth_All_Sampler, In.vTexUV).x;

	vector vColor[2];

	vColor[0] = tex2D(BaseSampler, In.vTexUV);
	vColor[1] = tex2D(Blur_Sampler, In.vTexUV);
	
	Out.vShade = vColor[0] + vColor[1];

	return Out;
}

PS_NORMAL_OUT PS_BLUR_DEPTH(PS_IN In)
{
	PS_NORMAL_OUT	Out = (PS_NORMAL_OUT)0.0f;

	Out.vShade = 1.0f;
	Out.vShade.x = tex2D(BlurDepth_Sampler, In.vTexUV).x;

	return Out;
}

PS_OUT PS_ALL_SPECULAR(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.0f;

	Out.vShade = 0.0f;
	Out.vSpecular = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

technique Default_Device
{
	//0
	pass LIGHT_DIRECTIONAL
	{
		ZEnable				= FALSE;
		ZWriteEnable		= FALSE;

		/*AlphaBlendEnable	= TRUE;
		srcblend			= one;
		destblend			= one;*/

		VertexShader		= NULL;
		PixelShader			= compile ps_3_0 PS_DIRECTIONAL();
	}

	//1
	pass LIGHT_NO_SHADOW_DIRECTIONAL
	{
		ZEnable				= FALSE;
		ZWriteEnable		= FALSE;

		VertexShader		= NULL;
		PixelShader			= compile ps_3_0 PS_NO_SHADOW_DIRECTIONAL();
	}

	//2
	pass LIGHT_POINT
	{
		ZEnable				= FALSE;
		ZWriteEnable		= FALSE;

		AlphaBlendEnable	= TRUE;
		srcblend			= one;
		destblend			= one;

		VertexShader		= NULL;
		PixelShader			= compile ps_3_0 PS_POINT();
	}

	//3
	pass LIGHT_ACC
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		AlphaBlendEnable = TRUE;
		srcblend = one;
		destblend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_LIGHT_ACC();
	}

	//4
	pass LIGHT_BLUR_X
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR_X();
	}

	//5
	pass LIGHT_BLUR_Y
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR_Y();
	}

	//6
	pass LIGHT_BLUR
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		AlphaBlendEnable = TRUE;
		srcblend = one;
		destblend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR();
	}

	//7
	pass LIGHT_POSTEFFECT
	{
		CULLMODE = NONE;
		ALPHABLENDENABLE = TRUE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_POSTEFFECT();
	}

	//8
	pass LIGHT_BLUR_XY
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;
		
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR_XY();
	}

	//9
	pass LIGHT_BLUR_YX
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR_YX();
	}

	//10
	pass LIGHT_BLUR_X_DEPTH
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR_X_DEPTH();
	}

	//11
	pass LIGHT_BLUR_Y_DEPTH
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR_Y_DEPTH();
	}

	//12
	pass LIGHT_BLUR_XY_DEPTH
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR_XY_DEPTH();
	}

	//13
	pass LIGHT_BLUR_YX_DEPTH
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR_YX_DEPTH();
	}

	//14
	pass LIGHT_BLUR_TOTAL
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_BLUR_DEPTH();
	}

	//15
	pass LIGHT_BLUR
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		AlphaBlendEnable = TRUE;
		srcblend = one;
		destblend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_ALL_SPECULAR();
	}
};