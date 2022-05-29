#include "Shader_Base.hpp"

float2 Center = float2(0.5f, 0.5f);
float fBlurStar = 1.0f; // blur offset
float fBlurWidth = -0.1f; //how big should be
int iNsamples = 10;

struct PS_IN
{
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor0 : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	float2 UV = In.vTexUV - Center;

	vector c = 0.0f;

	for (int i = 0; i < iNsamples; ++i)
	{
		float fScale = fBlurStar + fBlurWidth * (i / (float)(iNsamples - 1));
		c += tex2D(BaseSampler, UV * fScale + Center);
	}

	Out.vColor0 = c / iNsamples;
	
	return Out;
}

technique Default_Device
{
	pass Default
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}