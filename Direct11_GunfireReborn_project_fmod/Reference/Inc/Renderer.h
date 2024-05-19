#pragma once

#include "Component.h"

/* �ΰ��ӳ��� �׷������ϴ� ��ü���� ��Ƽ� ����(�׷����� �������.) �Ѵ�. */
/* ��ü���� �׸��� �Լ�(Render) �� ȣ�����ֳ�. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{	
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND_FIRST, RENDER_NONBLEND_SECOND, RENDER_NONLIGHT, RENDER_BLEND, RENDER_UI, RENDER_END };
public:
	explicit CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Add_Debug(class CComponent* pComponent);
	HRESULT Render();

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Lights();
	HRESULT Render_Blend();
	HRESULT Render_NonLight();
	HRESULT Render_Alpha();
	HRESULT Render_UI();

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif // _DEBUG

private:
	list<class CGameObject*>				m_RenderObjects[RENDER_END];
	typedef	list <class CGameObject* >		RENDEROBJECTS;

private:
	list<class CComponent*>					m_DebugObject;
	typedef list<class CComponent*>			DEBUGOBJECT;

private:
	class CTarget_Manager*					m_pTarget_Manager = nullptr;
	class CLight_Manager*					m_pLight_Manager = nullptr;

private:
	class CVIBuffer_Rect*					m_pVIBuffer = nullptr;
	class CShader*							m_pShader = nullptr;

private:
	_float4x4					m_WorldMatrix;
	_float4x4					m_ViewMatrix;
	_float4x4					m_ProjMatrix;

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();
};

END