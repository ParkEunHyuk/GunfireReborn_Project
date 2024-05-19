
#include "Shader_Defines.hpp" 

//cbuffer LightDesc
//{
//	float4		g_vLightDir;
//	float4		g_vLightPos;
//	float4		g_vLightDiffuse;
//	float4		g_vLightAmbient = (float4)1.f;
//	float4		g_vLightSpecular = (float4)1.f;
//};
//
//cbuffer MtrlDesc
//{
//	float4		g_vMtrlAmbient = float4(0.4f, 0.4f, 0.4f, 1.f);
//	float4		g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);
//};
//
//cbuffer CameraDesc
//{
//	float4		g_vCamPosition;
//};

struct BoneMatrixArray
{
	matrix				BoneMatrices[256];
};

cbuffer Matrices
{
	BoneMatrixArray		g_BoneMatrices;
};
texture2D			g_DiffuseTexture;
texture2D			g_NormalTexture;
texture2D			g_DissolveTexture;
float				g_fDissolveTime;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;

	//Dissolve��
	float4		vTangent : TANGENT;
	float4		vBinormal : BINORMAL;
};

VS_OUT VS_MAIN_DEFAULT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float		fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix		BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vProjPos = Out.vPosition;

	///Dissolve��
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
	Out.vBinormal = normalize(vector(cross(Out.vNormal.xyz, Out.vTangent.xyz), 0.f));

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;

	//Dissolve��
	float4		vTangent : TANGENT;
	float4		vBinormal : BINORMAL;
};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN_DEFAULT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//float		fShade = saturate(dot(normalize(g_vLightDir) * -1.f, In.vNormal));

	////fShade = ceil(fShade * 5) / 5.0; //vDiffuse�� Out.vColor�κ��� �ǵ帮�� ��¦ �̻���
	//								//�� ���̴���°� �ᱹ ����� �ǵ帮�°��̴� ���̴��κ��� �ǵ帮��. ���̴��� �ּ�Ǯ�� �� ��

	//float4		vReflect = reflect(normalize(g_vLightDir), In.vNormal);
	//float4		vLook = normalize(In.vWorldPos - g_vCamPosition);

	//float		fSpecular = pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f);

	//Out.vColor = (g_vLightDiffuse * vDiffuse) * (fShade + (g_vLightAmbient * g_vMtrlAmbient)) +
	//	(g_vLightSpecular * g_vMtrlSpecular) * fSpecular; //������ ���°͵� ������ ����

	////�ϴ� ��ī�� ����
	////if (Out.vColor.a < 0.1f)
	////	discard;

	Out.vDiffuse = vDiffuse;
	Out.vDiffuse.a = 1.f;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}

//Dissolve��
PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (0.1f >= Out.vDiffuse.a)
		discard;

	vector	vDissolve = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV);

	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 300.0f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	float	fClipAmount = vDissolve.r - g_fDissolveTime;

	clip(fClipAmount);

	//	Out.vDiffuse.a = fClipAmount;

	if (vDissolve.r >= g_fDissolveTime)
		Out.vDiffuse.a = 1.f;
	else
		Out.vDiffuse.a = 0.f;

	//if (vDissolve.r >= g_fDissolveTime - 0.05f && vDissolve.r <= g_fDissolveTime + 0.05f)
	//	Out.vDiffuse = vector(1.f, 0.f, 0.f, 1.f);

	//if (vDissolve.r >= g_fDissolveTime - 0.03f && vDissolve.r <= g_fDissolveTime + 0.03f)
	//	Out.vDiffuse = vector(1.f, 1.f, 0.f, 1.f);

	//if (vDissolve.r >= g_fDissolveTime - 0.025f && vDissolve.r <= g_fDissolveTime + 0.025f)
	//	Out.vDiffuse = vector(1.f, 1.f, 1.f, 1.f);

	if (vDissolve.r >= g_fDissolveTime - 0.05f && vDissolve.r <= g_fDissolveTime + 0.05f) //�������� ���� ������ ŭ
		Out.vDiffuse = vector(1.f, 0.f, 0.f, 1.f);

	if (vDissolve.r >= g_fDissolveTime - 0.03f && vDissolve.r <= g_fDissolveTime + 0.03f) //�������� ���� �ȿ� ����� ������ ����
		Out.vDiffuse = vector(1.f, 1.f, 0.f, 1.f);

	if (vDissolve.r >= g_fDissolveTime - 0.018f && vDissolve.r <= g_fDissolveTime + 0.018f) //����� ���� �ȿ� ����� ����
		Out.vDiffuse = vector(1.f, 1.f, 1.f, 1.f);

	return Out;
}

technique11		DefaultTechnique
{
	pass Rect
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}

	pass Dissolve //1
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN_DEFAULT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();
	}
}