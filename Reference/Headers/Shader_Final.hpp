#include "Shader_Base.hpp"

texture				g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

texture				g_DecalTexture;
sampler DecalSampler = sampler_state
{
	texture = g_DecalTexture;
	minfilter = linear;
	magfilter = linear;
};

texture				g_OutlineTexture;
sampler OutlineSampler = sampler_state
{
	texture = g_OutlineTexture;
	minfilter = linear;
	magfilter = linear;
};

struct PS_IN
{
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : COLOR0;
	vector vCopy : COLOR1;
};

vector g_vFog = vector(0.275f, 185.0f, 0.0f, 0.0f); // 1.농도, 2. End값
vector g_vHeightFog = vector(0.3f, 1000.0f, -10.0f, 0.0f); // 1.농도, 2.안개End거리, 3.안개높이

vector g_vColorFog = vector(0.3f, 0.3f, 0.3f, 0.0f);

float	g_fCamFar;
vector	g_vCamPosition;

matrix	g_matViewInv;
matrix	g_matProjInv;

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor += tex2D(DecalSampler, In.vTexUV);

	vector vOutline = tex2D(OutlineSampler, In.vTexUV);
	Out.vColor += vOutline;

	Out.vCopy = Out.vColor;

	return Out;
}

PS_OUT PS_FOG_EXP(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vBase = tex2D(BaseSampler, In.vTexUV);

	float fViewDepth = tex2D(DepthSampler, In.vTexUV).x;
	fViewDepth *= g_fCamFar;

	float fDist = fViewDepth / g_vFog.y * 5.0f;

	float f = exp(-fDist * g_vFog.x);

	Out.vColor = lerp(g_vColorFog, vBase, f);

	vector vOutline = tex2D(OutlineSampler, In.vTexUV);
	Out.vColor += vOutline;

	Out.vCopy = Out.vColor;

	return Out;
}

PS_OUT PS_FOG_SQURE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vBase = tex2D(BaseSampler, In.vTexUV);

	float fViewDepth = tex2D(DepthSampler, In.vTexUV).x;
	fViewDepth *= g_fCamFar;

	float fDist = fViewDepth / g_vFog.y * 5.0f;

	float f = exp(-(fDist * g_vFog.x) * (fDist * g_vFog.x));

	Out.vColor = lerp(g_vColorFog, vBase, f);

	vector vOutline = tex2D(OutlineSampler, In.vTexUV);
	Out.vColor += vOutline;

	Out.vCopy = Out.vColor;

	return Out;
}

PS_OUT PS_FOG_HIGH(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	float fFogEnd = g_vHeightFog.y;
	float fFogTop = g_vHeightFog.z;
	float fFogRange = 20.0f;

	vector vBase = tex2D(BaseSampler, In.vTexUV);

	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float fViewDepth = vDepth.x * g_fCamFar;

	vector vPosition = 0.0f;

	////뷰 영역상 위치에서 투영행렬을 곱해놓은 상태에서의 위치.
	vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewDepth;
	vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewDepth;
	vPosition.z = vDepth.y * fViewDepth;
	vPosition.w = fViewDepth;

	vector vViewPos, vWorldPos;

	vPosition = mul(vPosition, g_matProjInv);	//뷰 영역상 위치.
	vWorldPos = mul(vPosition, g_matViewInv);	//월드 영역상 위치.

	vViewPos = g_vCamPosition;

	vector vViewProj  = vViewPos;
	vViewProj.y = 0.0f;
	vector vWorldProj = vWorldPos;
	vWorldProj.y = 0.0f;

	//x-z평면에서의 거리 계산을 비례
	float fDeltaD = distance(vViewProj, vWorldProj) / fFogEnd * 2.0f;

	//높이 기반 계산
	float fDeltaY, fDensityIntegral;
	if (vViewPos.y > fFogTop)	//카메라 상단이 안개상단보다 높을 떄.
	{
		if (vWorldPos.y < fFogTop)	//월드 정점이 안개상단 보다 낮으면
		{
			fDeltaY = (fFogTop - vWorldPos.y) / fFogRange * 2.0f;
			fDensityIntegral = (fDeltaY * fDeltaY * g_vHeightFog.x);
		}
		else //월드 정점이 안개 상단보다 높거나 같으면 안개가 없는 것이므로 0으로 해준다.
		{
			fDeltaY = 0.0f;
			fDensityIntegral = 0.0f;
		}
	}
	else //카메라 상단이 안개 상단보다 낮을 때.
	{
		if (vWorldPos.y < fFogTop)	//월드 정점이 안개상단 보다 낮으면
		{
			float fDeltaA = (fFogTop - vViewPos.y) / fFogRange * 2.0f;
			float fDeltaB = (fFogTop - vWorldPos.y) / fFogRange * 2.0f;
			fDeltaY = abs(fDeltaA - fDeltaB);
			fDensityIntegral = abs((fDeltaA * fDeltaA * g_vHeightFog.x) - (fDeltaB * fDeltaB * g_vHeightFog.x));
		}
		else
		{
			fDeltaY = abs(fFogTop - vViewPos.y) / fFogRange * 2.0f;
			fDensityIntegral = abs(fDeltaY * fDeltaY * g_vHeightFog.x);
		}
	}
	
	float fDensity;

	if (fDeltaY != 0.0f) //0이 아닐 떄
	{
		fDensity = sqrt((1.0f + ((fDeltaD / fDeltaY) * (fDeltaD / fDeltaY)))) * fDensityIntegral;
	}
	else
	{
		fDensity = 0.0f;
	}

	float f = exp(-fDensity);

	Out.vColor = lerp(g_vColorFog, vBase, f);

	vector vOutline = tex2D(OutlineSampler, In.vTexUV);
	Out.vColor += vOutline;

	Out.vCopy = Out.vColor;

	return Out;
}

