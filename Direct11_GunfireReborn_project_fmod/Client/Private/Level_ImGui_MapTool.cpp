#include "stdafx.h"
#include "..\Public\Level_ImGui_MapTool.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"
#include "Terrain_MapTool.h"
#include "Cell.h"
#include "VIBuffer_Triangle.h"

#if defined(USE_IMGUI)
#include "ImGui_Manager.h"
#endif

//여기 바꿔줬으면 MainApp.cpp랑 Client_Define 바꿔줘야함
//여긴 char형이고 MainApp의 tchar랑 다름
const char* g_pModelComponetTag[MODEL_END] = {
	"Prototype_Component_Model_Fork",
	"Prototype_Component_Model_Fiona",
	"Prototype_Component_Model_Terrain_Stage1_Decoration_1",
	"Prototype_Component_Model_Terrain_Stage1_Decoration_2",
	"Prototype_Component_Model_Terrain_Stage1_Decoration_3",
	"Prototype_Component_Model_Terrain_Stage1_Decoration_4",
	"Prototype_Component_Model_Terrain_Stage1_Decoration_5",
	"Prototype_Component_Model_Terrain_Stage1_Decoration_6",
	"Prototype_Component_Model_Terrain_Stage1_Decoration_7",
	"Prototype_Component_Model_Terrain_Stage1_Decoration_8",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_1",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_2",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_3",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_4",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_5",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_6",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_7",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_8",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_9",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_10",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_11",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_12",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_13",
	"Prototype_Component_Model_Terrain_Stage1_Mountain_14",
	"Prototype_Component_Model_Terrain_Stage1_Ship_1",
	"Prototype_Component_Model_Terrain_Stage1_Ship_2",
	"Prototype_Component_Model_Terrain_Stage1_Wall_1",
	"Prototype_Component_Model_Terrain_Stage1_Wall_2",
	"Prototype_Component_Model_Terrain_Stage1_Wall_3",
	"Prototype_Component_Model_Terrain_Stage1_Wall_4",
	"Prototype_Component_Model_Terrain_Stage1_Wall_5",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_1",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_2",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_3",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_4",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_5",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_6",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_7",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_8",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_9",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_10",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_11",
	"Prototype_Component_Model_Terrain_Stage2_Decoration_12",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_1",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_2",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_3",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_4",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_5",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_6",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_7",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_8",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_9",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_10",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_11",
	"Prototype_Component_Model_Terrain_Stage2_Mountain_12",
	"Prototype_Component_Model_Terrain_Stage2_Snow_1",
	"Prototype_Component_Model_Terrain_Stage2_Snow_2",
	"Prototype_Component_Model_Terrain_Stage2_Snow_3",
	"Prototype_Component_Model_Terrain_Stage2_Wall_1",
	"Prototype_Component_Model_Terrain_Stage2_Wall_2",
	"Prototype_Component_Model_Terrain_Stage2_Wall_3",
	"Prototype_Component_Model_Terrain_Stage2_Wall_4",
	"Prototype_Component_Model_Terrain_Stage2_Wall_5",
	"Prototype_Component_Model_Terrain_Volcano_1",
	"Prototype_Component_Model_Terrain_Volcano_2",
	"Prototype_Component_Model_Terrain_Volcano_3",
	"Prototype_Component_Model_Terrain_Volcano_Rock_1",
	"Prototype_Component_Model_Terrain_Volcano_Rock_2",
	"Prototype_Component_Model_Terrain_Mountain_1",
	"Prototype_Component_Model_Terrain_Mountain_2",
	"Prototype_Component_Model_Terrain_Mountain_3",
	"Prototype_Component_Model_Terrain_Mountain_4",
	"Prototype_Component_Model_Terrain_Mountain_5",
	"Prototype_Component_Model_Terrain_Mountain_6",
	"Prototype_Component_Model_Terrain_Snow_Rock_1",
	"Prototype_Component_Model_Terrain_Snow_Rock_2",
	"Prototype_Component_Model_Terrain_Snow_Rock_3",
	"Prototype_Component_Model_Terrain_Rock_1",
	"Prototype_Component_Model_Terrain_Rock_2",
	"Prototype_Component_Model_Terrain_Rock_3",
	"Prototype_Component_Model_Terrain_Rock_4",
	"Prototype_Component_Model_Terrain_Decoration_1",
	"Prototype_Component_Model_Terrain_Decoration_2",
	"Prototype_Component_Model_Terrain_Decoration_3",
	"Prototype_Component_Model_Terrain_Decoration_4",
	"Prototype_Component_Model_Terrain_Snow_1",
	"Prototype_Component_Model_Terrain_Snow_2",
	"Prototype_Component_Model_Terrain_Snow_3",
	"Prototype_Component_Model_Terrain_Door_1",
	"Prototype_Component_Model_Terrain_Door_2",
	"Prototype_Component_Model_Terrain_Volcano_Far_1",
	"Prototype_Component_Model_Terrain_Volcano_Far_2",
	"Prototype_Component_Model_Terrain_SnowMountain_Far_1",
	"Prototype_Component_Model_Terrain_SnowMountain_Far_2",
	"Prototype_Component_Model_Terrain_SubDecoration_1",
	"Prototype_Component_Model_Terrain_SubDecoration_2",
	"Prototype_Component_Model_Terrain_SubDecoration_3",
	"Prototype_Component_Model_Terrain_SubDecoration_4",
	"Prototype_Component_Model_Terrain_SubDecoration_5",
	"Prototype_Component_Model_Terrain_SubDecoration_6",
	"Prototype_Component_Model_Terrain_SubDecoration_7",
	"Prototype_Component_Model_Terrain_SubDecoration_8",
	"Prototype_Component_Model_Terrain_SubDecoration_9",
	"Prototype_Component_Model_Lizard",
	"Prototype_Component_Model_GostDeer",
	"Prototype_Component_Model_Horse",
	"Prototype_Component_Model_Cube",
	"Prototype_Component_Model_PowerLizard",
	"Prototype_Component_Model_Raven",
	"Prototype_Component_Model_Lamp",
	"Prototype_Component_Model_Catfish",
	"Prototype_Component_Model_Diego",
	"Prototype_Component_Model_Boss_Dokkaebi",
	"Prototype_Component_Model_Boss_Griffin",
	"Prototype_Component_Model_Boss_Bear"
};

