#include "stdafx.h"
#include "..\Public\Terrain_MapTool.h"
#include "GameInstance.h"
#include "Light.h"

#if defined(USE_IMGUI)
#include "ImGui_Manager.h"
#endif

CTerrain_MapTool::CTerrain_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CTerrain_MapTool::CTerrain_MapTool(const CTerrain_MapTool& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain_MapTool::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_MapTool::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

#if defined(USE_IMGUI)
	//초기값설정
	m_vAddValues = 5.f;

	//_float4(13.f,1.f,0.35,0.44f)라이트
	//m_vDampning[0] = LIGHT_Dampening_13.x;
	//m_vDampning[1] = LIGHT_Dampening_13.y;
	//m_vDampning[2] = LIGHT_Dampening_13.z;
	//m_vDampning[3] = LIGHT_Dampening_13.w;

	//_float4(13.f,1.f,0.35,0.44f)
	m_vDampning[0] = 1.f;
	m_vDampning[1] = 1.f;
	m_vDampning[2] = 0.35f;
	m_vDampning[3] = 0.44f;
#endif
	return S_OK;
}

_int CTerrain_MapTool::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

#if defined(USE_IMGUI)
	Running_Window();
#endif

	return _int();
}

_int CTerrain_MapTool::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	RELEASE_INSTANCE(CGameInstance);

#if defined(USE_IMGUI)
	if (m_bIsChanged)
	{
		static_cast<CVIBuffer_Terrain_Dynamic*>(m_pVIBufferCom)->Apply_Data();
		m_bIsChanged = false;
	}
#endif
	return _int();
}

