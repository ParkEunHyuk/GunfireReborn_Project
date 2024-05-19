#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "Universal_Bullet.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CMonster_Cube final : public CLandObject
{
private:
	explicit CMonster_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Cube(const CMonster_Cube& rhs);
	virtual ~CMonster_Cube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT	SetUp_ConstantTable();

private:// 특정 이벤트들
	HRESULT	SetUp_Event_Tirgger(_double TimeDelta);
	HRESULT	SetUp_Fight(_double TimeDelta);
	HRESULT	Fight_On();
	HRESULT Once_Trigger();

private://애니메이션
	HRESULT	PlayAnim(_double TimeDelta);
	HRESULT	CoolTime_Manager(_double TimeDelta);
	HRESULT	Once_AnimMotion(_double TimeDelta);
	HRESULT	Infinity_AnimMotion(_double TimeDelta);

private://옵저버 트리거들
	virtual _int	Obsever_Hit(void* pArg = nullptr)override;
	virtual	_int	Obsever_Event(void* pArg = nullptr)override;
private:
	virtual	_int	Dead_Trigger();

private://유니버설 블렛을 위한 함수
	HRESULT	Universal_Boom();


private://기타 외 함수들
	HRESULT	Slow_On(); //슬로우에 관한 함수
	HRESULT	Notice(); //공격 당하면 주변 적들에게 알림

	HRESULT Boom_Particle();

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	//애니메이션 제어
	_bool				m_bOneTimeSwith = false;
	_uint				m_iOnceAnimNumber = 0;
	_uint				m_iInfinityAnimNumber = 0;

	//싸움 제어
	class CGameObject*		m_pPlayer = nullptr;
	class CTransform*		m_pPlayerTransform = nullptr;
	_bool					m_bFight = false;

	//애니메이션 동작 및 이벤트
	_double					m_dOnceCoolTime = 0;
	_double					m_dInfinityCoolTime = 0;
	_uint					m_iInfinityPattern = 0;
	_uint					m_iOncePattern = 0;
	_bool					m_MoveAndAttack = false;

	//거리
	_float								m_fDistance = 0;

	_bool								m_bOnce_Switch = true; //인게임 도중 한번만 실행시킬 불값

															   //슬로우 스킬을 위한 변수들
	_double				m_dSlowTime = 1;
	_float				m_fSlowBulletDistance = 101;
	CUniversal_Bullet* m_pTempSlow_Bullet = nullptr;

	_bool								m_bBoomOn = false;


public:
	static CMonster_Cube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END