#include "Shader_Base.hpp"

float3	Luminance = float3(0.212671f, 0.715160f, 0.072169f);

float3	R_to_X = float3(0.4674162f, 0.2944512f, 0.1886026f);
float3	G_to_Y = float3(0.2410115f, 0.6835475f, 0.0754410f);
float3	B_to_Z = float3(0.0219101f, 0.0736128f, 0.9933071f);

float3	X_to_R = float3( 2.7454669f, -1.1358136f, -0.4350269f);
float3	Y_to_G = float3(-0.9692660f,  1.8760108f,  0.0415560f);
float3	Z_to_B = float3( 0.0112723f, -0.1139754f,  1.0093968f);

float	g_fLum_Avg = 0.1f;	//휘도의 평균값. 
//마을 0.06f
//던전 
//흑백효과 -0.03f
float	g_fLightPos = 0.95f;

float	A = 6.20f;         
float	B = 0.15f;
float	C = 1.70f;
float	D = 0.14f;
//float	E = 0.02f;
//float	F = 0.30f;
//float	g_X = 50.0f;

texture g_BlurTexture;
sampler BlurSampler = sampler_state
{
	texture = g_BlurTexture;

	minfilter = linear;
	magfilter = linear;
};

texture g_CharTexture;
sampler CharSampler = sampler_state
{
	texture = g_CharTexture;

	minfilter = linear;
	magfilter = linear;
};

//float3 C9Tonemap(float3 fX)
//{
//	return ((fX * (A * fX + C * B)) + D * D) / (fX * (A * fX + B) + D * F) - (E / F);
//}

struct PS_IN
{
	float2  vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};

PS_OUT PS_HDR_ONE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vBlend = pow(abs(tex2D(BaseSampler, In.vTexUV)), 2.2f);
	
	//float fBlurAlpha = tex2D(BlurSampler, In.vTexUV).a;
	
	//vector vGamma = 0.0f;
	//vGamma = pow(abs(vBlend), 2.2f);	//감마계로 만든다.

	// RGB -> XYZ================================
	float3 XYZ = 0.0f;
	
	XYZ.r = dot(vBlend.rgb, R_to_X);
	XYZ.g = dot(vBlend.rgb, G_to_Y);
	XYZ.b = dot(vBlend.rgb, B_to_Z);
	//===========================================
	
	//XYZ -> xyY=================================
	//이중 Y는 휘도(Luminance)를 의미한다.

	float2 xy = 0.0f;

	float fXYZ_Total = XYZ.r + XYZ.g + XYZ.b;
	xy.x = XYZ.x / fXYZ_Total;
	xy.y = XYZ.y / fXYZ_Total;
	
	//vGamma.xyz = dot(vGamma.xyz, Luminance);

	float fLum = dot(vBlend.rgb, Luminance);//vGamma.r + vGamma.g + vGamma.b;	//루미넌스계로 만든다.

	fLum = (0.18f * fLum) / g_fLum_Avg;

	fLum = fLum / (1.0f + fLum);

	//fLum = (fLum * (1.0f + (fLum / pow(g_fLightPos, 2)))) / (1.0f + fLum);

	//=======================================

	//xyY -> XYZ	
	//float3 HDR_RGB = 0.0f;

	XYZ.x = (fLum * xy.x) / xy.y;
	XYZ.y = fLum;
	XYZ.z = (fLum * (1 - xy.x - xy.y)) / xy.y;

	//XYZ -> RGB
	Out.vColor.r = dot(XYZ, X_to_R);
	Out.vColor.g = dot(XYZ, Y_to_G);
	Out.vColor.b = dot(XYZ, Z_to_B);
	Out.vColor.w = vBlend.a;

	Out.vColor = pow(abs(Out.vColor), (1.0f/2.2f));

	return Out;
}

