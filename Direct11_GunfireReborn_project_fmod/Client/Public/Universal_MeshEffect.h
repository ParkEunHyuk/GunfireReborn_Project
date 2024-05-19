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

		_float			fSpeedPerSec;//총알 정보들
		_float3			vScale;
		_float3			vLook;
		_float4			vRotation;
		_float4			vTranslation;

		CGameObject*	pObject = nullptr; //플레이어가 아닌 몬스터 총알을 만들 때 쓰이는 정보
		_float3			fPositioning;			//만약 총알을 쓰는게 몬스터일 경우 vTranslation을 사용하지 않고 fPositioning를 사용한다. (트랜스폼은 게임 오브젝트로 가져온다.)

		_double			dDuration; //지속시간
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
	HRESULT	SetUp_Info();	//객체의 정보를 
	HRESULT	SetUp_MeshEffect(_double TimeDelta); //메쉬 이펙트 세팅 완료

	HRESULT	Lightning_Effect(_double TimeDelta);
	HRESULT	Mist_Effect(_double TimeDelta);

private: //데드 패턴
	virtual	_int Dead_Trigger()override;

public:
	UNIVERSAL_MESH_EFFECT_DESC	m_Universal_Mesh_Effect_Desc;

private://기본적인 멤버 변수들
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pObjectTransform = nullptr;

	//보관용 변수
	_float4						m_fTempPos;
	_float3						m_fTempLook;

	_double						m_dDeltaTime = 0; //총알이 남아있는 지속시간
	_double						m_dEasingTime = 0;

	//메쉬 내부에서 무언갈 만들때 사용됨
	_double						m_dFireTime = 0;
	_bool						m_bFire = true;
	_uint						m_iFireCount = 0;

	//움직임을 표현할 때 사용됨
	_double						m_dMovingTime = 0;
	_uint						m_iMovingCount = 0;

	_uint						m_iRotation = 0;



public:
	static CUniversal_MeshEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END