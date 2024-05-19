#pragma once

#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CWeapon_Launcher final : public CWeapon
{
private:
	explicit CWeapon_Launcher(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CWeapon_Launcher(const CWeapon_Launcher& rhs);
	virtual ~CWeapon_Launcher() = default;

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

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;

	CTransform*			m_pPlayerTransformCom = nullptr;

	_uint				m_iAnimation = 1;
	_uint				m_iNextAnimation = 1;

	_bool				m_bOneTimeSwith = false;

	//탄알집 관련된 변수
	class CUI_Bullet_Number*	m_pBlueMagazine = nullptr;


private:
	_bool				m_SoundOn = false;
	_double				m_SoundTime = 0;


public:
	static CWeapon_Launcher* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END