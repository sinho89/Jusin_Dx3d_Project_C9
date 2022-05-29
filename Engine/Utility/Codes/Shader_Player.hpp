#include "Shader_Base.hpp"

//행렬 팔레트
//Matrix	MatrixPalette[57];

//이 변수의 값은 애플리케이션에서 메시가 사용하는 정점당 영향을 미치는 최대 본의 수에 따라 세팅되어져야 한다.
//이 값은 ConvertToIndexedMesh()함수에서 반환되는 pMaxVertexInf1 값.
int numFrameInfluences = 2;

vector			g_vLightDir = (vector)0.f;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;

float			g_fPower = 1.f;
vector			g_vCamPosition;

vector			g_vMtrlDiffuse;
vector			g_vMtrlAmbient;
vector			g_vMtrlSpecular;

texture			g_VTF_Texture;

sampler VTFSampler = sampler_state
{
	texture = g_VTF_Texture;
};

struct VS_IN
{
	vector		vPosition	:	POSITION;
	vector		vNormal		:	NORMAL;
	float2		vTexUV		:	TEXCOORD0;

	vector		weights		:	BLENDWEIGHT0;
	int4		boneIndices :	BLENDINDICES0;
};

struct VS_OUT
{
	vector		vPosition	:	POSITION;
	vector		vShade		:	COLOR0;
	vector		vSpecular	:	COLOR1;
	float2		vTexUV		:	TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;

	Matrix	matWV, matVP, matWVP;
	matWV	=	mul(g_matWorld, g_matView);
	matVP	=	mul(g_matView, g_matProj);
	matWVP	=	mul(matWV, g_matProj);

	vector	p			=	vector(0.0f, 0.0f, 0.0f, 1.0f);
	float3	norm		=	float3(0.0f, 0.0f, 0.0f);
	float	lastweight	=	0.0f;
	int		n			=	numFrameInfluences - 1;

	In.vNormal = normalize(In.vNormal);

	const float fSize	=	48.0f;
	const int iRow		= 48.0f / 4;

	for(int i=0; i<n; ++i)
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
		p		+=	In.weights[i] * mul(In.vPosition, VtfMatrix);

		norm	+=	In.weights[i] * mul(In.vNormal, VtfMatrix);
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

	lastweight	=	1.0f - lastweight;
	p		+=	lastweight * mul(In.vPosition, VtfMatrix);

	norm	+=	lastweight * mul(In.vNormal, VtfMatrix);

	//정점의 포지션과 노말을 블렌딩.
	/*for(int i=0; i<n; ++i)
	{
	lastweight += In.weights[i];
	p		+=	In.weights[i] * mul(In.vPosition, MatrixPalette[In.boneIndices[i]]);

	norm	+=	In.weights[i] * mul(In.vNormal, MatrixPalette[In.boneIndices[i]]);
	}

	lastweight	=	1.0f - lastweight;
	p		+=	lastweight * mul(In.vPosition, MatrixPalette[In.boneIndices[n]]);

	norm	+=	lastweight * mul(In.vNormal, MatrixPalette[In.boneIndices[n]]);*/

	p.w	=	1.0f;

	vector posWorld =	mul(p, g_matWorld);

	Out.vPosition	=	mul(posWorld, matVP);
	Out.vTexUV		=	In.vTexUV;

	//난반사
	vector	vWorldNormal = mul(vector(In.vNormal.xyz, 0.0f), g_matWorld);
	vector	vWorldLight = g_vLightDir * -1.0f;

	float	fIntensity = max(dot(normalize(vWorldNormal), normalize(vWorldLight)), 0.0f);

	Out.vShade = fIntensity;
	Out.vShade.a = 1.0f;

	//정반사
	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vWorldLight));

	vector	vWorldPos = mul(vector(In.vPosition.xyz, 1.0f), g_matWorld);
	vector	vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.0f, normalize(vReflect)), 0.0f), g_fPower);
	Out.vSpecular.a = 1.0f;

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector	vShader		:	COLOR0;
	vector	vSpecular	:	COLOR1;
	float2	vTexUV		:	TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor		:	COLOR0;
};

PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);

	float fAlpha = vColor.a;

	Out.vColor = (vColor * (g_vLightDiffuse * g_vMtrlDiffuse)) * (In.vShader + (g_vLightAmbient * g_vMtrlAmbient)) + (g_vLightSpecular * g_vMtrlSpecular) * In.vSpecular;

	Out.vColor.a = fAlpha;

	return Out;
}

