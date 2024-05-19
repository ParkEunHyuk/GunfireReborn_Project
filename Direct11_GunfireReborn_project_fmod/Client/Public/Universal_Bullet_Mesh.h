#pragma once

#include "Client_Defines.h"
#include "UI.h"
#include "LandObject.h"
#include "Universal_Bullet.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
END

BEGIN(Client)

class CUniversal_Bullet_Mesh final : public CLandObject
{
public:
	enum MESHTYPE {
		BURN_BULLET,
		SUPPLEMENT_BULLET,
		METEORITE_BULLET,
		TERRAIN_BULLET,
		SHOCKWAVE_BULLET,
		SWORDWAVE_BULLET,
		CRACK,
		TORNADO_1,
		TORNADO_2,
		TRIANGLESWORD_BULLET,
		BLUNT_BULLET,
		BIDENT_SPEAR,
		BIG_SWORD,
		FATAL_RAPIER,
		SPHERE,
		LATERAL_BULLET,
		UPPER_BULLET,
		VERTICAL_BULLET,
		PHASE_TWO,
		REDSPHERE,
		SAURON,
		DEVIL,
		AWL,
		MACE_BULLET,
		MACE_BULLET_RIGHT,
		WARP,
		APPEAR,
		MESH_END
	};
public:
	typedef struct tagBullet_Universal_MeshDesc {
		_uint			iMeshNumber;
		OBSEVERGROUP	eType;

		_float			fSpeedPerSec;//총알 정보들
		_float3			vScale;
		_float3			vLook;
		_float4			vRotation;
		_float4			vTranslation;

		CGameObject*	pObject = nullptr; //플레이어가 아닌 몬스터 총알을 만들 때 쓰이는 정보
		_float3			fPositioning;			//만약 총알을 쓰는게 몬스터일 경우 vTranslation을 사용하지 않고 fPositioning를 사용한다. (트랜스폼은 게임 오브젝트로 가져온다.)

		_float3			fColliderSphereScale; //충돌체의 크기
		_float3			fColliderOBBScale;	//충돌체의 크기

		_double			dDuration; //지속시간
	}UNIVERSAL_BULLET_MESHDESC;
private:
	explicit CUniversal_Bullet_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUniversal_Bullet_Mesh(const CUniversal_Bullet_Mesh& rhs);
	virtual ~CUniversal_Bullet_Mesh() = default;

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
	HRESULT	SetUp_Collider(); //콜라이더를 셋팅중
	HRESULT	SetUp_Info();	//객체의 정보를 
	HRESULT	SetUp_Fire(_double TimeDelta);

private:
	//도깨비 보스의 함수들
	HRESULT	Burn_Bullet(_double TimeDelta); //도깨비의 불타는 총알
	HRESULT	Supplement_Bullet(_double TimeDelta);//도깨비의 추가 총알
	HRESULT	Meteorite_Bullet(_double TimeDelta); //도깨비의 운석 공격
	HRESULT	Terrain_Bullet(_double TimeDelta); //도깨비의 터레인 공격

	//베어 보스의 함수들
	HRESULT	SwordWave_Bullet(_double TimeDelta); //곰의 겸격
	HRESULT	ShockWave_Bullet(_double TimeDelta);//주변에 메쉬가 우뚝 솟아남
	HRESULT	Crack(_double TimeDelta); //균열
	HRESULT Tornado_Shield(_double TimeDelta); //토네이도 방패
	HRESULT	Tornado_Bullet(_double TimeDelta); //토네이도를 날림
	HRESULT	Sphere_Potal(_double TimeDelta); //구체 포탈 생성
	HRESULT Lateral_Bullet(_double TimeDelta); //횡베기 기술
	HRESULT	Upper_Bullet(_double TimeDelta); //올려베기 기술
	HRESULT	Vertical_Bullet(_double TimeDelta); //종베기 기술
	HRESULT	Red_Sphere(_double TimeDelta); //페이즈2 진입 의문의 구체
	HRESULT	Devil(_double TimeDelta); //악마 소환
	HRESULT	Sauron(_double TimeDelta); //사우론의 눈
	HRESULT	Awl(_double TimeDelta); //송곳
	HRESULT	Mace_Bullet(_double TimeDelta); //철퇴
	HRESULT	Mace_Bullet_Right(_double TimeDelta); //철퇴 오른쪽
	HRESULT	Warp(_double TimeDelta); //워프
	HRESULT Appear(_double TimeDelta); //등장씬
	HRESULT	Level_Change(); //레벨 체인지
	

private://ETC
	HRESULT	Slow_On();
	HRESULT	NavigationOn(_double TimeDelta); //네비게이션
	HRESULT	Renderer();

	HRESULT	Devil_Out_Particle();
	
private: // 옵저버 패턴
	virtual _int Obsever_Hit(void* pArg)override;
	virtual _int Obsever_Event(void* pArg)override;

private: //데드 패턴
	virtual	_int Dead_Trigger()override;


public:
	UNIVERSAL_BULLET_MESHDESC	m_Universal_Bullet_MeshDesc;

private://기본적인 멤버 변수들
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pModelCom = nullptr;
	CCollider*					m_pSphereCom = nullptr;
	CCollider*					m_pOBBCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	//플레이어 정보를 담고 있음
	class CTransform*			m_pPlayerTransform = nullptr;

	//보관용 변수
	_float4						m_fTempPos;
	_float3						m_fTempLook;

	_double						m_dDeltaTime = 0; //총알이 남아있는 지속시간
	_double						m_dEasingTime = 0;


	//슬로우를 위한 변수들
	CUniversal_Bullet*			m_pTempSlow_Bullet = nullptr;
	_double						m_dSlowTime = 1;
	_float						m_fSlowBulletDistance = 101;

	
	//메쉬 내부에서 무언갈 만들때 사용됨
	_double						m_dFireTime = 0;
	_bool						m_bFire = true;
	_uint						m_iFireCount = 0;

	//움직임을 표현할 때 사용됨
	_double						m_dMovingTime = 0;
	_uint						m_iMovingCount = 0;

	_uint						m_iRotation = 0;

	_bool						m_MeteoSwitch = false;


	//사운드
	_bool						m_bSoundSwitch = false;
	_double						m_dSoundTime = 10;

public:
	static CUniversal_Bullet_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END