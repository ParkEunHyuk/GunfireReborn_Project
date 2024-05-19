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

class CMonster_Npc final : public CLandObject
{
private:
	explicit CMonster_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Npc(const CMonster_Npc& rhs);
	virtual ~CMonster_Npc() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT	SetUp_ConstantTable();

private://�ִϸ��̼�
	HRESULT	PlayAnim(_double TimeDelta);

private:
	HRESULT	SetUp_Info();

private:
	HRESULT	Distance(); //�Ÿ��� ���

private:
	HRESULT	Enter_Effect(); //���� ����Ʈ�� ����
	HRESULT	Talk_Box(); // �޼����ڽ��� ����


private://������ Ʈ���ŵ�
	virtual _int	Obsever_Hit(void* pArg = nullptr)override;
	virtual	_int	Obsever_Event(void* pArg = nullptr)override;
private:
	virtual	_int	Dead_Trigger();
private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	//�ִϸ��̼� ����
	_bool				m_bOneTimeSwith = false;

	//�Ÿ�
	_float				m_fDistance = 0;

	//�Ұ���
	_bool				m_bMessageOn = false;
	_bool				m_bEnterOn = true;


	class CTransform*	m_pPlayerTransform = nullptr;

public:
	static CMonster_Npc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END