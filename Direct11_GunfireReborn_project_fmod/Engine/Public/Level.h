#pragma once

#include "Base.h"

/* Ŭ���̾�Ʈ���� ������ �������� �θ� �Ǵ� Ŭ������. */

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
public:
	explicit CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();


	virtual	HRESULT Level_InGame_Chage(_bool Level_Chage_Switch, _uint _INextLevel);

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	_bool			m_bLevel_Switch = false;
	_uint			m_INextLevel;
	
public:
	virtual void Free() override;
};

END