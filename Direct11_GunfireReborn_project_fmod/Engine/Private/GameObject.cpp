#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Component.h"

const _tchar* CGameObject::m_pTransformTag = TEXT("Com_Transform");

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDeviceContext(rhs.m_pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	return Find_Component(pComponentTag);	
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc)
{
	m_eNowLevelNum = GetSingle(CGameInstance)->Get_NowLevelIndex();



	m_pTransform = CTransform::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pTransform)
		return E_FAIL;

	if (nullptr != pTransformDesc)
		m_pTransform->Set_TransformDesc(*pTransformDesc);

	if (nullptr != Find_Component(m_pTransformTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_Components.emplace(m_pTransformTag, m_pTransform);

	Safe_AddRef(m_pTransform);

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

_int CGameObject::Tick(_double TimeDelta)
{
	return _int();
}

_int CGameObject::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

_int CGameObject::Dead_Trigger()
{
	return _int();
}

_int CGameObject::Obsever_Hit(void* pArg)
{
	return _int();
}

_int CGameObject::Obsever_StrongHit(void* pArg)
{
	return _int();
}

_int CGameObject::Obsever_Event(void* pArg)
{
	return _int();
}

_int CGameObject::Obsever_Sound(void * pArg)
{
	return _int();
}

_float CGameObject::Get_CamDistance()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float fDistance;

	_vector CamPos = pGameInstance->Get_CamPosition();
	_vector vDis = m_pTransform->Get_Distance(CamPos);

	XMStoreFloat(&fDistance, vDis);
	
	RELEASE_INSTANCE(CGameInstance);

	return fDistance;
}

void CGameObject::Set_Dead(_uint _bDead)
{
	m_iDead = _bDead;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CComponent*			pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;	
}



void CGameObject::Free()
{
	Safe_Release(m_pTransform);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();
}
