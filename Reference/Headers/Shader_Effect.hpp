#include "Shader_Base.hpp"

//For.VTF Sampler
texture			g_VTF_Texture;

sampler VTFSampler = sampler_state
{
	texture = g_VTF_Texture;
};

int				numFrameInfluences = 2;

vector			g_vCamPosition;
vector			g_vCamDir;

float3	Luminance = float3(0.212671f, 0.715160f, 0.072169f);

float3	R_to_X = float3(0.4674162f, 0.2944512f, 0.1886026f);
float3	G_to_Y = float3(0.2410115f, 0.6835475f, 0.0754410f);
float3	B_to_Z = float3(0.0219101f, 0.0736128f, 0.9933071f);

Matrix			g_matWorldInv;
Matrix			g_matViewInv;
Matrix			g_matProjInv; 

float			g_fGage;
vector			g_vColor;
float			g_fFar;
float			g_fReflect;
vector			g_vComPareVector;

texture	 g_MapDepthTexture;
texture  g_DepthTexture;
texture  g_BlendTexture;
texture  g_NormalTexture;
texture  g_SSDTexture;
texture  g_BlurTexture;
texture  g_DynamicDepthTexture;

vector	g_Deproject;

vector g_vViewPos;

float	g_fCubeScale;
vector	g_vCubePosition;
vector	g_vCamTopRight;

vector g_vLightDir;

vector	g_vCubeScale_0;
vector	g_vCubeScale_1;
vector	g_vCubeScale_2;
vector	g_vCubeScale_3;
vector	g_vCubeScale_4;
vector	g_vCubeScale_5;

sampler ParticleSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler MapDepthSampler = sampler_state
{
	texture = g_MapDepthTexture;
};

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

sampler BlendSampler = sampler_state
{
	texture = g_BlendTexture;
	
	minfilter = linear;
	magfilter = linear;
};

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

	minfilter = linear;
	magfilter = linear;
	//mipfilter = linear;
};

sampler SSDSampler = sampler_state
{
	texture = g_SSDTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler BlurSampler = sampler_state
{
	texture = g_BlurTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler DynamicDepthSampler = sampler_state
{
	texture = g_DynamicDepthTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0; //텍스쳐 좌표
};

struct VS_IN_TWO
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0; //텍스쳐 좌표
	
	float3		mTangent : TANGENT;
	float3		mBinormal : BINORMAL;
};

struct VS_OUT
{
	vector	vPosition		   : POSITION;
	float2	vTexUV			   : TEXCOORD0;
	vector	vEffectDepth	   : TEXCOORD1;
	vector  vEffectLengthDepth : TEXCOORD2; //Length 비교를 위한 vector
};

struct VS_OUT_TWO
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;

	float3			T : TEXCOORD1;
	float3			B : TEXCOORD2;
	float3			N : TEXCOORD3;
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

	Out.vEffectLengthDepth = mul(In.vPosition, g_matWorld);
	
	return Out;
}

VS_OUT_TWO VS_DECAL(VS_IN_TWO In)
{
	VS_OUT_TWO Out = (VS_OUT_TWO)0.0f;

	Out.N = normalize(mul(In.vNormal.xyz, (float3x3)g_matWorld));
	Out.T = normalize(mul(In.mTangent, (float3x3)g_matWorld));
	Out.B = normalize(mul(In.mBinormal, (float3x3)g_matWorld));

	return Out;
}

struct PS_IN
{
	float2  vTexUV			   : TEXCOORD0;
	vector	vEffectDepth	   : TEXCOORD1;
	vector  vEffectLengthDepth : TEXCOORD2; //Length 비교를 위한 vector
};

struct PS_IN_TWO
{
	float2  vTexUV : TEXCOORD0;

	float3		T : TEXCOORD1;
	float3		B : TEXCOORD2;
	float3		N : TEXCOORD3;
};

struct PS_LUM_IN
{
	float2  vTexUV : TEXCOORD0;
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
	
	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}