const char* g_pObjectLayerTag[LAYER_END] = {
	"Layer_Static",
	"Layer_Player",
	"Layer_Monster"
};

const char* g_pObjectTag[OBJECT_END] = {
	"Prototype_GameObject_NonAnimModel",
	"Prototype_GameObject_Player",
	"Prototype_GameObject_Lizard",
	"Prototype_GameObject_GostDeer",
	"Prototype_GameObject_Horse",
	"Prototype_GameObject_Cube",
	"Prototype_GameObject_PowerLizard",
	"Prototype_GameObject_Raven",
	"Prototype_GameObject_Lamp",
	"Prototype_GameObject_Catfish",
	"Prototype_GameObject_Diego",
	"Prototype_GameObject_Boss_Dokkaebi",
	"Prototype_GameObject_Boss_Griffin",
	"Prototype_GameObject_Boss_Bear"
};

_float CCW(_float3 vPoint1, _float3 vPoint2, _float3 vPoint3) {
	return (vPoint1.x * vPoint2.z) + (vPoint2.x * vPoint3.z) + (vPoint3.x * vPoint1.z) -
		((vPoint2.x * vPoint1.z) + (vPoint3.x * vPoint2.z) + (vPoint1.x * vPoint3.z));
}

_bool		g_bPicking = false;
_bool		g_bShow_NaviMesh = false;

CLevel_ImGui_MapTool::CLevel_ImGui_MapTool(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_ImGui_MapTool::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;
#if defined(USE_IMGUI)
	pImgui = CImGui_Manager::GetInstance();
#endif
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

#if defined(USE_IMGUI)
	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXCOL_DECLARATION::Elements, VTXCOL_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Triangle::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

#if defined(USE_IMGUI)
	if (pImgui == nullptr)
		E_FAIL;

	m_bPreventRender = false;
#endif

#if defined(USE_IMGUI)
	CGameInstance*  pGameInstance = GET_INSTANCE(CGameInstance);
	m_pObject_Prototypes = pGameInstance->Get_Prototypes();
	m_pObject_Layers = pGameInstance->Get_Layers();
	RELEASE_INSTANCE(CGameInstance);
#endif
	return S_OK;
}

_int CLevel_ImGui_MapTool::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

#if defined(USE_IMGUI)
	//pImgui->ImGui_Tick(); 이걸 잠시 main에 두겠음

	for (auto& pCell : m_Navi)
		pCell->Update(XMMatrixIdentity());

	ImGui_First_Frame("Frame_First"); // ImGui 데모

	ImGui_Stage("Stage_Change"); //여기에 게임플레이감

	//ImGui_Nevi("NeviMesh");

	//ImGui_Sec_Frame("Frame_Second");

	//ImGui_Window4("Frame_Window4"); //리스트 박스 있는거
	//안쓰는것들

	//윈도우 창 만들기
	//ImGui_File("Frame_File");
	//ImGui_ggg("ggg");

