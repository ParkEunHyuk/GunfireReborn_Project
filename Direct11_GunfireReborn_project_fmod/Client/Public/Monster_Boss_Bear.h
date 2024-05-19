#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "Universal_Bullet.h"
#include "Universal_Bullet_Mesh.h"
#include "Trail.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CMonster_Boss_Bear final : public CLandObject
{
private:
	explicit CMonster_Boss_Bear(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Boss_Bear(const CMonster_Boss_Bear& rhs);
	virtual ~CMonster_Boss_Bear() = default;

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
	HRESULT	Combo_AnimMotion(_double TimeDelta);

private://옵저버 트리거들
	virtual _int	Obsever_Hit(void* pArg = nullptr)override;
	virtual	_int	Obsever_Event(void* pArg = nullptr)override;
private:
	virtual	_int	Dead_Trigger();

private://몬스터 패턴들
	HRESULT	Appear(_double TimeDelta);// 등장 씬
	HRESULT	Jumping(_double TimeDelta); //점프 후 내려찍기
	HRESULT	Universal_Sword(_double TimeDelta); //발도
	HRESULT	Universal_Turn(_double TimeDelta);//돌려치기
	HRESULT	Universal_StrikeDown(_double TimeDelta); //그냥 내려찍기
	HRESULT	Universal_TripleAttack(_double TimeDelta); //보스의 트리플 어택


	//페이즈2
	HRESULT Phase_Two(_double TimeDelta);		//페이즈2 진입
	HRESULT Red_Sphere(_double TimeDelta);		//페이즈2 진입과 동시에 생기는 의문의 구체
	HRESULT	Jumping_PhaseTwo(_double TimeDelta); //점프 후 내려찍기 및 메쉬 생성
	HRESULT	Tornado_Shield(_double TimeDelta); //토네이도 방패를 생성함
	HRESULT	Tornado_Bullet(_double TimeDelta); //토네이도를 날림
	HRESULT	Sphere_Potal(_double TimeDelta); //구형 포탈을 만듬

	HRESULT	Rush_Attack_One(_double TimeDelta); //러쉬 어택 원
	HRESULT	Rush_Attack_Two(_double TimeDelta); //러쉬 어택 투
	HRESULT	Rush_Attack_Three(_double TimeDelta); //러쉬 어택 쓰리

	HRESULT TripleAttack_SwordWave(_double TimeDelta); //트리플 어택에서의 소드웨이브

	HRESULT	EyeOfSauron(_double TimeDelta);		//사우론의 눈 지금은 유니버셜 불렛 메쉬로 옮김 원형만 남겨두자

	HRESULT	Devil_Summons(_double TimeDelta); //악마 소환
	HRESULT	Mace_Summons(_double TimeDelta); //악마가 불렛을 소환함



private://트레일
	HRESULT	SetUp_Trail();

private://파티클
	HRESULT Devil_Summons_Particle();

private://실드와 관련된 함수
	HRESULT	SetUp_Shield();
	HRESULT	SetUp_Hp();

private://기타 외 함수들
	HRESULT	Slow_On(); //객체를 느리게 만듬
	HRESULT	Victory(); //플레이어가 이김(보스가 이긴게 아님!!!)
	HRESULT	Npc(); //NPC 소환 다른 클래스에선 Warp라는 이름으로 되어 있음!!!

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

	//추가콤보 또는 m_iOncePattern을 한번 더 사용해야할 때 사용
	_bool								m_bComboSwtich = false;
	_uint								m_iCurrentAnim = 0;
	_uint								m_iNextAnim = 0;
	_bool								m_bComboEnd = false;

	//점프 공격을 사용할 때 쓰이는 변수
	_bool								m_bLookAtOff = false;
	_float								m_fTempY = 0;


	//보스의 마지막 발악을 위한 변수
	_uint								m_iFinalAttack = 0;

	//새로운 스카이박스를 생성
	_bool								m_bSkyBoxOn = false;

	//악마 소환
	_bool								m_bDevilOn = false;

	//철퇴 소환
	_bool								m_bMace_BulletOn = false;
	_double								m_dMaceSummons_Time = 0;


private: //트레일
	CTrail*								m_pTrail = nullptr;


private://디졸브
	_float								m_fDissolveTime = 0.0f;
	_bool								m_bDissolveOn = false;

private://파티클
	_uint								m_ParticleCount = 0;
	_double								m_ParticleTime = 0;

private:
	_bool								m_bSoundSwitch = true;
	_double								m_dSoundTime = 0;


public:
	static CMonster_Boss_Bear* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END