#pragma once

#include "Client_Defines.h"
#include "UI.h"
#include "LandObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)

class CUniversal_Bullet final : public CLandObject
{
public:
	enum BULLETTYPE{
		REFLECT_SKILL,
		SLOWTIME_SKILL,
		LAUNCHER_BOOM,
		LIZARD, //리자드 총알,
		GHOSTDEER,
		POWERLIZARD,
		RAVEN,
		YELLOW_POTAL,
		MELEE_ATTACK,
		DOKKAEBI_ATTACK,
		DOKKAEBI_STRIKEDOWN,
		DOKKAEBI_TERRAIN_ATTACK_GUIDED,
		BEAR_ATTACK,
		CUBE_BOOM,
		NONTEXTURE,
		NOTICE,
		BULLET_END
	};

public:
	typedef struct tagUniversal_BulletDesc
	{
		_uint			iBulletNumber;   //_uint형으로 불렛 타입을 지정 중
		OBSEVERGROUP	eType;			//enum형으로 옵저버 타입을 지정


		_float			fSpeedPerSec;//총알 정보들
		_float3			vScale;
		_float3			vLook;
		_float4			vRotation;
		_float4			vTranslation;

		CGameObject*	pObject = nullptr; //플레이어가 아닌 몬스터 총알을 만들 때 쓰이는 정보
		_float3			fPositioning;			//만약 총알을 쓰는게 몬스터일 경우 vTranslation을 사용하지 않고 fPositioning를 사용한다. (트랜스폼은 게임 오브젝트로 가져온다.)

		_float3			fColliderSphereScale; //충돌체의 크기
		_float3			fColliderOBBScale;	//충돌체의 크기

		_uint			iTextureIndex = 0;

		_double			dDuration; //지속시간
	}UNIVERSAL_BULLETDESC;
private:
	explicit CUniversal_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUniversal_Bullet(const CUniversal_Bullet& rhs);
	virtual ~CUniversal_Bullet() = default;

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
	HRESULT	SetUp_Collider();


private:	//Bullet 준비단계
	HRESULT	SetUp_Info();
	HRESULT	SetUp_Fire(_double TimeDelta); //!@#!#!#@!#!@$찾기 쉽게

private: //Bullet들
	HRESULT	Reflect_Skill(_double TimeDelta); //플레이어 스킬
	HRESULT	SlowTime_Skill(_double TimeDelta); //플레이어 스킬
	HRESULT	Launcher_Boom(_double TimeDelta); //런처 붐
	HRESULT Lizard(_double TimeDelta);		//리자드의 총알 공격
	HRESULT	GhostDeer(_double TimeDelta); //고스트디어의 총알 공격
	HRESULT	PowerLizard(_double TimeDelta); //파워리자드의 총알 공격
	HRESULT	Raven(_double TimeDelta);	//레이븐의 총알 공격
	HRESULT	Cube_Boom(_double TimeDelta); //큐브 붐
	HRESULT	Dokkaebi_Attack(_double TimeDelta); //도깨비의 점프 후 충격파
	HRESULT Dokkaebi_StrikeDown(_double TimeDelta); //도깨비의 스트라이크 다운
	HRESULT	Dokkaebi_Terrain_Guided(_double TimeDelta); //이걸 통해 터레인으로 공격을 진행함
	HRESULT	Bear_Attack(_double TimeDelta); //베어의 어택
	HRESULT	Yellow_Potal(_double TimeDelta); //엘로우 포탈을 생성시킴 그것 말고는 특별한 기능은 없음
	HRESULT	NonTexture(_double TimeDelta); //텍스쳐가 없는것들(근접공격용)


private:
	HRESULT	Slow_Particle();

private:
	HRESULT				Billboard(); //빌보드

private://기타 외 함수들
	HRESULT	Slow_On(); //슬로우 스킬의 On Off 기능
	HRESULT	NavigationOn(_double TimeDelta);


private: // 옵저버 패턴
	virtual _int	Obsever_Hit(void* pArg)override;
	virtual _int	Obsever_Event(void* pArg)override;

private: //데드 패턴
	virtual	_int Dead_Trigger()override;


public:
	_bool	Get_NotMesh() { return m_bNotMesh; }

public:
	UNIVERSAL_BULLETDESC	m_Universal_BulletDesc;

	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;


private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;


	class CTransform*	m_pPlayerTransform = nullptr;

	_float4				m_fTempPos;
	_float3				m_fTempLook;
	

	_double				m_dDeltaTime = 0; //지속시간을 체크하기 위한 변수
	_double				m_dTextureTime = 0; //텍스쳐의 교체 타이밍을 위한 변수

	_double				m_dSlowTime = 1;
	_float				m_fSlowBulletDistance = 101;

	CUniversal_Bullet* m_pTempSlow_Bullet = nullptr;

	//GhostDeer를 위한 플룻값
	_float m_fDegrees = 0;

	//메쉬도 튕겨내기 위해 필요한 불값
	_bool				m_bNotMesh = true;

	//Terrain_Bullet을 생성하기 위한 변수
	_double				m_dFireTime = 0;


	_bool				m_bSlowParticleOn = false; //슬로우 파티클을 위한 변수


public:
	static CUniversal_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END