PS_OUT PS_FOG_EXP_HIGH(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	float fFogEnd = g_vHeightFog.y;
	float fFogTop = g_vHeightFog.z;
	float fFogRange = 20.0f;

	vector vBase = tex2D(BaseSampler, In.vTexUV);

	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float fViewDepth = vDepth.x * g_fCamFar;

	vector vPosition = 0.0f;

	////뷰 영역상 위치에서 투영행렬을 곱해놓은 상태에서의 위치.
	vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewDepth;
	vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewDepth;
	vPosition.z = vDepth.y * fViewDepth;
	vPosition.w = fViewDepth;

	vector vViewPos, vWorldPos;

	vPosition = mul(vPosition, g_matProjInv);	//뷰 영역상 위치.
	vWorldPos = mul(vPosition, g_matViewInv);	//월드 영역상 위치.

	vViewPos = g_vCamPosition;

	vector vViewProj = vViewPos;
	vViewProj.y = 0.0f;
	vector vWorldProj = vWorldPos;
	vWorldProj.y = 0.0f;

	//x-z평면에서의 거리 계산을 비례
	float fDeltaD = distance(vViewProj, vWorldProj) / fFogEnd * 2.0f;

	//높이 기반 계산
	float fDeltaY, fDensityIntegral;
	if (vViewPos.y > fFogTop)	//카메라 상단이 안개상단보다 높을 떄.
	{
		if (vWorldPos.y < fFogTop)	//월드 정점이 안개상단 보다 낮으면
		{
			fDeltaY = (fFogTop - vWorldPos.y) / fFogRange * 2.0f;
			fDensityIntegral = (fDeltaY * fDeltaY * g_vHeightFog.x);
		}
		else //월드 정점이 안개 상단보다 높거나 같으면 안개가 없는 것이므로 0으로 해준다.
		{
			fDeltaY = 0.0f;
			fDensityIntegral = 0.0f;
		}
	}
	else //카메라 상단이 안개 상단보다 낮을 때.
	{
		if (vWorldPos.y < fFogTop)	//월드 정점이 안개상단 보다 낮으면
		{
			float fDeltaA = (fFogTop - vViewPos.y) / fFogRange * 2.0f;
			float fDeltaB = (fFogTop - vWorldPos.y) / fFogRange * 2.0f;
			fDeltaY = abs(fDeltaA - fDeltaB);
			fDensityIntegral = abs((fDeltaA * fDeltaA * g_vHeightFog.x) - (fDeltaB * fDeltaB * g_vHeightFog.x));
		}
		else
		{
			fDeltaY = abs(fFogTop - vViewPos.y) / fFogRange * 2.0f;
			fDensityIntegral = abs(fDeltaY * fDeltaY * g_vHeightFog.x);
		}
	}

	float fDensity;

	if (fDeltaY != 0.0f) //0이 아닐 떄
	{
		fDensity = sqrt((1.0f + ((fDeltaD / fDeltaY) * (fDeltaD / fDeltaY)))) * fDensityIntegral;
	}
	else
	{
		fDensity = 0.0f;
	}

	float f_H = exp(-fDensity);	//높이

	float fDist = fViewDepth / g_vFog.y * 5.0f;

	float f_E = exp(-fDist * g_vFog.x);	//지수

	float f = f_H + f_E;

	Out.vColor = lerp(g_vColorFog, vBase, f);

	return Out;
}

