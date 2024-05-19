#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"
#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Level_ImGui_MapTool.h"
#include "Level_Stage1.h"
#include "Level_Stage1_BossRoom.h"
#include "Level_Stage2.h"
#include "Level_Stage2_BossRoom.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Loading(TEXT("Layer_Loading"))))
	{
		MSGBOX("Loading 이미지 생성 실패");
		return E_FAIL;
	}

	m_eNextLevel = eNextLevel;

	 m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, m_eNextLevel);

	return S_OK;
}

_int CLevel_Loading::Tick(_double TimeDelta)
{
	if(0 > __super::Tick(TimeDelta))
		return -1;


	return 0;
}

_int CLevel_Loading::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (true == m_pLoader->isFinished())
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CLevel*		pLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_LOGO:
			if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, CLevel_Logo::Create(m_pDevice, m_pDeviceContext, m_eNextLevel))))
				return E_FAIL;
			//pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
			break;
			case LEVEL_STAGE1:
			if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, CLevel_Stage1::Create(m_pDevice, m_pDeviceContext, m_eNextLevel))))
				return E_FAIL;
			break;
		case LEVEL_STAGE1_BOSSROOM:
			if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, CLevel_Stage1_BossRoom::Create(m_pDevice, m_pDeviceContext, m_eNextLevel))))
				return E_FAIL;
			break;
		case LEVEL_STAGE2:
			if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, CLevel_Stage2::Create(m_pDevice, m_pDeviceContext, m_eNextLevel))))
				return E_FAIL;
			break;
		case LEVEL_STAGE2_BOSSROOM:
			if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, CLevel_Stage2_BossRoom::Create(m_pDevice, m_pDeviceContext, m_eNextLevel))))
				return E_FAIL;
			break;
		case LEVEL_GAMEPLAY:
			if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, CLevel_GamePlay::Create(m_pDevice, m_pDeviceContext, m_eNextLevel))))
				return E_FAIL;
			break;
		case LEVEL_BOSS:
			break;
		case LEVEL_MAPTOOL:
			if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, CLevel_ImGui_MapTool::Create(m_pDevice, m_pDeviceContext, m_eNextLevel))))
				return E_FAIL;
			break;
		}				

		Safe_Release(pGameInstance);
	}

	return 0;
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("로딩레벨임. "));

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Loading(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_UI_Loading"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Loading_Bar(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_UI_Loading_Bar"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("Failed to Creating CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
