#pragma once

#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CWeapon_Cannon final : public CWeapon
{
private:
	explicit CWeapon_Cannon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CWeapon_Cannon(const CWeapon_Cannon& rhs);
	virtual ~CWeapon_Cannon() = default;

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
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;


	_uint				m_iAnimation = 0;
	_uint				m_iNextAnimation = 0;

	_bool				m_bOneTimeSwith = false;

	_bool				m_bNotBullet = false;//총알이 없을 경우


											 //탄알집 관련된 변수
	class CUI_Bullet_Number*	m_pBlueMagazine = nullptr;


private:
	_double				m_SoundTime = 5;


public:
	static CWeapon_Cannon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END