PS_OUT PS_TWO(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	//==============================================
	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	
	if (Out.vColor.r >= 2.f / 255.f && Out.vColor.g >= 10.f / 255.f && Out.vColor.b >= 10.f / 255.f)
	{
		Out.vColor.r += g_vColor.r;
		Out.vColor.g += g_vColor.g;
		Out.vColor.b += g_vColor.b;

		Out.vColor.a = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3.0f;
	}

	Out.vColor.a *= g_vColor.a;

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}
	
	return Out;
}

PS_OUT PS_THREE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);
	
	Out.vColor.r += g_vColor.r;
	Out.vColor.g += g_vColor.g;
	Out.vColor.b += g_vColor.b;
	Out.vColor.a *= g_vColor.a;

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.


	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;


	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;
	
	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}


PS_OUT PS_FOUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	Out.vColor.r = g_vColor.r;
	Out.vColor.g = g_vColor.g;
	Out.vColor.b = g_vColor.b;
	Out.vColor.a *= g_vColor.a;


	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;


	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}


PS_OUT PS_USEONE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	Out.vColor.r += g_vColor.r;
	Out.vColor.g += g_vColor.g;
	Out.vColor.b += g_vColor.b;
	Out.vColor.a *= g_vColor.a;


	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;


	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}


PS_OUT PS_REFLECT(PS_IN In)
{
	//굴절 부분
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	Out.vColor.a = 1.f;

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;
	

	////노말 부분	
	float3 Normal = tex2D(NormalSampler, In.vTexUV);

	Normal = normalize(Normal);

	Out.vColor.rgb = tex2D(BlendSampler, uv + Normal.xy * g_fReflect).rgb; //굴절률


    //=================================================================

	float fCurDepth = tex2D(DepthSampler, uv).y;

	//float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	//float fViewZ = In.vEffectDepth.w;

	//Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
		Out.vColor.a = 0.0f;


	return Out;
}



PS_OUT PS_REFLECTSHAPE(PS_IN In)
{
	//굴절 부분
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);
	
	Out.vColor.r = 0.f;
	Out.vColor.g = 0.f;
	Out.vColor.b = 0.f;

	if (Out.vColor.a > 0.2f)
		Out.vColor.a = 1.f;

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;


	////노말 부분	
	float3 Normal = tex2D(NormalSampler, In.vTexUV);

	Normal = normalize(Normal);

	Out.vColor.rgb = tex2D(BlendSampler, uv + Normal.xy * g_fReflect).rgb; //굴절률


	Out.vColor.r += g_vColor.r;
	Out.vColor.g += g_vColor.g;
	Out.vColor.b += g_vColor.b;
	//Out.vColor.a *= g_vColor.a;

//=================================================================

	float fCurDepth = tex2D(DepthSampler, uv).y;

	if (fEffectDepth > fCurDepth)
		Out.vColor.a = 0.0f;

	return Out;
}

PS_OUT PS_ALPHATRAIL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;
	
	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	Out.vColor.r = g_vColor.r;
	Out.vColor.g = g_vColor.g;
	Out.vColor.b = g_vColor.b;
	Out.vColor.a *= g_vColor.a;


	if (g_fGage > In.vTexUV.y)
		Out.vColor.a *= 0.f;
	
	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);
	
	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}
		
	//vector vBlur = tex2D(BlurSampler, In.vTexUV);
	//Out.vColor += vBlur;

	return Out;
}

PS_OUT PS_LENGTH(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;


	Out.vColor = tex2D(ParticleSampler, In.vTexUV);
	Out.vColor.r += g_vColor.r;
	Out.vColor.g += g_vColor.g;
	Out.vColor.b += g_vColor.b;
	Out.vColor.a *= g_vColor.a;


	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	//월드행렬의 좌표를 비교

	float Dist = length(In.vEffectLengthDepth.xyz - g_vComPareVector.xyz);


	Dist = 1.f - (Dist * 0.05f);

	Out.vColor.a *= max(Dist, 0.f);
	

	//지면과 뭉개기
	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}



