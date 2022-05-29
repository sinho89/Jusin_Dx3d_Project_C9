#include "Shader_Base.hpp"

matrix			g_matOldWorld;
matrix			g_matOldView;
matrix			g_matOldProj;

int				numFrameInfluences = 2;

vector			g_vLightDir = (vector)0.f;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;
vector			g_vLightPosition;

float			g_fPower = 1.f;
vector			g_vCamPosition;

vector			g_vCamDir;

vector			g_vMtrlDiffuse = (1.f, 1.f, 1.f, 1.f);
vector			g_vMtrlAmbient = (0.2f, 0.2f, 0.2f, 1.f);
vector			g_vMtrlSpecular = (1.f, 1.f, 1.f, 1.f);

float			g_fFar;
float			g_fLightCamFar;

matrix			g_matLightView;
matrix			g_matLightProj;

matrix			g_matLightProj_1;
matrix			g_matLightProj_2;

matrix			g_matOutlineWorld;

bool			g_bBlurCheck = false;

float			rimWidth = 0.15f;

float			g_fBrightness = 0.0f;

bool			g_Alphatest = false;

bool			g_bLimLight = false;
float			g_bOutlinePower;
vector			g_vColor;

bool			g_bOutline = false;
float			g_fColorCheck = 0.0f;
float			g_fOutlineThickness = 0.0005f;

//For.VTF Sampler
texture			g_VTF_Texture;

sampler VTFSampler = sampler_state
{
	texture = g_VTF_Texture;
};

//For.Old Vtf Sampler
texture			g_VTF_OldTexture;

sampler Old_VTFSampler = sampler_state
{
	texture = g_VTF_OldTexture;
};

//For.Normal map Sampler=============
texture			g_Normal_Texture;

sampler NormalSampler = sampler_state
{
	texture = g_Normal_Texture;

	minfilter = linear;
	magfilter = linear;
};

//For.Specular map Sampler
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

texture			g_Albedo_Texture;

sampler AlbedoSampler = sampler_state
{
	texture = g_Albedo_Texture;

	minfilter = linear;
	magfilter = linear;
};


struct VS_IN
{
	vector		vPosition	: POSITION;
	vector		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;

	vector		weights		: BLENDWEIGHT0;
	int4		boneIndices : BLENDINDICES0;

	float3		mTangent	: TANGENT;
	float3		mBinormal	: BINORMAL;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;

	float3		T			: TEXCOORD1;
	float3		B			: TEXCOORD2;
	float3		N			: TEXCOORD3;

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

	vector	p = vector(0.0f, 0.0f, 0.0f, 1.0f);
	float3	norm = float3(0.0f, 0.0f, 0.0f);
	float3	tangent = float3(0.0f, 0.0f, 0.0f);
	float3	binormal = float3(0.0f, 0.0f, 0.0f);

	float	lastweight = 0.0f;
	int		n = numFrameInfluences - 1;

	In.vNormal = normalize(In.vNormal);

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
		norm += In.weights[i] * mul(In.vNormal, VtfMatrix);
		tangent += In.weights[i] * mul(In.mTangent, VtfMatrix);
		binormal += In.weights[i] * mul(In.mBinormal, VtfMatrix);
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
	norm += lastweight * mul(In.vNormal, VtfMatrix);
	tangent += lastweight * mul(In.mTangent, VtfMatrix);
	binormal += lastweight * mul(In.mBinormal, VtfMatrix);

	p.w = 1.0f;

	vector posWorld = mul(p, g_matWorld);

	Out.vPosition = mul(posWorld, matVP);
	Out.vDepthPos = Out.vPosition;

	Out.vTexUV = In.vTexUV;

	Out.N = normalize(mul(norm.xyz, (float3x3)g_matWorld));
	Out.T = normalize(mul(tangent, (float3x3)g_matWorld));
	Out.B = normalize(mul(binormal, (float3x3)g_matWorld));

	Out.vLightDir = normalize(g_vLightDir.xyz);
	Out.vViewDir = posWorld;

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
	vector	vOutlinePos : TEXCOORD7;
};

struct PS_OUT
{
	vector	vColor		: COLOR0;
	//vector	vNormal		: COLOR1;
	//vector	vSpecular	: COLOR2;
	vector	vDepthMap	: COLOR1;
	vector	vCharSampler : COLOR2;
	vector	vOutlineChar : COLOR3;
};

PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	//Out.vColor = Out.vColor * g_vMtrlDiffuse + g_vMtrlAmbient;
	Out.vColor.rgb += float3(g_fBrightness, g_fBrightness, g_fBrightness);

	/*float fAlpha = tex2D(BaseSampler, In.vTexUV).a;

	if (fAlpha > 0.0f)
	{
		Out.vColor.rgb = tex2D(BaseSampler, In.vTexUV).rgb;
		Out.vColor.a = 1.0f;
	}
	else
	{
		Out.vColor = 0.0f;
	}*/

	//Out.vColor = Out.vColor * g_vMtrlDiffuse + g_vMtrlAmbient;
	
	if (g_bBlurCheck)
		Out.vCharSampler = vector(Out.vColor.rgb, 1.0f);

	//if (g_bOutline)
	//	Out.vOutlineChar = vector(In.vDepthPos.w / g_fFar, In.vDepthPos.z / In.vDepthPos.w, 0.0f, 1.0f);
	
	//Out.vColor.a = 1.0f;

	float3	tangentNormal = tex2D(NormalSampler, In.vTexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);
	float3	worldNormal = mul(TBN, tangentNormal);

	//Out.vNormal = vector(worldNormal.xyz * 0.5f + 0.5f, 0.0f);
	
	if (g_bOutline)
	{
		/*float3 vLimLightDir = normalize(g_vCamPosition.xyz - In.vViewDir.xyz);
		half rim = 1.0f - saturate(dot(vLimLightDir, worldNormal.xyz));
		float3 RimLightColor = g_vColor.rgb * pow(rim, g_bOutlinePower);

		Out.vOutlineChar = vector(RimLightColor, 0.0f);*/

		Out.vOutlineChar = vector(1.0f, g_fColorCheck, g_fOutlineThickness, 1.0f);
	}

	Out.vDepthMap = vector(In.vDepthPos.w / g_fFar,
							In.vDepthPos.z / In.vDepthPos.w,
							0.0f,
							1.0f);

	return Out;
}

//======================================
VS_OUT VS_SHADE(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Matrix	matWV, matVP, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matVP = mul(g_matView, g_matProj);
	matWVP = mul(matWV, g_matProj);

	vector	p = vector(0.0f, 0.0f, 0.0f, 1.0f);
	float3	norm = float3(0.0f, 0.0f, 0.0f);
	float3	tangent = float3(0.0f, 0.0f, 0.0f);
	float3	binormal = float3(0.0f, 0.0f, 0.0f);

	float	lastweight = 0.0f;
	int		n = numFrameInfluences - 1;

	In.vNormal = normalize(In.vNormal);

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
		norm += In.weights[i] * mul(In.vNormal, VtfMatrix);
		tangent += In.weights[i] * mul(In.mTangent, VtfMatrix);
		binormal += In.weights[i] * mul(In.mBinormal, VtfMatrix);
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
	norm += lastweight * mul(In.vNormal, VtfMatrix);
	tangent += lastweight * mul(In.mTangent, VtfMatrix);
	binormal += lastweight * mul(In.mBinormal, VtfMatrix);

	p.w = 1.0f;

	vector posWorld = mul(p, g_matWorld);

	Out.vPosition = mul(posWorld, matVP);

	Out.vTexUV = In.vTexUV;

	Out.N = normalize(mul(norm.xyz, (float3x3)g_matWorld));
	Out.T = normalize(mul(tangent, (float3x3)g_matWorld));
	Out.B = normalize(mul(binormal, (float3x3)g_matWorld));

	Out.vLightDir = normalize(g_vLightDir.xyz);
	Out.vViewDir = normalize(posWorld.xyz - g_vCamPosition.xyz);

	Out.vDepthPos = posWorld;

	return Out;
}

struct PS_SHADE_OUT
{
	vector	vNormal		: COLOR0;
	vector	vSpecular	: COLOR1;
	vector	vD_Specular : COLOR2;
	vector	vLimLight	: COLOR3;
};

PS_SHADE_OUT PS_SHADE(PS_IN In)
{
	PS_SHADE_OUT Out = (PS_SHADE_OUT)0;

	//Out.vColor = tex2D(BaseSampler, In.vTexUV);
	//Out.vColor.a = 1.0f;

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
		specular *= specularIntensity.xyz;
	//}

	Out.vSpecular = vector(specular.xyz, 0.0f);// *(g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	
	if (g_bLimLight)
	{
		float3 vLimLightDir = normalize(g_vCamPosition.xyz - In.vDepthPos.xyz);
		//float RimLightColor = smoothstep(1.0f - rimWidth, 1.0f, 1.0f - max(0.0f, dot(worldNormal.xyz, vLimLightDir.xyz)));

		half rim = 1.0f - saturate(dot(vLimLightDir, worldNormal.xyz));
		//float3 RimLightColor = float3(1.0f, 1.0f, 1.0f) * pow(rim, 10.5f);
		float3 RimLightColor = g_vColor.rgb * pow(rim, g_bOutlinePower);

		Out.vLimLight = vector(RimLightColor, 0.0f);//vector(RimLightColor, RimLightColor, RimLightColor, 0.0f);
	}

	Out.vD_Specular = vector(specular.xyz, 0.0f);

	if (g_Alphatest)
	{
		float fAlpha = tex2D(AlbedoSampler, In.vTexUV).a;
		if (fAlpha == 0.0f)
		{
			Out.vNormal.a = 0.0f;
			Out.vSpecular.a = 0.0f;
			Out.vLimLight.a = 0.0f;
			Out.vD_Specular.a = 0.0f;
		}
		else
		{
			Out.vNormal.a = 1.0f;
			Out.vSpecular.a = 1.0f;
			Out.vLimLight.a = 1.0f;
			Out.vD_Specular.a = 1.0f;
		}
	}

	return Out;
}
//===================================

