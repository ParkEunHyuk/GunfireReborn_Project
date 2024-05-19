#include "stdafx.h"
#include "Trail.h"
#include "GameInstance.h"
#include "Monster_Catfish.h"
#include "Monster_Horse.h"

#ifdef USE_IMGUI
#include "ImGui_Manager.h"
#endif

const 	_tchar*	 m_pTrailTag[CTrail::TRAIL_END] = {
	TEXT("Prototype_Component_Texture_Universal_Trail")
};

CTrail::CTrail(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CTrail::CTrail(const CTrail & rhs)
	: CGameObject(rhs)
{

}

HRESULT CTrail::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrail::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TrailDesc, pArg, sizeof(TRAILDESC));

	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pObject = static_cast<CGameObject*>(m_TrailDesc.pObject);
	m_pObjectModel = static_cast<CModel*>(m_pObject->Get_Component(TEXT("Com_Model")));

	m_BoneMatrixPtr = m_pObjectModel->Get_BoneMatrixPtr(m_TrailDesc.pSocketName);
	m_SocketTransformMatrix = m_pObjectModel->Get_TransformMatrix();


	MonsterLink();

	return S_OK;
}

_int CTrail::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	if (true == m_bStop_Compute_Trail)
	{
		return OBJ_DEAD;
	}

	Pivot_Matrix();

	VTXTEX* pVertices = m_pVIBufferCom->Vertex_Map();

	if (m_pVIBufferCom->Get_NumVertices() <= m_iVtxCnt)
	{
		_uint iRemoveCount = m_iVtxCnt / 6;
		m_iVtxCnt -= iRemoveCount;
		for (_uint i = 0; i < m_iVtxCnt; i += 2)
		{
			pVertices[i].vPosition = pVertices[iRemoveCount + i].vPosition;
			pVertices[i + 1].vPosition = pVertices[iRemoveCount + i + 1].vPosition;
		}
	}
	else
	{
		_vector vHighPos, vLowPos;

		vHighPos = XMVector3TransformCoord(XMLoadFloat3(&m_TrailDesc.vHighPos), XMLoadFloat4x4(&m_TransformMatrix));
		vLowPos = XMVector3TransformCoord(XMLoadFloat3(&m_TrailDesc.vLowPos), XMLoadFloat4x4(&m_TransformMatrix));


		XMStoreFloat3(&pVertices[m_iVtxCnt].vPosition, vLowPos);
		XMStoreFloat3(&pVertices[m_iVtxCnt + 1].vPosition, vHighPos);

		m_iVtxCnt += 2;

		for (_uint i = 0; i < m_iVtxCnt; i += 2)
		{
			pVertices[i].vTexUV = _float2((_float)i / ((_float)m_iVtxCnt - 2.f), 1.f);
			pVertices[i + 1].vTexUV = _float2((_float)i / ((_float)m_iVtxCnt - 2.f), 0.f);
		}
	}

	m_pVIBufferCom->Vertex_Unmap();

	return _int();
}

_int CTrail::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (false == m_isNotRendering)
	{
		//카메라 거리 재서 알파소팅하려고
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
	}

	return _int();
}

HRESULT CTrail::Render()
{
	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(m_pShaderCom, 2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrail::MonsterLink()
{
	switch (m_TrailDesc.eMonsterType)
	{
	case CATFISH:
	{
		static_cast<CMonster_Catfish*>(m_TrailDesc.pObject)->Set_Trail(this);
		break;
	}
	case HORSE:
	{
		static_cast<CMonster_Horse*>(m_TrailDesc.pObject)->Set_Trail(this);
		break;
	}
	default:
		break;
	}
	return S_OK;
}

HRESULT CTrail::Pivot_Matrix()
{
	_matrix TransformMatrix = XMLoadFloat4x4(m_BoneMatrixPtr.pOffsetMatrix) * XMLoadFloat4x4(m_BoneMatrixPtr.pCombinedMatrix) * XMLoadFloat4x4(&m_SocketTransformMatrix);

	TransformMatrix.r[0] = XMVector3Normalize(TransformMatrix.r[0]);
	TransformMatrix.r[1] = XMVector3Normalize(TransformMatrix.r[1]);
	TransformMatrix.r[2] = XMVector3Normalize(TransformMatrix.r[2]);

	XMStoreFloat4x4(&m_TransformMatrix, m_pTransform->Get_WorldMatrix() * TransformMatrix * m_TrailDesc.pTransform->Get_WorldMatrix());

	return S_OK;
}

HRESULT CTrail::SetUp_Components()
{

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXTEX"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_VIBuffer_RectTrail"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/*For.Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_pTrailTag[m_TrailDesc.iTrailTextureIndex], TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrail::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_TrailDesc.vColor, sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CTrail * CTrail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTrail* pInstance = new CTrail(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed To Creating CTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrail::Clone(void * pArg)
{
	CTrail* pInstance = new CTrail(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed To Copying CTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
