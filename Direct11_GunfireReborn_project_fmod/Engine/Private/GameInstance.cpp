#include "..\Public\GameInstance.h"
#include "Sound_Manager.h"
#include "VIBuffer_Terrain_Dynamic.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pSound_Manager(CSound_Manager::GetInstance())
	, m_pPicking(CPicking::GetInstance())
	, m_pObsever_Manager(CObsever_Manager::GetInstance())
	, m_pEasingMgr(CEasingMgr::GetInstance())
	, m_pFrustum_Manager(CFrustum_Manager::GetInstance())
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pFrustum_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pSound_Manager);
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pObsever_Manager);
	Safe_AddRef(m_pEasingMgr);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInstance, _uint iNumLevels, const CGraphic_Device::GRAPHICDESC & GraphicDesc, ID3D11Device ** ppDeviceOut, ID3D11DeviceContext ** ppDeviceContextOut, _float fDoubleInterver)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pObject_Manager|| nullptr == m_pSound_Manager)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pInput_Device->Ready_Input_Device(hInstance, GraphicDesc.hWnd, fDoubleInterver)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pPicking->NativeConstruct(*ppDeviceOut, *ppDeviceContextOut, GraphicDesc.hWnd, GraphicDesc.iWinCX, GraphicDesc.iWinCY)))
		return E_FAIL;

	if (FAILED(m_pFrustum_Manager->Initialize()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Initialize(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pLight_Manager->Initialize(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;

	FAILED_CHECK(m_pSound_Manager->Initialize_FMOD()); //자동으로 브레이크를 걸어줌

	return S_OK;
}

_int CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager || nullptr == m_pObject_Manager
		|| m_pSound_Manager == nullptr || m_pFrustum_Manager == nullptr)
		return -1;

	if (FAILED(m_pInput_Device->SetUp_InputDeviceState(TimeDelta)))
		return -1;

	FAILED_CHECK(m_pSound_Manager->Update_FMOD(TimeDelta));

	if (0 > m_pObject_Manager->Tick(TimeDelta))//각각 객체들 내부에 에드 옵저버 하면서 콜라이더 세팅
		return -1;

	if (0 > m_pLevel_Manager->Tick(TimeDelta))
		return -1;

	m_pPipeLine->Update();
	m_pFrustum_Manager->Update();

	if (FAILED(m_pObsever_Manager->Obsever_Quest(TimeDelta)))
	{
		MSGBOX("게임 인스턴스의 Obsever_Quest에 문제가 발생했습니다. ");
		return E_FAIL;
	}

	//여기에 Obsever_Quest 콜리전 내부 탐색? 파이프라인 다음이 맞음

	if (FAILED(m_pPicking->Transform_ToWorldSpace()))
		return -1;

	if (0 > m_pObject_Manager->LateTick(TimeDelta))
		return -1;

	if (0 > m_pLevel_Manager->LateTick(TimeDelta))
		return -1;

	return _int();
}



HRESULT CGameInstance::Clear_LevelResource(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	/*모델 때문에 잠시 오브젝트 매니저와 컴포넌트 매니저의 위치를 바꿈 (얉은 복사때문에 생기는 문제)*/
	if (FAILED(m_pObject_Manager->Clear_LevelObject(iLevelIndex)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Clear_LevelObject(iLevelIndex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

_byte CGameInstance::Get_DIKeyState(_ubyte eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(eKeyID);
}

_long CGameInstance::Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMoveState(eMouseMoveState);
}

_byte CGameInstance::Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseButtonState(eMouseButtonState);
}

_double CGameInstance::Get_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(pTimerTag);
}

HRESULT CGameInstance::OpenLevel(_uint iLevelIndex, CLevel * pNextLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->OpenLevel(iLevelIndex, pNextLevel);
}

HRESULT CGameInstance::Render_Level()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Render();
}

_uint CGameInstance::Get_NowLevelIndex()
{
	if (m_pLevel_Manager == nullptr)
		return -1;


	return m_pLevel_Manager->Get_NowLevelIndex();
}

void CGameInstance::Set_NowLevelIndex(_int iLevelNum)
{
	if (m_pLevel_Manager == nullptr)
		return;


	return m_pLevel_Manager->Set_NowLevelIndex(iLevelNum);
}

CLevel * CGameInstance::Get_Level()
{
	return m_pLevel_Manager->Get_Level();
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

CGameObject * CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iLayerIndex)
{
	if (pLayerTag == nullptr || m_pObject_Manager == nullptr)
		return nullptr;
	return m_pObject_Manager->Get_GameObject(iLevelIndex, pLayerTag, iLayerIndex);
}

list<CGameObject*>* CGameInstance::Get_ObjectList_from_Layer(_uint iLevelIndex, const _tchar * tagLayer)
{
	if (tagLayer == nullptr || m_pObject_Manager == nullptr)
		return nullptr;

	return m_pObject_Manager->Get_ObjectList_from_Layer(iLevelIndex, tagLayer);
}

HRESULT CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	return m_pPipeLine->Set_Transform(eStateType, TransformMatrix);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::TRANSFORMSTATETYPE eStateType)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eStateType);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATETYPE eStateType)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4(eStateType);
}