PS_OUT PS_SSD(PS_IN_TWO In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	/*float3 screenposition = In.vEffectDepth.xyz / In.vEffectDepth.w;
	screenposition.y = -screenposition.y;
	screenposition = screenposition * 0.5f + 0.5f;

	float depth = tex2D(DepthSampler, screenposition.xy);

	float3 ViewRay = float3(lerp(-g_vCamTopRight.xy, g_vCamTopRight.xy, screenposition.xy), g_vCamTopRight.z);

	vector ViewPos = vector(ViewRay.xyz * depth, 1.0f);
	vector ObjPos = mul(ViewPos, g_matViewInv);

	float3 ObjAbs = abs(ObjPos.xyz);
	clip(0.5f - ObjAbs);

	float2 uv = ObjPos.xz + 0.5f;
	vector col = tex2D(SSDSampler, uv);

	float dist = abs(ObjPos.z);
	float scaleDistance = max(dist * 2.0f, 1.0f);
	float fadeOut = 1.0f - scaleDistance;
	col.a *= fadeOut;

	Out.vColor = col * (1.0f - max((ObjAbs.z - 0.25f) / 0.25f, 0.0f));*/
		
	float fDepth = tex2D(DepthSampler, In.vTexUV).y;

	vector vMapDepth = tex2D(MapDepthSampler, In.vTexUV);
	float fViewZ = vMapDepth.x * g_fFar;

	vector vPosition = 0.0f;

	vPosition.x = (In.vTexUV.x * 2.0f - 1.0f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.0f + 1.0f) * fViewZ;
	vPosition.z = vMapDepth.y * fViewZ;
	vPosition.w = fViewZ;
		
	vPosition = mul(vPosition, g_matProjInv);	//뷰 영역상 위치.
	vPosition = mul(vPosition, g_matViewInv);	//월드 영역상 위치.
	vPosition = mul(vPosition, g_matWorldInv);	//로컬 영역상 위치.
	
	vector vSSD = 0.0f;

	if (g_vCubeScale_0.x <= vPosition.x && g_vCubeScale_1.x >= vPosition.x &&
		g_vCubeScale_2.y <= vPosition.y && g_vCubeScale_3.y >= vPosition.y &&
		g_vCubeScale_4.z <= vPosition.z && g_vCubeScale_5.z >= vPosition.z)
	{
		float2 uv = vPosition.xy / vPosition.w;
		uv.y = -uv.y;
		uv = uv * 0.5f + 0.5f;

		if (vMapDepth.y > fDepth)
			vSSD = 0.0f;
		else
		{
			vSSD = tex2D(SSDSampler, uv);

			/*float3	tangentNormal = tex2D(NormalSampler, uv).xyz;
			tangentNormal = normalize(tangentNormal * 2 - 1);

			float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
			TBN = transpose(TBN);
			float3	worldNormal = mul(TBN, tangentNormal);

			worldNormal = saturate(dot(worldNormal, -g_vLightDir));

			vSSD.rgb *= worldNormal;*/
		}
	}
	else
	{
		vSSD = 0.0f;
	}

	//Out.vColor = tex2D(BaseSampler, In.vTexUV);
	//Out.vColor += vSSD;
	Out.vColor = vSSD;

	//clip(0.5f - abs(vPosition.xyz));

	////데칼 텍스쳐의 UV
	//float2 Decal_uv = vPosition.xz + 0.5f;

	//Out.vColor = tex2D(SSDSampler, uv);

	return Out;
}