HRESULT CTerrain_MapTool::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(m_pShaderCom, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_MapTool::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC,TEXT("Prototype_Component_Renderer"),TEXT("Com_Renderer"),(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum,TEXT("Prototype_Component_Shader_VtxNorTex"),TEXT("Com_Shader"),(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(m_eNowLevelNum,TEXT("Prototype_Component_VIBuffer_Terrain_Dynamic"),TEXT("Com_VIBuffer"),(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture_Diffuse */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Texture_Terrain"), TEXT("Com_Texture_Diffuse"), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE])))
		return E_FAIL;

	/* For.Com_Texture_Filter */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Texture_Filter"), TEXT("Com_Texture_Filter"), (CComponent**)&m_pTextureCom[TYPE_FILTER])))
		return E_FAIL;

	/* For.Com_Texture_Brush */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Texture_Brush"), TEXT("Com_Texture_Brush"), (CComponent**)&m_pTextureCom[TYPE_BRUSH])))
		return E_FAIL;

	return S_OK;

}

HRESULT CTerrain_MapTool::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_SourDiffuseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_DestDiffuseTexture", 1)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_BRUSH]->SetUp_OnShader(m_pShaderCom, "g_BrushTexture", 0)))
		return E_FAIL;

	/*if (FAILED(m_pTextureCom[TYPE_FILTER]->SetUp_OnShader(m_pShaderCom, "g_FilterTexture", 0)))
	return E_FAIL;*/

	m_pShaderCom->Set_Texture("g_FilterTexture", m_pSRV);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;

}

HRESULT CTerrain_MapTool::Create_FilterTexture()
{
	return E_NOTIMPL;
}

CTerrain_MapTool* CTerrain_MapTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTerrain_MapTool*	pInstance = new CTerrain_MapTool(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CTerrain_MapTool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTerrain_MapTool::Clone(void* pArg)
{
	CTerrain_MapTool*	pInstance = new CTerrain_MapTool(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CTerrain_MapTool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain_MapTool::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);


	//Safe_Release(m_pSRV);
	Safe_Release(m_pShaderCom);
}

//IMGUI========================================================================================================================================================================================
#if defined(USE_IMGUI)

void CTerrain_MapTool::Running_Window()
{

	++m_iFPS;
	if (m_iFPS > 101)
		m_iFPS = 0;

	//2, 윈도우 기능추가
	//static ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	//ImGui::Begin("Edit_Terrain_Height", 0, 0);
	ImGui::Begin("VIBuffer_Terrain_Dynamic", 0, 0);
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);

	//3. 높이맵 기능설정
	Set_Range_And_Dampning();
	//pImguiManager->Small_Separator();
	Set_Value_Option();
	//pImguiManager->Small_Separator();
	Set_Edit_Type_Option();
	pImguiManager->Small_Separator();
	Save_Map();
	pImguiManager->Small_Separator();
	Load_Map();
	RELEASE_INSTANCE(CImGui_Manager);
	ImGui::End();

	//5. 수정사항적용
	Edit_Input();

}

void CTerrain_MapTool::Running_Editer_Window()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);
	//윈도우 시작
	ImGui::Begin("Editer Window");
	Widget_ObjectList();
	pImguiManager->Small_Separator();
	Widget_PrototypeList();
	pImguiManager->Small_Separator();
	Widget_Add_Object();
	pImguiManager->Small_Separator();
	Widget_Save_Load_Objects();
	pImguiManager->Small_Separator();

	// 창에 필요한 기능을 다 넣었다면 End 로 닫아줘야함
	ImGui::End();
	RELEASE_INSTANCE(CImGui_Manager);
}

void CTerrain_MapTool::Widget_ObjectList()
{

}

void CTerrain_MapTool::Widget_PrototypeList()
{
}

void CTerrain_MapTool::Widget_Save_Load_Objects()
{
}

void CTerrain_MapTool::Widget_Add_Object()
{
}


void CTerrain_MapTool::Edit_Input()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float4 vValues = { 0.f,0.f,0.f,0.f };

	//일단 임시로 여기에 이렇게 만듬
	//실제론 Set_Edit_Type_Option에 주석처리된걸 풀어서 사용해야함
	_uint m_eEditMode = 0;

	switch (m_eEditMode)
	{
	case TERRAIN_EDIT_TYPE_HEIGHT:
		vValues.x = m_vAddValues;
		break;
	case TERRAIN_EDIT_TYPE_TEXTURE_01:
		vValues.y = m_vAddValues;
		break;
	case TERRAIN_EDIT_TYPE_TEXTURE_02:
		vValues.z = m_vAddValues;
		break;
	case TERRAIN_EDIT_TYPE_TEXTURE_03:
		vValues.w = m_vAddValues;
		break;
	}

	if (pGameInstance->Get_DIKeyState(DIK_LCONTROL) & DIS_Press)
	{
		int a = 10;
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) && (!(m_iFPS % 5)))
		{
			_float3 vPickedPos;
			if (m_pVIBufferCom->Pick_Dynamic(m_pTransform->Get_WorldMatrixInverse_Float4x4(), &vPickedPos))
			{
				_float4 vTemp;
				vTemp.x = m_vDampning[0];
				vTemp.y = m_vDampning[1];
				vTemp.z = m_vDampning[2];
				vTemp.w = m_vDampning[3];
				m_bIsChanged = true;
				static_cast<CVIBuffer_Terrain_Dynamic*>(m_pVIBufferCom)->Add_Height(vPickedPos, vTemp, vValues, m_fMaxHeight, m_fMinHeight);
			}

		}
		else
		{
			if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON)& DIS_Down)
			{
				_float3 vPickedPos;
				if (m_pVIBufferCom->Pick_Dynamic(m_pTransform->Get_WorldMatrixInverse_Float4x4(), &vPickedPos))
				{
					_float4 vTemp;
					vTemp.x = m_vDampning[0];
					vTemp.y = m_vDampning[1];
					vTemp.z = m_vDampning[2];
					vTemp.w = m_vDampning[3];
					m_bIsChanged = true;
					static_cast<CVIBuffer_Terrain_Dynamic*>(m_pVIBufferCom)->Add_Height(vPickedPos, vTemp, vValues, m_fMaxHeight, m_fMinHeight);
				}
			}

		}

	}



	RELEASE_INSTANCE(CGameInstance);
}

