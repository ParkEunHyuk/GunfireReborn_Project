#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "Weapon.h"


BEGIN(Engine)
///////////////////////////���̴��� ������ ���� ���߿� ��ġ Ȯ���Ҷ��� Ȯ������
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

//Ű �Է� ����
private:
	HRESULT				Key_Input(_double TimeDelta);
	HRESULT				Moving_Input(_double TimeDelta);
	HRESULT				Mouse_Input(_double TimeDelta);
	HRESULT				Fast_Moving_Input(_double TimeDelta);
	HRESULT				Event_Input(_double TimeDelta);
	HRESULT				Jumping(_double TimeDelta);
	HRESULT				Skill_Input(_double TimeDelta);

private:
	_bool				Lock_Rotation(_float3 vAxis, _float fDegree); //ī�޶� ȸ�� ����

//������ �����̱�
private:
	_bool				Go_Interpolation_Straight(_float fTime,_double TimeDelta,_float fAcceleration); //�ִ�ð�,��ŸŸ��, ���ӵ�
	_bool				Go_Interpolation_Right(_float fTime, _double TimeDelta, _float fAcceleration);
	_bool				Go_Interpolation_Left(_float fTime, _double TimeDelta, _float fAcceleration);
	_bool				Go_Interpolation_Backward(_float fTime, _double TimeDelta, _float fAcceleration);

	//��Ÿ��
private:
	HRESULT				CoolTime_Manager(_double TimeDelta);

	//����
private:
	HRESULT				SetUp_Weapon();
	HRESULT				Weapon_Slot(_double TimeDelta);

private://UI
	HRESULT				SetUp_UI();

private://Effect
	HRESULT	Hit_Effect(); //������ ���� ����Ʈ
	HRESULT	DashStraight_Effect(); //������ ������ ����Ʈ
	HRESULT	DashRight_Effect(); //������ �뽬�� ������ ����Ʈ
	HRESULT	DashLeft_Effect(); //������ �뽬�� ������ ����Ʈ
	HRESULT Environment_Particle();// ȯ�� ��ƼŬ
	HRESULT	Mist_MeshEffect(); //�Ȱ� ȿ��

	//������ ����
private:
	virtual	_int		Obsever_Hit(void* pArg = nullptr)override;


	//��Ÿ �� ���
private:
	_bool Once_Trigger(); //�ѹ��� ���� Ʈ���� 
	HRESULT	PivotMatrix(); //�ǹ�
	HRESULT	Level_Change(); //�� ��ȯ
	HRESULT Warp();//��������� ����� �Լ�
	HRESULT	Torchlight();
	HRESULT Torchlight_Pos_Stage1();
	HRESULT Torchlight_Pos_Stage1_BossRoom();
	HRESULT Torchlight_Pos_Stage2();
	HRESULT Torchlight_Pos_Stage2_BossRoom();

	//������ ������� �ʰ� �������� �ٽ� ����Լ�
private:
	HRESULT MaptoolNotPosition();

private:
	///////////////////////////���̴��� ������ ���� ���߿� ��ġ Ȯ���Ҷ��� Ȯ������
	//CShader*			m_pShaderCom = nullptr;
	//CTexture*			m_pTextureCom = nullptr;
	//CVIBuffer_Cube*		m_pVIBufferCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	////////////////////////////////////////////////////
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	//�����ϱ�
private:
	JUMPDESC				m_JumpDesc;

	//������ �����̱�
private:
	_bool					m_bGoInterpolationSwitch[4] = { false ,false ,false ,false };
	_bool					m_bMovingSwitch = false;
	_double					m_dMovingTime = 0;
	_float3					m_fFastMoving;

	//����
private:
	CWeapon*				m_pWeapon[WEAPONEND];

	_bool					Weapon_SetUp = false;
	_bool					m_bInUseWeapon[WEAPONEND]={ false,false, false, false, false, false };
	class CUI_Weapon*		m_pUI_Weapon = nullptr;

	//ü��
private:
	class CUI_MpBar*		m_pMpBar = nullptr;
	class CUI_HpBar*		m_pHpBar = nullptr;


private://��ų ��Ÿ��
	_double					m_dFastCoolTime = 0;
	_bool					m_bFastSwitch = true;

	_double					m_dReflectCoolTime = 0;
	_bool					m_bReflectSwitch = true;

	_double					m_dSlowTimeCoolTime = 0;
	_bool					m_bSlowTimeSwitch = true;



	//��Ÿ �� ���
private:
	_bool					m_bOnce_Switch = false;

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END