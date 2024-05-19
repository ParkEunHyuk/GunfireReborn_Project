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

private:// Ư�� �̺�Ʈ��
	HRESULT	SetUp_Event_Tirgger(_double TimeDelta);
	HRESULT	SetUp_Fight(_double TimeDelta);
	HRESULT	Fight_On();

private://�ִϸ��̼�
	HRESULT	PlayAnim(_double TimeDelta);
	HRESULT	CoolTime_Manager(_double TimeDelta);
	HRESULT	Once_AnimMotion(_double TimeDelta);
	HRESULT	Infinity_AnimMotion(_double TimeDelta);
	HRESULT	Combo_AnimMotion(_double TimeDelta);

private://������ Ʈ���ŵ�
	virtual _int	Obsever_Hit(void* pArg = nullptr)override;
	virtual	_int	Obsever_Event(void* pArg = nullptr)override;
private:
	virtual	_int	Dead_Trigger();

private://���� ���ϵ�
	HRESULT	Appear(_double TimeDelta);// ���� ��
	HRESULT	Jumping(_double TimeDelta); //���� �� �������
	HRESULT	Universal_Sword(_double TimeDelta); //�ߵ�
	HRESULT	Universal_Turn(_double TimeDelta);//����ġ��
	HRESULT	Universal_StrikeDown(_double TimeDelta); //�׳� �������
	HRESULT	Universal_TripleAttack(_double TimeDelta); //������ Ʈ���� ����


	//������2
	HRESULT Phase_Two(_double TimeDelta);		//������2 ����
	HRESULT Red_Sphere(_double TimeDelta);		//������2 ���԰� ���ÿ� ����� �ǹ��� ��ü
	HRESULT	Jumping_PhaseTwo(_double TimeDelta); //���� �� ������� �� �޽� ����
	HRESULT	Tornado_Shield(_double TimeDelta); //����̵� ���и� ������
	HRESULT	Tornado_Bullet(_double TimeDelta); //����̵��� ����
	HRESULT	Sphere_Potal(_double TimeDelta); //���� ��Ż�� ����

	HRESULT	Rush_Attack_One(_double TimeDelta); //���� ���� ��
	HRESULT	Rush_Attack_Two(_double TimeDelta); //���� ���� ��
	HRESULT	Rush_Attack_Three(_double TimeDelta); //���� ���� ����

	HRESULT TripleAttack_SwordWave(_double TimeDelta); //Ʈ���� ���ÿ����� �ҵ���̺�

	HRESULT	EyeOfSauron(_double TimeDelta);		//������ �� ������ ���Ϲ��� �ҷ� �޽��� �ű� ������ ���ܵ���

	HRESULT	Devil_Summons(_double TimeDelta); //�Ǹ� ��ȯ
	HRESULT	Mace_Summons(_double TimeDelta); //�Ǹ��� �ҷ��� ��ȯ��



private://Ʈ����
	HRESULT	SetUp_Trail();

private://��ƼŬ
	HRESULT Devil_Summons_Particle();

private://�ǵ�� ���õ� �Լ�
	HRESULT	SetUp_Shield();
	HRESULT	SetUp_Hp();

private://��Ÿ �� �Լ���
	HRESULT	Slow_On(); //��ü�� ������ ����
	HRESULT	Victory(); //�÷��̾ �̱�(������ �̱�� �ƴ�!!!)
	HRESULT	Npc(); //NPC ��ȯ �ٸ� Ŭ�������� Warp��� �̸����� �Ǿ� ����!!!

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	CTexture*			m_pDissolveTextrueCom = nullptr;

	//�ִϸ��̼� ����
	_bool				m_bOneTimeSwith = true;
	_uint				m_iOnceAnimNumber = 0;
	_uint				m_iInfinityAnimNumber = 1;

	//�ο� ����
	class CGameObject*		m_pPlayer = nullptr;
	class CTransform*		m_pPlayerTransform = nullptr;
	_bool					m_bFight = false;

	//�ִϸ��̼� ���� �� �̺�Ʈ
	_double					m_dOnceCoolTime = 0;
	_double					m_dInfinityCoolTime = 0;
	_uint					m_iInfinityPattern = 0;
	_uint					m_iOncePattern = 99;

	//�Ÿ�
	_float								m_fDistance = 0;

	//Bullet�� ���õ� �����
	_bool								m_bFire = true;  //�ѹ��� �߻��ϱ� ���� �Ұ�
	_double								m_dFireTime = 0; //Ÿ�̹��� ���� ������

														 //���ο� ��ų�� ���� ������
	_double								m_dSlowTime = 1;
	_float								m_fSlowBulletDistance = 101;
	CUniversal_Bullet*					m_pTempSlow_Bullet = nullptr;

	//����� ���� ����
	_bool								m_bPhase_One = true;

	//�ִϸ��̼��� �߰��� �ڸ��ų� �� �� ���
	_double								m_dAnimCutTime = 0;
	_bool								m_bAnimCutOn = false;

	//�߰��޺� �Ǵ� m_iOncePattern�� �ѹ� �� ����ؾ��� �� ���
	_bool								m_bComboSwtich = false;
	_uint								m_iCurrentAnim = 0;
	_uint								m_iNextAnim = 0;
	_bool								m_bComboEnd = false;

	//���� ������ ����� �� ���̴� ����
	_bool								m_bLookAtOff = false;
	_float								m_fTempY = 0;


	//������ ������ �߾��� ���� ����
	_uint								m_iFinalAttack = 0;

	//���ο� ��ī�̹ڽ��� ����
	_bool								m_bSkyBoxOn = false;

	//�Ǹ� ��ȯ
	_bool								m_bDevilOn = false;

	//ö�� ��ȯ
	_bool								m_bMace_BulletOn = false;
	_double								m_dMaceSummons_Time = 0;


private: //Ʈ����
	CTrail*								m_pTrail = nullptr;


private://������
	_float								m_fDissolveTime = 0.0f;
	_bool								m_bDissolveOn = false;

private://��ƼŬ
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