#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	if (iIndex >= m_Objects.size())
		return nullptr;

	auto	iter = m_Objects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);	
}

CGameObject * CLayer::Get_GameObject(_uint iLayerIndex)
{
	if (iLayerIndex >= m_Objects.size())
	{
		MSGBOX("오브젝트 사이즈가 맞지 않음");
		return nullptr;
	}

	auto iter = m_Objects.begin();

	for (_uint i = 0; i < iLayerIndex; ++iLayerIndex)
		iter++;
	return (*iter);
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	m_Objects.push_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_double TimeDelta)
{
	auto iter = m_Objects.begin();

	for (; iter != m_Objects.end();)
	{
		_int iEvent = (*iter)->Tick(TimeDelta);
		if (OBJ_DEAD == iEvent)
		{
			(*iter)->Dead_Trigger();
			Safe_Release(*iter);
			iter = m_Objects.erase(iter);
		}
		else {
			++iter;
		}
	}

	return 0;
}

_int CLayer::LateTick(_double TimeDelta)
{
	for (auto& pGameObject : m_Objects)
	{
		if (0 > pGameObject->LateTick(TimeDelta))
			return -1;
	}

	return 0;
}

CLayer * CLayer::Create()
{
	CLayer* pLayer = new CLayer();

	return pLayer;	
}

void CLayer::Free()
{
	for (auto& pGameObject : m_Objects)
		Safe_Release(pGameObject);

	m_Objects.clear();
}