PS_OUT PS_FOG_SQURE_HIGH(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	float fFogEnd = g_vHeightFog.y;
	float fFogTop = g_vHeightFog.z;
	float fFogRange = 20.0f;

	vector vBase = tex2D(BaseSampler, In.vTexUV);

	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float fViewDepth = vDepth.x * g_fCamFar;

	vector vPosition = 0.0f;

	////뷰 영역상 위치에서 투영행렬을 곱해놓은 상태에서의 위치.
	vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewDepth;
	vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewDepth;
	vPosition.z = vDepth.y * fViewDepth;
	vPosition.w = fViewDepth;

	vector vViewPos, vWorldPos;

	vPosition = mul(vPosition, g_matProjInv);	//뷰 영역상 위치.
	vWorldPos = mul(vPosition, g_matViewInv);	//월드 영역상 위치.

	vViewPos = g_vCamPosition;

	vector vViewProj = vViewPos;
	vViewProj.y = 0.0f;
	vector vWorldProj = vWorldPos;
	vWorldProj.y = 0.0f;

	//x-z평면에서의 거리 계산을 비례
	float fDeltaD = distance(vViewProj, vWorldProj) / fFogEnd * 2.0f;

	//높이 기반 계산
	float fDeltaY, fDensityIntegral;
	if (vViewPos.y > fFogTop)	//카메라 상단이 안개상단보다 높을 떄.
	{
		if (vWorldPos.y < fFogTop)	//월드 정점이 안개상단 보다 낮으면
		{
			fDeltaY = (fFogTop - vWorldPos.y) / fFogRange * 2.0f;
			fDensityIntegral = (fDeltaY * fDeltaY * g_vHeightFog.x);
		}
		else //월드 정점이 안개 상단보다 높거나 같으면 안개가 없는 것이므로 0으로 해준다.
		{
			fDeltaY = 0.0f;
			fDensityIntegral = 0.0f;
		}
	}
	else //카메라 상단이 안개 상단보다 낮을 때.
	{
		if (vWorldPos.y < fFogTop)	//월드 정점이 안개상단 보다 낮으면
		{
			float fDeltaA = (fFogTop - vViewPos.y) / fFogRange * 2.0f;
			float fDeltaB = (fFogTop - vWorldPos.y) / fFogRange * 2.0f;
			fDeltaY = abs(fDeltaA - fDeltaB);
			fDensityIntegral = abs((fDeltaA * fDeltaA * g_vHeightFog.x) - (fDeltaB * fDeltaB * g_vHeightFog.x));
		}
		else
		{
			fDeltaY = abs(fFogTop - vViewPos.y) / fFogRange * 2.0f;
			fDensityIntegral = abs(fDeltaY * fDeltaY * g_vHeightFog.x);
		}
	}

	float fDensity;

	if (fDeltaY != 0.0f) //0이 아닐 떄
	{
		fDensity = sqrt((1.0f + ((fDeltaD / fDeltaY) * (fDeltaD / fDeltaY)))) * fDensityIntegral;
	}
	else
	{
		fDensity = 0.0f;
	}

	float f_H = exp(-fDensity);	//높이

	float fDist = fViewDepth / g_vFog.y * 5.0f;

	float f_S = exp(-(fDist * g_vFog.x) * (fDist * g_vFog.x));	//지수제곱

	float f = f_H + f_S;

	Out.vColor = lerp(g_vColorFog, vBase, f);

	vector vOutline = tex2D(OutlineSampler, In.vTexUV);
	Out.vColor += vOutline;

	Out.vCopy = Out.vColor;

	return Out;
}

technique Default_Device
{
	//0
	pass Final
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	//1
	pass Final_Fog_Exp
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_FOG_EXP();
	}

		//2
	pass Final_Fog_Squre
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_FOG_SQURE();
	}

		//3
	pass Final_Fog_High
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_FOG_HIGH();
	}

		//4
	pass Final_Fog_Exp_High
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_FOG_EXP_HIGH();
	}

		//5
	pass Final_Fog_Squre_High
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_FOG_SQURE_HIGH();
	}
}