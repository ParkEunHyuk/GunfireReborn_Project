#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameObject.h"
#include "Layer.h"

BEGIN(Engine)
class CCell;
class CShader;
class CVIBuffer_Triangle;
END

BEGIN(Client)

class CLevel_ImGui_MapTool final : public CLevel
{
public:
	explicit CLevel_ImGui_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_ImGui_MapTool() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Lights();

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);

private:
	HRESULT ImGui_First_Frame(const char * szFrameName);
	HRESULT ImGui_Sec_Frame(const char * szFrameName);
	HRESULT	ImGui_Stage(const char*	szFrameName);
	HRESULT	ImGui_Window4(const char*	szFrameName);
	HRESULT	ImGui_File(const char* szFrameName);
	HRESULT	ImGui_ggg(const char* szFrameName);

	
private:
	map<const _tchar*, CGameObject*>*    m_pObject_Prototypes = nullptr;
	map<const _tchar*, CLayer*>*		 m_pObject_Layers = nullptr;

public:
	static CLevel_ImGui_MapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint m_eNextLevel);
	virtual void Free() override;

//ImGui======================================================================================
#if defined(USE_IMGUI)
private:
	class CImGui_Manager* pImgui = nullptr;
	_bool m_bPreventRender = false;
	_bool m_bTest = false;

private:
	void Set_Model(_uint iModelIndex, _uint iLayerIndex, _uint iObjectIndex);
	void Show_LayerList();
	void All_Layer_Save(const char* pFilePath);
	void All_Layer_Load(const char* pFilePath);


	///테스트 중
private:/* Navi_Tool */
	HRESULT	ImGui_Nevi(const char* szFrameName);
	void Navi_Tool();
	void Make_Navi(_float3 vPickPos);
	void Save_Navi(const char* pNaviFilePath);
	void Load_Navi(const char* pNaviFilePath);
	void Show_Cells();
	void Check_Collider(_float3* pPickPos);
	void Move_Collider();
	void Sort_CCW(_float3* pPoints);

private:
	vector<CCell*>						 m_Navi;
	_float3								 m_vPoints[3];
	_uint								 m_iCount = 0;
	CVIBuffer_Triangle*					 m_pVIBuffer = nullptr;
	CShader*							 m_pShader = nullptr;



	_bool								m_bPlayer = false; //플레이어 임시 생성
	//테스트중

#endif
//ImGui======================================================================================
};

END