void CTerrain_MapTool::Set_Range_And_Dampning()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);
	//@@@@@@@@@@@@기존 코드
	//3. 최대 최소높이 조절
	//pImguiManager->Small_Separator();
	//ImGui::Text("Range&Dampening");
	//ImGui::Text("Set High,Low");
	//ImGui::DragFloatRange2(
	//	"Max,Min",
	//	&m_fMinHeight,
	//	&m_fMaxHeight,
	//	0.25f,
	//	0.f,
	//	255.f,
	//	"Min: %.1f",
	//	"Max: %.1f",
	//	ImGuiSliderFlags_AlwaysClamp);
	//ImGui::InputFloat4("Ran,Damp", m_vDampning); 범위,빛1,빛2,빛3 ???

	ImGui::Text("Set High, Low, Range");
	ImGui::DragFloatRange2(
		"Min,Max",
		&m_fMinHeight,
		&m_fMaxHeight,
		0.25f,
		0.f,
		255.f,
		"Min: %.1f",
		"Max: %.1f",
		ImGuiSliderFlags_AlwaysClamp);
	ImGui::InputFloat("Range", m_vDampning);
	RELEASE_INSTANCE(CImGui_Manager);

}

void CTerrain_MapTool::Set_Value_Option()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);

	//@@@@@@@@@@@@기존 코드
	//4. 감쇄값,범위 조절
	//ImGui::BulletText("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//pImguiManager->Small_Separator();
	//ImGui::Text("Value_Option");
	//ImGui::InputFloat("Add_Value", &m_vAddValues, 0.05f, 0, "%.3f");

	//힘조절
	ImGui::Text("Set_Strength");
	ImGui::InputFloat("Strength", &m_vAddValues, 0.05f, 0, "%.3f");

	RELEASE_INSTANCE(CImGui_Manager);
}

void CTerrain_MapTool::Set_Edit_Type_Option()
{
	//테스트
	//높이, ARGB 관련됨
	//CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);

	//pImguiManager->Small_Separator();
	//ImGui::Text("Edit_Type_Option");

	//static int e = 0;
	//ImGui::RadioButton("Height", &e, 0); ImGui::SameLine();
	//ImGui::RadioButton("Tex1", &e, 1); ImGui::SameLine();
	//ImGui::RadioButton("Tex2", &e, 2); ImGui::SameLine();
	//ImGui::RadioButton("Tex3", &e, 3);

	//m_eEditMode = e;

	//RELEASE_INSTANCE(CImGui_Manager);

}

void CTerrain_MapTool::Save_Map()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);
	pImguiManager->Small_Separator();
	ImGui::Text("Save Terrain");

	//패스입력
	static char str0[128] = "../Bin/Save/";
	ImGui::InputText("SaveFileName", str0, IM_ARRAYSIZE(str0));

	//세이브 버튼
	static int clicked = 0;
	if (ImGui::Button("Save_File"))
		clicked++;
	if (clicked & 1)
	{
		clicked = 0;
		static_cast<CVIBuffer_Terrain_Dynamic*>(m_pVIBufferCom)->Create_Filter_Image(_bstr_t(str0));

		strcpy_s(str0, "../Bin/Save/");
	}

	RELEASE_INSTANCE(CImGui_Manager);
}

void CTerrain_MapTool::Load_Map()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);
	pImguiManager->Small_Separator();
	ImGui::Text("Load Terrain");

	//패스입력
	static char LoadFileName[128] = "../Bin/Save/";
	ImGui::InputText("LoadFileName", LoadFileName, IM_ARRAYSIZE(LoadFileName));

	//세이브 버튼
	static int clicked = 0;
	if (ImGui::Button("Load_File"))
		clicked++;
	if (clicked & 1)
	{
		clicked = 0;
		static_cast<CVIBuffer_Terrain_Dynamic*>(m_pVIBufferCom)->Load_Image(_bstr_t(LoadFileName));

		strcpy_s(LoadFileName, "../Bin/Save/");
	}

	//빈 세이브 없애고 해야함 ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ

	RELEASE_INSTANCE(CImGui_Manager);
}
#endif
//IMGUI========================================================================================================================================================================================
