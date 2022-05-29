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

vector g_vFog = vector(0.275f, 185.0f, 0.0f, 0.0f); // 1.��, 2. End��
vector g_vHeightFog = vector(0.3f, 1000.0f, -10.0f, 0.0f); // 1.��, 2.�Ȱ�End�Ÿ�, 3.�Ȱ�����

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

	////�� ������ ��ġ���� ��������� ���س��� ���¿����� ��ġ.
	vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewDepth;
	vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewDepth;
	vPosition.z = vDepth.y * fViewDepth;
	vPosition.w = fViewDepth;

	vector vViewPos, vWorldPos;

	vPosition = mul(vPosition, g_matProjInv);	//�� ������ ��ġ.
	vWorldPos = mul(vPosition, g_matViewInv);	//���� ������ ��ġ.

	vViewPos = g_vCamPosition;

	vector vViewProj  = vViewPos;
	vViewProj.y = 0.0f;
	vector vWorldProj = vWorldPos;
	vWorldProj.y = 0.0f;

	//x-z��鿡���� �Ÿ� ����� ���
	float fDeltaD = distance(vViewProj, vWorldProj) / fFogEnd * 2.0f;

	//���� ��� ���
	float fDeltaY, fDensityIntegral;
	if (vViewPos.y > fFogTop)	//ī�޶� ����� �Ȱ���ܺ��� ���� ��.
	{
		if (vWorldPos.y < fFogTop)	//���� ������ �Ȱ���� ���� ������
		{
			fDeltaY = (fFogTop - vWorldPos.y) / fFogRange * 2.0f;
			fDensityIntegral = (fDeltaY * fDeltaY * g_vHeightFog.x);
		}
		else //���� ������ �Ȱ� ��ܺ��� ���ų� ������ �Ȱ��� ���� ���̹Ƿ� 0���� ���ش�.
		{
			fDeltaY = 0.0f;
			fDensityIntegral = 0.0f;
		}
	}
	else //ī�޶� ����� �Ȱ� ��ܺ��� ���� ��.
	{
		if (vWorldPos.y < fFogTop)	//���� ������ �Ȱ���� ���� ������
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

	if (fDeltaY != 0.0f) //0�� �ƴ� ��
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

	////�� ������ ��ġ���� ��������� ���س��� ���¿����� ��ġ.
	vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewDepth;
	vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewDepth;
	vPosition.z = vDepth.y * fViewDepth;
	vPosition.w = fViewDepth;

	vector vViewPos, vWorldPos;

	vPosition = mul(vPosition, g_matProjInv);	//�� ������ ��ġ.
	vWorldPos = mul(vPosition, g_matViewInv);	//���� ������ ��ġ.

	vViewPos = g_vCamPosition;

	vector vViewProj = vViewPos;
	vViewProj.y = 0.0f;
	vector vWorldProj = vWorldPos;
	vWorldProj.y = 0.0f;

	//x-z��鿡���� �Ÿ� ����� ���
	float fDeltaD = distance(vViewProj, vWorldProj) / fFogEnd * 2.0f;

	//���� ��� ���
	float fDeltaY, fDensityIntegral;
	if (vViewPos.y > fFogTop)	//ī�޶� ����� �Ȱ���ܺ��� ���� ��.
	{
		if (vWorldPos.y < fFogTop)	//���� ������ �Ȱ���� ���� ������
		{
			fDeltaY = (fFogTop - vWorldPos.y) / fFogRange * 2.0f;
			fDensityIntegral = (fDeltaY * fDeltaY * g_vHeightFog.x);
		}
		else //���� ������ �Ȱ� ��ܺ��� ���ų� ������ �Ȱ��� ���� ���̹Ƿ� 0���� ���ش�.
		{
			fDeltaY = 0.0f;
			fDensityIntegral = 0.0f;
		}
	}
	else //ī�޶� ����� �Ȱ� ��ܺ��� ���� ��.
	{
		if (vWorldPos.y < fFogTop)	//���� ������ �Ȱ���� ���� ������
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

	if (fDeltaY != 0.0f) //0�� �ƴ� ��
	{
		fDensity = sqrt((1.0f + ((fDeltaD / fDeltaY) * (fDeltaD / fDeltaY)))) * fDensityIntegral;
	}
	else
	{
		fDensity = 0.0f;
	}

	float f_H = exp(-fDensity);	//����

	float fDist = fViewDepth / g_vFog.y * 5.0f;

	float f_E = exp(-fDist * g_vFog.x);	//����

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

	////�� ������ ��ġ���� ��������� ���س��� ���¿����� ��ġ.
	vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewDepth;
	vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewDepth;
	vPosition.z = vDepth.y * fViewDepth;
	vPosition.w = fViewDepth;

	vector vViewPos, vWorldPos;

	vPosition = mul(vPosition, g_matProjInv);	//�� ������ ��ġ.
	vWorldPos = mul(vPosition, g_matViewInv);	//���� ������ ��ġ.

	vViewPos = g_vCamPosition;

	vector vViewProj = vViewPos;
	vViewProj.y = 0.0f;
	vector vWorldProj = vWorldPos;
	vWorldProj.y = 0.0f;

	//x-z��鿡���� �Ÿ� ����� ���
	float fDeltaD = distance(vViewProj, vWorldProj) / fFogEnd * 2.0f;

	//���� ��� ���
	float fDeltaY, fDensityIntegral;
	if (vViewPos.y > fFogTop)	//ī�޶� ����� �Ȱ���ܺ��� ���� ��.
	{
		if (vWorldPos.y < fFogTop)	//���� ������ �Ȱ���� ���� ������
		{
			fDeltaY = (fFogTop - vWorldPos.y) / fFogRange * 2.0f;
			fDensityIntegral = (fDeltaY * fDeltaY * g_vHeightFog.x);
		}
		else //���� ������ �Ȱ� ��ܺ��� ���ų� ������ �Ȱ��� ���� ���̹Ƿ� 0���� ���ش�.
		{
			fDeltaY = 0.0f;
			fDensityIntegral = 0.0f;
		}
	}
	else //ī�޶� ����� �Ȱ� ��ܺ��� ���� ��.
	{
		if (vWorldPos.y < fFogTop)	//���� ������ �Ȱ���� ���� ������
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

	if (fDeltaY != 0.0f) //0�� �ƴ� ��
	{
		fDensity = sqrt((1.0f + ((fDeltaD / fDeltaY) * (fDeltaD / fDeltaY)))) * fDensityIntegral;
	}
	else
	{
		fDensity = 0.0f;
	}

	float f_H = exp(-fDensity);	//����

	float fDist = fViewDepth / g_vFog.y * 5.0f;

	float f_S = exp(-(fDist * g_vFog.x) * (fDist * g_vFog.x));	//��������

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