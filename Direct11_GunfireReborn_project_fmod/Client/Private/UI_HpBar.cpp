#include "stdafx.h"
#include "..\Public\UI_HpBar.h"
#include "GameInstance.h"

CUI_HpBar::CUI_HpBar(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{

}

CUI_HpBar::CUI_HpBar(const CUI_HpBar & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_HpBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_HpBar::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//m_fX와 m_fY로 위치 조정하기
	m_fX = g_iWinCX * 0.5f - 310;
	m_fY = g_iWinCY * 0.5f + 250;
	m_fSizeX = 300.f;
	m_fSizeY = 20.f;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	//x좌표,y좌표 ,사이즈x 사이즈y
	m_fRect = { 330 , 610, 300,20 };

	m_fTempRect = m_fRect;

	Set_UI_Transform(m_fRect, g_iWinCX, g_iWinCY);

	//깊이 조절
	m_fDepth = 2.f;

	return S_OK;
}

_int CUI_HpBar::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;


	return _int();
}

_int CUI_HpBar::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI_HpBar::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return S_OK;
}

void CUI_HpBar::SetUp_HpBar(_float _fHpBarPercent)
{
	_float m_fHpSizeX = m_fTempRect.z * _fHpBarPercent;


	m_fRect.z -= m_fHpSizeX;
	m_fRect.x -= (m_fHpSizeX*0.5);

	// HP 수정할 때 꼭 읽어볼 것!!! 이건 기존에 있던 체력이 다 깎이면 사라지게 만든 코드이다.
	// 밑에 주석처리 하지 않는건 MP를 참고해서 만든것이므로 잘못된게 있으면 바로 잡도록 하자!!!
	//if (m_fRect.z <= 0 || m_fRect.z > m_fTempRect.z) // ||뒤에 m_fRect.z > m_fTempRect.z 추가함
	//{
	//	m_fRect.z = m_fTempRect.z;
	//	m_fRect.x = m_fTempRect.x;
	//}

	if (m_fRect.z <= 0)
	{
		m_fRect.z = 0.001;
		m_fRect.x = (m_fTempRect.x * 0.5) + 19.5;
	}
	else if (m_fRect.z > m_fTempRect.z)
	{
		m_fRect.z = m_fTempRect.z;
		m_fRect.x = m_fTempRect.x;
	}



	Set_UI_Transform(m_fRect, g_iWinCX, g_iWinCY);

}

HRESULT CUI_HpBar::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_HpBar"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	return S_OK;
}

HRESULT CUI_HpBar::SetUp_ConstantTable()
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



CUI_HpBar * CUI_HpBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_HpBar*	pInstance = new CUI_HpBar(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUI_HpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_HpBar::Clone(void * pArg)
{
	CUI_HpBar*	pInstance = new CUI_HpBar(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUI_HpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_HpBar::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