//	m_bPreventRender = true;

	ImGui::Begin("Model");

	if (ImGui::CollapsingHeader("ObectList"))
		Show_LayerList();

	CImGui_Manager::GetInstance()->Small_Separator();
	CImGui_Manager::GetInstance()->Small_Separator();

	if (ImGui::CollapsingHeader("Select"))
	{
		static _int iModeIndex = 0, iLayerIndex = 0, iObjectIndex = 0;
		ImGui::ListBox(" Static Model", &iModeIndex, g_pModelComponetTag, IM_ARRAYSIZE(g_pModelComponetTag));
		ImGui::ListBox(" Layer", &iLayerIndex, g_pObjectLayerTag, IM_ARRAYSIZE(g_pObjectLayerTag));
		ImGui::ListBox(" Object", &iObjectIndex, g_pObjectTag, IM_ARRAYSIZE(g_pObjectTag));
		Set_Model(iModeIndex, iLayerIndex, iObjectIndex);
	}

	CImGui_Manager::GetInstance()->Small_Separator();
	CImGui_Manager::GetInstance()->Small_Separator();

	//if (ImGui::CollapsingHeader("SaveFile"))
	//{
	ImGui::Text("Save Object");
	static char szSaveFilePath[MAX_PATH] = "../Bin/Save/";
	ImGui::InputText(" SaveFilePath", szSaveFilePath, IM_ARRAYSIZE(szSaveFilePath));
	if (true == ImGui::Button("SaveObject"))
		All_Layer_Save(szSaveFilePath);
	//}

	CImGui_Manager::GetInstance()->Small_Separator();
	CImGui_Manager::GetInstance()->Small_Separator();

	//if (ImGui::CollapsingHeader("LoadFile"))
	//{
	ImGui::Text("Load Object");
	static char szLoadFilePath[MAX_PATH] = "../Bin/Save/";
	ImGui::InputText(" LoadFilePath", szLoadFilePath, IM_ARRAYSIZE(szLoadFilePath));
	if (true == ImGui::Button("LoadObject"))
		All_Layer_Load(szLoadFilePath);
	//}

	ImGui::End();

#endif

	return _int();
}

_int CLevel_ImGui_MapTool::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;


	return _int();
}

HRESULT CLevel_ImGui_MapTool::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("ToolMode"));

#if defined(USE_IMGUI)
	if (false == m_Navi.empty() && true == g_bShow_NaviMesh)
	{
		CGameInstance*  pGameInstance = GET_INSTANCE(CGameInstance);

		m_pShader->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
		m_pShader->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
		m_pShader->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

		for (auto& pCell : m_Navi)
		{
			if (nullptr != pCell)
			{
				pCell->Render(m_pVIBuffer, m_pShader, _float4(1.f, 1.f, 1.f, 1.f));
			}
		}

		RELEASE_INSTANCE(CGameInstance);
	}
#endif

	//if (m_bPreventRender)         잠시 메인에 두겠음
	//{
	//	pImgui->ImGui_Render();
	//	m_bPreventRender = false;
	//}

	return S_OK;


}

HRESULT CLevel_ImGui_MapTool::Ready_Lights()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC		LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	//점광원인데 아직 문제가 많음
	/*ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vPosition = _float4(5.f, 3.f, 5.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
	return E_FAIL;
	*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_ImGui_MapTool::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성한다. */
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, 10.f, -7.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAPTOOL, pLayerTag, TEXT("Prototype_GameObject_Camera_Dynamic"), &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_ImGui_MapTool::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAPTOOL, pLayerTag, TEXT("Prototype_GameObject_Terrain_MapTool"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAPTOOL, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_ImGui_MapTool::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAPTOOL, pLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_ImGui_MapTool::ImGui_First_Frame(const char * szFrameName)
{
#if defined(USE_IMGUI)
	_bool temp = true;
	ImGui::ShowDemoWindow(&temp);
	//이게 데모 윈도우 띄워주는 창
#endif

	return S_OK;
}



HRESULT CLevel_ImGui_MapTool::ImGui_Sec_Frame(const char * szFrameName)
{
#if defined(USE_IMGUI)
	pImgui->Begin_Frame(szFrameName, 0);

	_bool temp = false;

	ImGui::Text("This is some useful text.");   // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &temp);      // Edit bools storing our window open/close state
												//ImGui::Checkbox("Another Window", &show_another_window);

												//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
												//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

												//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
												//	counter++;
												//ImGui::SameLine();
												//ImGui::Text("counter = %d", counter);

												//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

												//슬라이드
	static int ix = 0;
	static int iy = 0;
	static int iz = 0;

	ImGui::SliderInt("X", &ix, -1, 3);
	ImGui::SliderInt("Y", &iy, -1, 3);
	ImGui::SliderInt("Z", &iz, -1, 3);

	//컬러
	static bool alpha_preview = true;
	static bool alpha_half_preview = false;
	static bool drag_and_drop = true;
	static bool options_menu = true;
	static bool hdr = false;
	static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
	ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

	ImGui::Text("Color widget:");
	ImGui::ColorEdit3("MyColor##1", (float*)&color, misc_flags);

	//드래그
	static int i1 = 50, i2 = 42;
	ImGui::DragInt("drag int", &i1, 1);
	ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);

	static float f1 = 1.00f, f2 = 0.0067f;
	ImGui::DragFloat("drag float", &f1, 0.005f);
	ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
	//ImGui::SameLine(); 



	//if (ImGui::TreeNode("Popups"))
	//{
	//	ImGui::TextWrapped(
	//		"When a popup is active, it inhibits interacting with windows that are behind the popup. "
	//		"Clicking outside the popup closes it.");

	//	static int selected_fish = -1;
	//	const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
	//	static bool toggles[] = { true, false, false, false, false };

	//	// Simple selection popup (if you want to show the current selection inside the Button itself,
	//	// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
	//	if (ImGui::Button("Select.."))
	//		ImGui::OpenPopup("my_select_popup");
	//	ImGui::SameLine();
	//	ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
	//	if (ImGui::BeginPopup("my_select_popup"))
	//	{
	//		ImGui::Text("Aquarium");
	//		ImGui::Separator();
	//		for (int i = 0; i < IM_ARRAYSIZE(names); i++)
	//			if (ImGui::Selectable(names[i]))
	//				selected_fish = i;
	//		ImGui::EndPopup();
	//	}

	//}



	pImgui->End_Frame();
#endif
	return S_OK;
}

