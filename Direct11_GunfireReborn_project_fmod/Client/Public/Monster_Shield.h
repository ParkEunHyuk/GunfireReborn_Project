#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Model.h"
#include "LandObject.h"

BEGIN(Engine)
class CRenderer;
class CCollider;
END

BEGIN(Client)
class CMonster_Shield final : public CGameObject
{
public:
	enum SHIELDTYPE {
		HORSE_SHIELD,
		POWERLIZARD_SHIELD,
		SHIELD_END
	};
public:
	typedef struct tagShieldDesc
	{
		SHIELDTYPE		eShieldNumber;
		CModel*			pModel = nullptr;
		CTransform*		pTransform = nullptr;
		const char*		pSocketName = nullptr;
		void*			pObject = nullptr;
	}SHIELDDESC;

private:
	CMonster_Shield(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMonster_Shield(const CMonster_Shield& rhs);
	virtual ~CMonster_Shield() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT	SetUp_Shield();

protected:
	virtual	_int	Obsever_Hit(void* pArg = nullptr) override;
	virtual	_int	Obsever_Event(void* pArg = nullptr)override;

private:
	SHIELDDESC				m_ShieldDesc;
	CModel::BONEMATRIX_PTR	m_BoneMatrixPtr;
	_float4x4				m_SocketTransformMatrix;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;

	CLandObject*		m_pMonsterObject = nullptr;

private:
	HRESULT SetUp_Components();

public:
	static CMonster_Shield* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END