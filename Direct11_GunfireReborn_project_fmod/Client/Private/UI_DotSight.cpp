#include "stdafx.h"
#include "..\Public\UI_DotSight.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "Weapon_Sniper.h"

CUI_DotSight::CUI_DotSight(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{

}

CUI_DotSight::CUI_DotSight(const CUI_DotSight & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_DotSight::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_DotSight::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	//X = m_fX, Y = m_fY, Z = m_fSizeX, W = m_fSizeY;
	m_fRect = { (_float)g_iWinCX * 0.5f ,(_float)g_iWinCY * 0.5f ,(_float)50,(_float)50 };

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	m_fDepth = 1.f;

	return S_OK;
}

_int CUI_DotSight::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_dTimer = TimeDelta;

	Mouse_Input(TimeDelta);

	return _int();
}

_int CUI_DotSight::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	//m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
	//m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.9f, 1.f));

	Set_UI_Transform(m_fRect, g_iWinCX, g_iWinCY);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI_DotSight::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return S_OK;
}

HRESULT CUI_DotSight::Mouse_Input(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera_Player* Camera = static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")));
	CWeapon_Sniper* pTemp = static_cast<CWeapon_Sniper*> (pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Weapon_Sniper")));

	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press && false == pTemp->SetUp_Zoom())
	{
		m_fRect = { (_float)g_iWinCX * 0.5f ,(_float)g_iWinCY * 0.5f ,(_float)50,(_float)50 };
		Camera->Camera_Fovy(60.f);

		m_iFrameKey = 1;
	}
	else if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Press)
	{

		if(true == pTemp->SetUp_Zoom())
		{
			m_fRect = { (_float)g_iWinCX * 0.5f ,(_float)g_iWinCY * 0.5f ,(_float)g_iWinCX,(_float)g_iWinCY };
			Camera->Camera_Fovy(10.f);
			m_iFrameKey = 2;
		}
	}
	else {
		m_fRect = { (_float)g_iWinCX * 0.5f ,(_float)g_iWinCY * 0.5f ,(_float)50,(_float)50 };
		Camera->Camera_Fovy(60.f);
		m_iFrameKey = 0;
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CUI_DotSight::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DotSight"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_DotSight::SetUp_ConstantTable()
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

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", m_iFrameKey)))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->SetUp_OnShader_AutoFrame(m_pShaderCom, "g_DiffuseTexture", m_dTimer, 6)))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CUI_DotSight * CUI_DotSight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_DotSight*	pInstance = new CUI_DotSight(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUI_DotSight");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_DotSight::Clone(void * pArg)
{
	CUI_DotSight*	pInstance = new CUI_DotSight(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUI_DotSight");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_DotSight::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