//Blur
PS_OUT PS_EFFECT_TRAIL_LUMINANCE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	Out.vColor.r = g_vColor.r;
	Out.vColor.g = g_vColor.g;
	Out.vColor.b = g_vColor.b;
	Out.vColor.a *= g_vColor.a;
	
	if (g_fGage > In.vTexUV.y)
		Out.vColor.a *= 0.f;
	
	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;
	
	/*float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);*/
	
	//Out.vColor.a = fEffectDepth;

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}
	/*else
	{
		Out.vColor.a = fEffectDepth;
		float3 vLumColor = pow(abs(Out.vColor.rgb), 2.2f);

		float fLum = dot(vLumColor, Luminance);
		fLum = (0.18f * fLum) / 0.12f;

		if (1.5f > fLum)
			Out.vColor = 0.0f;
		else
			Out.vColor.rgb = float3(1.0f, 1.0f, 1.0f);
	}	*/

	return Out;
}

PS_OUT PS_TRAIL_LUMINANCE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	//==============================================
	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	if (Out.vColor.r >= 2.f / 255.f && Out.vColor.g >= 10.f / 255.f && Out.vColor.b >= 10.f / 255.f)
	{
		Out.vColor.r += g_vColor.r;
		Out.vColor.g += g_vColor.g;
		Out.vColor.b += g_vColor.b;

		Out.vColor.a = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3.0f;
	}

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}
	else
	{
		Out.vColor.r *= 1.005f;
		Out.vColor.g *= 1.005f;
		Out.vColor.b *= 1.005f;

		Out.vColor = pow(Out.vColor, 4);

		if (Out.vColor.r + Out.vColor.g + Out.vColor.b < 0.95f)
			Out.vColor = 0.0f;

		Out.vColor = smoothstep(0.7f, 0.95f, Out.vColor);
		Out.vColor = pow(Out.vColor, 12);
	}

	return Out;
}

struct VS_DYNAMIC_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

	vector		weights : BLENDWEIGHT0;
	int4		boneIndices : BLENDINDICES0;
};

struct VS_DYNAMIC_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

	float3		vLightDir : TEXCOORD1;
	float3		vViewDir : TEXCOORD2;

	vector		vDepthPos : TEXCOORD3;
};

