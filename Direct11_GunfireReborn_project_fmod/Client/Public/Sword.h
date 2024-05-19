#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Model.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
END

BEGIN(Client)

class CSword final : public CGameObject
{
public:
	typedef struct tagSwordDesc
	{
		CModel*			pModel = nullptr;
		CTransform*		pTransform = nullptr;
		const char*		pSocketName = nullptr;
	}SWORDDESC;

private:
	CSword(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CSword(const CSword& rhs);
	virtual ~CSword() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	SWORDDESC				m_SwordDesc;
	CModel::BONEMATRIX_PTR	m_BoneMatrixPtr;
	_float4x4				m_SocketTransformMatrix;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	static CSword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END