#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "Universal_Bullet.h"
#include "Universal_Bullet_Mesh.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CMonster_Boss_Dokkaebi final : public CLandObject
{
private:
	explicit CMonster_Boss_Dokkaebi(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Boss_Dokkaebi(const CMonster_Boss_Dokkaebi& rhs);
	virtual ~CMonster_Boss_Dokkaebi() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT	SetUp_ConstantTable();

private:
	HRESULT	SetUp_UI();

private:// 특정 이벤트들
	HRESULT	SetUp_Event_Tirgger(_double TimeDelta);
	HRESULT	SetUp_Fight(_double TimeDelta);
	HRESULT	Fight_On();

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

private://몬스터 패턴들
	HRESULT	Jumping(_double TimeDelta);
	HRESULT	Universal_Bullet(_double TimeDelta); //총알 발사
	HRESULT	Universal_StrikeDown(_double TimeDelta);

	//페이즈2
	HRESULT	Jumping_PhaseTwo(_double TimeDelta);
	HRESULT	Universal_Bullet_PhaseTwo(_double TimeDelta);
	HRESULT	Universal_Bullet_SupplementAttack(_double TimeDelta);
	HRESULT	Meteorite_Bullet(_double TimeDelta);
	HRESULT	Terrain_Attack(_double TimeDelta);

private://실드와 관련된 함수
	HRESULT	SetUp_Shield();
	HRESULT	SetUp_Hp();

private://기타 외 함수들
	HRESULT	Slow_On(); //객체를 느리게 만듬
	HRESULT Warp();//워프기능을 만드는 함수
	HRESULT	Victory(); //플레이어가 이김(보스가 이긴게 아님!!!)

		//맵툴을 사용하지 않고 포지션을 다시 잡는함수
private:
	HRESULT MaptoolNotPosition();


private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	CTexture*			m_pDissolveTextrueCom = nullptr;

	//애니메이션 제어
	_bool				m_bOneTimeSwith = true;
	_uint				m_iOnceAnimNumber = 0;
	_uint				m_iInfinityAnimNumber = 1;

	//싸움 제어
	class CGameObject*		m_pPlayer = nullptr;
	class CTransform*		m_pPlayerTransform = nullptr;
	_bool					m_bFight = false;

	//애니메이션 동작 및 이벤트
	_double					m_dOnceCoolTime = 0;
	_double					m_dInfinityCoolTime = 0;
	_uint					m_iInfinityPattern = 0;
	_uint					m_iOncePattern = 99;

	//거리
	_float								m_fDistance = 0;

	//Bullet과 관련된 내용들
	_bool								m_bFire = true;  //한번만 발사하기 위한 불값
	_double								m_dFireTime = 0; //타이밍을 위한 더블형

	//슬로우 스킬을 위한 변수들
	_double								m_dSlowTime = 1;
	_float								m_fSlowBulletDistance = 101;
	CUniversal_Bullet*					m_pTempSlow_Bullet = nullptr;

	//페이즈에 관한 내용
	_bool								m_bPhase_One = true;

	//애니메이션을 중간에 자르거나 할 때 사용
	_double								m_dAnimCutTime = 0;
	_bool								m_bAnimCutOn = false;

	//점프 공격을 사용할 때 쓰이는 변수
	_bool								m_bJumpingOn = false;
	_float								m_fTempY = 0;

	//Universal_Bullet_ThreeAttack()를 위해 존재하는 변수
	_uint								m_iSupplementAttack = 0;

	//디졸브
	_float								m_fDissolveTime = 0.0f;

	_bool								m_bDissolveOn = false;


	//사운드 스위치
	_bool								m_bSoundSwitch = false;

public:
	static CMonster_Boss_Dokkaebi* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END