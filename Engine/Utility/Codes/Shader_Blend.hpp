bool			m_bGaussianBlur = false;
bool			m_bHDR = false;
bool			g_bSSAO = false;

texture				g_AlbedoTexture;
sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;

minfilter = linear;
magfilter = linear;
};

texture				g_ShadeTexture;
sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_SpecularTexture;
sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;

minfilter = linear;
magfilter = linear;
};

texture				g_ShadowTexture;
sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;

minfilter = linear;
magfilter = linear;
};

texture				g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;

minfilter = linear;
magfilter = linear;
};

texture				g_EffectTexture;
sampler EffectSampler = sampler_state
{
	texture = g_EffectTexture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_CharTexture;
sampler CharSampler = sampler_state
{
	texture = g_CharTexture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_MotionBlurTexture;
sampler MotionBlurSampler = sampler_state
{
	texture = g_MotionBlurTexture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_BlurTexture;
sampler BlurSampler = sampler_state
{
	texture = g_BlurTexture;

	minfilter = linear;
	magfilter = linear;
};

texture				g_SSAOTexture;
sampler SSAOSampler = sampler_state
{
	texture = g_SSAOTexture;

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
	//vector vCopy : COLOR1;
	//vector vHDR : COLOR2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0.0f;

	vector			vBlend;

	vector			vColor[2];

	vColor[0] = tex2D(AlbedoSampler, In.vTexUV);
	vColor[1] = tex2D(ShadeSampler, In.vTexUV);

	vBlend = vColor[0] * vColor[1];

	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);

	vBlend = vBlend + vSpecular;

	if (m_bGaussianBlur)
	{
		vector vBlur = tex2D(BlurSampler, In.vTexUV);
		vBlend = (vBlend + vBlur) / 2.0f;
	}

	float fShadow = tex2D(ShadowSampler, In.vTexUV).x;

	if (g_bSSAO)
	{
		float fSSAO = tex2D(SSAOSampler, In.vTexUV).x;
		vBlend.rgb *= fSSAO;
	}

	vBlend.rgb *= fShadow;

	vector vEffect = tex2D(EffectSampler, In.vTexUV);

	vBlend += vEffect;

	float fDepth = tex2D(DepthSampler, In.vTexUV).y;
	/*if(fDepth >= 0.98f)
	{
	Out.vColor.a = 0.0f;
	}
	else
	{
	Out.vColor.a = 1.0f;
	}*/

	float fAlpha = min(abs(fDepth * 25.0f - 25.0f), 1.0f);

	vBlend.a = fAlpha;
		
	Out.vColor = vBlend;	
	//Out.vCopy = vBlend;
	//Out.vHDR = 0.0f;

	return Out;
}

PS_OUT PS_NO_SHADOW(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0.0f;

	vector			vBlend;
	
	vector			vColor[2];

	vColor[0] = tex2D(AlbedoSampler, In.vTexUV);
	vColor[1] = tex2D(ShadeSampler, In.vTexUV);

	vBlend = vColor[0] * vColor[1];

	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);

	vBlend = vBlend + vSpecular;

	if (m_bGaussianBlur)
	{
		vector vBlur = tex2D(BlurSampler, In.vTexUV);
		vBlend = (vBlend + vBlur) / 2.0f;
	}

	if (g_bSSAO)
	{
		float fSSAO = tex2D(SSAOSampler, In.vTexUV).x;
		vBlend.rgb *= fSSAO;
	}
	
	vector vEffect = tex2D(EffectSampler, In.vTexUV);

	vBlend += vEffect;

	float fDepth = tex2D(DepthSampler, In.vTexUV).y;

	float fAlpha = min(abs(fDepth * 25.0f - 25.0f), 1.0f);

	vBlend.a = fAlpha;

	Out.vColor = vBlend;
	//Out.vCopy = vBlend;
	//Out.vHDR = 0.0f;

	return Out;
}

PS_OUT PS_CHAR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vShade = tex2D(ShadeSampler, In.vTexUV);
	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector vChar = pow(abs(tex2D(CharSampler, In.vTexUV)), 2.2f);

	if (vChar.r != 0.0f && vChar.g != 0.0f && vChar.b != 0.0f)
	{
		Out.vColor = vChar * vShade + vSpecular;
		Out.vColor.a = 1.0f;

		Out.vColor = pow(abs(Out.vColor), (1.0f / 2.2f));
	}

	return Out;
}

