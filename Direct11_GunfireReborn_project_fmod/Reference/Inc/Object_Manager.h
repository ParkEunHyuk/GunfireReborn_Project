#pragma once

#include "Base.h"

/* ��ü���� ������ �����ؼ� ��Ƴ��´�. */
/* ������ ã�Ƽ� ��������, ��Ƽ� �����Ѵ�. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
public:
	CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iLayerIndex);
	class list<CGameObject*>* Get_ObjectList_from_Layer(_uint iLevelIndex, const _tchar* tagLayer);
public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype); /* ������ü�� �߰��Ѵ�. */
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg); /* �纻(���ӳ��� �����ؾ��� ��ü) ��ü�� �߰��Ѵ�. */
	HRESULT Clear_LevelObject(_uint iLevelIndex);

public:
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);
private:
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;
private:
	map<const _tchar*, class CLayer*>*			m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>	LAYERS;

	_uint			m_iNumLevels = 0;

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;

//IMGUI========================================================================================================================================================================================
#if defined(USE_IMGUI)
public:
	HRESULT Clear_LevelObject_For_Editer(_uint iLevelIndex);
	map<const _tchar*, class CGameObject*>* Get_Prototypes() {
		return &m_Prototypes;
	}

	map<const _tchar*, class CLayer*>* Get_Layers() const {
		return m_pLayers;
	}

#endif
//IMGUI========================================================================================================================================================================================
};

END