//===================================
//For.MotionBlur

struct VS_BLUR_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;

	vector		weights : BLENDWEIGHT0;
	int4		boneIndices : BLENDINDICES0;
};

struct VS_BLUR_OUT
{
	vector		vPosition	: POSITION;

	float2		vTexUV		: TEXCOORD0;
	vector		vVelocity	: TEXCOORD1;
};

VS_BLUR_OUT	VS_BLUR(VS_IN In)
{
	VS_BLUR_OUT	Out = (VS_BLUR_OUT)0;

	Matrix	matWV, matVP, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matVP = mul(g_matView, g_matProj);
	matWVP = mul(matWV, g_matProj);

	Matrix	matOldWV, matOldVP, matOldWVP;
	matOldWV = mul(g_matOldWorld, g_matOldView);
	matOldVP = mul(g_matOldView, g_matOldProj);
	matOldWVP = mul(matWV, g_matOldProj);

	vector	p = vector(0.0f, 0.0f, 0.0f, 1.0f);
	float3	norm = float3(0.0f, 0.0f, 0.0f);

	//MotionBlur를 위한 1 프레임 이전의 위치를 저장할 위치 벡터
	vector	vOldPos = 0.0f;

	float	lastweight = 0.0f;
	int		n = numFrameInfluences - 1;

	In.vNormal = normalize(In.vNormal);

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
		norm += In.weights[i] * mul(In.vNormal, VtfMatrix);

		//=========================================
		//For. Motion Blur = Old Position
		float4x4 Old_VtfMatrix =
		{
			tex2Dlod(Old_VTFSampler, uv),
			tex2Dlod(Old_VTFSampler, uv + float4(1.0f / fSize, 0.0f, 0.0f, 0.0f)),
			tex2Dlod(Old_VTFSampler, uv + float4(2.0f / fSize, 0.0f, 0.0f, 0.0f)),
			tex2Dlod(Old_VTFSampler, uv + float4(3.0f / fSize, 0.0f, 0.0f, 0.0f))
		};

		vOldPos += In.weights[i] * mul(In.vPosition, Old_VtfMatrix);
		//=========================================
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
	norm += lastweight * mul(In.vNormal, VtfMatrix);

	p.w = 1.0f;

	//=========================================
	//For. Motion Blur = Old Position
	float4x4 Old_VtfMatrix =
	{
		tex2Dlod(Old_VTFSampler, uv),
		tex2Dlod(Old_VTFSampler, uv + float4(1.0f / fSize, 0.0f, 0.0f, 0.0f)),
		tex2Dlod(Old_VTFSampler, uv + float4(2.0f / fSize, 0.0f, 0.0f, 0.0f)),
		tex2Dlod(Old_VTFSampler, uv + float4(3.0f / fSize, 0.0f, 0.0f, 0.0f))
	};

	vOldPos += lastweight * mul(In.vPosition, Old_VtfMatrix);

	vOldPos.w = 1.0f;
	//=========================================

	//Out.vTexUV = In.vTexUV;

	//===========================================
	//For.Motion Blur 연산 작업.

	vector vViewPos = mul(p, matWV);
	vector vViewOldPos = mul(vOldPos, matOldWV);

	//속도
	vector vVelocity = (vViewPos - vViewOldPos);
	float3 vViewNormal = mul(norm.xyz, (float3x3)matWV);

	bool bFront = (0 <= dot(vViewNormal, vVelocity.xyz));	//속도의 방향으로 가고 있는가?
	vector vCurPos = bFront ? vViewPos : vViewOldPos;		//방향에 따라 위치를 정한다.

	Out.vPosition = mul(vCurPos, g_matProj);	//투영공간으로.

	Out.vTexUV.x = (Out.vPosition.x + Out.vPosition.w);
	Out.vTexUV.y = (-Out.vPosition.y + Out.vPosition.w);
	
	float fDepth = 2.0f * Out.vPosition.w;

	Out.vTexUV /= fDepth;
												//Texture 좌표로 속도를 구해 보도록하자.
	vector s0 = mul(vViewOldPos, g_matProj);
	s0 /= s0.w;

	vector s1 = mul(vViewPos, g_matProj);
	s1 /= s1.w;

	Out.vVelocity = (s1 - s0);
	Out.vVelocity.x *= 0.5f;
	Out.vVelocity.y *= -0.5f;
	//===========================================

	return Out;
}