struct VS_OUT_ALPHA
{
	vector		vPosition	:	POSITION;
	vector		vShade		:	COLOR0;
	vector		vSpecular	:	COLOR1;
	float2		vTexUV		:	TEXCOORD0;
};

struct PS_IN_ALPHA
{
	vector	vShader		:	COLOR0;
	vector	vSpecular	:	COLOR1;
	float2	vTexUV		:	TEXCOORD0;
};

VS_OUT_ALPHA VS_MAIN_ALPHATEST(VS_IN In)
{
	//VS_OUT_ALPHA		Out = (VS_OUT_ALPHA)0;

	//Matrix		matWV, matWVP;

	//matWV = mul(g_matWorld, g_matView);
	//matWVP = mul(matWV, g_matProj);

	//Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	//Out.vTexUV = In.vTexUV;

	////빛~
	//vector		vLightDirInv = normalize(g_vLightDir) * -1.f;

	//vector		vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	//vWorldNormal = normalize(vWorldNormal);

	//Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	VS_OUT_ALPHA		Out = (VS_OUT_ALPHA)0;

	Matrix	matWV, matVP, matWVP;
	matWV	=	mul(g_matWorld, g_matView);
	matVP	=	mul(g_matView, g_matProj);
	matWVP	=	mul(matWV, g_matProj);

	vector	p			=	vector(0.0f, 0.0f, 0.0f, 1.0f);
	float3	norm		=	float3(0.0f, 0.0f, 0.0f);
	float	lastweight	=	0.0f;
	int		n			=	numFrameInfluences - 1;

	In.vNormal = normalize(In.vNormal);

	const float fSize	= 48.0f;
	const int iRow		= 48.0f / 4;

	for(int i=0; i<n; ++i)
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
		p		+=	In.weights[i] * mul(In.vPosition, VtfMatrix);

		norm	+=	In.weights[i] * mul(In.vNormal, VtfMatrix);
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

	lastweight	=	1.0f - lastweight;
	p		+=	lastweight * mul(In.vPosition, VtfMatrix);

	norm	+=	lastweight * mul(In.vNormal, VtfMatrix);

	//정점의 포지션과 노말을 블렌딩.
	/*for(int i=0; i<n; ++i)
	{
	lastweight += In.weights[i];
	p		+=	In.weights[i] * mul(In.vPosition, MatrixPalette[In.boneIndices[i]]);

	norm	+=	In.weights[i] * mul(In.vNormal, MatrixPalette[In.boneIndices[i]]);
	}

	lastweight	=	1.0f - lastweight;
	p		+=	lastweight * mul(In.vPosition, MatrixPalette[In.boneIndices[n]]);

	norm	+=	lastweight * mul(In.vNormal, MatrixPalette[In.boneIndices[n]]);*/

	p.w	=	1.0f;

	vector posWorld =	mul(p, g_matWorld);

	Out.vPosition	=	mul(posWorld, matVP);
	Out.vTexUV		=	In.vTexUV;

	//난반사
	vector	vWorldNormal = mul(vector(In.vNormal.xyz, 0.0f), g_matWorld);
	vector	vWorldLight = g_vLightDir * -1.0f;

	float	fIntensity = max(dot(normalize(vWorldNormal), normalize(vWorldLight)), 0.0f);

	Out.vShade = fIntensity;
	Out.vShade.a = 1.0f;

	//정반사
	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vWorldLight));

	vector	vWorldPos = mul(vector(In.vPosition.xyz, 1.0f), g_matWorld);
	vector	vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.0f, normalize(vReflect)), 0.0f), g_fPower);
	Out.vSpecular.a = 1.0f;

	Out.vTexUV = In.vTexUV;

	return Out;
}


PS_OUT PS_MAIN_ALPHATEST(PS_IN_ALPHA In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);

	//float fAlpha = vColor.a;

	Out.vColor = (vColor * (g_vLightDiffuse * g_vMtrlDiffuse)) * (In.vShader + (g_vLightAmbient * g_vMtrlAmbient)) + (g_vLightSpecular * g_vMtrlSpecular) * In.vSpecular;

	Out.vColor.a = g_fAlpha;

	return Out;
}

technique Default_Device
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader	 = compile ps_3_0 PS_MAIN();
	}

	pass Tool
	{
		alphatestenable = false;

		//alphatestenable = false;
		alphafunc = greater;
		alpharef = 0x0000003f;

		VertexShader = compile vs_3_0 VS_MAIN_ALPHATEST();
		PixelShader	 = compile ps_3_0 PS_MAIN_ALPHATEST();
	}
};