HRESULT CLevel_ImGui_MapTool::ImGui_Stage(const char * szFrameName)
{
#if defined(USE_IMGUI)
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pImgui->Begin_Frame(szFrameName, 0);

	//if (ImGui::Button("Save"))
	//{
	//	HANDLE hFile = CreateFile(TEXT("../Bin/Resources/MapData/test.dat"), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	//	if (INVALID_HANDLE_VALUE == hFile)
	//	{
	//		MSGBOX("세이브 실패");
	//		return 0;
	//	}

	//}

	//if (ImGui::Button("Load"))
	//{
	//	int a = 10;
	//}

	if (ImGui::Button("Stage Level_GamePlay"))
	{

		pGameInstance->GetInstance()->Release_ObseverGroup(); //이건 옵저버 패턴을 사용하고 있는 와중에 다음 레벨로 넘어가서 오브젝트는 없는데  읽으려고 해서 생기는 문제이다. 그래서 넣어줌

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_GAMEPLAY))))
		{
			MSGBOX("Failed to Create LEVEL_GAMEPLAY from Level_ImGui_MapTool");
			return E_FAIL;
		}
	}

	if (ImGui::Button("Stage Level_Stage1"))
	{

		pGameInstance->GetInstance()->Release_ObseverGroup(); //이건 옵저버 패턴을 사용하고 있는 와중에 다음 레벨로 넘어가서 오브젝트는 없는데  읽으려고 해서 생기는 문제이다. 그래서 넣어줌

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE1))))
		{
			MSGBOX("Failed to Create LEVEL_STAGE1 from Level_ImGui_MapTool");
			return E_FAIL;
		}
	}

	if (ImGui::Button("Stage Level_Stage1_BossRoom"))
	{

		pGameInstance->GetInstance()->Release_ObseverGroup(); //이건 옵저버 패턴을 사용하고 있는 와중에 다음 레벨로 넘어가서 오브젝트는 없는데  읽으려고 해서 생기는 문제이다. 그래서 넣어줌

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE1_BOSSROOM))))
		{
			MSGBOX("Failed to Create LEVEL_STAGE1_BOSSROOM from Level_ImGui_MapTool");
			return E_FAIL;
		}
	}

	if (ImGui::Button("Stage Level_Stage2"))
	{

		pGameInstance->GetInstance()->Release_ObseverGroup(); //이건 옵저버 패턴을 사용하고 있는 와중에 다음 레벨로 넘어가서 오브젝트는 없는데  읽으려고 해서 생기는 문제이다. 그래서 넣어줌

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE2))))
		{
			MSGBOX("Failed to Create LEVEL_STAGE2 from Level_ImGui_MapTool");
			return E_FAIL;
		}
	}
	if (ImGui::Button("Stage Level_Stage1"))
	{

		pGameInstance->GetInstance()->Release_ObseverGroup(); //이건 옵저버 패턴을 사용하고 있는 와중에 다음 레벨로 넘어가서 오브젝트는 없는데  읽으려고 해서 생기는 문제이다. 그래서 넣어줌

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE1))))
		{
			MSGBOX("Failed to Create LEVEL_STAGE1 from Level_ImGui_MapTool");
			return E_FAIL;
		}
	}

	if (ImGui::Button("Stage Level_Stage1_BossRoom"))
	{

		pGameInstance->GetInstance()->Release_ObseverGroup(); //이건 옵저버 패턴을 사용하고 있는 와중에 다음 레벨로 넘어가서 오브젝트는 없는데  읽으려고 해서 생기는 문제이다. 그래서 넣어줌

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE1_BOSSROOM))))
		{
			MSGBOX("Failed to Create LEVEL_STAGE1_BOSSROOM from Level_ImGui_MapTool");
			return E_FAIL;
		}
	}
	if (ImGui::Button("Stage Level_Stage2_BossRoom"))
	{

		pGameInstance->GetInstance()->Release_ObseverGroup(); //이건 옵저버 패턴을 사용하고 있는 와중에 다음 레벨로 넘어가서 오브젝트는 없는데  읽으려고 해서 생기는 문제이다. 그래서 넣어줌

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE2_BOSSROOM))))
		{
			MSGBOX("Failed to Create LEVEL_STAGE_BOSSROOM from Level_ImGui_MapTool");
			return E_FAIL;
		}
	}

	//if (ImGui::Button("Make"))
	//{
	//	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
	//		return E_FAIL;
	//}




	pImgui->End_Frame();

	RELEASE_INSTANCE(CGameInstance);

#endif
	return S_OK;
}

HRESULT CLevel_ImGui_MapTool::ImGui_Window4(const char * szFrameName)
{
#if defined(USE_IMGUI)
	pImgui->Begin_Frame(szFrameName, 0);

	if (ImGui::TreeNode("List boxes"))
	{
		// Using the generic BeginListBox() API, you have full control over how to display the combo contents.
		// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
		// stored in the object itself, etc.)
		const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
		static int item_current_idx = 0; // Here we store our selection data as an index.
		if (ImGui::BeginListBox("listbox 1"))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		// Custom size: use all width, 5 items tall
		ImGui::Text("Full-width:");
		if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		ImGui::TreePop();
	}

	pImgui->End_Frame();
#endif
	return S_OK;
}

