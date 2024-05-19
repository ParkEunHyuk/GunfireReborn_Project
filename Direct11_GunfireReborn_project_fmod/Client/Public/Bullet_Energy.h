#pragma once

#include "Client_Defines.h"
#include "Bullet.h"

BEGIN(Engine)
class CShader;
class CModel;
class CRenderer;
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CBullet_Energy final : public CBullet
{
private:
	explicit CBullet_Energy(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBullet_Energy(const CBullet_Energy& rhs);
	virtual ~CBullet_Energy() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

	virtual	_int	Dead_Trigger() override;

	HRESULT		Fire_Input(_double TimeDelta);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	_bool				SetUp_BulletPos();

	HRESULT				SetUp_Magnet(_double TimeDelta); //이거 젤다의 자석처럼 움직여짐
	HRESULT				SetUp_Collider();

private: //옵저버 트리거
	virtual _int		Obsever_Hit(void* pArg = nullptr)override;
	virtual	_int		Obsever_Event(void* pArg = nullptr)override;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	CTransform*			m_pWeaponTransformCom = nullptr;

	_bool				m_bFire = false;

	_bool				m_bColliderOn = false;

	_double				m_dDeadTime = 0;
	_double				m_Energy_Damage = 0;

	_bool				m_bNavigationDead = false;

public:
	static CBullet_Energy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject * Clone(void * pArg) override;
	virtual void Free() override;
};

END