#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Texture.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Terrain_Dynamic;
END

class CTerrain_MapTool final : public CGameObject
{
public:
	enum TEXTURETYPE
	{
		TYPE_DIFFUSE, TYPE_FILTER, TYPE_BRUSH, TYPE_END
	};
private:
	CTerrain_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTerrain_MapTool(const CTerrain_MapTool& rhs);
	virtual ~CTerrain_MapTool() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom[TYPE_END] = { nullptr };
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Terrain_Dynamic*	m_pVIBufferCom = nullptr;

	ID3D11ShaderResourceView*	m_pSRV = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	HRESULT	Create_FilterTexture();


public:
	static CTerrain_MapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


#if defined(USE_IMGUI)
public:
	virtual void Running_Window();
	virtual void Running_Editer_Window();
private:
	void Widget_ObjectList();
	void Widget_PrototypeList();
	void Widget_Save_Load_Objects();
	void Widget_Add_Object();

public:
	typedef enum eTerrain_Edit_Type
	{
		TERRAIN_EDIT_TYPE_HEIGHT,
		TERRAIN_EDIT_TYPE_TEXTURE_01,
		TERRAIN_EDIT_TYPE_TEXTURE_02,
		TERRAIN_EDIT_TYPE_TEXTURE_03,
		TERRAIN_EDIT_TYPE_END
	}TERRAIN_EDIT_TYPE;

public:	//맵툴 도구
	TERRAIN_EDIT_TYPE		m_eEdit_Type = TERRAIN_EDIT_TYPE_END;
	_float					m_vDampning[4];
	_float					m_vAddValues;
	_float					m_fMaxHeight = 50.f;
	_float					m_fMinHeight = 0.f;
	_uint					m_eEditMode = TERRAIN_EDIT_TYPE_END;
	_uint					m_iFPS = 0;
	_bool					m_bIsChanged = false;
public:	//맵툴 함수
	void Edit_Input();	//툴로 지정한 값들로 터레인을 수정합니다.
	void Set_Range_And_Dampning();
	void Set_Value_Option();
	void Set_Edit_Type_Option();
	void Save_Map();
	void Load_Map();
#endif

};

