#pragma once

#include "Client_Defines.h"
#include "Bullet.h"
#include "Universal_Bullet.h"

BEGIN(Engine)
class CShader;
class CModel;
class CRenderer;
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CBullet_Launcher final : public CBullet
{
private:
	explicit CBullet_Launcher(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBullet_Launcher(const CBullet_Launcher& rhs);
	virtual ~CBullet_Launcher() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

	virtual	_int	Dead_Trigger() override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	HRESULT				Set_GuidedBullets(_double TimeDelta);

	_bool				Set_BulletPos();

	HRESULT				Boom_Particle();

private: //옵저버 트리거
	virtual _int		Obsever_Hit(void* pArg = nullptr)override;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	CTransform*			m_pWeaponTransformCom = nullptr;

	class CCamera_Player*		m_Camera = nullptr;

	_double				m_dDeadTime = 0;


	_bool				m_bFire = false;

public:
	static CBullet_Launcher* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject * Clone(void * pArg) override;
	virtual void Free() override;
};

END