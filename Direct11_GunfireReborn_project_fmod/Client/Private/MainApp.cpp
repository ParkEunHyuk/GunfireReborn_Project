#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "Main_Logo.h"
#include "Sky.h"
#include "Camera_Dynamic.h"
#include "Camera_Player.h"
#include "Terrain.h"
#include "Player.h"
#include "UI_Loading.h"
#include "Terrain_MapTool.h"

#if defined(USE_IMGUI)
#include "ImGui_Manager.h"
#endif

//여기 바꿔줬으면 Level_ImGui_MapTool이랑 Client_Define 바꿔줘야함
//여긴 _tchar형이고 Level_ImGui_MapTool의 char랑 다름
const _tchar* g_pModelTag[MODEL_END] = {
	TEXT("Prototype_Component_Model_Fork"),
	TEXT("Prototype_Component_Model_Fiona"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_1"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_2"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_3"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_4"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_5"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_6"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_7"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_8"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_1"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_2"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_3"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_4"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_5"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_6"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_7"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_8"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_9"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_10"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_11"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_12"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_13"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_14"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Ship_1"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Ship_2"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_1"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_2"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_3"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_4"),
	TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_5"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_1"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_2"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_3"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_4"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_5"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_6"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_7"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_8"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_9"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_10"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_11"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_12"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_1"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_2"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_3"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_4"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_5"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_6"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_7"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_8"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_9"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_10"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_11"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_12"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_1"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_2"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_3"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_1"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_2"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_3"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_4"),
	TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_5"),
	TEXT("Prototype_Component_Model_Terrain_Volcano_1"),
	TEXT("Prototype_Component_Model_Terrain_Volcano_2"),
	TEXT("Prototype_Component_Model_Terrain_Volcano_3"),
	TEXT("Prototype_Component_Model_Terrain_Volcano_Rock_1"),
	TEXT("Prototype_Component_Model_Terrain_Volcano_Rock_2"),
	TEXT("Prototype_Component_Model_Terrain_Mountain_1"),
	TEXT("Prototype_Component_Model_Terrain_Mountain_2"),
	TEXT("Prototype_Component_Model_Terrain_Mountain_3"),
	TEXT("Prototype_Component_Model_Terrain_Mountain_4"),
	TEXT("Prototype_Component_Model_Terrain_Mountain_5"),
	TEXT("Prototype_Component_Model_Terrain_Mountain_6"),
	TEXT("Prototype_Component_Model_Terrain_Snow_Rock_1"),
	TEXT("Prototype_Component_Model_Terrain_Snow_Rock_2"),
	TEXT("Prototype_Component_Model_Terrain_Snow_Rock_3"),
	TEXT("Prototype_Component_Model_Terrain_Rock_1"),
	TEXT("Prototype_Component_Model_Terrain_Rock_2"),
	TEXT("Prototype_Component_Model_Terrain_Rock_3"),
	TEXT("Prototype_Component_Model_Terrain_Rock_4"),
	TEXT("Prototype_Component_Model_Terrain_Decoration_1"),
	TEXT("Prototype_Component_Model_Terrain_Decoration_2"),
	TEXT("Prototype_Component_Model_Terrain_Decoration_3"),
	TEXT("Prototype_Component_Model_Terrain_Decoration_4"),
	TEXT("Prototype_Component_Model_Terrain_Snow_1"),
	TEXT("Prototype_Component_Model_Terrain_Snow_2"),
	TEXT("Prototype_Component_Model_Terrain_Snow_3"),
	TEXT("Prototype_Component_Model_Terrain_Door_1"),
	TEXT("Prototype_Component_Model_Terrain_Door_2"),
	TEXT("Prototype_Component_Model_Terrain_Volcano_Far_1"),
	TEXT("Prototype_Component_Model_Terrain_Volcano_Far_2"),
	TEXT("Prototype_Component_Model_Terrain_SnowMountain_Far_1"),
	TEXT("Prototype_Component_Model_Terrain_SnowMountain_Far_2"),
	TEXT("Prototype_Component_Model_Terrain_SubDecoration_1"),
	TEXT("Prototype_Component_Model_Terrain_SubDecoration_2"),
	TEXT("Prototype_Component_Model_Terrain_SubDecoration_3"),
	TEXT("Prototype_Component_Model_Terrain_SubDecoration_4"),
	TEXT("Prototype_Component_Model_Terrain_SubDecoration_5"),
	TEXT("Prototype_Component_Model_Terrain_SubDecoration_6"),
	TEXT("Prototype_Component_Model_Terrain_SubDecoration_7"),
	TEXT("Prototype_Component_Model_Terrain_SubDecoration_8"),
	TEXT("Prototype_Component_Model_Terrain_SubDecoration_9"),
	TEXT("Prototype_Component_Model_Lizard"),
	TEXT("Prototype_Component_Model_GostDeer"),
	TEXT("Prototype_Component_Model_Horse"),
	TEXT("Prototype_Component_Model_Cube"),
	TEXT("Prototype_Component_Model_PowerLizard"),
	TEXT("Prototype_Component_Model_Raven"),
	TEXT("Prototype_Component_Model_Lamp"),
	TEXT("Prototype_Component_Model_Catfish"),
	TEXT("Prototype_Component_Model_Diego"),
	TEXT("Prototype_Component_Model_Boss_Dokkaebi"),
	TEXT("Prototype_Component_Model_Boss_Griffin"),
	TEXT("Prototype_Component_Model_Boss_Bear")
};

