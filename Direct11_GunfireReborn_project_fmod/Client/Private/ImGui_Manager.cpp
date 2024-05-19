#include "stdafx.h"
#include "..\Public\ImGui_Manager.h"


#if defined(USE_IMGUI)

#include "GameInstance.h"
#include "GameObject.h"
IMPLEMENT_SINGLETON(CImGui_Manager)

CImGui_Manager::CImGui_Manager()
{
}

void CImGui_Manager::ImGui_SetUp(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	// ImGui 초기화
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	//암구이 색상선택
	ImGui::StyleColorsDark();

	//암구이 디바이스 및 디바이스 컨텍스트, 윈도우 핸들 셋팅
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);

	//암구이 매니저 내에서 사용할 디바이스 및 디바이스 컨텍스트, 윈도우핸들
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);




	//사용하려는 플랫폼과 렌더러(장치) 세팅
	ImGui_ImplWin32_Init(g_hWnd); // Client.cpp 에서 가져옴
	ImGui_ImplDX11_Init(m_pDevice, m_pDeviceContext);

	//ImGui_Manager 의 멤버변수로 장치랑 윈도우핸들 설정 (생성자에서 미리 설정하면 가끔 문제 생겨서 초기화 할 때 설정함)
	//m_pGraphic_Device = m_pGraphic_Device->Get_Device();
	//m_hWnd	=	g_hWnd;
	//Safe_AddRef(m_pGraphic_Device);
}

