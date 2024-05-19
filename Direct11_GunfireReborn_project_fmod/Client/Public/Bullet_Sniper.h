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

class CBullet_Sniper final : public CBullet
{
private:
	explicit CBullet_Sniper(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBullet_Sniper(const CBullet_Sniper& rhs);
	virtual ~CBullet_Sniper() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	HRESULT				SetUp_BulletPos();
	HRESULT				Hit_Effect();

	HRESULT				MeshEffect();

private: //옵저버 트리거
	virtual _int	Obsever_Hit(void* pArg = nullptr)override;
	virtual _int	Obsever_Sound(void* pArg = nullptr)override;

	virtual	_int	Dead_Trigger();

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	CTransform*			m_pWeaponTransformCom = nullptr;

	_double				m_dDeadTime = 0;

public:
	static CBullet_Sniper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject * Clone(void * pArg) override;
	virtual void Free() override;
};

END