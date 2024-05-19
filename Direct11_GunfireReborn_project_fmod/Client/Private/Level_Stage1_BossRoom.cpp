#include "stdafx.h"
#include "..\Public\Level_Stage1_BossRoom.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"

CLevel_Stage1_BossRoom::CLevel_Stage1_BossRoom(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Stage1_BossRoom::NativeConstruct()
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

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	{
		MSGBOX("도깨비 보스는 맵툴에서 실수로 안넣어서 코드로 넣음 근데 혹시 문제가 생길 수 있으니 메세지 박스를 출력");
		return E_FAIL;
	}


	//터레인 데이터는 로더에서
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = CreateFile(TEXT("../Bin/Save/Level_Stage1_BossRoom/Object.dat"), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, g_pLayerTag[tObjDataDesc.iLayerTagIndex], g_pPrototypeObjectTag[tObjDataDesc.iObjectTagIndex], &tObjDataDesc)))
		{
			MSGBOX("생성 실패");
			return E_FAIL;
		}
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	//FAILED_CHECK(GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_BGM, 0.2));
	FAILED_CHECK(GetSingle(CGameInstance)->PlayBGM((L"Level_Stage1_BossRoom.mp3")));

	return S_OK;
}

_int CLevel_Stage1_BossRoom::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	//if (m_bScene_Switch == true)
	//{
	//	switch (m_INextScene)
	//	{
	//	case LEVEL::LEVEL_MAPTOOL:
	//	{
	//		if(FAILED(pGameInstance->OpenLevel(m_eNextLevel, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, m_eNextLevel))))
	//	}
	//	}
	//}

	//if (pGameInstance->Get_DIKeyState(DIK_F1) & DIS_Down)
	//{

	//	pGameInstance->GetInstance()->Release_ObseverGroup(); //이건 옵저버 패턴을 사용하고 있는 와중에 다음 레벨로 넘어가서 오브젝트는 없는데  읽으려고 해서 생기는 문제이다. 그래서 넣어줌

	//	if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_MAPTOOL))))
	//		return E_FAIL;
	//}

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

_int CLevel_Stage1_BossRoom::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

HRESULT CLevel_Stage1_BossRoom::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("스테이지1 보스방입니다. "));

	return S_OK;
}

HRESULT CLevel_Stage1_BossRoom::Ready_Lights()
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

	RELEASE_INSTANCE(CGameInstance);

	Position_Point_Lights();

	return S_OK;
}

HRESULT CLevel_Stage1_BossRoom::Position_Point_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	////////////////////점광원
	LIGHTDESC		LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(107.018f, 3.3f, 81.032f, 1.f);
	LightDesc.fRange = 8.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(107.018f, 3.3f, 81.032f, 1.f);
	LightDesc.fRange = 4.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;
	///////////////////////////////////////
	
	///////////////////////////////////////
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(36.706f, 3.3f, 88.078f, 1.f);
	LightDesc.fRange = 8.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(36.706f, 3.3f, 88.078f, 1.f);
	LightDesc.fRange = 4.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;
	///////////////////////////////////////

	///////////////////////////////////////
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(63.287f, 3.3f, 86.754f, 1.f);
	LightDesc.fRange = 8.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(63.287f, 3.3f, 86.754f, 1.f);
	LightDesc.fRange = 4.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;
	///////////////////////////////////////

	///////////////////////////////////////
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(35.087f, 3.3f, 26.032f, 1.f);
	LightDesc.fRange = 8.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(35.087f, 3.3f, 26.032f, 1.f);
	LightDesc.fRange = 4.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;
	///////////////////////////////////////

	///////////////////////////////////////
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(104.442f, 3.3f, 38.868f, 1.f);
	LightDesc.fRange = 8.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	LightDesc.vPosition = _float4(104.442f, 3.3f, 38.868f, 1.f);
	LightDesc.fRange = 4.0f;

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;
	///////////////////////////////////////

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1_BossRoom::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, pLayerTag, TEXT("Prototype_GameObject_Camera_Player"), &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1_BossRoom::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1_BossRoom::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, pLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1_BossRoom::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_UI_Stamina"), TEXT("Prototype_GameObject_UI_Stamina"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_UI_HpBar"), TEXT("Prototype_GameObject_UI_HpBar"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_UI_MpBar"), TEXT("Prototype_GameObject_UI_MpBar"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_UI_DotSight"), TEXT("Prototype_GameObject_UI_DotSight"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_UI_Weapon"), TEXT("Prototype_GameObject_UI_Weapon"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_UI_Bullet_Number"), TEXT("Prototype_GameObject_UI_Bullet_Number"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1_BossRoom::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1_BossRoom::Ready_Layer_Weapon(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Weapon_Cannon"), TEXT("Prototype_GameObject_Cannon"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Weapon_Sniper"), TEXT("Prototype_GameObject_Sniper"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Weapon_Launcher"), TEXT("Prototype_GameObject_Launcher"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Weapon_Pistol"), TEXT("Prototype_GameObject_Pistol"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Weapon_Shotgun"), TEXT("Prototype_GameObject_Shotgun"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Weapon_PishSword"), TEXT("Prototype_GameObject_PishSword"))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage1_BossRoom::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, pLayerTag, TEXT("Prototype_GameObject_Boss_Dokkaebi"))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Stage1_BossRoom * CLevel_Stage1_BossRoom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint m_eNextLevel)
{
	CLevel_Stage1_BossRoom*	pInstance = new CLevel_Stage1_BossRoom(pDevice, pDeviceContext);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Set_NowLevelIndex(m_eNextLevel);
	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Stage1_BossRoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage1_BossRoom::Free()
{
	__super::Free();
}
