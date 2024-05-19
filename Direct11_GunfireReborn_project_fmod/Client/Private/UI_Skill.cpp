#include "stdafx.h"
#include "..\Public\UI_Skill.h"
#include "GameInstance.h"


CUI_Skill::CUI_Skill(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{

}

CUI_Skill::CUI_Skill(const CUI_Skill & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Skill::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Skill::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_UI_SkillDesc, pArg, sizeof(UI_SKILLDESC));
	}
	else
	{
		MSGBOX("UI_Skill에 pArg가 Nullptr입니다.");
	}

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//m_fX와 m_fY로 위치 조정하기
	m_fSizeX = m_UI_SkillDesc.fSizeX;
	m_fSizeY = m_UI_SkillDesc.fSizeY;
	m_fX = m_UI_SkillDesc.fX;	//(g_iWinCX * 0.5f + 525) = 1165;
	m_fY = m_UI_SkillDesc.fY;	//(g_iWinCY * 0.5f + 250) = 610;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	m_fDepth = m_UI_SkillDesc.fDepth;

	m_TickSizeY = m_UI_SkillDesc.fSizeY / (60 * m_UI_SkillDesc.dDuration);
	m_TickSizeX = m_UI_SkillDesc.fSizeX / (60 * m_UI_SkillDesc.dDuration);
	//깊이 조절

	
	return S_OK;
}

_int CUI_Skill::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	//_double test = 60 * TimeDelta;

	//m_TickSizeY = m_UI_SkillDesc.fSizeY / (test * m_UI_SkillDesc.dDuration);
	//m_TickSizeX = m_UI_SkillDesc.fSizeX / (test * m_UI_SkillDesc.dDuration);

	SetUp_Skill(TimeDelta);

	return m_iDead;
}

_int CUI_Skill::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f)); //Z로 앞 뒤 조절하는듯?
																																	 //m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(200, -200, 0.f, 1.f));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI_Skill::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return S_OK;
}

HRESULT CUI_Skill::SetUp_Skill(_double TimeDelta)
{
	switch (m_UI_SkillDesc.eType)
	{
	case ALPHA:
	{
		if (m_UI_SkillDesc.dDuration >= m_dDeltaTime)
		{
			m_dDeltaTime += TimeDelta;

			m_fSizeY -= m_TickSizeY;
			m_fSizeX -= m_TickSizeX;

		}
		else {
			Set_Dead(OBJ_DEAD);
		}
		break;
	}
	default:
		break;
	}

	return S_OK;
}

HRESULT CUI_Skill::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Skill"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	return S_OK;
}

HRESULT CUI_Skill::SetUp_ConstantTable()
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

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", m_UI_SkillDesc.eType)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CUI_Skill * CUI_Skill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_Skill*	pInstance = new CUI_Skill(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUI_Skill");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Skill::Clone(void * pArg)
{
	CUI_Skill*	pInstance = new CUI_Skill(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUI_Skill");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Skill::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
