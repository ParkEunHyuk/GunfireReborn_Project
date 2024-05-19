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

private:// Ư�� �̺�Ʈ��
	HRESULT	SetUp_Event_Tirgger(_double TimeDelta);
	HRESULT	SetUp_Fight(_double TimeDelta);
	HRESULT	Fight_On();

private://�ִϸ��̼�
	HRESULT	PlayAnim(_double TimeDelta);
	HRESULT	CoolTime_Manager(_double TimeDelta);
	HRESULT	Once_AnimMotion(_double TimeDelta);
	HRESULT	Infinity_AnimMotion(_double TimeDelta);

private://������ Ʈ���ŵ�
	virtual _int	Obsever_Hit(void* pArg = nullptr)override;
	virtual	_int	Obsever_Event(void* pArg = nullptr)override;
private:
	virtual	_int	Dead_Trigger();

private://���� ���ϵ�
	HRESULT	Jumping(_double TimeDelta);
	HRESULT	Universal_Bullet(_double TimeDelta); //�Ѿ� �߻�
	HRESULT	Universal_StrikeDown(_double TimeDelta);

	//������2
	HRESULT	Jumping_PhaseTwo(_double TimeDelta);
	HRESULT	Universal_Bullet_PhaseTwo(_double TimeDelta);
	HRESULT	Universal_Bullet_SupplementAttack(_double TimeDelta);
	HRESULT	Meteorite_Bullet(_double TimeDelta);
	HRESULT	Terrain_Attack(_double TimeDelta);

private://�ǵ�� ���õ� �Լ�
	HRESULT	SetUp_Shield();
	HRESULT	SetUp_Hp();

private://��Ÿ �� �Լ���
	HRESULT	Slow_On(); //��ü�� ������ ����
	HRESULT Warp();//��������� ����� �Լ�
	HRESULT	Victory(); //�÷��̾ �̱�(������ �̱�� �ƴ�!!!)

		//������ ������� �ʰ� �������� �ٽ� ����Լ�
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

	//���� ������ ����� �� ���̴� ����
	_bool								m_bJumpingOn = false;
	_float								m_fTempY = 0;

	//Universal_Bullet_ThreeAttack()�� ���� �����ϴ� ����
	_uint								m_iSupplementAttack = 0;

	//������
	_float								m_fDissolveTime = 0.0f;

	_bool								m_bDissolveOn = false;


	//���� ����ġ
	_bool								m_bSoundSwitch = false;

public:
	static CMonster_Boss_Dokkaebi* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END