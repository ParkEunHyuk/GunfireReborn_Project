#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include <comdef.h>

#if defined(USE_IMGUI)
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
BEGIN(Engine)
class CTransform;
class CGameObject;
END

BEGIN(Client)

class CImGui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager)

private:
	CImGui_Manager();
	virtual ~CImGui_Manager() = default;

public:
	void	ImGui_SetUp(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void	ImGui_Tick();
	HRESULT	Begin_Frame(const char* szFrameName, _uint iFlag);
	HRESULT	End_Frame();

	void	ImGui_Render();

public:
	void Small_Separator();
	void Big_Separator();

public:
	//HRESULT Show_Transform(void** ppArg, const char* pName);
	//HRESULT	Make_Terrain(void** ppArg, const char* pName);

	HRESULT Show_Transform(CGameObject* pGameObject, const char* pName);
	HRESULT	Show_ObjectList(const list<CGameObject*>&Layerlist,const char* pLayerTag);

	CGameInstance*			 m_pGameInstance = nullptr;
	ID3D11Device*            m_pDevice = nullptr;
	ID3D11DeviceContext*     m_pDeviceContext = nullptr;

public:
	virtual	void Free()override;


};

END

#endif