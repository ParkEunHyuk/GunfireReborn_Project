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
		LIZARD, //���ڵ� �Ѿ�,
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
		_uint			iBulletNumber;   //_uint������ �ҷ� Ÿ���� ���� ��
		OBSEVERGROUP	eType;			//enum������ ������ Ÿ���� ����


		_float			fSpeedPerSec;//�Ѿ� ������
		_float3			vScale;
		_float3			vLook;
		_float4			vRotation;
		_float4			vTranslation;

		CGameObject*	pObject = nullptr; //�÷��̾ �ƴ� ���� �Ѿ��� ���� �� ���̴� ����
		_float3			fPositioning;			//���� �Ѿ��� ���°� ������ ��� vTranslation�� ������� �ʰ� fPositioning�� ����Ѵ�. (Ʈ�������� ���� ������Ʈ�� �����´�.)

		_float3			fColliderSphereScale; //�浹ü�� ũ��
		_float3			fColliderOBBScale;	//�浹ü�� ũ��

		_uint			iTextureIndex = 0;

		_double			dDuration; //���ӽð�
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


private:	//Bullet �غ�ܰ�
	HRESULT	SetUp_Info();
	HRESULT	SetUp_Fire(_double TimeDelta); //!@#!#!#@!#!@$ã�� ����

private: //Bullet��
	HRESULT	Reflect_Skill(_double TimeDelta); //�÷��̾� ��ų
	HRESULT	SlowTime_Skill(_double TimeDelta); //�÷��̾� ��ų
	HRESULT	Launcher_Boom(_double TimeDelta); //��ó ��
	HRESULT Lizard(_double TimeDelta);		//���ڵ��� �Ѿ� ����
	HRESULT	GhostDeer(_double TimeDelta); //��Ʈ����� �Ѿ� ����
	HRESULT	PowerLizard(_double TimeDelta); //�Ŀ����ڵ��� �Ѿ� ����
	HRESULT	Raven(_double TimeDelta);	//���̺��� �Ѿ� ����
	HRESULT	Cube_Boom(_double TimeDelta); //ť�� ��
	HRESULT	Dokkaebi_Attack(_double TimeDelta); //�������� ���� �� �����
	HRESULT Dokkaebi_StrikeDown(_double TimeDelta); //�������� ��Ʈ����ũ �ٿ�
	HRESULT	Dokkaebi_Terrain_Guided(_double TimeDelta); //�̰� ���� �ͷ������� ������ ������
	HRESULT	Bear_Attack(_double TimeDelta); //������ ����
	HRESULT	Yellow_Potal(_double TimeDelta); //���ο� ��Ż�� ������Ŵ �װ� ����� Ư���� ����� ����
	HRESULT	NonTexture(_double TimeDelta); //�ؽ��İ� ���°͵�(�������ݿ�)


private:
	HRESULT	Slow_Particle();

private:
	HRESULT				Billboard(); //������

private://��Ÿ �� �Լ���
	HRESULT	Slow_On(); //���ο� ��ų�� On Off ���
	HRESULT	NavigationOn(_double TimeDelta);


private: // ������ ����
	virtual _int	Obsever_Hit(void* pArg)override;
	virtual _int	Obsever_Event(void* pArg)override;

private: //���� ����
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
	

	_double				m_dDeltaTime = 0; //���ӽð��� üũ�ϱ� ���� ����
	_double				m_dTextureTime = 0; //�ؽ����� ��ü Ÿ�̹��� ���� ����

	_double				m_dSlowTime = 1;
	_float				m_fSlowBulletDistance = 101;

	CUniversal_Bullet* m_pTempSlow_Bullet = nullptr;

	//GhostDeer�� ���� �÷�
	_float m_fDegrees = 0;

	//�޽��� ƨ�ܳ��� ���� �ʿ��� �Ұ�
	_bool				m_bNotMesh = true;

	//Terrain_Bullet�� �����ϱ� ���� ����
	_double				m_dFireTime = 0;


	_bool				m_bSlowParticleOn = false; //���ο� ��ƼŬ�� ���� ����


public:
	static CUniversal_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END