VS_DYNAMIC_OUT VS_DYNAMIC(VS_DYNAMIC_IN In)
{
	VS_DYNAMIC_OUT Out = (VS_DYNAMIC_OUT)0.0f;

	Matrix	matWV, matVP, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matVP = mul(g_matView, g_matProj);
	matWVP = mul(matWV, g_matProj);

	vector	p = vector(0.0f, 0.0f, 0.0f, 1.0f);

	float	lastweight = 0.0f;
	int		n = numFrameInfluences - 1;

	const float fSize = 48.0f;
	const int iRow = 48.0f / 4.0f;

	for (int i = 0; i<n; ++i)
	{
		float uv_X = ((float)((In.boneIndices[i] % iRow) * 4) + 0.5f) / fSize;
		float uv_Y = (float)((In.boneIndices[i] / iRow) + 0.5f) / fSize;

		float4 uv = float4(uv_X, uv_Y, 0.0f, 0.0f);

		float4x4 VtfMatrix =
		{
			tex2Dlod(VTFSampler, uv),
			tex2Dlod(VTFSampler, uv + float4(1.0f / fSize, 0.0f, 0.0f, 0.0f)),
			tex2Dlod(VTFSampler, uv + float4(2.0f / fSize, 0.0f, 0.0f, 0.0f)),
			tex2Dlod(VTFSampler, uv + float4(3.0f / fSize, 0.0f, 0.0f, 0.0f))
		};

		lastweight += In.weights[i];

		p += In.weights[i] * mul(In.vPosition, VtfMatrix);
	}

	float uv_X = ((float)((In.boneIndices[n] % iRow) * 4) + 0.5f) / fSize;
	float uv_Y = (float)((In.boneIndices[n] / iRow) + 0.5f) / fSize;

	float4 uv = float4(uv_X, uv_Y, 0.0f, 0.0f);

	float4x4 VtfMatrix =
	{
		tex2Dlod(VTFSampler, uv),
		tex2Dlod(VTFSampler, uv + float4(1.0f / fSize, 0.0f, 0.0f, 0.0f)),
		tex2Dlod(VTFSampler, uv + float4(2.0f / fSize, 0.0f, 0.0f, 0.0f)),
		tex2Dlod(VTFSampler, uv + float4(3.0f / fSize, 0.0f, 0.0f, 0.0f))
	};

	lastweight = 1.0f - lastweight;
	p += lastweight * mul(In.vPosition, VtfMatrix);

	p.w = 1.0f;

	vector posWorld = mul(p, g_matWorld);

	Out.vPosition = mul(posWorld, matVP);
	Out.vDepthPos = Out.vPosition;

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_DYNAMIC_IN
{
	float2	vTexUV : TEXCOORD0;
	vector	vDepthPos : TEXCOORD3;
};

PS_OUT PS_DYNAMIC(PS_DYNAMIC_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	vector vColor = tex2D(BlurSampler, In.vTexUV);

	if (vColor.r >= 0.175f || vColor.g >= 0.175f || vColor.b >= 0.175f)
	{
		vColor.r = vColor.r * 0.21f;
		vColor.g = vColor.g * 0.72f;
		vColor.b = vColor.b * 0.07f;

		Out.vColor.rgb = vColor.rgb + g_vColor.rgb;
		Out.vColor.a = vColor.a;
	}

	float fEffectDepth = In.vDepthPos.z / In.vDepthPos.w;	//이펙트의 깊이.

	float2 uv = In.vDepthPos.xy / In.vDepthPos.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;
	float fCurDepth = tex2D(DepthSampler, uv).y;

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}

PS_OUT PS_THIRTEEN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0.0f;

	//==============================================
	Out.vColor = tex2D(ParticleSampler, In.vTexUV);


	if (Out.vColor.r >= 2.0f / 255.f && Out.vColor.g >= 2.0f / 255.f && Out.vColor.b >= 2.0f / 255.f)
	{
		Out.vColor.r += g_vColor.r;
		Out.vColor.g += g_vColor.g;
		Out.vColor.b += g_vColor.b;

		Out.vColor.a = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3.0f;
	}

	Out.vColor.a *= g_vColor.a;

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float fCurDepth = tex2D(DepthSampler, uv).y;

	float fBackViewZ = tex2D(DepthSampler, uv).r * g_fFar;
	float fViewZ = In.vEffectDepth.w;

	Out.vColor.a = Out.vColor.a * max(saturate(fBackViewZ - fViewZ), 0.f);

	if (fEffectDepth > fCurDepth)
	{
		Out.vColor.a = 0.0f;
	}

	return Out;
}


//14
PS_OUT PS_REFLECTSHAPECOLOR(PS_IN In)
{
	//굴절 부분
	PS_OUT Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(ParticleSampler, In.vTexUV);

	Out.vColor.r = 0.f;
	Out.vColor.g = 0.f;
	Out.vColor.b = 0.f;

	if (Out.vColor.a > 0.2f)
		Out.vColor.a = 1.f;

	float fEffectDepth = In.vEffectDepth.z / In.vEffectDepth.w;	//이펙트의 깊이.

	float2 uv = In.vEffectDepth.xy / In.vEffectDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;


	////노말 부분	
	float3 Normal = tex2D(NormalSampler, In.vTexUV);

	Normal = normalize(Normal);

	Out.vColor.rgb = tex2D(BlendSampler, uv + Normal.xy * g_fReflect).rgb; //굴절률


	Out.vColor.r += g_vColor.r;
	Out.vColor.g += g_vColor.g;
	Out.vColor.b += g_vColor.b;
	Out.vColor.a *= g_vColor.a;

	if (g_fGage > In.vTexUV.y)
		Out.vColor.a *= 0.f;

	//=================================================================

	float fCurDepth = tex2D(DepthSampler, uv).y;

	if (fEffectDepth > fCurDepth)
		Out.vColor.a = 0.0f;

	return Out;
}







