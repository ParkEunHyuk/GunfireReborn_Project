#pragma once

#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CWeapon_Sniper final : public CWeapon
{
private:
	explicit CWeapon_Sniper(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CWeapon_Sniper(const CWeapon_Sniper& rhs);
	virtual ~CWeapon_Sniper() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	_bool	SetUp_Zoom();
private:
	HRESULT SetUp_Components();
	HRESULT	SetUp_ConstantTable();

	HRESULT	SetUp_Motion(_double TimeDelta);

private:
	virtual HRESULT		Set_WeaponPos();

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;

	CTransform*			m_pPlayerTransformCom = nullptr;

	_uint				m_iAnimation = 1;
	_uint				m_iNextAnimation = 1;

	_bool				m_bOneTimeSwith = false;
	_bool				m_bZoom = false;

	//탄알집 관련된 변수
	class CUI_Bullet_Number*	m_pGreenMagazine = nullptr;

public:
	static CWeapon_Sniper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END