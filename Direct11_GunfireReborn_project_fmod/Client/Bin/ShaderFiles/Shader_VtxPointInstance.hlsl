
#include "Shader_Defines.hpp"

cbuffer CameraDesc
{
	vector			g_vCamPosition;
};

cbuffer SizeDesc
{
	float			g_fSize = 1;
};

texture2D			g_DiffuseTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float		fPSize : PSIZE;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float		fPSize : PSIZE;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	Out.vPosition = mul(vPosition, g_WorldMatrix);
	Out.fPSize = In.fPSize;

	return Out;
}

struct GS_IN
{	
	float4		vPosition : POSITION;
	float		fPSize : PSIZE;	
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;	
	float2		vTexUV : TEXCOORD0;
};

[maxvertexcount(6)]
void GS_MAIN(in point GS_IN In[1], inout TriangleStream<GS_OUT> Trianglestream)
{
	GS_OUT			Out[4];

	float3		vLook = (g_vCamPosition - In[0].vPosition).xyz;
	float3		vAxisY = float3(0.f, 1.f, 0.f);
	float3		vRight = cross(vAxisY, vLook);
	float3		vUp = cross(vLook, vRight);

	vLook = normalize(vLook);
	vRight = normalize(vRight);
	vUp = normalize(vUp);
	//float	fHalfSize = In[0].fPSize * 0.5f; //선생님 코드
	float	fHalfSize = In[0].fPSize * 0.5f *  g_fSize; //내 코드

	matrix		matVP;
	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3		vPosition;

	vPosition = In[0].vPosition.xyz + vRight * fHalfSize + vUp * fHalfSize;
	Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[0].vTexUV = float2(0.f, 0.f);	

	vPosition = In[0].vPosition.xyz - vRight * fHalfSize + vUp * fHalfSize;
	Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[1].vTexUV = float2(1.f, 0.f);	

	vPosition = In[0].vPosition.xyz - vRight * fHalfSize - vUp * fHalfSize;
	Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[2].vTexUV = float2(1.f, 1.f);

	vPosition = In[0].vPosition.xyz + vRight * fHalfSize - vUp * fHalfSize;
	Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[3].vTexUV = float2(0.f, 1.f);

	Trianglestream.Append(Out[0]);
	Trianglestream.Append(Out[1]);
	Trianglestream.Append(Out[2]);
	Trianglestream.RestartStrip();

	Trianglestream.Append(Out[0]);
	Trianglestream.Append(Out[2]);
	Trianglestream.Append(Out[3]);
	Trianglestream.RestartStrip();
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	//Out.vColor.gb = 0.f;

	//은혁아 잘 봐라  Out.vColor.a < 0.9 이건 알파값 0.9이하를 다 자르는거고
	//위에 Out.vColor.gb = 0.f;는 그린과 블루를 전부 없애는거다.
	//그리고 Out.vColor.r = 1.f;를 통해 레드만 출력되겠끔 만든거다.
	//그래서 눈이 빨간색으로 나오는거다.
	//나중에 이거 쓰려면 바꿔줘야한다. 알파값 Out.vColor.a<0.1 정도로 해주고
	//	Out.vColor.gb = 0.f;랑 	Out.vColor.r = 1.f;이 제외시켜주면 될 듯?
	//그리고 보간을 통해 휘날리게 바꾸자
	if (Out.vColor.a < 0.9f)
		discard;

	//Out.vColor.r = 1.f;

	return Out;
}

technique11		DefaultTechnique
{
	pass Point
	{
		SetBlendState(NonBlending, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(ZTestAndWriteState, 0);
		SetRasterizerState(CullMode_ccw);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}	
}