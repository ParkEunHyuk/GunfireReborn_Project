#pragma once

#include "Component.h"

BEGIN(Engine)


class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
	enum BOUNDING { BOUNDING_ORIGINAL, BOUNDING_TRANSFORM, BOUNDING_END };

public:
	typedef struct tagOBBDesc
	{
		_float3		vCenterAxis[3];
		_float3		vAlignAxis[3];
	}OBBDESC;

public:
	typedef struct tagColliderDesc
	{
		_float3 vScale; /* 배수 */
		_float4 vRotation;
		_float4 vTranslation;
	} COLLIDERDESC;

private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg);

	void Update_Transform(_fmatrix Transform);
public:
	_bool Collision_Sphere(CCollider* pTargetCollider);
	_bool Collision_AABB(CCollider* pTargetCollider);
	_bool Collision_OBB(CCollider* pTargetCollider);

	OBBDESC	Compute_OBBDesc();


#ifdef _DEBUG
public:
	virtual HRESULT Render() override;
#endif // _DEBUG

private:
	BoundingBox*				m_pAABB[BOUNDING_END] = { nullptr }; //AABB
	BoundingOrientedBox*		m_pOBB[BOUNDING_END] = { nullptr }; //OBB
	BoundingSphere*				m_pSphere[BOUNDING_END] = { nullptr }; //Sphere
	TYPE						m_eType = TYPE_END;
	COLLIDERDESC				m_ColliderDesc;

	_bool						m_isCollision = false;

#ifdef _DEBUG
private:
	/* 정점들을 그릴수 있는 셰이더를 제공한다. */
	BasicEffect*									m_pBasicEffect = nullptr;

	ID3D11InputLayout*								m_pInputLayout = nullptr;

	/* 기본적인 도형을 그려나갈수 있는 정점 버퍼, 인덱스 버퍼를 제공한다. */
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = nullptr;
#endif // _DEBUG

private:
	_matrix Remove_Rotation(_fmatrix TransformMatrix); //변화된 객체의 크기에 맞춰 바뀐다.


#ifdef _DEBUG
	//!@#!@#@!!@#!@#!@#!@#@!툴 용 콜 라이더
public:
	typedef struct ColliderDesc {
		TYPE		eType;
		_float3		vScale;
		_float4		vRotation;
		_float4		vPosition;
	}COLLIDERTOOLDESC;

public:
	COLLIDERTOOLDESC Get_SaveData(); //저장
	_bool	Check_RayCast(_fvector vRayDir, _fvector vRayPos);//마우스 픽킹을 위한 함수
	void	Update_Position(_float3 vPos); //오리지널의 위치값을 바꿈
	COLLIDERTOOLDESC					m_tColliderDesc;

	_float4x4										m_TransformMatrix;
	//!@#!@#!@#!@#!@#!@#!@#!@# 툴 용 콜 라이더
#endif


public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;


};

END