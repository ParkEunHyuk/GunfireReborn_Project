#include "stdafx.h"
#include "..\Public\UI_Start_Button.h"
#include "GameInstance.h"
#include "Level.h"


CUI_Start_Button::CUI_Start_Button(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{

}

CUI_Start_Button::CUI_Start_Button(const CUI_Start_Button & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Start_Button::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Start_Button::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 233;
	m_fSizeY = 114;
	m_fX = g_iWinCX * 0.5f - 200.f; //내가 원하는 위치 쓰면 될듯?
	m_fY = g_iWinCY * 0.5f + 100.f;//내가 원하는 위치로

	//직교투영행렬
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));
	
	//레프트, 탑, 라이트, 바텀  //왼쪽 상단, 오른쪽 하단 
	m_rcRect.left = LONG(m_fX - m_fSizeX*0.5f);
	m_rcRect.top = LONG(m_fY - m_fSizeY*0.5f);
	m_rcRect.right = LONG(m_fX + m_fSizeX*0.5f);
	m_rcRect.bottom = LONG(m_fY + m_fSizeY*0.5f);

	m_fDepth = 1.f;

	return S_OK;
}

_int CUI_Start_Button::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	if (PtInRect(&m_rcRect, ptMouse))
		m_bOnMouse = true;
	else
		m_bOnMouse = false;


	if (m_bOnMouse == true)
	{
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
		{
			TextureChageNum = 2;
			if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Up)
			{
				CLevel* TempLevel = pGameInstance->Get_Level();
				TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE1);
				
			}
		}
		else
		{
			TextureChageNum = 1;
		}
	}
	else if (m_bOnMouse == false)//
	{
		TextureChageNum = 0;
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();

}

_int CUI_Start_Button::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI_Start_Button::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return S_OK;
}

HRESULT CUI_Start_Button::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXTEX"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Texture_UI_Start_Button"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	return S_OK;
}

HRESULT CUI_Start_Button::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", TextureChageNum)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CUI_Start_Button * CUI_Start_Button::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_Start_Button*	pInstance = new CUI_Start_Button(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUI_Start_Button");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Start_Button::Clone(void * pArg)
{
	CUI_Start_Button*	pInstance = new CUI_Start_Button(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUI_Start_Button");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Start_Button::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
