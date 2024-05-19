#pragma once

#include "Client_Defines.h"
#include "UI.h"
#include "LandObject.h"
#include "Universal_Bullet.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CUniversal_MeshEffect final : public CLandObject
{
public:
	enum MESH_EFFECT_TYPE {
		LIGHTNING_EFFECT,
		MIST_EFFECT,
		MESH_EFFECT_END
	};
public:
	typedef struct tagUniversal_Mesh_Effect_Desc {
		_uint			iMeshEffectNumber;

		_float			fSpeedPerSec;//�Ѿ� ������
		_float3			vScale;
		_float3			vLook;
		_float4			vRotation;
		_float4			vTranslation;

		CGameObject*	pObject = nullptr; //�÷��̾ �ƴ� ���� �Ѿ��� ���� �� ���̴� ����
		_float3			fPositioning;			//���� �Ѿ��� ���°� ������ ��� vTranslation�� ������� �ʰ� fPositioning�� ����Ѵ�. (Ʈ�������� ���� ������Ʈ�� �����´�.)

		_double			dDuration; //���ӽð�
	}UNIVERSAL_MESH_EFFECT_DESC;
private:
	explicit CUniversal_MeshEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUniversal_MeshEffect(const CUniversal_MeshEffect& rhs);
	virtual ~CUniversal_MeshEffect() = default;

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
	HRESULT	SetUp_Info();	//��ü�� ������ 
	HRESULT	SetUp_MeshEffect(_double TimeDelta); //�޽� ����Ʈ ���� �Ϸ�

	HRESULT	Lightning_Effect(_double TimeDelta);
	HRESULT	Mist_Effect(_double TimeDelta);

private: //���� ����
	virtual	_int Dead_Trigger()override;

public:
	UNIVERSAL_MESH_EFFECT_DESC	m_Universal_Mesh_Effect_Desc;

private://�⺻���� ��� ������
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pObjectTransform = nullptr;

	//������ ����
	_float4						m_fTempPos;
	_float3						m_fTempLook;

	_double						m_dDeltaTime = 0; //�Ѿ��� �����ִ� ���ӽð�
	_double						m_dEasingTime = 0;

	//�޽� ���ο��� ���� ���鶧 ����
	_double						m_dFireTime = 0;
	_bool						m_bFire = true;
	_uint						m_iFireCount = 0;

	//�������� ǥ���� �� ����
	_double						m_dMovingTime = 0;
	_uint						m_iMovingCount = 0;

	_uint						m_iRotation = 0;



public:
	static CUniversal_MeshEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END