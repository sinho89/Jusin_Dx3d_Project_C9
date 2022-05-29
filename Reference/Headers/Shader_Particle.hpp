#include "Shader_Base.hpp"

Matrix			g_matWorldInv;
Matrix			g_matViewInv;
Matrix			g_matProjInv;

texture			g_DepthTexture;

vector			g_vColor;
float			g_fFar;

sampler ParticleSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};


struct VS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV	  : TEXCOORD0; //ÅØ½ºÃÄ ÁÂÇ¥
};


struct VS_OUT
{
	vector		vPosition    : POSITION;
	float2		vTexUV	     : TEXCOORD0;
	vector		vEffectDepth : TEXCOORD1;
};



VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Matrix matMV, matMVP;

	matMV = mul(g_matWorld, g_matView);
	matMVP = mul(matMV, g_matProj);

	Out.vPosition = mul(In.vPosition, matMVP);
	Out.vEffectDepth = Out.vPosition;
	Out.vTexUV = In.vTexUV;


	return Out;
}


struct PS_IN
{
	float2   vTexUV		 : TEXCOORD0;
	vector	vEffectDepth : TEXCOORD1;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};



PS_OUT PS_ONE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	Out.vColor.a *= g_vColor.a;

	//float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//ÀÌÆåÆ®ÀÇ ±íÀÌ.

	//float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	//uv.y = -uv.y;
	//uv = uv * 0.5f + 0.5f;

	//float fCurDepth = tex2D(DepthSampler, uv).y;

	//float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	//float fViewZ = In.vEffectDepth.w;

	//Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	//if (fEffectDepth > fCurDepth)
	//{
	//	Out.vColor.a = 0.0f;
	//}

	return Out;
}





PS_OUT PS_TWO(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);



  if(Out.vColor.r >= 2.f/255.f && Out.vColor.g >= 10.f/255.f && Out.vColor.b >= 10.f/255.f)
  {
	  Out.vColor.r  += g_vColor.r;
	  Out.vColor.g  += g_vColor.g;
	  Out.vColor.b  += g_vColor.b;
	  
	  Out.vColor.a = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3.0f;
  }

  Out.vColor.a *= g_vColor.a;

  //float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//ÀÌÆåÆ®ÀÇ ±íÀÌ.

  //float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
  //uv.y = -uv.y;
  //uv = uv * 0.5f + 0.5f;

  //float fCurDepth = tex2D(DepthSampler, uv).y;

  //float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
  //float fViewZ = In.vEffectDepth.w;

  //Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

  //if (fEffectDepth > fCurDepth)
  //{
	 // Out.vColor.a = 0.0f;
  //}


	return Out;
}




PS_OUT PS_THREE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);


	Out.vColor.r  += g_vColor.r;
	Out.vColor.g  += g_vColor.g;
	Out.vColor.b  += g_vColor.b;
	Out.vColor.a  *= g_vColor.a;

	//float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//ÀÌÆåÆ®ÀÇ ±íÀÌ.


	//float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	//uv.y = -uv.y;
	//uv = uv * 0.5f + 0.5f;

	//float fCurDepth = tex2D(DepthSampler, uv).y;


	//float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	//float fViewZ = In.vEffectDepth.w;

	//Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	//if (fEffectDepth > fCurDepth)
	//{
	//	Out.vColor.a = 0.0f;
	//}


	return Out;
}



PS_OUT PS_FOUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	Out.vColor.r   = g_vColor.r;
	Out.vColor.g   = g_vColor.g;
	Out.vColor.b   = g_vColor.b;
	Out.vColor.a  *= g_vColor.a;

	//float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//ÀÌÆåÆ®ÀÇ ±íÀÌ.

	//float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	//uv.y = -uv.y;
	//uv = uv * 0.5f + 0.5f;

	//float fCurDepth = tex2D(DepthSampler, uv).y;


	//float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	//float fViewZ = In.vEffectDepth.w;

	//Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	//if (fEffectDepth > fCurDepth)
	//{
	//	Out.vColor.a = 0.0f;
	//}

	return Out;
}


technique PARTICLE_Technique
{
	//0
	pass PARTICLE_One
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;


		ALPHABLENDENABLE = TRUE;
		ZWRITEENABLE = FALSE;

		SRCBLEND = SRCALPHA;			//SRC : ±×¸®·Á°í ÇÏ´Â°ª
		DESTBLEND = ONE;				//Dst : ¹è°æ°ª


		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_ONE();
	}

	//1
	pass PARTICLE_Two
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;


		ALPHABLENDENABLE = TRUE;
		ZWRITEENABLE = FALSE;

		SRCBLEND = SRCALPHA;
		DESTBLEND = ONE;


		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_TWO();
	}
		
	//2
	pass PARTICLE_Three
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;


		ALPHABLENDENABLE = TRUE;
		ZWRITEENABLE = FALSE;
		
		//SRC : ZERO 
		//DEST : SRCCOLOR Èò»öÀº Åõ¸íÇÏ°Ô Ãâ·Â
		
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_THREE();
	}


	//3
	pass PARTICLE_Four
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;
		
		ALPHABLENDENABLE = TRUE;
		ZWRITEENABLE = FALSE;

		SRCBLEND = SRCALPHA;
		DESTBLEND = ONE;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader  = compile ps_3_0 PS_FOUR();
	}
};