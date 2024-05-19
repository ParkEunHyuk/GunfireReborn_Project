#include "stdafx.h"
#include "..\Public\UI_MpBar.h"
#include "GameInstance.h"

CUI_MpBar::CUI_MpBar(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{

}

CUI_MpBar::CUI_MpBar(const CUI_MpBar & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_MpBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MpBar::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//m_fX와 m_fY로 위치 조정하기

	//m_fX = g_iWinCX * 0.5f - 330;
	//m_fY = g_iWinCY * 0.5f + 280;
	//m_fSizeX = 270.f;
	//m_fSizeY = 15.f;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	//x좌표,y좌표 ,사이즈x 사이즈y
	m_fRect = { 310 , 640, 270,15 };

	m_fTempRect = m_fRect;

	Set_UI_Transform(m_fRect,g_iWinCX,g_iWinCY);

	//깊이 조절
	m_fDepth = 2.f;

	return S_OK;
}

_int CUI_MpBar::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	Set_UI_Transform(m_fRect, g_iWinCX, g_iWinCY);

	return _int();
}

_int CUI_MpBar::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	//m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
	//m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f)); //Z로 앞 뒤 조절하는듯?
																																	 //m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(200, -200, 0.f, 1.f));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI_MpBar::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return S_OK;
}

_bool CUI_MpBar::Use_Mp(_float _fMpBarPercent)
{
	_float TempMp = m_fTempRect.z * _fMpBarPercent;

	if (m_fRect.z > TempMp)
	{
		return true;
	}
	else {
		return false;
	}
}

void CUI_MpBar::SetUp_MpBar(_float _fMpBarPercent)
{
	_float m_fMpSizeX = m_fTempRect.z * _fMpBarPercent;


	m_fRect.z -= m_fMpSizeX;
	m_fRect.x -= (m_fMpSizeX*0.5);

	if (m_fRect.z <= 0 )
	{
		m_fRect.z = 0.001;
		m_fRect.x = (m_fTempRect.x * 0.5) + 19.5;
	}else if (m_fRect.z > m_fTempRect.z)
	{
		m_fRect.z = m_fTempRect.z;
		m_fRect.x = m_fTempRect.x;
	}


	Set_UI_Transform(m_fRect, g_iWinCX, g_iWinCY);
}

HRESULT CUI_MpBar::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MpBar"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	return S_OK;
}

HRESULT CUI_MpBar::SetUp_ConstantTable()
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

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CUI_MpBar * CUI_MpBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_MpBar*	pInstance = new CUI_MpBar(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUI_MpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_MpBar::Clone(void * pArg)
{
	CUI_MpBar*	pInstance = new CUI_MpBar(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUI_MpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_MpBar::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
