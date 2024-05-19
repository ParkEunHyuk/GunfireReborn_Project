#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "UI_Bullet_Number.h"
#include "Camera_Player.h"

BEGIN(Engine)

class CWeapon abstract : public CGameObject
{
	enum WEAPONTYPE { PISHSWORD, PISTOL, SNIPER, SHOTGUN, LAUNCHER, CANNON, WEAPONEND };
protected:
	explicit CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc = nullptr);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

	void	Set_SlotNumber(_uint _iSlotNumber);

private:
	virtual  HRESULT			Set_WeaponPos();
public:
	void						SetUp_TransformOn(_bool _bTransformOn);
	
	CTransform*					m_pPlayerTransformCom = nullptr;
	_bool						m_bTransformOn = false;

public:
	CUI_Bullet_Number*			m_pBullet_Number = nullptr;
	_uint						m_iNumber[WEAPONEND] = {0,8,5,4,1,2 };
	_uint						m_iSlotNumber = 0;

public:
	CCamera_Player*				m_pCamera_Player = nullptr;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END