technique PARTICLE_Technique
{
	//0
	pass PARTICLE_One
	{
		LIGHTING			= FALSE;
		CULLMODE			= NONE;
		
		ZWRITEENABLE		= FALSE;
		
		ALPHABLENDENABLE	= TRUE;
		
		SRCBLEND			= SRCALPHA;			//SRC : 그리려고 하는값
		DESTBLEND			= ONE;				//Dst : 배경값
		
		VertexShader		= compile vs_3_0 VS_MAIN();
		PixelShader			= compile ps_3_0 PS_ONE();
	}

	//1
	pass PARTICLE_Two
	{
		LIGHTING			= FALSE;
		CULLMODE			= NONE;

		ZWRITEENABLE		= FALSE;
		
		ALPHABLENDENABLE	= TRUE;	

		SRCBLEND			= SRCALPHA;
		DESTBLEND			= ONE;

		VertexShader		= compile vs_3_0 VS_MAIN();
		PixelShader			= compile ps_3_0 PS_TWO();
	}

	//2
	pass PARTICLE_Three
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;


		ALPHABLENDENABLE = TRUE;
		ZWRITEENABLE = FALSE;

		//SRC : ZERO 
		//DEST : SRCCOLOR 흰색은 투명하게 출력

		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_THREE();
	}

	//3
	pass PARTICLE_Four
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;

		ALPHABLENDENABLE = TRUE;
		ZWRITEENABLE = FALSE;

		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_FOUR();
	}


	//굴절4
	pass PARTICLE_Reflect
	{
		CULLMODE = NONE;
		ALPHABLENDENABLE = TRUE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_REFLECT();
	}

	//데칼5
	pass SSDecal
	{
		CULLMODE = NONE;

		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0x0000003f;

		//VertexShader	= compile vs_3_0 VS_MAIN();
		VertexShader = NULL;//compile vs_3_0 VS_DECAL();
		PixelShader		= compile ps_3_0 PS_SSD();
	}

	//6
	pass Trail_Alpha
	{
		CULLMODE = NONE;
		ALPHABLENDENABLE = TRUE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_ALPHATRAIL();
	}

	//7
	pass PARTICLE_ReflectSpape
	{
		CULLMODE = NONE;
		ALPHABLENDENABLE = TRUE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_REFLECTSHAPE();
	}

	//8.
	//고휘도 추출
	pass Effect_Luminance
	{
		CULLMODE = NONE;
		ALPHABLENDENABLE = TRUE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_EFFECT_TRAIL_LUMINANCE();
	}

	//9
	pass PARTICLE_USEONE
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;

		ZWRITEENABLE = FALSE;
		ALPHABLENDENABLE = TRUE;

		SRCBLEND = SRCALPHA;			//SRC : 그리려고 하는값
		DESTBLEND = ONE;				//Dst : 배경값

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_USEONE();
	}

	//10
	pass Effect_Trail_Luminance
	{
		CULLMODE = NONE;
		ALPHABLENDENABLE = TRUE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_TRAIL_LUMINANCE();
	}
	//11 Length비교를 통한 알파값 을 준다.
	pass PARTICLE_LENGTH
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;

		ZWRITEENABLE = FALSE;
		ALPHABLENDENABLE = TRUE;

		SRCBLEND = SRCALPHA;			//SRC : 그리려고 하는값
		DESTBLEND = ONE;				//Dst : 배경값

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_LENGTH();

	}
	
	//12
	pass Effect_Dynamic
	{
		ALPHABLENDENABLE = TRUE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader	= compile vs_3_0 VS_DYNAMIC();
		PixelShader		= compile ps_3_0 PS_DYNAMIC();
	}

		//13
	pass PARTICLE_THIRTEEN
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;
		
		ALPHABLENDENABLE = TRUE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		//SRC : ZERO 
		//DEST : SRCCOLOR 흰색은 투명하게 출력

		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_THIRTEEN();
	}


	//14
	pass PARTICLE_REFLECTSHAPECOLOR
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;

		ALPHABLENDENABLE = TRUE;
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		//SRC : ZERO 
		//DEST : SRCCOLOR 흰색은 투명하게 출력

		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_REFLECTSHAPECOLOR();
	}
};