#include "Shader_Base.hpp"
float				g_fFar;

Matrix				g_matViewInv;
Matrix				g_matProjInv;

Matrix				g_matLightView;
Matrix				g_matLightProj;

matrix				g_matLightProj_1;
matrix				g_matLightProj_2;

texture				g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture				g_StageMap_DepthTexture;
sampler StageMap_DepthSampler = sampler_state
{
	texture = g_StageMap_DepthTexture;
};

texture				g_ShadowTexture;
sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;

	minfilter = linear;
	magfilter = linear;
	//mipfilter	= lin ear;

	//AddressU	= Border;
	//AddressV	= Border;
	//BorderColor = 0xFFFFFFFF;
};

texture				g_CACD_Shadow_0_Texture;
sampler CACD_Shadow_0_wSampler = sampler_state
{
	texture = g_CACD_Shadow_0_Texture;

minfilter = linear;
magfilter = linear;
};

texture				g_CACD_Shadow_1_Texture;
sampler CACD_Shadow_1_wSampler = sampler_state
{
	texture = g_CACD_Shadow_1_Texture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_CACD_Shadow_2_Texture;
sampler CACD_Shadow_2_wSampler = sampler_state
{
	texture = g_CACD_Shadow_2_Texture;

	minfilter = linear;
	magfilter = linear;
};

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

struct PS_IN
{
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vShadow : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.0f;

	vector	vDepth = tex2D(DepthSampler, In.vTexUV);

	float	fViewZ = vDepth.x * g_fFar;

	vector vPosition = 0.0f;

	////뷰 영역상 위치에서 투영행렬을 곱해놓은 상태에서의 위치.
	vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewZ;
	vPosition.z = vDepth.y * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);	//뷰 영역상 위치.
	vPosition = mul(vPosition, g_matViewInv);	//월드 영역상 위치.
	
	//그림자.
	vector vLightPos = 0.0f;
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
		if (CurDepth > fShadowDepth + 0.008f)
		{
			Out.vShadow.rgb *= 0.4f;
		}
	}

	return Out;
}

PS_OUT PS_STAGEMAP(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.0f;

	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	vector	vStageMapDepth = tex2D(StageMap_DepthSampler, In.vTexUV);
	
	Out.vShadow = 1.0f;

	bool bShadowDraw;
	
	if (vDepth.y < vStageMapDepth.y)
		bShadowDraw = false;
	else
		bShadowDraw = true;

	if (bShadowDraw)
	{
		float	fViewZ = vStageMapDepth.x * g_fFar;

		vector vPosition = 0.0f;

		////뷰 영역상 위치에서 투영행렬을 곱해놓은 상태에서의 위치.
		vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewZ;
		vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewZ;
		vPosition.z = vStageMapDepth.y * fViewZ;
		vPosition.w = fViewZ;

		vPosition = mul(vPosition, g_matProjInv);	//뷰 영역상 위치.
		vPosition = mul(vPosition, g_matViewInv);	//월드 영역상 위치.

													//그림자.
		vector vLightPos = 0.0f;
		vLightPos = mul(vPosition, g_matLightView);
		vLightPos = mul(vLightPos, g_matLightProj);

		float CurDepth = vLightPos.z / vLightPos.w;

		float2 uv = vLightPos.xy / vLightPos.w;
		uv.y = -uv.y;
		uv = uv * 0.5f + 0.5f;

		float fShadowDepth = tex2D(ShadowSampler, uv).r;

		if (uv.x >= 0.0f && uv.x <= 1.0f && uv.y >= 0.0f && uv.y <= 1.0f)
		{
			if (CurDepth > fShadowDepth + 0.005f)
			{
				Out.vShadow.rgb *= 0.4f;
			}
		}
	}

	return Out;
}

PS_OUT PS_STAGEMAP_SOFT(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.0f;

	vector vColor = 0.0f;

	for (int i = 0; i < 7; ++i)
	{
		vColor += tex2D(BaseSampler,
			float2(In.vTexUV.x + gaussFilterX[i].x * (1.0f / 128.0f) + 0.0f,
				In.vTexUV.y + gaussFilterX[i].y * (1.0f / 128.0f) + 0.0f)) * gaussFilterX[i].w;
	}

	Out.vShadow = vColor;

	return Out;
}