PS_OUT PS_MOTION_BLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0; 

	float fAlpha = tex2D(CharSampler, In.vTexUV).a;

	if (fAlpha != 0.0f)
		Out.vColor = 0.0f;
	else
		Out.vColor = tex2D(MotionBlurSampler, In.vTexUV);

	//if (vMotionBlur.r <= 0.15f && vMotionBlur.g <= 0.15f && vMotionBlur.b <= 0.15f)
	//{
	//	vMotionBlur.a = 0.0f;
	//}

	return Out; 
}

PS_OUT PS_HDR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector			vBlend;

	vector			vColor[2];

	vColor[0] = pow(abs(tex2D(AlbedoSampler, In.vTexUV)), 2.2f);
	vColor[1] = pow(abs(tex2D(ShadeSampler, In.vTexUV)), 2.2f);
	//vColor[1] = tex2D(ShadeSampler, In.vTexUV);

	vBlend = vColor[0] * vColor[1];

	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);

	vBlend = vBlend + vSpecular;

	if (m_bGaussianBlur)
	{
		vector vBlur = tex2D(BlurSampler, In.vTexUV);
		vBlend = (vBlend + vBlur) / 2.0f;
	}

	if (g_bSSAO)
	{
		float fSSAO = tex2D(SSAOSampler, In.vTexUV).x;
		vBlend.rgb *= fSSAO;
	}

	float fShadow = tex2D(ShadowSampler, In.vTexUV).x;

	vBlend.rgb *= fShadow;

	vector vEffect = tex2D(EffectSampler, In.vTexUV);

	vBlend += vEffect;

	float fDepth = tex2D(DepthSampler, In.vTexUV).y;

	float fAlpha = min(abs(fDepth * 25.0f - 25.0f), 1.0f);

	vBlend.a = fAlpha;
	
	Out.vColor = pow(abs(vBlend), (1.0f / 2.2f));
	//Out.vCopy = pow(abs(vBlend), (1.0f / 2.2f));
	//Out.vHDR = pow(abs(vBlend), (1.0f / 2.2f));

	return Out;
}

PS_OUT PS_HDR_NO_SHADOW(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector			vBlend;

	vector			vColor[2];

	vColor[0] = pow(abs(tex2D(AlbedoSampler, In.vTexUV)), 2.2f);
	vColor[1] = pow(abs(tex2D(ShadeSampler, In.vTexUV)), 2.2f);//tex2D(ShadeSampler, In.vTexUV);

	vBlend = vColor[0] * vColor[1];

	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);

	vBlend = vBlend + vSpecular;

	if (m_bGaussianBlur)
	{
		vector vBlur = tex2D(BlurSampler, In.vTexUV);
		vBlend = (vBlend + vBlur) / 2.0f;
	}

	if (g_bSSAO)
	{
		float fSSAO = tex2D(SSAOSampler, In.vTexUV).x;
		vBlend.rgb *= fSSAO;
	}
	
	vector vEffect = tex2D(EffectSampler, In.vTexUV);

	vBlend += vEffect;

	float fDepth = tex2D(DepthSampler, In.vTexUV).y;

	float fAlpha = min(abs(fDepth * 25.0f - 25.0f), 1.0f);

	vBlend.a = fAlpha;

	Out.vColor = pow(abs(vBlend), (1.0f / 2.2f));
	//Out.vCopy = pow(abs(vBlend), (1.0f / 2.2f));
	//Out.vHDR = pow(abs(vBlend), (1.0f / 2.2f));

	return Out;
}

technique Default_Device
{
	//0
	pass LIGHT_DIRECTIONAL
	{
		AlphaBlendEnable	= true;
		srcblend			= one;
		destblend			= invsrcalpha;

		ZENABLE				= FALSE;
		ZWRITEENABLE		= FALSE;

		VertexShader		= NULL;
		PixelShader			= compile ps_3_0 PS_MAIN();
	}

	//1
	pass BLEND_NO_SHADOW
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_NO_SHADOW();
	}

	//2
	pass BLEND_NO_SHADOW
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_CHAR();
	}

	//3
	pass BLEND_MOTION_BLUR
	{
		AlphaBlendEnable = true;
		srcblend = SRCALPHA;
		destblend = invsrcalpha;

		//ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MOTION_BLUR();
	}

	//4
	pass BLEND_HDR
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_HDR();
	}

	//5
	pass BLEND_HDR_NO_SHADOW
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_HDR_NO_SHADOW();
	}
};