PS_OUT PS_HDR_TWO(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vBlend = tex2D(BaseSampler, In.vTexUV);

	float3 Color = pow(abs(vBlend.rgb), 2.2f);

	float3 x = max(0.0f, Color - 0.004f);
	//Color = (x * (6.2f * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);

	Color = (x * (A * x + B)) / (x * (A * x * C) + D);

	Out.vColor = vector(Color, vBlend.a);
	//Out.vColor = vector(pow(abs(Color), (1.0f / 2.2f)), vBlend.a);

	//vector vChar = tex2D(CharSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_HDR_DIE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vBlend = tex2D(BaseSampler, In.vTexUV);

	//vector vGamma = 0.0f;
	//vGamma = pow(abs(vBlend), 2.2f);	//감마계로 만든다.

	// RGB -> XYZ================================
	float3 XYZ = 0.0f;

	float3 vtemp = 0.0f;

	XYZ.r = dot(vBlend.rgb, R_to_X);
	XYZ.g = dot(vBlend.rgb, G_to_Y);
	XYZ.b = dot(vBlend.rgb, B_to_Z);
	//===========================================

	//XYZ -> xyY=================================
	//이중 Y는 휘도(Luminance)를 의미한다.

	float2 xy = 0.0f;

	float fXYZ_Total = XYZ.r + XYZ.g + XYZ.b;
	xy.x = XYZ.x / fXYZ_Total;
	xy.y = XYZ.y / fXYZ_Total;

	//vGamma.xyz = dot(vGamma.xyz, Luminance);

	float fLum = dot(vBlend.rgb, Luminance);//vGamma.r + vGamma.g + vGamma.b;	//루미넌스계로 만든다.

	fLum = (0.18f * fLum) / (-0.03f);

	fLum = fLum / (1.0f + fLum);

	//fLum = (fLum * (1.0f + (fLum / pow(g_fLightPos, 2)))) / (1.0f + fLum);

	//=======================================

	//xyY -> XYZ	
	//float3 HDR_RGB = 0.0f;

	XYZ.x = (fLum * xy.x) / xy.y;
	XYZ.y = fLum;
	XYZ.z = (fLum * (1 - xy.x - xy.y)) / xy.y;

	//XYZ -> RGB
	Out.vColor.r = dot(XYZ, X_to_R);
	Out.vColor.g = dot(XYZ, Y_to_G);
	Out.vColor.b = dot(XYZ, Z_to_B);
	Out.vColor.w = vBlend.a;

	return Out;
}

PS_OUT PS_HDR_CHAR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vBlend = tex2D(BaseSampler, In.vTexUV);
	
	// RGB -> XYZ================================
	float3 XYZ = 0.0f;

	XYZ.r = dot(vBlend.rgb, R_to_X);
	XYZ.g = dot(vBlend.rgb, G_to_Y);
	XYZ.b = dot(vBlend.rgb, B_to_Z);
	//===========================================

	//XYZ -> xyY=================================
	//이중 Y는 휘도(Luminance)를 의미한다.

	float2 xy = 0.0f;

	float fXYZ_Total = XYZ.r + XYZ.g + XYZ.b;
	xy.x = XYZ.x / fXYZ_Total;
	xy.y = XYZ.y / fXYZ_Total;
		
	float fLum = dot(vBlend.rgb, Luminance);//루미넌스계로 만든다.

	fLum = (0.18f * fLum) / g_fLum_Avg;

	fLum = fLum / (1.0f + fLum);

	//fLum = (fLum * (1.0f + (fLum / pow(g_fLightPos, 2)))) / (1.0f + fLum);

	//=======================================

	//xyY -> XYZ	
	//float3 HDR_RGB = 0.0f;

	XYZ.x = (fLum * xy.x) / xy.y;
	XYZ.y = fLum;
	XYZ.z = (fLum * (1 - xy.x - xy.y)) / xy.y;

	//XYZ -> RGB
	Out.vColor.r = dot(XYZ, X_to_R);
	Out.vColor.g = dot(XYZ, Y_to_G);
	Out.vColor.b = dot(XYZ, Z_to_B);
	Out.vColor.w = vBlend.a;

	return Out;
}

technique HDR_Technique
{
	//0
	pass HDR_ONE
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_HDR_ONE();
	}

	//1
	pass HDR_TWO
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_HDR_TWO();
	}
		
	//2
	pass HDR_DIE
	{
		AlphaBlendEnable = true;
		srcblend = one;
		destblend = invsrcalpha;

		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_HDR_DIE();
	}

	//3
	pass HDR_CHAR
	{
		AlphaBlendEnable = true;
		srcblend = ONE;
		destblend = invsrcalpha;

		//ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_HDR_CHAR();
	}
}