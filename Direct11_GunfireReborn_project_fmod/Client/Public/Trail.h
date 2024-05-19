#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Model.h"

BEGIN(Engine)
class CVIBuffer_RectTrail;
class CRenderer;
class CShader;
class CTexture;
END

BEGIN(Client)

class CTrail final : public CGameObject
{
public:
	enum TRAILTEXTURETYPE {
		UNIVERSAL_TRAIL,
		TRAIL_END
	};
	enum MONSTERTYPE { //RandObject에 Trail을 냅둬서 범용적으로 하는 방법도 있음
		NON,
		CATFISH,
		HORSE,
		MONSTER_END
	};
public:
	typedef struct tagTrailDesc {
		_uint		  iTrailTextureIndex;

		MONSTERTYPE		eMonsterType;

		CModel*		pModel = nullptr;
		const char*		pSocketName = "";
		void*			pObject = nullptr;
		CTransform*		pTransform = nullptr; //혹시 트랜스폼을 사용할 일이 있을때 일단 트랜스폼만 써도 될 듯


		_float3			vHighPos, vLowPos;
		_float4			vColor;
	}TRAILDESC;
	//소드웨펀에 자료 있음
public:
	void	Set_NotRendering(_bool isRedering) {
		m_isNotRendering = isRedering;
	}

	void	Stop_ComputeTrail() {
		m_bStop_Compute_Trail = true;
	}

private:
	explicit CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTrail(const CTrail& rhs);
	virtual ~CTrail() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	MonsterLink();

private:
	HRESULT	Pivot_Matrix();

private:
	CVIBuffer_RectTrail*		m_pVIBufferCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

private:
	TRAILDESC					m_TrailDesc;
	_uint						m_iVtxCnt = 0;
	_bool						m_isNotRendering = false;
	_bool						m_bStop_Compute_Trail = false;

private:
	_uint						m_iCatmulRomIndex[4] = {};


private:
	CModel::BONEMATRIX_PTR		m_BoneMatrixPtr;
	_float4x4					m_SocketTransformMatrix;
	_float4x4					m_TransformMatrix;

	CGameObject*				m_pObject = nullptr;
	CModel*						m_pObjectModel = nullptr;

private:
	HRESULT		SetUp_Components();
	HRESULT		SetUp_ConstantTable();

public:
	static CTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END