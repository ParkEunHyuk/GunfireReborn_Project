#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "Weapon.h"


BEGIN(Engine)
///////////////////////////셰이더에 쓰이지 않음 나중에 위치 확인할때만 확인하자
//class CShader;
//class CTexture;
class CRenderer;
//class CVIBuffer_Cube;
////////////////////////////////////////////////////
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CPlayer final : public CLandObject
{
	enum WEAPONTYPE {PISHSWORD,PISTOL,SNIPER,SHOTGUN,LAUNCHER,CANNON,WEAPONEND};
	typedef	struct tagJumpDesc
	{
		_bool	bJump;
		_float	fJumpY, fJumpPower;
		_double	dTime;
	}JUMPDESC;
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

//키 입력 관련
private:
	HRESULT				Key_Input(_double TimeDelta);
	HRESULT				Moving_Input(_double TimeDelta);
	HRESULT				Mouse_Input(_double TimeDelta);
	HRESULT				Fast_Moving_Input(_double TimeDelta);
	HRESULT				Event_Input(_double TimeDelta);
	HRESULT				Jumping(_double TimeDelta);
	HRESULT				Skill_Input(_double TimeDelta);

private:
	_bool				Lock_Rotation(_float3 vAxis, _float fDegree); //카메라 회전 방향

//빠르게 움직이기
private:
	_bool				Go_Interpolation_Straight(_float fTime,_double TimeDelta,_float fAcceleration); //최대시간,델타타임, 가속도
	_bool				Go_Interpolation_Right(_float fTime, _double TimeDelta, _float fAcceleration);
	_bool				Go_Interpolation_Left(_float fTime, _double TimeDelta, _float fAcceleration);
	_bool				Go_Interpolation_Backward(_float fTime, _double TimeDelta, _float fAcceleration);

	//쿨타임
private:
	HRESULT				CoolTime_Manager(_double TimeDelta);

	//무기
private:
	HRESULT				SetUp_Weapon();
	HRESULT				Weapon_Slot(_double TimeDelta);

private://UI
	HRESULT				SetUp_UI();

private://Effect
	HRESULT	Hit_Effect(); //맞을때 나올 이펙트
	HRESULT	DashStraight_Effect(); //전진시 나오는 이펙트
	HRESULT	DashRight_Effect(); //오른쪽 대쉬시 나오는 이펙트
	HRESULT	DashLeft_Effect(); //오른쪽 대쉬시 나오는 이펙트
	HRESULT Environment_Particle();// 환경 파티클
	HRESULT	Mist_MeshEffect(); //안개 효과

	//옵저버 패턴
private:
	virtual	_int		Obsever_Hit(void* pArg = nullptr)override;


	//기타 외 기능
private:
	_bool Once_Trigger(); //한번만 도는 트리거 
	HRESULT	PivotMatrix(); //피벗
	HRESULT	Level_Change(); //씬 전환
	HRESULT Warp();//워프기능을 만드는 함수
	HRESULT	Torchlight();
	HRESULT Torchlight_Pos_Stage1();
	HRESULT Torchlight_Pos_Stage1_BossRoom();
	HRESULT Torchlight_Pos_Stage2();
	HRESULT Torchlight_Pos_Stage2_BossRoom();

	//맵툴을 사용하지 않고 포지션을 다시 잡는함수
private:
	HRESULT MaptoolNotPosition();

private:
	///////////////////////////셰이더에 쓰이지 않음 나중에 위치 확인할때만 확인하자
	//CShader*			m_pShaderCom = nullptr;
	//CTexture*			m_pTextureCom = nullptr;
	//CVIBuffer_Cube*		m_pVIBufferCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	////////////////////////////////////////////////////
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	//점프하기
private:
	JUMPDESC				m_JumpDesc;

	//빠르게 움직이기
private:
	_bool					m_bGoInterpolationSwitch[4] = { false ,false ,false ,false };
	_bool					m_bMovingSwitch = false;
	_double					m_dMovingTime = 0;
	_float3					m_fFastMoving;

	//무기
private:
	CWeapon*				m_pWeapon[WEAPONEND];

	_bool					Weapon_SetUp = false;
	_bool					m_bInUseWeapon[WEAPONEND]={ false,false, false, false, false, false };
	class CUI_Weapon*		m_pUI_Weapon = nullptr;

	//체력
private:
	class CUI_MpBar*		m_pMpBar = nullptr;
	class CUI_HpBar*		m_pHpBar = nullptr;


private://스킬 쿨타임
	_double					m_dFastCoolTime = 0;
	_bool					m_bFastSwitch = true;

	_double					m_dReflectCoolTime = 0;
	_bool					m_bReflectSwitch = true;

	_double					m_dSlowTimeCoolTime = 0;
	_bool					m_bSlowTimeSwitch = true;



	//기타 외 기능
private:
	_bool					m_bOnce_Switch = false;

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END