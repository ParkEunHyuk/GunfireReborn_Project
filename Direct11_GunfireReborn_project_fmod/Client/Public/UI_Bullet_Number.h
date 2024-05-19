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

class CUI_Bullet_Number final : public CUI
{
public:
	typedef struct tagUI_NumberDesc
	{
		MAGAZINETAG eType;
		_float	fSizeX;
		_float	fSizeY;
		_float	fX;
		_float	fY;
	}UI_NUMBERDESC;
private:
	explicit CUI_Bullet_Number(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Bullet_Number(const CUI_Bullet_Number& rhs);
	virtual ~CUI_Bullet_Number() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

	void	Set_Number(_uint _iNumber);
	_uint	Get_Number() { return m_iNumber; }
	_vector Get_UI_NumberPos() { return XMLoadFloat4(&m_fTempPos);}

	//탄창과 관련된 내용
public:
	void	Set_Magazine(_uint	_iMagazine);
	_bool	SetUp_MagazineNumber(_uint	_iTenth,_uint _Once);
	_bool	Get_CountingNumberOn() { return m_bCountingNumberOn; }

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

public:
	UI_NUMBERDESC		m_UI_NumberDesc;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	_uint				m_iNumber = 0;

	_float4				m_fTempPos;;

private:
	_bool				m_bCountingNumberOn = true;


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();


public:
	static CUI_Bullet_Number* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END