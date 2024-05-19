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

		_float			fSpeedPerSec;//�Ѿ� ������
		_float3			vScale;
		_float3			vLook;
		_float4			vRotation;
		_float4			vTranslation;

		CGameObject*	pObject = nullptr; //�÷��̾ �ƴ� ���� �Ѿ��� ���� �� ���̴� ����
		_float3			fPositioning;			//���� �Ѿ��� ���°� ������ ��� vTranslation�� ������� �ʰ� fPositioning�� ����Ѵ�. (Ʈ�������� ���� ������Ʈ�� �����´�.)

		_float3			fColliderSphereScale; //�浹ü�� ũ��
		_float3			fColliderOBBScale;	//�浹ü�� ũ��

		_double			dDuration; //���ӽð�
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
	HRESULT	SetUp_Collider(); //�ݶ��̴��� ������
	HRESULT	SetUp_Info();	//��ü�� ������ 
	HRESULT	SetUp_Fire(_double TimeDelta);

private:
	//������ ������ �Լ���
	HRESULT	Burn_Bullet(_double TimeDelta); //�������� ��Ÿ�� �Ѿ�
	HRESULT	Supplement_Bullet(_double TimeDelta);//�������� �߰� �Ѿ�
	HRESULT	Meteorite_Bullet(_double TimeDelta); //�������� � ����
	HRESULT	Terrain_Bullet(_double TimeDelta); //�������� �ͷ��� ����

	//���� ������ �Լ���
	HRESULT	SwordWave_Bullet(_double TimeDelta); //���� ���
	HRESULT	ShockWave_Bullet(_double TimeDelta);//�ֺ��� �޽��� ��� �ھƳ�
	HRESULT	Crack(_double TimeDelta); //�տ�
	HRESULT Tornado_Shield(_double TimeDelta); //����̵� ����
	HRESULT	Tornado_Bullet(_double TimeDelta); //����̵��� ����
	HRESULT	Sphere_Potal(_double TimeDelta); //��ü ��Ż ����
	HRESULT Lateral_Bullet(_double TimeDelta); //Ⱦ���� ���
	HRESULT	Upper_Bullet(_double TimeDelta); //�÷����� ���
	HRESULT	Vertical_Bullet(_double TimeDelta); //������ ���
	HRESULT	Red_Sphere(_double TimeDelta); //������2 ���� �ǹ��� ��ü
	HRESULT	Devil(_double TimeDelta); //�Ǹ� ��ȯ
	HRESULT	Sauron(_double TimeDelta); //������ ��
	HRESULT	Awl(_double TimeDelta); //�۰�
	HRESULT	Mace_Bullet(_double TimeDelta); //ö��
	HRESULT	Mace_Bullet_Right(_double TimeDelta); //ö�� ������
	HRESULT	Warp(_double TimeDelta); //����
	HRESULT Appear(_double TimeDelta); //�����
	HRESULT	Level_Change(); //���� ü����
	

private://ETC
	HRESULT	Slow_On();
	HRESULT	NavigationOn(_double TimeDelta); //�׺���̼�
	HRESULT	Renderer();

	HRESULT	Devil_Out_Particle();
	
private: // ������ ����
	virtual _int Obsever_Hit(void* pArg)override;
	virtual _int Obsever_Event(void* pArg)override;

private: //���� ����
	virtual	_int Dead_Trigger()override;


public:
	UNIVERSAL_BULLET_MESHDESC	m_Universal_Bullet_MeshDesc;

private://�⺻���� ��� ������
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pModelCom = nullptr;
	CCollider*					m_pSphereCom = nullptr;
	CCollider*					m_pOBBCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	//�÷��̾� ������ ��� ����
	class CTransform*			m_pPlayerTransform = nullptr;

	//������ ����
	_float4						m_fTempPos;
	_float3						m_fTempLook;

	_double						m_dDeltaTime = 0; //�Ѿ��� �����ִ� ���ӽð�
	_double						m_dEasingTime = 0;


	//���ο츦 ���� ������
	CUniversal_Bullet*			m_pTempSlow_Bullet = nullptr;
	_double						m_dSlowTime = 1;
	_float						m_fSlowBulletDistance = 101;

	
	//�޽� ���ο��� ���� ���鶧 ����
	_double						m_dFireTime = 0;
	_bool						m_bFire = true;
	_uint						m_iFireCount = 0;

	//�������� ǥ���� �� ����
	_double						m_dMovingTime = 0;
	_uint						m_iMovingCount = 0;

	_uint						m_iRotation = 0;

	_bool						m_MeteoSwitch = false;


	//����
	_bool						m_bSoundSwitch = false;
	_double						m_dSoundTime = 10;

public:
	static CUniversal_Bullet_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END