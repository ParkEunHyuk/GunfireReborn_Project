#include "stdafx.h"
#include "..\Public\Universal_MeshEffect.h"
#include "GameInstance.h"
#include "Player.h"
#include "Level.h"

const 	_tchar*	 m_pModel_Mesh_EffectTag[CUniversal_MeshEffect::MESH_EFFECT_END] = {
	TEXT("Prototype_Component_Model_MeshEffect_Lightning"),
	TEXT("Prototype_Component_Model_MeshEffect_Mist")
};

CUniversal_MeshEffect::CUniversal_MeshEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CUniversal_MeshEffect::CUniversal_MeshEffect(const CUniversal_MeshEffect & rhs)
	: CLandObject(rhs)
{
}

HRESULT CUniversal_MeshEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUniversal_MeshEffect::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Universal_Mesh_Effect_Desc, pArg, sizeof(UNIVERSAL_MESH_EFFECT_DESC));
	}
	else
	{
		MSGBOX("CUniversal_MeshEffect에 pArg가 Nullptr입니다.");
	}

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = m_Universal_Mesh_Effect_Desc.fSpeedPerSec;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	SetUp_Info();

	return S_OK;
}

_int CUniversal_MeshEffect::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	if (m_dDeltaTime > m_Universal_Mesh_Effect_Desc.dDuration)
	{
		return OBJ_DEAD;
	}

	m_dDeltaTime += TimeDelta;

	SetUp_MeshEffect(TimeDelta);




	return OBJ_NOEVENT;
}

_int CUniversal_MeshEffect::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f) || PHASE_TWO == m_Universal_Bullet_MeshDesc.iMeshNumber)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CUniversal_MeshEffect::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("CUniversal_MeshEffect의 Render()에 문제가 발생했습니다.");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, 0, i);
	}

	return S_OK;
}

HRESULT CUniversal_MeshEffect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_NonAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_pModel_Mesh_EffectTag[m_Universal_Mesh_Effect_Desc.iMeshEffectNumber], TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUniversal_MeshEffect::SetUp_ConstantTable()
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


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUniversal_MeshEffect::SetUp_Info()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pObjectTransform = static_cast<CTransform*>(m_Universal_Mesh_Effect_Desc.pObject->Get_Component(TEXT("Com_Transform")));

	m_pTransform->Scaled(XMLoadFloat3(&m_Universal_Mesh_Effect_Desc.vScale));

	_vector		vPosition = pObjectTransform->Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(pObjectTransform->Get_State(CTransform::STATE_RIGHT)) *m_Universal_Mesh_Effect_Desc.fPositioning.x;
	vPosition += XMVector3Normalize(pObjectTransform->Get_State(CTransform::STATE_UP)) * m_Universal_Mesh_Effect_Desc.fPositioning.y;
	vPosition += XMVector3Normalize(pObjectTransform->Get_State(CTransform::STATE_LOOK)) * m_Universal_Mesh_Effect_Desc.fPositioning.z;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, m_pTransform->Get_State(CTransform::STATE_POSITION));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUniversal_MeshEffect::SetUp_MeshEffect(_double TimeDelta)
{
	switch (m_Universal_Mesh_Effect_Desc.iMeshEffectNumber)
	{
	case LIGHTNING_EFFECT:
	{
		Lightning_Effect(TimeDelta);
		break;
	}
	case MIST_EFFECT:
	{
		Mist_Effect(TimeDelta);
		break;
	}
	}
	return S_OK;
}

HRESULT CUniversal_MeshEffect::Lightning_Effect(_double TimeDelta)
{
	_float4 fPos;

	XMStoreFloat4(&fPos,m_pTransform->Get_State(CTransform::STATE_POSITION));

	if (fPos.y > 0.f)
	{
		m_pTransform->Go_Down(TimeDelta *100);
	}
	else {
		fPos.y = 0;
		m_pTransform->Set_State(CTransform::STATE_POSITION,XMLoadFloat4(&fPos));
	}

	return S_OK;
}

HRESULT CUniversal_MeshEffect::Mist_Effect(_double TimeDelta)
{
	m_pObjectTransform = static_cast<CTransform*>(m_Universal_Mesh_Effect_Desc.pObject->Get_Component(TEXT("Com_Transform")));

	_vector vPosition = m_pObjectTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}


_int CUniversal_MeshEffect::Dead_Trigger()
{
	return _int();
}
CUniversal_MeshEffect * CUniversal_MeshEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUniversal_MeshEffect*	pInstance = new CUniversal_MeshEffect(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUniversal_MeshEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUniversal_MeshEffect::Clone(void * pArg)
{
	CUniversal_MeshEffect*	pInstance = new CUniversal_MeshEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUniversal_MeshEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUniversal_MeshEffect::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}