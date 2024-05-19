#include "stdafx.h"
#include "..\Public\UI_MonsterBar.h"
#include "GameInstance.h"

CUI_MonsterBar::CUI_MonsterBar(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{

}

CUI_MonsterBar::CUI_MonsterBar(const CUI_MonsterBar & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_MonsterBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MonsterBar::NativeConstruct(void* pArg)
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

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));

	RELEASE_INSTANCE(CGameInstance);


	m_pTransform->Scaled(XMVectorSet(5, 1, 1.f, 0.0f));

	m_fDepth = 2.f;

	return S_OK;
}

_int CUI_MonsterBar::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	SetUp_Pos(TimeDelta);


	if (m_pMonsterObject->m_Info.fHp <= 0.01)
	{
		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

_int CUI_MonsterBar::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;



	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

	return _int();
}

HRESULT CUI_MonsterBar::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 1);

	return S_OK;
}

HRESULT CUI_MonsterBar::SetUp_Pos(_double TimeDelta)
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

	vPosition += XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * 0.005;
	vPosition += XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_UP)) * 3;

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

HRESULT CUI_MonsterBar::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterBar"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MonsterBar::SetUp_ConstantTable()
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



CUI_MonsterBar * CUI_MonsterBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_MonsterBar*	pInstance = new CUI_MonsterBar(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUI_MonsterBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_MonsterBar::Clone(void * pArg)
{
	CUI_MonsterBar*	pInstance = new CUI_MonsterBar(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUI_MonsterBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_MonsterBar::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