struct PS_BLUR_IN
{
	float2	vTexUV		: TEXCOORD0;
	vector	vVelocity	: TEXCOORD1;
};

struct PS_BLUR_OUT
{
	vector		vColor : COLOR0;
};

PS_BLUR_OUT	PS_BLUR(PS_BLUR_IN In)
{
	PS_BLUR_OUT	Out = (PS_BLUR_OUT)0;

	//Out.vColor = tex2D(BaseSampler, In.vTexUV);

	vector BlurBase = 0;
	const int	SAMPLES = 22;
	const float	samples = SAMPLES;
	
	for (int i = 1; i<=SAMPLES; ++i)
	{
		float t = i / samples;
		BlurBase += tex2D(BaseSampler, In.vTexUV + (t * In.vVelocity));
		//BlurBase += tex2D(BaseSampler, (In.vTexUV / In.vTexUV.w) + (t * In.vVelocity));
	}

	BlurBase /= samples;
	Out.vColor = BlurBase;	

	return Out;
}

//===================================
//Shadow=============================
//===================================

struct VS_SHADOW_IN
{
	vector		vPosition	: POSITION;

	vector		weights		: BLENDWEIGHT0;
	int4		boneIndices : BLENDINDICES0;
};

struct VS_SHADOW_OUT
{
	vector		vPosition	: POSITION;
	vector		vDepthPos	: TEXCOORD0;
};

VS_SHADOW_OUT VS_SHADOW_MAIN(VS_SHADOW_IN In)
{
	VS_SHADOW_OUT Out = (VS_SHADOW_OUT)0.0f;

	Matrix	LightWV, LightWVP;
	LightWV = mul(g_matWorld, g_matLightView);
	LightWVP = mul(LightWV, g_matLightProj);

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

	Out.vPosition = mul(p, LightWVP);
	Out.vDepthPos = Out.vPosition;
	//Out.vDepthPos = Out.vPosition.z / Out.vPosition.w;

	return Out;
}

struct PS_SHADOW_IN
{
	vector		vDepthPos : TEXCOORD0;
};

struct PS_SHADOW_OUT
{
	vector		vColor : COLOR0;
};

PS_SHADOW_OUT PS_SHADOW_MAIN(PS_SHADOW_IN In)
{
	PS_SHADOW_OUT Out = (PS_SHADOW_OUT)0.0f;

	//float fDepth = In.vDepthPos.z / In.vDepthPos.w;	원근투영일 때.
	float fDepth = In.vDepthPos.z;		//직교 투영일 때.

	Out.vColor = vector(fDepth.xxx, 1.0f);

	//Out.vColor = vector(In.vDepthPos.xyz, 1.0f);

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

	vector		weights : BLENDWEIGHT0;
	int4		boneIndices : BLENDINDICES0;
};

struct VS_OUTLINE_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vDepthPos : TEXCOORD1;
};

VS_OUTLINE_OUT VS_OUTLINE(VS_OUTLINE_IN In)
{
	VS_OUTLINE_OUT	Out = (VS_OUTLINE_OUT)0;

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

	if(fMapDepth > fDepth)
		Out.vColor.rgb = vector(0.0f, fDepth, 0.0f, 0.0f);

	return Out;
}

//===================================
//===================================

technique Default_Device
{
	//0
	pass Normal
	{
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}

	//1
	pass ShadeTexture
	{	
		VertexShader	= compile vs_3_0 VS_SHADE();
		PixelShader		= compile ps_3_0 PS_SHADE();
	}

	//2
	pass MotionBlur
	{
		ZENABLE = FALSE;
		ZWRITEENABLE = FALSE;

		/*ALPHABLENDENABLE = TRUE;
		SRCBLEND	= SRCALPHA;
		DESTBLEND	= INVSRCALPHA;*/

		VertexShader	= compile vs_3_0 VS_BLUR();
		PixelShader		= compile ps_3_0 PS_BLUR();
	}
	
	//3
	pass Shadow
	{
		VertexShader	= compile vs_3_0 VS_SHADOW_MAIN();
		PixelShader		= compile ps_3_0 PS_SHADOW_MAIN();
	}

	//4
	pass CACD_Shadow
	{
		VertexShader = compile vs_3_0 VS_SHADOW_MAIN();
		PixelShader = compile ps_3_0 PS_CACD_SHADOW_MAIN();
	}

	//5
	pass Outline
	{
		VertexShader = compile vs_3_0 VS_OUTLINE();
		PixelShader  = compile ps_3_0 PS_OUTLINE();
	}

	//6
	/*pass HairTest
	{
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 0x0000003f;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}*/
};