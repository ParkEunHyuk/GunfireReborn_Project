#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("레이어의 이름은 각각 넣었음"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Weapon(TEXT("레이어의 이름은 각각 넣었음"))))
		return E_FAIL;


	//터레인 데이터는 로더에서
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = CreateFile(TEXT("../Bin/Save/Level_GamePlay/Object.dat"), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSGBOX("불러오기 실패");
		return E_FAIL;
	}

	_ulong dwByte = 0;
	OBJECTDATADESC tObjDataDesc;

	while (true)
	{
		ZeroMemory(&tObjDataDesc, sizeof(OBJECTDATADESC));
		ReadFile(hFile, &tObjDataDesc, sizeof(OBJECTDATADESC), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, g_pLayerTag[tObjDataDesc.iLayerTagIndex], g_pPrototypeObjectTag[tObjDataDesc.iObjectTagIndex], &tObjDataDesc)))
		{
			MSGBOX("생성 실패");
			return E_FAIL;
		}
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	//FAILED_CHECK(GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_BGM, 0.1));
	FAILED_CHECK(GetSingle(CGameInstance)->PlayBGM((L"OST.mp3")));

	return S_OK;
}

_int CLevel_GamePlay::Tick(_double TimeDelta)
{
	if(0 > __super::Tick(TimeDelta))
		return -1;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (m_bLevel_Switch == true)
	{
		pGameInstance->GetInstance()->Release_ObseverGroup();

		switch (m_INextLevel)
		{
		case LEVEL::LEVEL_STAGE1:
		{
			if (FAILED(GetSingle(CGameInstance)->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE1))))
			{
				MSGBOX("LEVEL_STAGE1에 진입하지 못했습니다.");
				return E_FAIL;
			}
			break;
		}
		case LEVEL::LEVEL_STAGE1_BOSSROOM:
		{
			if (FAILED(GetSingle(CGameInstance)->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE1_BOSSROOM))))
			{
				MSGBOX("LEVEL_STAGE1_BOSSROOM에 진입하지 못했습니다.");
				return E_FAIL;
			}
			break;
		}
		case LEVEL::LEVEL_STAGE2:
		{
			if (FAILED(GetSingle(CGameInstance)->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE2))))
			{
				MSGBOX("LEVEL_STAGE2에 진입하지 못했습니다.");
				return E_FAIL;
			}
			break;
		}
		case LEVEL::LEVEL_STAGE2_BOSSROOM:
		{
			if (FAILED(GetSingle(CGameInstance)->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE2_BOSSROOM))))
			{
				MSGBOX("LEVEL_STAGE2_BOSSROOM에 진입하지 못했습니다.");
				return E_FAIL;
			}
			break;
		}
		case LEVEL::LEVEL_GAMEPLAY:
		{
			if (FAILED(GetSingle(CGameInstance)->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_GAMEPLAY))))
			{
				MSGBOX("LEVEL_GAMEPLAY에 진입하지 못했습니다.");
				return E_FAIL;
			}
			break;
		}
		case LEVEL::LEVEL_MAPTOOL:
		{
			if (FAILED(GetSingle(CGameInstance)->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_MAPTOOL))))
			{
				MSGBOX("LEVEL_MAPTOOL에 진입하지 못했습니다.");
				return E_FAIL;
			}
			break;
		}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CLevel_GamePlay::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	return 0;
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	//SetWindowText(g_hWnd, TEXT("게임플레이임. "));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC		LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f); 일단 스펙큘러 끔
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(5.f, 3.f, 5.f, 1.f);
	LightDesc.fRange = 8.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;


	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(5.f, 5.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(5.f, 3.f, 5.f, 1.f);
	LightDesc.fRange = 4.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Player"), &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_Stamina"), TEXT("Prototype_GameObject_UI_Stamina"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_HpBar"), TEXT("Prototype_GameObject_UI_HpBar"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_MpBar"), TEXT("Prototype_GameObject_UI_MpBar"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_DotSight"), TEXT("Prototype_GameObject_UI_DotSight"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_Weapon"), TEXT("Prototype_GameObject_UI_Weapon"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_UI_Bullet_Number"), TEXT("Prototype_GameObject_UI_Bullet_Number"))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_BlueEffect"))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_RedEffect"))))
	//	return E_FAIL;

	//for (_uint i = 0; i < 30; ++i)//다른 기수
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect"))))
	//		return E_FAIL;
	//}	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Weapon(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Weapon_Cannon"), TEXT("Prototype_GameObject_Cannon"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Weapon_Sniper"), TEXT("Prototype_GameObject_Sniper"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Weapon_Launcher"), TEXT("Prototype_GameObject_Launcher"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Weapon_Pistol"), TEXT("Prototype_GameObject_Pistol"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Weapon_Shotgun"), TEXT("Prototype_GameObject_Shotgun"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Weapon_PishSword"), TEXT("Prototype_GameObject_PishSword"))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint m_eNextLevel)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pDeviceContext);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Set_NowLevelIndex(m_eNextLevel);
	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