const _tchar* g_pLayerTag[LAYER_END] = {
	TEXT("Layer_Static"),
	TEXT("Layer_Player"),
	TEXT("Layer_Monster")
};

const _tchar* g_pPrototypeObjectTag[OBJECT_END] = {
	TEXT("Prototype_GameObject_NonAnimModel"),
	TEXT("Prototype_GameObject_Player"),
	TEXT("Prototype_GameObject_Lizard"),
	TEXT("Prototype_GameObject_GostDeer"),
	TEXT("Prototype_GameObject_Horse"),
	TEXT("Prototype_GameObject_Cube"),
	TEXT("Prototype_GameObject_PowerLizard"),
	TEXT("Prototype_GameObject_Raven"),
	TEXT("Prototype_GameObject_Lamp"),
	TEXT("Prototype_GameObject_Catfish"),
	TEXT("Prototype_GameObject_Diego"),
	TEXT("Prototype_GameObject_Boss_Dokkaebi"),
	TEXT("Prototype_GameObject_Boss_Griffin"),
	TEXT("Prototype_GameObject_Boss_Bear")
};

int g_iMagazine[MAGAZINETYPE_END] = { 0,5,0,5 };

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	CGraphic_Device::GRAPHICDESC GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = CGraphic_Device::MODE_WIN;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pDeviceContext)))
	{
		MSGBOX("failed CMainApp::NativeConstruct Initialize_Engine");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_Components()))
	{
		MSGBOX("failed CMainApp::NativeConstruct Ready_Prototype_Components");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObjects()))
	{
		MSGBOX("failed CMainApp::NativeConstruct Ready_Prototype_GameObjects");
		return E_FAIL;
	}

	
	if (FAILED(Open_Level(LEVEL_LOGO)))
	{
		MSGBOX("failed CMainApp::NativeConstruct Open_Level(LEVEL_LOGO)");
		return E_FAIL;
	}


#if defined(USE_IMGUI)
	//ImGui_Manager 초기화
	CImGui_Manager::GetInstance()->ImGui_SetUp(m_pDevice, m_pDeviceContext);
#endif

	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return -1;
#if defined(USE_IMGUI)
	CImGui_Manager::GetInstance()->ImGui_Tick(); // MapTool에 있던걸 옮김
#endif

#ifdef _DEBUG
	m_dTimerAcc += TimeDelta;
#endif // _DEBUG

	if (0 > m_pGameInstance->Tick_Engine(TimeDelta))
		return -1;

	return _int();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	//나중에 바꾸던가 해야지 이거 잘 보면 엔진 타입뎊에 있는건데 _float4로 받는건 없음 그래서 이렇게 넣어줘야함
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.0f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	if (FAILED(m_pRenderer->Render()))
	{
		MSGBOX("failed CMainApp::Render m_pRenderer->Render()");
		return E_FAIL;
	}

#ifdef _DEBUG
	m_pGameInstance->GetInstance()->Obsever_Render();
#endif
		
	m_pGameInstance->Render_Level();

#if defined(USE_IMGUI)
	CImGui_Manager::GetInstance()->ImGui_Render();
#endif

	m_pGameInstance->Present();

	m_pGameInstance->GetInstance()->Release_ObseverGroup();
	//여기에 옵저버 그룹 릴리즈

#ifdef _DEBUG
	++m_dwNumRender;

	if (m_dTimerAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_dwNumRender);
		SetWindowText(g_hWnd, m_szFPS);

		m_dwNumRender = 0;
		m_dTimerAcc = 0.f;
	}
#endif // _DEBUG

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelIndex)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	HRESULT		hr = 0;

	switch (eLevelIndex)
	{
	default:
		hr = m_pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, eLevelIndex));
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;

}

HRESULT CMainApp::Ready_Prototype_Components()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_Component_Renderer */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), 
		m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;	

	/* For.Prototype_Component_Transform */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VTXTEX */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXTEX"), 
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_OBB))))
		return E_FAIL;

#pragma region Prototype_Component_Texture
	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Default%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Loading */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Loading/Loading_%d.png"), 7))))
		return E_FAIL;


#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObjects()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),CTerrain::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_Terrain");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain_MapTool"), CTerrain_MapTool::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_Terrain_MapTool");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),CSky::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_Sky");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Camera_Dynamic */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),CCamera_Dynamic::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_Camera_Dynamic");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Camera_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Player"), CCamera_Player::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_Camera_Player");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_Player");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_UI_Loading */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading"), CUI_Loading::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CUI_Loading 생성 실패");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);

#if defined(USE_IMGUI)
	CImGui_Manager::DestroyInstance();
#endif
	CGameInstance::Release_Engine();
}