_float4x4 CGameInstance::Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATETYPE eStateType)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4_TP(eStateType);
}

_vector CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return XMVectorZero();

	return m_pPipeLine->Get_CamPosition();
}

_int CGameInstance::Channel_VolumeUp(CHANNELID eID, _float _vol)
{
	NULL_CHECK_MSG(m_pSound_Manager, L"Not Have m_pSoundMgr");
	return m_pSound_Manager->Channel_VolumeUp(eID, _vol);
}

_int CGameInstance::Channel_VolumeDown(CHANNELID eID, _float _vol)
{
	NULL_CHECK_MSG(m_pSound_Manager, L"Not Have m_pSoundMgr");

	return m_pSound_Manager->Channel_VolumeDown(eID, _vol);
}

_int CGameInstance::Channel_Pause(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSound_Manager, L"Not Have m_pSoundMgr");

	return m_pSound_Manager->Channel_Pause(eID);
}

HRESULT CGameInstance::PlaySound(TCHAR * pSoundKey, CHANNELID eID, _float fLouderMultiple)
{
	NULL_CHECK_MSG(m_pSound_Manager, L"Not Have m_pSoundMgr");

	return m_pSound_Manager->PlaySound(pSoundKey, eID, fLouderMultiple);
}

HRESULT CGameInstance::PlayBGM(TCHAR * pSoundKey, _float fLouderMultiple)
{
	NULL_CHECK_MSG(m_pSound_Manager, L"Not Have m_pSoundMgr");

	return m_pSound_Manager->PlayBGM(pSoundKey, fLouderMultiple);
}

void CGameInstance::Stop_ChannelSound(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSound_Manager, L"Not Have m_pSoundMgr");

	m_pSound_Manager->Stop_ChannelSound(eID);

	return;
}

void CGameInstance::Stop_AllChannel()
{
	NULL_CHECK_MSG(m_pSound_Manager, L"Not Have m_pSoundMgr");
	m_pSound_Manager->Stop_AllChannel();
	return;
}

_float CGameInstance::Get_Channel_Volume(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSound_Manager, L"Not Have m_pSoundMgr");
	return 	m_pSound_Manager->Get_Channel_Volume(eID);
}

_bool CGameInstance::Get_Channel_IsPaused(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSound_Manager, L"Not Have m_pSoundMgr");
	return 	m_pSound_Manager->Get_Channel_IsPaused(eID);
}

const LIGHTDESC * CGameInstance::Get_LightDesc(_uint iIndex) const
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pDeviceContext, LightDesc);
}

HRESULT CGameInstance::Release_Light()
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Release_Light();
}

HRESULT CGameInstance::Add_ObseverGroup(OBSEVERGROUP eObseverGroup, class CGameObject* _pGameObject)
{
	if (nullptr == m_pObsever_Manager)
	{
		MSGBOX("Obsever_Manager가 Nullptr입니다.");
		return E_FAIL;
	}

	return m_pObsever_Manager->Add_ObseverGroup(eObseverGroup, _pGameObject);
}

