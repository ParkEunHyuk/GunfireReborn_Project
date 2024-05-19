#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "Universal_Bullet.h"
#include "Trail.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CMonster_Catfish final : public CLandObject
{
private:
	explicit CMonster_Catfish(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Catfish(const CMonster_Catfish& rhs);
	virtual ~CMonster_Catfish() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT	SetUp_ConstantTable();

private:// Ư�� �̺�Ʈ��
	HRESULT	SetUp_Event_Tirgger(_double TimeDelta);
	HRESULT	SetUp_Fight(_double TimeDelta);
	HRESULT	Fight_On();
	HRESULT Once_Trigger();

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

private://���Ϲ��� ������ ���� �Լ�
	HRESULT	Universal_Wield(_double TimeDelta); //Į�� �ֵѷ��� ����
	HRESULT	Universal_RunAndAttack(_double TimeDelta);


private://��Ÿ �� �Լ���
	HRESULT	Slow_On();
	HRESULT	Notice(); //���� ���ϸ� �ֺ� ���鿡�� �˸�

private://Ʈ����
	HRESULT	SetUp_Trail();

public:
	void	Set_Trail(CTrail* pTrail);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	CTexture*			m_pDissolveTextrueCom = nullptr;

	//�ִϸ��̼� ����
	_bool				m_bOneTimeSwith = false;
	_uint				m_iOnceAnimNumber = 0;
	_uint				m_iInfinityAnimNumber = 0;

	//�ο� ����
	class CGameObject*		m_pPlayer = nullptr;
	class CTransform*		m_pPlayerTransform = nullptr;
	_bool					m_bFight = false;

	//�ִϸ��̼� ���� �� �̺�Ʈ
	_double					m_dOnceCoolTime = 0;
	_double					m_dInfinityCoolTime = 0;
	_uint					m_iInfinityPattern = 0;
	_uint					m_iOncePattern = 0;
	_bool					m_MoveAndAttack = false;

	//�Ÿ�
	_float								m_fDistance = 0.f;

	//Bullet�� ���õ� �����
	_bool								m_bFire = true;  //�ѹ��� �߻��ϱ� ���� �Ұ�
	_double								m_dFireTime = 0; //Ÿ�̹��� ���� ������

	_bool								m_bOnce_Switch = true; //�ΰ��� ���� �ѹ��� �����ų �Ұ�

	//���ο� ��ų�� ���� ������
	_double				m_dSlowTime = 1;
	_float				m_fSlowBulletDistance = 101;
	CUniversal_Bullet* m_pTempSlow_Bullet = nullptr;

private: //Ʈ����
	CTrail*								m_pTrail = nullptr;

	//������
	_float								m_fDissolveTime = 0.f;
	_bool								m_bDissolveOn = false;

public:
	static CMonster_Catfish* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END