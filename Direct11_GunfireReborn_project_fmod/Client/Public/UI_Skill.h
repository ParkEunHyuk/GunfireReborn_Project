#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Skill final : public CUI
{
public:
	enum SKILLTYPE {
		SHIFT_SKILL,
		Q_SKILL,
		E_SKILL,
		SHIFT,
		Q,
		E,
		ALPHA,
		UI_BULLETTYPE,
		UI_SKILL_END
	};
public:
	typedef struct tagUI_SkillDesc
	{
		SKILLTYPE	eType;
		_float		fSizeX;
		_float		fSizeY;
		_float		fX;
		_float		fY;
		_float		fDepth;
		_double		dDuration;
	}UI_SKILLDESC;
private:
	explicit CUI_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Skill(const CUI_Skill& rhs);
	virtual ~CUI_Skill() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT				SetUp_Skill(_double TimeDelta);

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	UI_SKILLDESC		m_UI_SkillDesc;

	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	_float				m_TickSizeX;
	_float				m_TickSizeY;

	_double				m_dDeltaTime = 0;


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();


public:
	static CUI_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END