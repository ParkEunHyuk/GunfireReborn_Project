#include "stdafx.h"
#include "..\Public\UI_Bullet_Number.h"
#include "GameInstance.h"

CUI_Bullet_Number::CUI_Bullet_Number(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{

}

CUI_Bullet_Number::CUI_Bullet_Number(const CUI_Bullet_Number & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Bullet_Number::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Bullet_Number::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//m_fX와 m_fY로 위치 조정하기

	if (nullptr == pArg)
	{
		m_fSizeX = 40;
		m_fSizeY = 40;
		m_fX = g_iWinCX * 0.5f + 462;
		m_fY = g_iWinCY * 0.5f + 330;

		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

		m_fDepth = 10.f;
		//깊이 조절

		m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f)); //Z로 앞 뒤 조절하는듯?
																																		 //m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(200, -200, 0.f, 1.f));
		m_fTempPos = { m_fX - (g_iWinCX * 0.5f),-m_fY + (g_iWinCY * 0.5f),0.f,1.f };

		m_UI_NumberDesc.eType = DEFAULT;
	}
	else {
		memcpy(&m_UI_NumberDesc, pArg, sizeof(UI_NUMBERDESC));

		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

		m_fDepth = 1.f;

		m_pTransform->Scaled(XMVectorSet(m_UI_NumberDesc.fSizeX, m_UI_NumberDesc.fSizeY, 1.f, 0.0f));
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_UI_NumberDesc.fX - (g_iWinCX * 0.5f), -m_UI_NumberDesc.fY + (g_iWinCY * 0.5f), 0.f, 1.f));


	}

	return S_OK;
}

_int CUI_Bullet_Number::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	switch (m_UI_NumberDesc.eType)
	{
	case GREENMAGAZINETENTH:
	{
		m_iNumber = g_iMagazine[GREENMAGAZINETENTH];
		break;
	}
	case GREENMAGAZINE:
	{
		m_bCountingNumberOn = SetUp_MagazineNumber(GREENMAGAZINETENTH, GREENMAGAZINE);
		m_iNumber = g_iMagazine[GREENMAGAZINE];
		break;
	}
	case BLUEMAGAZINETENTH:
	{
		m_iNumber = g_iMagazine[BLUEMAGAZINETENTH];
		break;
	}
	case BLUEMAGAZINE:
	{
		m_bCountingNumberOn = SetUp_MagazineNumber(BLUEMAGAZINETENTH, BLUEMAGAZINE);
		m_iNumber = g_iMagazine[BLUEMAGAZINE];
		break;
	}
	default:
		break;
	}

	return _int();
}

_int CUI_Bullet_Number::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI_Bullet_Number::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return S_OK;
}

void CUI_Bullet_Number::Set_Number(_uint _iNumber)
{
	m_iNumber = _iNumber;
}

void CUI_Bullet_Number::Set_Magazine(_uint	_iMagazine)
{
	g_iMagazine[m_UI_NumberDesc.eType] += _iMagazine;
}

_bool CUI_Bullet_Number::SetUp_MagazineNumber(_uint	_iTenth, _uint _Once)
{
	if (10 <= g_iMagazine[_Once])
	{
		if (9 != g_iMagazine[_iTenth])
		{
			g_iMagazine[_iTenth] += 1;
			g_iMagazine[_Once] = 0;
			return true;
		}
		else {
			g_iMagazine[_iTenth] = 9;
			g_iMagazine[_Once] = 9;
			return true;
		}
	}

	if (-1 >= g_iMagazine[_Once])
	{
		if (0 != g_iMagazine[_iTenth])
		{
			g_iMagazine[_iTenth] -= 1;
			g_iMagazine[_Once] = 9;
			return true;
		}
		else {
			g_iMagazine[_iTenth] = 0;
			g_iMagazine[_Once] = 0;
			return false;
		}
	}

	if (0 == g_iMagazine[_iTenth] && 0 == g_iMagazine[_Once])
	{
		return false;
	}
	else {
		return true;
	}
}

HRESULT CUI_Bullet_Number::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Bullet_Number"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	return S_OK;
}

HRESULT CUI_Bullet_Number::SetUp_ConstantTable()
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

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", m_iNumber)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CUI_Bullet_Number * CUI_Bullet_Number::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_Bullet_Number*	pInstance = new CUI_Bullet_Number(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUI_Bullet_Number");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Bullet_Number::Clone(void * pArg)
{
	CUI_Bullet_Number*	pInstance = new CUI_Bullet_Number(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUI_Bullet_Number");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Bullet_Number::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
