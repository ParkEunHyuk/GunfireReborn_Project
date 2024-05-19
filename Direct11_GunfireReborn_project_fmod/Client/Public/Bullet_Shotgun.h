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

class CBullet_Shotgun final : public CBullet
{
private:
	explicit CBullet_Shotgun(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBullet_Shotgun(const CBullet_Shotgun& rhs);
	virtual ~CBullet_Shotgun() = default;

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
	//_bool				Lock_Rotation(); 사용하진 않지만 고생 많이한거 코드 읽고 쓸만한거 가져가자
	HRESULT				Bullet_Shot(_double TimeDelta);
	HRESULT				SetUp_Collider();

private:
	HRESULT				Hit_Effect();

private: //옵저버 트리거
	virtual _int		Obsever_Hit(void* pArg = nullptr)override;
	virtual	_int		Dead_Trigger();


private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	CTransform*			m_pWeaponTransformCom = nullptr;
	_float				m_fRandom = 0;
	_float				m_fRandom2 = 0;

	_double				m_bDeadTime = 0;

public:
	static CBullet_Shotgun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject * Clone(void * pArg) override;
	virtual void Free() override;
};

END