HRESULT CGameInstance::Obsever_Quest(_double DeltaTime)
{
	if (nullptr == m_pObsever_Manager)
	{
		MSGBOX("Obsever_Manager가 Nullptr입니다.");
		return E_FAIL;
	}

	return m_pObsever_Manager->Obsever_Quest(DeltaTime);
}

#ifdef _DEBUG
HRESULT CGameInstance::Obsever_Render()
{
	if (nullptr == m_pObsever_Manager)
	{
		MSGBOX("Obsever_Manager가 Nullptr입니다.");
		return E_FAIL;
	}


	return m_pObsever_Manager->Obsever_Render();
}
#endif // _DEBUG

HRESULT CGameInstance::Release_ObseverGroup()
{
	if (nullptr == m_pObsever_Manager)
		return E_FAIL;


	return m_pObsever_Manager->Release_ObseverGroup();
}

HRESULT CGameInstance::Transform_ToLocalSpace(_float4x4 WorldMatrixInverse)
{
	if (nullptr == m_pPicking)
	{
		MSGBOX("Transform_ToLocalSpace : m_pPicking is nullptr in CGameInstance");
		return E_FAIL;
	}

	return m_pPicking->Transform_ToLocalSpace(WorldMatrixInverse);
}

_bool CGameInstance::isPick(_float3* pLocalPoints, _float3 * pOut)
{
	if (nullptr == m_pPicking)
	{
		MSGBOX("isPick : m_pPicking is nullptr in CGameInstance");
		return false;
	}

	return m_pPicking->isPick(pLocalPoints, pOut);
}

_float3 & CGameInstance::Get_RayDirInLocal()
{
	if (nullptr == m_pPicking)
	{
		MSGBOX("Get_RayDirInLocal : m_pPicking is nullptr in CGameInstance");
		return _float3(0.f, 0.f, 0.f);
	}

	return m_pPicking->Get_RayDirInLocal();
}

_float3 & CGameInstance::Get_RayPosInLocal()
{
	if (nullptr == m_pPicking)
	{
		MSGBOX("Get_RayPosInLocal : m_pPicking is nullptr in CGameInstance");
		return _float3(0.f, 0.f, 0.f);
	}

	return m_pPicking->Get_RayPosInLocal();
}

_vector CGameInstance::Easing(_uint eEasingType, _vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	if (nullptr == m_pEasingMgr)
	{
		MSGBOX("Easing 함수에 문제가 발생");
		return _vector();
	}

	return m_pEasingMgr->Easing(eEasingType, vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
}

_bool CGameInstance::isIn_WorldSpace(_fvector vPoint, _float fRange)
{
	if (nullptr == m_pFrustum_Manager)
		return false;

	return m_pFrustum_Manager->isIn_WorldSpace(vPoint, fRange);
}

void CGameInstance::Release_Engine()
{
	if (0 != CGameInstance::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CGameInstance");

	if (0 != CTimer_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CTimer_Manager");

	if (0 != CObject_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CObject_Manager");

	if (0 != CComponent_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CComponent_Manager");

	if (0 != CLevel_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CLevel_Manager");

	if (0 != CPipeLine::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CPipeLine");

	if (0 != CSound_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CSound_Manager");

	if (0 != CInput_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CInput_Device");

	if (0 != CLight_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CLight_Manager");

	if (0 != CPicking::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CPicking");

	if (0 != CObsever_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CObsever_Manager");

	if (0 != CEasingMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CEasingMgr");

	if (0 != CFrustum_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CFrustum");

	if (0 != CTarget_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CTarget_Manager");


	if (0 != CGraphic_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CGraphic_Device");
}

void CGameInstance::Free()
{
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pFrustum_Manager);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pPicking);
	Safe_Release(m_pObsever_Manager);
	Safe_Release(m_pEasingMgr);
}

//IMGUI========================================================================================================================================================================================
#if defined(USE_IMGUI)
HRESULT CGameInstance::Clear_LevelObject_For_Editer(_uint iLevelIndex)
{
	if (m_pObject_Manager == nullptr)
		return E_FAIL;

	return m_pObject_Manager->Clear_LevelObject_For_Editer(iLevelIndex);
}
#endif
//IMGUI========================================================================================================================================================================================