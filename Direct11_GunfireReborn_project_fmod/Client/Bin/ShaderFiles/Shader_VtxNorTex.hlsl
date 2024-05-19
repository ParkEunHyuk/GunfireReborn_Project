
#include "Shader_Defines.hpp" 

/* Phong Shader */
/* 그라우드 셰이더. */

cbuffer BrushDesc
{
	float4		g_vBrushPos = float4(10.0f, 0.0f, 10.f, 1.f);
	float		g_fRadius = 3.f;
};

texture2D		g_DiffuseTexture;
texture2D		g_SourDiffuseTexture;
texture2D		g_DestDiffuseTexture;
texture2D		g_FilterTexture;
texture2D		g_BrushTexture;


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN_TERRAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN_TERRAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vSourMtrlDiffuse = g_SourDiffuseTexture.Sample(DefaultSampler, In.vTexUV * 30.f); //TexUV를 30번 곱했으니 그만큼 TexUV가 커져 여러개로 보임
	vector	vDestMtrlDiffuse = g_DestDiffuseTexture.Sample(DefaultSampler, In.vTexUV * 30.f);
	// vector	vBrushColor = g_BrushTexture.Sample(DefaultSampler, In.vTexUV);	
	vector	vBrushColor = (vector)0.f;
	vector	vFilterColor = g_FilterTexture.Sample(PointSampler, In.vTexUV); //포인터샘플러로 하면 두개로 나뉘어보이고 DefaultSampler로 하면 자연스럽게 섞임

	if ( g_vBrushPos.x - g_fRadius < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos.x + g_fRadius && 
		g_vBrushPos.z - g_fRadius < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos.z + g_fRadius)// 브러쉬의 위치
	{
		float2		vBrushUV;

		vBrushUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fRadius)) / (2.f * g_fRadius);
		vBrushUV.y = ((g_vBrushPos.z + g_fRadius) - In.vWorldPos.z) / (2.f * g_fRadius);  //브러쉬의 텍스쳐 갯수를 지정중 


		vBrushColor = g_BrushTexture.Sample(DefaultSampler, vBrushUV);
	}



	vector	vMtrlDiffuse = vSourMtrlDiffuse * vFilterColor.r + vDestMtrlDiffuse * (1.f - vFilterColor.r) + vBrushColor;
		
	Out.vDiffuse = vMtrlDiffuse;
	Out.vDiffuse.a = 1.f;

	/* -1 -> 0*/
	/* 1 - > 1*/
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}

technique11		DefaultTechnique
{
	pass Terrain
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN();
	}
}