void CImGui_Manager::ImGui_Tick()
{
	// 매 Tick마다 들어가야하는 업데이트 코드
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

HRESULT CImGui_Manager::Begin_Frame(const char * szFrameName, _uint iFlag)
{
	_bool Open = true;
	ImGui::Begin(szFrameName, &Open, iFlag);

	return S_OK;
}

HRESULT CImGui_Manager::End_Frame()
{
	ImGui::End();
	return S_OK;
}

void CImGui_Manager::ImGui_Render()
{
	// Rendering
	ImGui::Render();
	//const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	//g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	//g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

//HRESULT	CImGui_Manager::Show_Transform(void** ppArg, const char* pName)
//{
//	if (nullptr == ppArg)
//		return E_FAIL;
//
//	CTransform* pTransform = *(CTransform**)ppArg;
//	
//	ImGui::Begin(pName);
//	ImGui::Text("Transform");
//
//	_float4 vPos;
//
//	XMStoreFloat4(&vPos, pTransform->Get_State(CTransform::STATE_POSITION));
//	ImGui::Text("Position : %f,%f,%f", vPos.x, vPos.y, vPos.z);
//
//	_float4 vScale, vStore;
//	XMStoreFloat4(&vStore, pTransform->Get_Scale(CTransform::STATE_RIGHT));
//	vScale.x = vStore.x;
//	XMStoreFloat4(&vStore, pTransform->Get_Scale(CTransform::STATE_UP));
//	vScale.y = vStore.y;
//	XMStoreFloat4(&vStore, pTransform->Get_Scale(CTransform::STATE_LOOK));
//	vScale.z = vStore.z;
//
//	ImGui::DragFloat3("Scale", (_float*)&vScale, 1.0f,0.01f,10);
//
//	pTransform->Scaled(XMLoadFloat4(&vScale));
//
//	//구조체가 없어서 실험용으로 스태틱으로 만듬 나중에 구조체 제대로 만들것
//	static _float4 vTurn = { 0.f,0.f,0.f,0.f };
//
//	ImGui::InputFloat4("Rotation", (_float*)&vTurn);
//	_vector vT = XMLoadFloat4(&vTurn);
//
//	if (ImGui::Button("Select"))
//		pTransform->Rotation(vT, XMConvertToRadians(vTurn.w));
//
//	static char szBuf[MAX_PATH] = "";
//	ImGui::InputText("File Name", szBuf, MAX_PATH);
//
//
//	if (true == ImGui::Button("Save"))
//	{
//		_tchar* pFileName = new _tchar[strlen(szBuf)+1];
//
//		MultiByteToWideChar(CP_ACP, 0,szBuf, strlen(szBuf) + 1, pFileName, strlen(szBuf) + 1);
//
//		TRANSFORMDESC tTransformDesc;
//		ZeroMemory(&tTransformDesc, sizeof(TRANSFORMDESC));
//
//		tTransformDesc.vPos = vPos;
//		tTransformDesc.vScale = vScale;
//
//		HANDLE hFile = CreateFile(pFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
//
//		if (INVALID_HANDLE_VALUE == hFile)
//		{
//			MSGBOX("저장 실패");
//			CloseHandle(hFile);
//			Safe_Delete_Array(pFileName);
//			ImGui::End();
//			return 0;
//		}
//
//		_ulong dwByte = 0;
//
//		WriteFile(hFile, &tTransformDesc, sizeof(TRANSFORMDESC), &dwByte, nullptr);
//
//		CloseHandle(hFile);
//		Safe_Delete_Array(pFileName);
//
//	}
//
//
//	ImGui::End();
//
//	return S_OK;
//}

//HRESULT CImGui_Manager::Make_Terrain(void ** ppArg, const char * pName)
//{
//	CVIBuffer_Terrain* pVIBuffer = *(CVIBuffer_Terrain**)ppArg;
//
//	ImGui::Begin(pName);
//
//	static _int iX = 0, iY = 0;
//
//	ImGui::InputInt(" X", &iX);
//	ImGui::InputInt(" Y", &iY);
//#if defined(USE_IMGUI)
//	if (true == ImGui::Button("Setting"))
//	{
//		pVIBuffer->ReMake_Terrain(iX, iY);
//	}
//#endif
//	ImGui::End();
//
//	return S_OK;
//}

void CImGui_Manager::Small_Separator()
{
	ImGui::Dummy(ImVec2(0.0f, 2.5f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 2.5f));
}

void CImGui_Manager::Big_Separator()
{
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}

HRESULT CImGui_Manager::Show_Transform(CGameObject* pGameObject, const char* pName)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	CTransform* pTransform = (CTransform*)pGameObject->Get_Component(TEXT("Com_Transform"));

	//ImGui::Begin(pName);

	_float4 vPos;
	XMStoreFloat4(&vPos, pTransform->Get_State(CTransform::STATE_POSITION));
	ImGui::DragFloat3(" Position", (_float*)&vPos);
	pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

	_float3 vScale, vTemp;
	XMStoreFloat3(&vTemp, pTransform->Get_Scale(CTransform::STATE_RIGHT));
	vScale.x = vTemp.x;
	XMStoreFloat3(&vTemp, pTransform->Get_Scale(CTransform::STATE_UP));
	vScale.y = vTemp.y;
	XMStoreFloat3(&vTemp, pTransform->Get_Scale(CTransform::STATE_LOOK));
	vScale.z = vTemp.z;
	ImGui::DragFloat3(" Scale", (_float*)&vScale, 0.001f, 0.001f);
	pTransform->Scaled(XMLoadFloat3(&vScale));

	static _float3 vAxis = { 0.f, 0.f, 0.f };
	static _float fAngle = 0.f;
	ImGui::InputFloat3(" Axis", (_float*)&vAxis);
	ImGui::InputFloat(" Angle", &fAngle);
	if (true == ImGui::Button("Apply"))
	{
		if (0 < vAxis.x || 0 < vAxis.y || 0 < vAxis.z)
			pTransform->Rotation(XMLoadFloat3(&vAxis), XMConvertToRadians(fAngle));
	}

	//ImGui::End();

	return S_OK;
}

HRESULT CImGui_Manager::Show_ObjectList(const list<CGameObject*>& Layerlist, const char * pLayerTag)
{
	ImGui::Begin(pLayerTag);

	_uint iIndex = 0;
	char LayerTag[MAX_PATH] = "";
	strcpy_s(LayerTag, pLayerTag);
	strcat_s(LayerTag, " %d");

	for (auto& pGameObject : Layerlist)
	{
		char pLayer[MAX_PATH] = "";
		strcpy_s(pLayer, LayerTag);
		sprintf_s(pLayer, pLayer, iIndex);
		if (ImGui::CollapsingHeader(pLayer))
			Show_Transform(pGameObject, pLayer);
		++iIndex;
	}

	ImGui::End();

	return S_OK;
}


void CImGui_Manager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pGameInstance);
}

#endif