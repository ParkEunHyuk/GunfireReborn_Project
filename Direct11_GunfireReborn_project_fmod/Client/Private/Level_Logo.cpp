#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Dynamic.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;


	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	/* 현재 레벨이 생성될 때, 레벨에서 사용하고자하는 사본객체를 생성한다. */
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	//	return E_FAIL;	

	if (FAILED(Ready_Layer_Main_Logo(TEXT("Layer_Main_Logo"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button(TEXT("Layer_UI_Button"))))
		return E_FAIL;

	FAILED_CHECK(GetSingle(CGameInstance)->PlayBGM((L"Logo.mp3")));
	//FAILED_CHECK(GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_BGM, 0.1));

	return S_OK;
}

_int CLevel_Logo::Tick(_double TimeDelta)
{
	if(0 > __super::Tick(TimeDelta))
		return -1;

	return 0;
}

_int CLevel_Logo::LateTick(_double TimeDelta)
{

	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_RETURN) & DIS_Down)
	{
		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_GAMEPLAY))))
			return E_FAIL;
	}

	if (pGameInstance->Get_DIKeyState(DIK_F1) & DIS_Down)
	{
		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE1))))
			return E_FAIL;
	}

	if (pGameInstance->Get_DIKeyState(DIK_F2) & DIS_Down)
	{
		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE1_BOSSROOM))))
			return E_FAIL;
	}

	if (pGameInstance->Get_DIKeyState(DIK_F3) & DIS_Down)
	{
		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE2))))
			return E_FAIL;
	}

	if (pGameInstance->Get_DIKeyState(DIK_F4) & DIS_Down)
	{
		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE2_BOSSROOM))))
			return E_FAIL;
	}

#ifdef _DEBUG
	if (pGameInstance->Get_DIKeyState(DIK_T) & DIS_Down)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_MAPTOOL))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}
#endif // _DEBUG

	if (m_bLevel_Switch == true)
	{
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
		}
	}


	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("로고레벨임. "));	

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Prototype_GameObject()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 원본객체 생성 */

	/* For.Prototype_GameObject_BackGround */
	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	/*CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, 3.f, -3.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_Camera_Dynamic"), &CameraDesc)))
		return E_FAIL;
*/
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Main_Logo(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_Main_Logo"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Button(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_UI_Start_Button"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_UI_Cancel_Button"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint m_eNextLevel)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pDeviceContext);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Set_NowLevelIndex(m_eNextLevel);
	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();

}
