#include "stdafx.h"
#include "..\Public\Sword.h"
#include "GameInstance.h"

CSword::CSword(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CSword::CSword(const CSword & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSword::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSword::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_SwordDesc, pArg, sizeof(SWORDDESC));

	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.0f)))))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransform->Scaled(XMVectorSet(0.1f, 0.1f, 0.1f, 1.f));
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.0f));


	m_BoneMatrixPtr = m_SwordDesc.pModel->Get_BoneMatrixPtr(m_SwordDesc.pSocketName);
	m_SocketTransformMatrix = m_SwordDesc.pModel->Get_TransformMatrix();

	return S_OK;
}

_int CSword::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	/* ¸ðµ¨ÀÇ »À : offsetMatrix, CombinedTransformationMatrix, TransformMatrix */

	_matrix			TransformMatrix = XMLoadFloat4x4(m_BoneMatrixPtr.pOffsetMatrix) * XMLoadFloat4x4(m_BoneMatrixPtr.pCombinedMatrix) * XMLoadFloat4x4(&m_SocketTransformMatrix) * m_SwordDesc.pTransform->Get_WorldMatrix();

	m_pColliderCom->Update_Transform(TransformMatrix);

	return _int();
}

_int CSword::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	return _int();
}

HRESULT CSword::Render()
{
	if (nullptr == m_pColliderCom)
		return E_FAIL;

	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();



	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, 1, i, "g_BoneMatrices");
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CSword::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.2f, 0.2f, 0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.0f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fork"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSword::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_matrix			TransformMatrix = XMLoadFloat4x4(m_BoneMatrixPtr.pOffsetMatrix) * XMLoadFloat4x4(m_BoneMatrixPtr.pCombinedMatrix) * XMLoadFloat4x4(&m_SocketTransformMatrix);

	TransformMatrix.r[0] = XMVector3Normalize(TransformMatrix.r[0]);
	TransformMatrix.r[1] = XMVector3Normalize(TransformMatrix.r[1]);
	TransformMatrix.r[2] = XMVector3Normalize(TransformMatrix.r[2]);

	TransformMatrix = m_pTransform->Get_WorldMatrix() * TransformMatrix *  m_SwordDesc.pTransform->Get_WorldMatrix();

	m_pShaderCom->Set_RawValue("g_SocketMatrix", &XMMatrixTranspose(TransformMatrix), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CSword * CSword::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CSword*	pInstance = new CSword(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSword::Clone(void * pArg)
{
	CSword*	pInstance = new CSword(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSword::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
}