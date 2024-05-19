#include "stdafx.h"
#include "..\Public\UI_Monster_HpBar.h"
#include "GameInstance.h"


CUI_Monster_HpBar::CUI_Monster_HpBar(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{

}

CUI_Monster_HpBar::CUI_Monster_HpBar(const CUI_Monster_HpBar & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Monster_HpBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_HpBar::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pMonsterObject = reinterpret_cast<CLandObject*>(pArg);
		m_pMonsterTransform = static_cast<CTransform*>(m_pMonsterObject->Get_Component(TEXT("Com_Transform")));
	}
	if (nullptr == m_pMonsterObject || nullptr == m_pMonsterTransform )
	{
		MSGBOX("UI_Monster_HpBar와 m_pMonsterObject가 nullptr입니다.");
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));

	RELEASE_INSTANCE(CGameInstance);

	m_fSizeX = 4.f;
	m_fSizeY = 0.2;


	m_Monster_Info = m_pMonsterObject->Get_Info();
	m_fTickHp = m_fSizeX / m_pMonsterObject->m_Info.fMaxHp;

	m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));


	m_fDepth = 10.f;


	return S_OK;
}

_int CUI_Monster_HpBar::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;


	SetUp_Hp();
	SetUp_Pos(TimeDelta);

	//몬스터 체력 가져와서 저장 후
	//	이전 몬스터 체력 - 깍인 체력
	//	포지션도 깍인 체력만큼 - 시키켜주자

	if (m_pMonsterObject->m_Info.fHp <= 0.01)
	{
		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

_int CUI_Monster_HpBar::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

	return _int();
}

HRESULT CUI_Monster_HpBar::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 1);

	return S_OK;
}

HRESULT CUI_Monster_HpBar::SetUp_Pos(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vRight = m_pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
	_vector vUp = m_pPlayerTransform->Get_State(CTransform::STATE_UP);
	_vector vLook = m_pPlayerTransform->Get_State(CTransform::STATE_LOOK);

	vRight = m_pTransform->Get_Scale(CTransform::STATE_RIGHT) * XMVector3Normalize(vRight);
	vUp = m_pTransform->Get_Scale(CTransform::STATE_UP) * XMVector3Normalize(vUp);
	vLook = m_pTransform->Get_Scale(CTransform::STATE_LOOK) * XMVector3Normalize(vLook);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

	_vector vPosition = m_pMonsterTransform->Get_State(CTransform::STATE_POSITION);

	vPosition -= XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT))* (m_fDecreaseHp*0.5);
	vPosition += XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_UP)) * 3;

	//_vector test = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	//vPosition += test;
	//vPosition += XMVectorSetX(vPosition,(m_fDecreaseHp*0.5));
	//vPosition += XMVectorSetX(vPosition,(m_fDecreaseHp*0.5));

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);


	//이거 SR때 참고해서 빌보드 만드려고 했는데... 그냥 열받아서 내 방식대로 빌보드만듬...
	//m_ViewMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);

	////m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION),TimeDelta);

	//m_pTransform->Set_State(CTransform::STATE_RIGHT,XMLoadFloat3(&(*(_float3*)&m_ViewMatrix.m[0][0])));
	//m_pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat3(&(*(_float3*)&m_ViewMatrix.m[1][0])));
	//m_pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat3(&(*(_float3*)&m_ViewMatrix.m[2][0])));

	//m_pTransform->Set_State(CTransform::STATE_POSITION, m_pMonsterTransform->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

void CUI_Monster_HpBar::SetUp_Hp()
{
	if (m_Monster_Info.fHp != m_pMonsterObject->Get_Info().fHp)
	{
		if (m_pMonsterObject->Get_Info().fHp < 0)
		{
			m_Monster_Info.fHp = 0;
		}else
		{
			m_Monster_Info.fHp = m_pMonsterObject->Get_Info().fHp;
		}
	}
	
	_float fCurrentHp = m_fTickHp * m_Monster_Info.fHp;
	m_fDecreaseHp = m_fSizeX - fCurrentHp;
	m_pTransform->Scaled(XMVectorSet(fCurrentHp, m_fSizeY, 1.f, 0.0f));


}

HRESULT CUI_Monster_HpBar::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_HpBar"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_HpBar::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CUI_Monster_HpBar * CUI_Monster_HpBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_Monster_HpBar*	pInstance = new CUI_Monster_HpBar(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUI_Monster_HpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Monster_HpBar::Clone(void * pArg)
{
	CUI_Monster_HpBar*	pInstance = new CUI_Monster_HpBar(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUI_Monster_HpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Monster_HpBar::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
