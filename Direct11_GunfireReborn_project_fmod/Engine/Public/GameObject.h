#pragma once

//#include "Base.h"
#include "Transform.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	class CComponent* Get_Component(const _tchar* pComponentTag);
	
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc = nullptr);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
	virtual	_int	Dead_Trigger();
	virtual void	Set_Dead(_uint _iDead);
	virtual _uint	Get_Dead() { return m_iDead; }

	virtual _int	Obsever_Hit(void* pArg = nullptr);
	virtual _int	Obsever_StrongHit(void* pArg = nullptr);
	virtual _int	Obsever_Event(void* pArg = nullptr);
	virtual _int	Obsever_Sound(void* pArg = nullptr);

	_float	Get_CamDistance();


	virtual void Set_NowSceneNum(_uint eNowSceneNum) { m_eNowLevelNum = eNowSceneNum; };

public:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	static const _tchar*	m_pTransformTag;

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	_uint					m_eNowLevelNum = 0;

	OBJECTDATADESC			m_tObjectDataDesc;

	_uint					m_iDead = OBJ_NOEVENT;

protected:
	class CTransform*		m_pTransform = nullptr;

protected:
	CComponent*	Find_Component(const _tchar* pComponentTag);
	
protected:
	map<const _tchar*, CComponent*>			m_Components;
	typedef map<const _tchar*, CComponent*>	COMPONENTS;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

//ImGui=================================================
#if defined(USE_IMGUI)
public:
	OBJECTDATADESC* Get_ObjectDataDesc() {
		return &m_tObjectDataDesc;
	}
#endif
//ImGui=================================================
};

END