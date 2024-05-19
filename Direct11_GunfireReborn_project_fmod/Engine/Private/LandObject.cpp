#include "..\Public\LandObject.h"
#include "GameInstance.h"

CLandObject::CLandObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CLandObject::CLandObject(const CLandObject & rhs)
	: CGameObject(rhs)
{

}

HRESULT CLandObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::NativeConstruct(const CTransform::TRANSFORMDESC * pTransformDesc)
{
	if (FAILED(__super::NativeConstruct(pTransformDesc)))
		return E_FAIL;

	return S_OK;
}

_int CLandObject::Tick(_double TimeDelta)
{
	return _int();
}

_int CLandObject::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CLandObject::Render()
{
	return S_OK;
}

void CLandObject::SetUp_OnTerrain(_uint iLevelID, const _tchar* pTerrainLayerTag, _uint iIndex, const _tchar* pBufferComTag)
{
	CObject_Manager*		pObject_Manager = GET_INSTANCE(CObject_Manager);

	CTransform*		pTerrainTransform = (CTransform*)pObject_Manager->Get_Component(iLevelID, pTerrainLayerTag, CGameObject::m_pTransformTag, iIndex);
	if (nullptr == pTerrainTransform)
		return;

	CVIBuffer_Terrain*		pVIBuffer = (CVIBuffer_Terrain*)pObject_Manager->Get_Component(iLevelID, pTerrainLayerTag, pBufferComTag, iIndex);
	if (nullptr == pVIBuffer)
		return;

	_vector		vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_matrix		WorldMatrix = pTerrainTransform->Get_WorldMatrix();
	_matrix		WorldMatrixInverse = XMMatrixInverse(nullptr, WorldMatrix);

	vPosition = pVIBuffer->Get_Height(XMVector3TransformCoord(vPosition, WorldMatrixInverse));
	vPosition = XMVector3TransformCoord(vPosition, WorldMatrix);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CObject_Manager);
}

HRESULT CLandObject::SetUp_OnNavigation(CNavigation * pNavigation, _uint iCurrentLevel, const _tchar* pLayerTerrainTag, _uint iIndex)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pTerrainTransform = (CTransform*)pGameInstance->Get_Component(iCurrentLevel, pLayerTerrainTag, CGameObject::m_pTransformTag, iIndex);
	if (nullptr == pTerrainTransform)
	{
		MSGBOX("Failed To Find TerrainTransform in CLandObject");
		return E_FAIL;
	}

	_vector		vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_matrix		WorldMatrix = pTerrainTransform->Get_WorldMatrix();
	_matrix		WorldMatrixInverse = XMMatrixInverse(nullptr, WorldMatrix);

	vPosition = pNavigation->Get_Height(XMVector3TransformCoord(vPosition, WorldMatrixInverse));
	vPosition = XMVector3TransformCoord(vPosition, WorldMatrix);

	//vPosition.m128_f32[1] = 1.5f;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLandObject::Free()
{
	__super::Free();

}
