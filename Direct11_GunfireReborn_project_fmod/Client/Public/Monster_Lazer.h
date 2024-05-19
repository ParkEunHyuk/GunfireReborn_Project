#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Model.h"
#include "LandObject.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)

class CMonster_Lazer final : public CGameObject
{
public:
	typedef struct tagLazerDesc
	{
		CModel*			pModel = nullptr;
		CTransform*		pTransform = nullptr;
		const char*		pSocketName = nullptr;
		void*			pObject = nullptr;
	}LAZERDESC;

private:
	CMonster_Lazer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMonster_Lazer(const CMonster_Lazer& rhs);
	virtual ~CMonster_Lazer() = default;

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
	HRESULT	NavigationOn(_double TimeDelta);

private:
	HRESULT	TargetOn(_double TimeDelta);
	HRESULT	Pivot(_double TimeDelta);

protected:
	virtual	_int	Obsever_Hit(void* pArg = nullptr) override;
	virtual	_int	Obsever_Event(void* pArg = nullptr)override;

private:
	LAZERDESC				m_LazerDesc;
	CModel::BONEMATRIX_PTR	m_BoneMatrixPtr;
	_float4x4				m_SocketTransformMatrix;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	CNavigation*		m_pNavigationCom = nullptr;


	CLandObject*		m_pMonsterObject = nullptr;

	CTransform*			m_pPlayerTransform = nullptr;

	_float				fTempDis = 0;


public:
	static CMonster_Lazer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END