#pragma once

#include "Client_Defines.h"
#include "Weapon.h"
#include "Universal_Bullet.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CWeapon_PishSword	final : public CWeapon
{
private:
	explicit CWeapon_PishSword(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CWeapon_PishSword(const CWeapon_PishSword& rhs);
	virtual ~CWeapon_PishSword() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT	SetUp_ConstantTable();
	HRESULT	SetUp_Motion(_double TimeDelta);

//private://트레일
//	HRESULT	SetUp_Trail();

private:
	virtual HRESULT				Set_WeaponPos();

private:
	HRESULT				Attack(_double TimeDelta); //공격

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;

	CTransform*			m_pPlayerTransformCom = nullptr;

	_uint				m_iAnimation = 0;
	_uint				m_iNextAnimation = 0;

	_bool				m_bOneTimeSwith = false;

	_double				m_dAttackTime = 0;
	_bool				m_bAttack = true;

//private: //트레일
//	CTrail*								m_pTrail = nullptr;

public:
	static CWeapon_PishSword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END