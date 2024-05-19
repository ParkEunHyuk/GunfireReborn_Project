#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Player final : public CCamera
{
	explicit CCamera_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera_Player(const CCamera_Player& rhs);
	virtual ~CCamera_Player() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	void		Camera_Fovy(_float _Radians);

private:
	HRESULT		Set_CameraPos();
	void		FocusCursorToScreen();

public:
	HRESULT		Start_CameraQuake(_uint _iQuake, _double _fQuakeTime);// 지진범위와 지진 지속시간

private:
	HRESULT		Camera_Quaking(_double TimeDelta);

	_double			m_dQuakeTime = 0.f;		//지속시간
	_bool			m_bQuakeStart = false;	//지진 Yes of No
	_uint			m_iQuake = 0;			//범위
	_double			m_dQuakeMaxTime = 0.f;	//최대 시간

private:
	CTransform*		m_pPlayerTransformCom = nullptr;

public:
	static CCamera_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END