#include "..\Public\Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(pComponentTag, iIndex);	
}

CGameObject * CObject_Manager::Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iLayerIndex)
{
	if (iLevelIndex >= m_iNumLevels || m_pLayers == nullptr)
	{
		MSGBOX("오브젝트 매니저에서 Get_GameObject()에서 문제가 발생");
		return nullptr;
	}

	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (pLayer == nullptr)
		return nullptr;

	return pLayer->Get_GameObject(iLayerIndex);
}

HRESULT CObject_Manager::Reserve_Container(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pLayers = new LAYERS[m_iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == pPrototype || 
		nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void* pArg)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;

		m_pLayers[iLevelIndex].insert(LAYERS::value_type(pLayerTag, pLayer));
	}
	else
	{
		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;
	}	

	return S_OK;
}

HRESULT CObject_Manager::Clear_LevelObject(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();

	return S_OK;
}

list<CGameObject*>* CObject_Manager::Get_ObjectList_from_Layer(_uint iLevelIndex, const _tchar * tagLayer)
{
	if (iLevelIndex >= m_iNumLevels || m_pLayers == nullptr)
	{
		MSGBOX("오브젝트 매니저에서 iLevelIndex가 맞지 않음");
		return nullptr;
	}

	CLayer* layer = Find_Layer(iLevelIndex, tagLayer);
	if (!layer)
		return nullptr;
	else
		return layer->Get_GameObjectList();
}

_int CObject_Manager::Tick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (0 > Pair.second->Tick(TimeDelta))
				return -1;
		}
	}

	return _int();
}

_int CObject_Manager::LateTick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (0 > Pair.second->LateTick(TimeDelta))
				return -1;
		}
	}

	return _int();
}

CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTagFinder(pPrototypeTag));

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();
}


//IMGUI========================================================================================================================================================================================
#if defined(USE_IMGUI)

HRESULT CObject_Manager::Clear_LevelObject_For_Editer(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	auto& iter = m_pLayers[iLevelIndex].begin();

	for (; iter != m_pLayers[iLevelIndex].end();)
	{
		if ((0 != lstrcmp(iter->first, L"Layer_BackGround")) &&
			(0 != lstrcmp(iter->first, L"Layer_Map_Static")) &&
			(0 != lstrcmp(iter->first, L"Layer_Map_Dynamic")) &&
			(0 != lstrcmp(iter->first, L"Layer_Camera")))
		{
			Safe_Release(iter->second);
			iter = m_pLayers[iLevelIndex].erase(iter);
		}
		else
		{
			++iter;
		}
	}

	return S_OK;

}
#endif
//IMGUI========================================================================================================================================================================================