HRESULT CLevel_ImGui_MapTool::ImGui_File(const char * szFrameName)
{
#if defined(USE_IMGUI)
	bool my_tool_active = true;
	ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Edit a color (stored as ~4 floats)
	static _float myColor[4] = { 0,0,0,0 };

	ImGui::ColorEdit4("Color", myColor);

	// Plot some values
	const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();
#endif

	return S_OK;
}

HRESULT CLevel_ImGui_MapTool::ImGui_ggg(const char * szFrameName)
{
#if defined(USE_IMGUI)
	static bool pOpen[2] = {};
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Example: Simple layout", pOpen, ImGuiWindowFlags_MenuBar))
	{

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close")) *pOpen = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Left
		static int selected = 0;
		{
			ImGui::BeginChild("left pane", ImVec2(150, 0), true);
			for (int i = 0; i < 100; i++)
			{
				// FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
				char label[128];
				sprintf_s(label, "MyObject %d", i);
				if (ImGui::Selectable(label, selected == i))
					selected = i;
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();

		// Right
		{
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("MyObject: %d", selected);
			ImGui::Separator();
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Description"))
				{
					ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Details"))
				{
					ImGui::Text("ID: 0123456789");
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			if (ImGui::Button("Revert")) {}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {}
			ImGui::EndGroup();
		}
	}
	ImGui::End();
#endif
	return S_OK;
}

#if defined(USE_IMGUI)

HRESULT CLevel_ImGui_MapTool::ImGui_Nevi(const char * szFrameName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pImgui->Begin_Frame(szFrameName, 0);

	Navi_Tool();

	pImgui->End_Frame();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLevel_ImGui_MapTool::Navi_Tool()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTerrain_MapTool* pTerrain = dynamic_cast<CTerrain_MapTool*>(pGameInstance->Get_GameObject(LEVEL_MAPTOOL, TEXT("Layer_BackGround")));
	CVIBuffer_Terrain_Dynamic* pVIBuffer_Terrain_Dynamic = (CVIBuffer_Terrain_Dynamic*)pTerrain->Get_Component(TEXT("Com_VIBuffer"));

	static _bool s_bNavi = false;
	ImGui::Checkbox("Navigation", &s_bNavi);
	ImGui::SameLine(100.f);
	ImGui::Checkbox("Render Navi", &g_bShow_NaviMesh);

	if (true == s_bNavi)
	{
		ImGui::Text("Current Cell Count : %d", m_iCount);
		ImGui::Text("All Cell Count : %d", (_uint)m_Navi.size());
		if (pGameInstance->Get_DIKeyState(DIK_C) & DIS_Press)
		{
			if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
			{
				_float3 vPickPos;
				_bool isCheck = pVIBuffer_Terrain_Dynamic->Pick_Dynamic(((CTransform*)pTerrain->Get_Component(CGameObject::m_pTransformTag))->Get_WorldMatrixInverse_Float4x4(), &vPickPos);
				if (true == isCheck) Make_Navi(vPickPos);
			}
		}
	}

	//static _bool s_bMove = false;
	//ImGui::Checkbox(" Move", &s_bMove);
	//if (true == s_bMove)
	//{
	//	Move_Collider();
	//}

	static char szSavePath[MAX_PATH] = "../Bin/Save/";
	ImGui::InputText(" SaveFilePath##2", szSavePath, IM_ARRAYSIZE(szSavePath));
	if (true == ImGui::Button("Save##2"))
		Save_Navi(szSavePath);

	static char szLoadPath[MAX_PATH] = "../Bin/Save/";
	ImGui::InputText(" LoadFilePath##2", szLoadPath, IM_ARRAYSIZE(szLoadPath));
	if (true == ImGui::Button("Load##2"))
		Load_Navi(szLoadPath);

	static _bool bShow = false;
	ImGui::Checkbox(" Show Cells", &bShow);
	if (true == bShow)
		Show_Cells();

	RELEASE_INSTANCE(CGameInstance);
}

void CLevel_ImGui_MapTool::Make_Navi(_float3 vPickPos)
{
	if (0 == m_iCount)
	{
		Check_Collider(&vPickPos);
		m_vPoints[m_iCount++] = vPickPos;
	}
	else if (1 == m_iCount)
	{
		Check_Collider(&vPickPos);
		m_vPoints[m_iCount++] = vPickPos;
	}
	else if (2 == m_iCount)
	{
		Check_Collider(&vPickPos);
		m_vPoints[m_iCount] = vPickPos;
		m_iCount = 0;
		Sort_CCW(m_vPoints);
		CCell* pCell = CCell::Create(m_pDevice, m_pDeviceContext, m_vPoints, (_uint)m_Navi.size());
		pCell->Create_Collider(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"));
		m_Navi.emplace_back(pCell);
	}
}

void CLevel_ImGui_MapTool::Save_Navi(const char * pNaviFilePath)
{
	_tchar szFilePath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, pNaviFilePath, -1, szFilePath, IM_ARRAYSIZE(szFilePath));

	wcscat_s(szFilePath, TEXT(".dat"));

	HANDLE hFile = CreateFile(szFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSGBOX("네비 메쉬 저장 실패..");
		return;
	}

	_ulong	dwByte = 0;

	for (auto& pCell : m_Navi)
		WriteFile(hFile, pCell->Get_Points(), sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);
}

void CLevel_ImGui_MapTool::Load_Navi(const char * pNaviFilePath)
{
	_tchar szFilePath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, pNaviFilePath, -1, szFilePath, IM_ARRAYSIZE(szFilePath));

	wcscat_s(szFilePath, TEXT(".dat"));

	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSGBOX("네비 메쉬 불러오기 실패..");
		return;
	}

	for (auto& pCell : m_Navi)
		Safe_Release(pCell);

	m_Navi.clear();

	_ulong	dwByte = 0;
	_float3	m_vPoints[3];
	_uint	iIndex = 0;

	while (true)
	{
		ReadFile(hFile, m_vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell* pCell = CCell::Create(m_pDevice, m_pDeviceContext, m_vPoints, iIndex++);
		if (nullptr == pCell)
			return;

		pCell->Create_Collider(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"));

		m_Navi.emplace_back(pCell);
	}

	CloseHandle(hFile);
}

void CLevel_ImGui_MapTool::Show_Cells()
{
	_uint iIndex = 0;
	char szBuff[64] = "";
	static _bool bShow[128] = { false };
	auto& iter = m_Navi.begin();
	for (; iter != m_Navi.end(); )
	{
		sprintf_s(szBuff, "Cell%d", iIndex);
		ImGui::Checkbox(szBuff, &bShow[iIndex]);
		if (true == bShow[iIndex])
		{
			ImGui::Begin(szBuff);
			for (_uint i = 0; i < 3; ++i)
			{
				sprintf_s(szBuff, "Cell%d Point%d", iIndex, i);
				ImGui::DragFloat3(szBuff, (_float*)&(*iter)->Get_Points()[i], 0.001f);
			}
			ImGui::End();
		}

		ImGui::SameLine();
		sprintf_s(szBuff, "Delete##%d", iIndex);
		if (true == ImGui::SmallButton(szBuff))
		{
			Safe_Release(*iter);
			iter = m_Navi.erase(iter);
		}
		else
			++iter;

		++iIndex;
	}

	if (true == ImGui::Button("All Cell Delete"))
	{
		for (auto& pCell : m_Navi)
			Safe_Release(pCell);

		m_Navi.clear();
	}
}

void CLevel_ImGui_MapTool::Check_Collider(_float3 * pPickPos)
{
	if (true == m_Navi.empty())
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float4x4 _fIdentity;
	XMStoreFloat4x4(&_fIdentity,XMMatrixIdentity());

	pGameInstance->Transform_ToLocalSpace(_fIdentity);

	_float3	vPos;
	for (auto& pCell : m_Navi)
	{
		if (true == pCell->Check_Collider(&pGameInstance->Get_RayDirInLocal(), &pGameInstance->Get_RayPosInLocal(), &vPos))
		{
			*pPickPos = vPos;
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CLevel_ImGui_MapTool::Move_Collider()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTerrainTransform = dynamic_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround"), CGameObject::m_pTransformTag));

	if (pGameInstance->Get_DIKeyState(DIK_Q) & 0x81)
	{
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON))
		{
			CVIBuffer_Terrain_Dynamic* pVIBufferCom = (CVIBuffer_Terrain_Dynamic*)pGameInstance->Get_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"));

			_float3 vPos;

			if (pVIBufferCom->Pick_Dynamic(pTerrainTransform->Get_WorldMatrixInverse_Float4x4(), &vPos))
			{
				_float4x4 _fIdentity;
				XMStoreFloat4x4(&_fIdentity, XMMatrixIdentity());
				pGameInstance->Transform_ToLocalSpace(_fIdentity);

				for (auto& pCell : m_Navi)
					pCell->Move_Collider(&pGameInstance->Get_RayDirInLocal(), &pGameInstance->Get_RayPosInLocal(), vPos);
			}
		}
		else if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON))
		{
			_long	MouseMove = 0;

			if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
			{
				_float4x4 _fIdentity;
				XMStoreFloat4x4(&_fIdentity, XMMatrixIdentity());

				pGameInstance->Transform_ToLocalSpace(_fIdentity);

				for (auto& pCell : m_Navi)
					pCell->Move_Collider(&pGameInstance->Get_RayDirInLocal(), &pGameInstance->Get_RayPosInLocal(), (_float)MouseMove * -0.05f);
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CLevel_ImGui_MapTool::Sort_CCW(_float3 * pPoints)
{
	if (0.f > CCW(pPoints[0], pPoints[1], pPoints[2]))
		return;
	else
	{
		_float3 vTemp = pPoints[1];
		pPoints[1] = pPoints[2];
		pPoints[2] = vTemp;
	}
}
#endif

CLevel_ImGui_MapTool * CLevel_ImGui_MapTool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint m_eNextLevel)
{
	CLevel_ImGui_MapTool*	pInstance = new CLevel_ImGui_MapTool(pDevice, pDeviceContext);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Set_NowLevelIndex(m_eNextLevel);
	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_ToolMode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_ImGui_MapTool::Free()
{
	__super::Free();

#if defined(USE_IMGUI)

	for (auto& pCell : m_Navi)
		Safe_Release(pCell);

	m_Navi.clear();

#endif

#if defined(USE_IMGUI)
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
#endif
}


//어쩔수 없이 ImGui를 위에 쓴 것도 존재함.
//ImGui================================================================================================
#if defined(USE_IMGUI)
void CLevel_ImGui_MapTool::Set_Model(_uint iModelIndex, _uint iLayerIndex, _uint iObjectIndex)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CVIBuffer_Terrain_Dynamic* pVIBufferCom = (CVIBuffer_Terrain_Dynamic*)pGameInstance->Get_Component(LEVEL_MAPTOOL, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"));

	if (pGameInstance->Get_DIKeyState(DIK_Z) & DIS_Press)
	{
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON)& DIS_Down)
		{
			_float3 vPos;
			_float4x4 WorldMatrixInverse;
			XMStoreFloat4x4(&WorldMatrixInverse, XMMatrixIdentity());
			if (pVIBufferCom->Pick_Dynamic(WorldMatrixInverse, &vPos))
			{
				OBJECTDATADESC tObjDataDesc;
				ZeroMemory(&tObjDataDesc, sizeof(OBJECTDATADESC));
				XMStoreFloat4x4(&tObjDataDesc.TransformMatrix, XMMatrixTranslation(vPos.x, vPos.y, vPos.z));
				tObjDataDesc.iLayerTagIndex = iLayerIndex;
				tObjDataDesc.iModelTagIndex = iModelIndex;
				tObjDataDesc.iObjectTagIndex = iObjectIndex;
				pGameInstance->Add_GameObject(LEVEL_MAPTOOL, g_pLayerTag[iLayerIndex], g_pPrototypeObjectTag[iObjectIndex], &tObjDataDesc);
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

}

void CLevel_ImGui_MapTool::Show_LayerList()
{
	if (true == ImGui::TreeNode("Object"))
	{
		auto iter = m_pObject_Layers[LEVEL_MAPTOOL].begin();

		for (size_t i = 0; i < m_pObject_Layers[LEVEL_MAPTOOL].size(); ++i)
		{
			_bstr_t Layer_Tag(iter->first);

			if (!lstrcmp(TEXT("Layer_Camera"), iter->first) || !lstrcmp(TEXT("Layer_BackGround"), iter->first)
				|| !lstrcmp(TEXT("Layer_UI_GreenMagazine"), iter->first) || !lstrcmp(TEXT("Layer_UI_BlueMagazine"), iter->first)
				|| !lstrcmp(TEXT("Layer_UI_Skill"), iter->first) || !lstrcmp(TEXT("Layer_UI_GreenMagazineTenth"), iter->first)
				|| !lstrcmp(TEXT("Layer_UI_BlueMagazineTenth"), iter->first) || !lstrcmp(TEXT("Layer_Monster_Shield"), iter->first)
				|| !lstrcmp(TEXT("Layer_UI_Universal"), iter->first) || !lstrcmp(TEXT("Layer_UI_Universal_Stamina"), iter->first)
				|| !lstrcmp(TEXT("Layer_UI_Universal_Hp"), iter->first) || !lstrcmp(TEXT("Layer_UI_Universal_Shield"), iter->first))
			{
				++iter;
				continue;
			}

			if (ImGui::TreeNode((void*)(intptr_t)i, Layer_Tag, i))
			{
				CLayer* pLayer = iter->second;
				list<CGameObject*>* Object = pLayer->Get_GameObjectList();
				auto& Layeriter = Object->begin();

				if (ImGui::SmallButton(Layer_Tag))
					m_bTest = true;
				if (true == m_bTest)
				{
					CImGui_Manager::GetInstance()->Show_ObjectList(*Object, Layer_Tag);
					if (ImGui::SmallButton("Exit"))
						m_bTest = false;
				}

				for (size_t j = 0; j < Object->size(); ++j)
				{
					char szObjectTag[MAX_PATH] = "";
					sprintf_s(szObjectTag, Layer_Tag);
					strcat_s(szObjectTag, "%d");
					sprintf_s(szObjectTag, szObjectTag, (_uint)j);

					ImGui::Text(szObjectTag);
					ImGui::SameLine();

					sprintf_s(szObjectTag, "Delete %d", (_uint)j);

					if (ImGui::SmallButton(szObjectTag))
					{
						Safe_Release(*Layeriter);
						Layeriter = Object->erase(Layeriter); //list 컨테이너의 erase 멤버함수의 반환값이 다음 반복자를 반환한다.
					}
					else
						++Layeriter;

				}
				ImGui::TreePop();
			}
			//char szLayerTag[MAX_PATH] = "Delete ";
			//strcat_s(szLayerTag, Layer_Tag);
			//if (ImGui::SmallButton(szLayerTag))
			//{
			//	CLayer* pLayer = iter->second;
			//	list<CGameObject*> Object = *(pLayer->Get_GameObjectList());
			//	auto& LayerIter = Object.begin();
			//	for (; LayerIter != Object.end();)
			//	{
			//		Safe_Release(*LayerIter);
			//		LayerIter = Object.erase(LayerIter);
			//	}

			//	iter = m_pObject_Layers[LEVEL_MAPTOOL].erase(iter);
			//}
			//else
				++iter;
		}

		ImGui::TreePop();
	}

}

void CLevel_ImGui_MapTool::All_Layer_Save(const char* pFilePath)
{
	_tchar szFilePath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, pFilePath, -1, szFilePath, IM_ARRAYSIZE(szFilePath));

	wcscat_s(szFilePath, TEXT(".dat"));

	HANDLE hFile = CreateFile(szFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSGBOX("세이브 실패");
		return;
	}

	_ulong dwByte = 0;

	auto iter = m_pObject_Layers[LEVEL_MAPTOOL].begin();

	for (_uint i = 0; i < m_pObject_Layers[LEVEL_MAPTOOL].size(); ++i)
	{
		if (!lstrcmp(TEXT("Layer_Camera"), iter->first) || !lstrcmp(TEXT("Layer_BackGround"), iter->first) || !lstrcmp(TEXT("Layer_Player"), iter->first)
			|| !lstrcmp(TEXT("Layer_UI_GreenMagazine"), iter->first) || !lstrcmp(TEXT("Layer_UI_BlueMagazine"), iter->first)
			|| !lstrcmp(TEXT("Layer_UI_Skill"), iter->first) || !lstrcmp(TEXT("Layer_UI_GreenMagazineTenth"), iter->first)
			|| !lstrcmp(TEXT("Layer_UI_BlueMagazineTenth"), iter->first) || !lstrcmp(TEXT("Layer_Monster_Shield"), iter->first)
			|| !lstrcmp(TEXT("Layer_UI_Universal"), iter->first) || !lstrcmp(TEXT("Layer_UI_Universal_Stamina"), iter->first)
			|| !lstrcmp(TEXT("Layer_UI_Universal_Hp"), iter->first) || !lstrcmp(TEXT("Layer_UI_Universal_Shield"), iter->first))
		{
			++iter;
			continue;
		}

		CLayer* pLayer = iter->second;
		const list<CGameObject*>* Object = pLayer->Get_GameObjectList();
		auto& Layeriter = Object->begin();

		for (_uint j = 0; j < Object->size(); ++j)
		{
			OBJECTDATADESC tObjDataDesc;
			ZeroMemory(&tObjDataDesc, sizeof(OBJECTDATADESC));

			tObjDataDesc.iLayerTagIndex = (*Layeriter)->Get_ObjectDataDesc()->iLayerTagIndex;
			tObjDataDesc.iModelTagIndex = (*Layeriter)->Get_ObjectDataDesc()->iModelTagIndex;
			tObjDataDesc.iObjectTagIndex = (*Layeriter)->Get_ObjectDataDesc()->iObjectTagIndex;
			tObjDataDesc.TransformMatrix = static_cast<CTransform*>((*Layeriter)->Get_Component(TEXT("Com_Transform")))->Get_WorldFloat4x4();

			WriteFile(hFile, &tObjDataDesc, sizeof(OBJECTDATADESC), &dwByte, nullptr);

			if (0 == dwByte)
			{
				MSGBOX("dwByte is Zero");
				return;
			}

			++Layeriter;
		}

		++iter;
	}

	CloseHandle(hFile);
}


void CLevel_ImGui_MapTool::All_Layer_Load(const char * pFilePath)
{

	_tchar szFilePath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, pFilePath, -1, szFilePath, IM_ARRAYSIZE(szFilePath));

	wcscat_s(szFilePath, TEXT(".dat"));

	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSGBOX("로드 실패");
		return;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_LevelObject_For_Editer(LEVEL_MAPTOOL);
	RELEASE_INSTANCE(CGameInstance);

	_ulong dwByte = 0;
	OBJECTDATADESC tObjDataDesc;

	while (true)
	{
		ZeroMemory(&tObjDataDesc, sizeof(OBJECTDATADESC));
		ReadFile(hFile, &tObjDataDesc, sizeof(OBJECTDATADESC), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (false == m_bPlayer) //이걸 써야 몬스터들 나옴
		{
			m_bPlayer = true;
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAPTOOL, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player"))))
			{
				MSGBOX("플레이어 임시 생성 실패");
			}
			RELEASE_INSTANCE(CGameInstance);
		}
		//if(LAYER_MONSTER !=tObjDataDesc.iLayerTagIndex) //몬스터를 제외하고 싶을때
		//{
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAPTOOL, g_pLayerTag[tObjDataDesc.iLayerTagIndex], g_pPrototypeObjectTag[tObjDataDesc.iObjectTagIndex], &tObjDataDesc)))
			{
				MSGBOX("생성 실패");
				return;
			}
		//}
	}

	CloseHandle(hFile);

}
#endif

//ImGui================================================================================================