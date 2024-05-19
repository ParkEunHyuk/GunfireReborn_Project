#pragma once

#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CWeapon_Pistol final : public CWeapon
{
private:
	explicit CWeapon_Pistol(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CWeapon_Pistol(const CWeapon_Pistol& rhs);
	virtual ~CWeapon_Pistol() = default;

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

private:
	virtual HRESULT				Set_WeaponPos();

	HRESULT	Muzzle_Effect(); //√—±∏ ¿Ã∆Â∆Æ

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;

	_uint				m_iAnimation = 1;
	_uint				m_iNextAnimation = 1;

	_bool				m_bOneTimeSwith = false;


private:
	_bool				m_SoundOn = false;
	_double				m_SoundTime = 0;

public:
	static CWeapon_Pistol* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END