PS_OUT PS_STAGEMAP_CACD(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	vector	vStageMapDepth = tex2D(StageMap_DepthSampler, In.vTexUV);

	Out.vShadow = 1.0f;

	bool bShadowDraw;

	if (vDepth.y < vStageMapDepth.y)
		bShadowDraw = false;
	else
		bShadowDraw = true;

	if (bShadowDraw)
	{
		float	fViewZ = vStageMapDepth.x * g_fFar;

		vector vPosition = 0.0f;

		////뷰 영역상 위치에서 투영행렬을 곱해놓은 상태에서의 위치.
		vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewZ;
		vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewZ;
		vPosition.z = vStageMapDepth.y * fViewZ;
		vPosition.w = fViewZ;

		vPosition = mul(vPosition, g_matProjInv);	//뷰 영역상 위치.
		vPosition = mul(vPosition, g_matViewInv);	//월드 영역상 위치.

		//그림자.
		vector vViewPos = mul(vPosition, g_matLightView);
		
		vector vLight_0_Pos = mul(vViewPos, g_matLightProj);
		vector vLight_1_Pos = mul(vViewPos, g_matLightProj_1);
		vector vLight_2_Pos = mul(vViewPos, g_matLightProj_2);

		float CurDepth_0 = vLight_0_Pos.z / vLight_0_Pos.w;	//가장 가까움
		float CurDepth_1 = vLight_1_Pos.z / vLight_1_Pos.w;	//두번째
		float CurDepth_2 = vLight_2_Pos.z / vLight_2_Pos.w;	//새번쩨

		float2 First_uv = vLight_0_Pos.xy / vLight_0_Pos.w;
		First_uv.y = -First_uv.y;
		First_uv = First_uv * 0.5f + 0.5f;

		float2 Second_uv = vLight_1_Pos.xy / vLight_1_Pos.w;
		Second_uv.y = -Second_uv.y;
		Second_uv = Second_uv * 0.5f + 0.5f;

		float2 Third_uv = vLight_2_Pos.xy / vLight_2_Pos.w;
		Third_uv.y = -Third_uv.y;
		Third_uv = Third_uv * 0.5f + 0.5f;

		float fShadow_0_Depth = tex2D(CACD_Shadow_0_wSampler, First_uv).r;
		float fShadow_1_Depth = tex2D(CACD_Shadow_1_wSampler, Second_uv).r;
		float fShadow_2_Depth = tex2D(CACD_Shadow_2_wSampler, Third_uv).r;

		if (First_uv.x >= 0.0f && First_uv.x <= 1.0f && First_uv.y >= 0.0f && First_uv.y <= 1.0f)
		{
			if (CurDepth_0 > fShadow_0_Depth + 0.005f)
			{
				Out.vShadow.rgb *= 0.4f;
			}
		}
		else if (Second_uv.x >= 0.0f && Second_uv.x <= 1.0f && Second_uv.y >= 0.0f && Second_uv.y <= 1.0f)
		{
			if (CurDepth_1 > fShadow_1_Depth + 0.005f)
			{
				Out.vShadow.rgb *= 0.4f;
			}
		}
		else if (Third_uv.x >= 0.0f && Third_uv.x <= 1.0f && Third_uv.y >= 0.0f && Third_uv.y <= 1.0f)
		{
			if (CurDepth_2 > fShadow_2_Depth + 0.005f)
			{
				Out.vShadow.rgb *= 0.4f;
			}
		}
	}

	return Out;
}

technique Default_Device
{
	pass SHADOW
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass STAGEMAP_SHADOW
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_STAGEMAP();
	}

	pass STAGEMAP_SOFT_SHADOW
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_STAGEMAP_SOFT();
	}

	pass STAGEMAP_CACD_SHADOW
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_STAGEMAP_CACD();
	}
}