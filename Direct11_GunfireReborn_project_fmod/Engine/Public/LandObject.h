#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLandObject abstract : public CGameObject
{
public:
	typedef struct tagInfoDesc
	{
		_float	fHp;
		_float	fMaxHp;
		_float	fShield;
		_float	fMaxShield;
	}INFODESC;
protected:
	CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc = nullptr);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	void SetUp_OnTerrain(_uint iLevelID, const _tchar* pTerrainLayerTag, _uint iIndex, const _tchar* pBufferComTag);
	HRESULT	SetUp_OnNavigation(CNavigation* pNavigation, _uint iCurrentLevel, const _tchar* pLayerTerrainTag, _uint iIndex = 0);

public:
	INFODESC			Get_Info() { return m_Info; }
	_float				Get_Shield() { return m_Info.fShield; }
	_float				Get_MaxShield() { return m_Info.fMaxShield; }
	_float				Get_Hp() { return m_Info.fHp; }
	_float				Get_MaxHp() { return m_Info.fMaxHp; }


	INFODESC			m_Info;



public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END