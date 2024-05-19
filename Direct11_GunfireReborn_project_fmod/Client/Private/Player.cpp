#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"
#include "UI_HpBar.h"
#include "UI_MpBar.h"
#include "UI_Weapon.h"
#include "Weapon_PishSword.h"
#include "Weapon_Pistol.h"
#include "Weapon_Sniper.h"
#include "Weapon_Shotgun.h"
#include "Weapon_Launcher.h"
#include "Weapon_Cannon.h"
#include "Universal_Bullet.h"
#include "UI_Skill.h"
#include "UI_Universal.h"
#include "Universal_Bullet_Mesh.h"
#include "Universal_Point_Instance.h"
#include "Universal_TextureEffect.h"
#include "Universal_MeshEffect.h"

#include "Level.h"

#if defined(USE_IMGUI)
#include "ImGui_Manager.h"
#endif

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CLandObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("플레이어의 Components에 문제가 생김");
		return E_FAIL;
	}

	ZeroMemory(&m_JumpDesc, sizeof(JUMPDESC));

	m_JumpDesc.fJumpPower = 5.f;

	////@@@@@@@@@@@@@테스트중
	HANDLE	hFile = CreateFile(TEXT("../../ObjectTransform.dat"), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);


	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSGBOX("불러오기 실패");
		CloseHandle(hFile);
		return 0;
	}

	_float4 vScale;
	_float4 vPos;
	_ulong dwByte = 0;

	ReadFile(hFile, &vScale, sizeof(_float4), &dwByte, nullptr);
	ReadFile(hFile, &vPos, sizeof(_float4), &dwByte, nullptr);

	m_pTransform->Scaled(XMLoadFloat4(&vScale));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

	CloseHandle(hFile);



	m_pTransform->Scaled(XMLoadFloat3(&_float3(1.f, 1.f, 1.f)));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&_float4(1.f, 1.f, 1.f, 1.f)));

	SetUp_UI();

	MaptoolNotPosition();

	Environment_Particle();


	return S_OK;
}

_int CPlayer::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	if (false == m_bOnce_Switch)
	{
		m_bOnce_Switch = Once_Trigger();
		Torchlight();
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->GetInstance()->Add_ObseverGroup(PLAYER, this)))
	{
		MSGBOX("Monster_Bullet의 Add_ObseverGroup에 문제가 발생했습니다");
		return E_FAIL;
	}

	if (false == Weapon_SetUp &&  LEVEL_MAPTOOL != m_eNowLevelNum)
	{
		SetUp_Weapon();
	}


	//if (pGameInstance->Get_DIKeyState(DIK_H) & DIS_DoubleDown)
	//{
	//	pGameInstance->PlaySound(L"M1_5.mp3", CHANNEL_PLAYER);
	//	GetSingle(CGameInstance)->PlaySound(L"M1_5.mp3", CHANNEL_PLAYER);

	//}


	RELEASE_INSTANCE(CGameInstance);

#ifdef _DEBUG
	Level_Change();
#endif // _DEBUG

	return _int();
}

_int CPlayer::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (LEVEL_MAPTOOL != m_eNowLevelNum)
	{
		SetUp_OnNavigation(m_pNavigationCom, m_eNowLevelNum, TEXT("Layer_BackGround"), 0);

		PivotMatrix();//피벗 매트리스는 키_인풋보다 앞에 있어야함
	}
	Key_Input(TimeDelta);
	CoolTime_Manager(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	RELEASE_INSTANCE(CGameInstance);

#ifdef _DEBUG
	if (LEVEL_MAPTOOL != m_eNowLevelNum)
	{
		m_pRendererCom->Add_Debug(m_pNavigationCom);
	}
#endif

	return _int();
}




HRESULT CPlayer::Render()
{
	///////////////////////////셰이더에 쓰이지 않음 나중에 위치 확인할때만 확인하자
	//if (nullptr == m_pVIBufferCom)
	//	return E_FAIL;
	////////////////////////////////////////////////////

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	///////////////////////////셰이더에 쓰이지 않음 나중에 위치 확인할때만 확인하자
	//if (FAILED(m_pVIBufferCom->Render(m_pShaderCom, 1)))
	//	return E_FAIL;
	////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CPlayer::SetUp_Components()
{
	///////////////////////////셰이더에 쓰이지 않음 나중에 위치 확인할때만 확인하자
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	///* For.Com_Shader */
	//if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_Cube"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	//	return E_FAIL;

	///* For.Com_VIBuffer */
	//if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	//	return E_FAIL;

	///* For.Com_Texture */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;

	////////////////////////////////////////////////////

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;


	ColliderDesc.vScale = _float3(1.2f, 1.2f, 1.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	if (LEVEL_MAPTOOL != m_eNowLevelNum)
	{
		/* For.Com_Navigation */
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CNavigation::NAVIDESC		NaviDesc;
		NaviDesc.pParent = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_BackGround"), CGameObject::m_pTransformTag));;
		NaviDesc.iCurrentIndex = 0;

		if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);
	}

	return S_OK;
}

HRESULT CPlayer::SetUp_ConstantTable()
{
	///////////////////////////셰이더에 쓰이지 않음 나중에 위치 확인할때만 확인하자
	//if (nullptr == m_pShaderCom)
	//	return E_FAIL;
	////////////////////////////////////////////////////

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	///////////////////////////셰이더에 쓰이지 않음 나중에 위치 확인할때만 확인하자
	//if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
	//	return E_FAIL;
	////////////////////////////////////////////////////

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_bool CPlayer::Once_Trigger()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pMpBar = static_cast<CUI_MpBar*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_MpBar")));
	m_pHpBar = static_cast<CUI_HpBar*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_HpBar")));

	m_Info.fHp = 0;

	RELEASE_INSTANCE(CGameInstance);

	Warp();

	return true;
}

HRESULT CPlayer::PivotMatrix()
{
	//__super::SetUp_OnTerrain(m_eNowLevelNum, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	_vector vTempPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	_float4 fTempPos;

	XMStoreFloat4(&fTempPos, vTempPos);

	fTempPos.y = 1.5;
	//fTempPos.y += 10.5;

	m_JumpDesc.fJumpY = fTempPos.y;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&fTempPos));

	return S_OK;
}

HRESULT CPlayer::Level_Change()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_F1) & DIS_Down)
	{
		CLevel* TempLevel = pGameInstance->Get_Level();
		TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE1);

		pGameInstance->Release_Light();
	}

	if (pGameInstance->Get_DIKeyState(DIK_F2) & DIS_Down)
	{
		CLevel* TempLevel = pGameInstance->Get_Level();
		TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE1_BOSSROOM);

		pGameInstance->Release_Light();
	}

	if (pGameInstance->Get_DIKeyState(DIK_F3) & DIS_Down)
	{
		CLevel* TempLevel = pGameInstance->Get_Level();
		TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE2);

		pGameInstance->Release_Light();
	}

	if (pGameInstance->Get_DIKeyState(DIK_F4) & DIS_Down)
	{
		CLevel* TempLevel = pGameInstance->Get_Level();
		TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE2_BOSSROOM);

		pGameInstance->Release_Light();
	}

	if (pGameInstance->Get_DIKeyState(DIK_F6) & DIS_Down)
	{
		CLevel* TempLevel = pGameInstance->Get_Level();
		TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_GAMEPLAY);

		pGameInstance->Release_Light();
	}

	if (pGameInstance->Get_DIKeyState(DIK_F7) & DIS_Down)
	{
		CLevel* TempLevel = pGameInstance->Get_Level();
		TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_MAPTOOL);

		pGameInstance->Release_Light();
	}

	//if (pGameInstance->Get_DIKeyState(DIK_RETURN) & DIS_Down)
	//{
	//	CLevel* TempLevel = pGameInstance->Get_Level();
	//	TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_GAMEPLAY);
	//}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer::Warp()
{
	switch (m_eNowLevelNum)
	{
	case LEVEL_STAGE1:
	{

		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Npc"), TEXT("Prototype_GameObject_Npc"))))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		break;
	}
	case LEVEL_STAGE2:
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Npc"), TEXT("Prototype_GameObject_Npc"))))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);
		break;
	}
	default:
		break;
	}
	return S_OK;
}

HRESULT CPlayer::Torchlight()
{
	switch (m_eNowLevelNum)
	{
	case LEVEL_STAGE1:
	{
		Torchlight_Pos_Stage1();
		break;
	}
	case LEVEL_STAGE1_BOSSROOM:
	{
		Torchlight_Pos_Stage1_BossRoom();
		break;
	}
	case LEVEL_STAGE2:
	{
		Torchlight_Pos_Stage2();
		break;
	}
	case LEVEL_STAGE2_BOSSROOM:
	{
		Torchlight_Pos_Stage2_BossRoom();
		break;
	}
	}


	return S_OK;
}

HRESULT CPlayer::Torchlight_Pos_Stage1()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC Universal_EffectDesc;
	ZeroMemory(&Universal_EffectDesc, sizeof(CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC));
	Universal_EffectDesc.iEffectNumber = CUniversal_TextureEffect::EFFECTTYPE::TORCHLIGHT_EFFECT;
	Universal_EffectDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	Universal_EffectDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_EffectDesc.dDuration = 99999;
	Universal_EffectDesc.fPositioning = _float3(27.169f, 3.3f, 32.741f);
	Universal_EffectDesc.iTextureIndex = 0;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(69.889f, 3.3f, 120.862f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(2.935f, 3.3f, 86.012f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(41.050f, 3.3f, 17.132f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(124.003f, 3.3f, 16.136f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(123.903f, 3.3f, 108.711f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(16.864f, 3.3f, 122.496f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(126.800f, 3.3f, 57.395);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(42.491f, 3.3f, 101.808f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(41.130f, 3.3f, 62.095f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(83.850f, 3.3f, 25.625f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(91.279f, 3.3f, 24.698f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Torchlight_Pos_Stage1_BossRoom()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC Universal_EffectDesc;
	ZeroMemory(&Universal_EffectDesc, sizeof(CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC));
	Universal_EffectDesc.iEffectNumber = CUniversal_TextureEffect::EFFECTTYPE::TORCHLIGHT_EFFECT;
	Universal_EffectDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	Universal_EffectDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_EffectDesc.dDuration = 99999;
	Universal_EffectDesc.fPositioning = _float3(107.018f, 3.3f, 81.032f);
	Universal_EffectDesc.iTextureIndex = 0;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(36.706f, 3.3f, 88.078f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(63.287f, 3.3f, 86.754f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(35.087f, 3.3f, 26.032f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(104.442f, 3.3f, 38.868f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Torchlight_Pos_Stage2()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC Universal_EffectDesc;
	ZeroMemory(&Universal_EffectDesc, sizeof(CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC));
	Universal_EffectDesc.iEffectNumber = CUniversal_TextureEffect::EFFECTTYPE::TORCHLIGHT_EFFECT;
	Universal_EffectDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	Universal_EffectDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_EffectDesc.dDuration = 99999;
	Universal_EffectDesc.fPositioning = _float3(44.740f, 3.3f, 2.010f);
	Universal_EffectDesc.iTextureIndex = 0;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(79.430f, 3.3f, 1.825f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(122.079f, 3.3f, 29.329f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(118.871f, 3.3f, 62.660f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(49.919f, 3.3f, 70.908f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(5.757f, 3.3f, 47.741f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(13.099f, 3.3f, 122.659f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(65.900f, 3.3f, 116.795f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(121.326f, 3.3f, 82.189f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(105.489f, 3.3f, 4.412f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(78.428f, 3.3f, 123.750f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Torchlight_Pos_Stage2_BossRoom()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC Universal_EffectDesc;
	ZeroMemory(&Universal_EffectDesc, sizeof(CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC));
	Universal_EffectDesc.iEffectNumber = CUniversal_TextureEffect::EFFECTTYPE::TORCHLIGHT_EFFECT;
	Universal_EffectDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	Universal_EffectDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_EffectDesc.dDuration = 99999;
	Universal_EffectDesc.fPositioning = _float3(48.539f, 3.3f, 123.456f);
	Universal_EffectDesc.iTextureIndex = 0;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(76.903f, 3.3f, 119.816f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(20.101f, 3.3f, 19.806f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(112.449f, 3.3f, 103.995f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(19.063f, 3.3f, 106.007f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	Universal_EffectDesc.fPositioning = _float3(94.224f, 3.3f, 13.247f);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE2_BOSSROOM, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Hit_Effect()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUI_Universal::UI_Universal_DESC UI_UniversalDesc;

	UI_UniversalDesc.iUI_Number = CUI_Universal::HIT_PLAYER;
	UI_UniversalDesc.fSizeX = 1280;
	UI_UniversalDesc.fSizeY = 720;
	UI_UniversalDesc.fX = 640;
	UI_UniversalDesc.fY = 360;
	UI_UniversalDesc.fDepth = 50.f;
	UI_UniversalDesc.iTextureIndex = 0;
	UI_UniversalDesc.pGameObject = this;
	UI_UniversalDesc.dDuration = 0.2;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Stamina"), TEXT("Prototype_GameObject_UI_Universal"), &UI_UniversalDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::DashStraight_Effect()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUI_Universal::UI_Universal_DESC UI_UniversalDesc;

	UI_UniversalDesc.iUI_Number = CUI_Universal::DASH_STRAIGHT;
	UI_UniversalDesc.fSizeX = 1280;
	UI_UniversalDesc.fSizeY = 720;
	UI_UniversalDesc.fX = 640;
	UI_UniversalDesc.fY = 360;
	UI_UniversalDesc.fDepth = 50.f;
	UI_UniversalDesc.iTextureIndex = 2;
	UI_UniversalDesc.pGameObject = this;
	UI_UniversalDesc.dDuration = 0.2;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Effect"), TEXT("Prototype_GameObject_UI_Universal"), &UI_UniversalDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::DashRight_Effect()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUI_Universal::UI_Universal_DESC UI_UniversalDesc;

	UI_UniversalDesc.iUI_Number = CUI_Universal::DASH_RIGHT;
	UI_UniversalDesc.fSizeX = 1280;
	UI_UniversalDesc.fSizeY = 720;
	UI_UniversalDesc.fX = 640;
	UI_UniversalDesc.fY = 360;
	UI_UniversalDesc.fDepth = 50.f;
	UI_UniversalDesc.iTextureIndex = 2;
	UI_UniversalDesc.pGameObject = this;
	UI_UniversalDesc.dDuration = 0.2;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Effect"), TEXT("Prototype_GameObject_UI_Universal"), &UI_UniversalDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::DashLeft_Effect()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUI_Universal::UI_Universal_DESC UI_UniversalDesc;

	UI_UniversalDesc.iUI_Number = CUI_Universal::DASH_LEFT;
	UI_UniversalDesc.fSizeX = 1280;
	UI_UniversalDesc.fSizeY = 720;
	UI_UniversalDesc.fX = 640;
	UI_UniversalDesc.fY = 360;
	UI_UniversalDesc.fDepth = 50.f;
	UI_UniversalDesc.iTextureIndex = 2;
	UI_UniversalDesc.pGameObject = this;
	UI_UniversalDesc.dDuration = 0.2;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Effect"), TEXT("Prototype_GameObject_UI_Universal"), &UI_UniversalDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Environment_Particle()
{
	switch (m_eNowLevelNum)
	{
	case LEVEL_STAGE1:
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC Universal_PointDesc;

		Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::RAIN_PARTICLE;
		Universal_PointDesc.iParticleType = CVIBuffer_Point_Instance::PARTICLE_DOWN;
		Universal_PointDesc.fMinSpeed = 6.f;
		Universal_PointDesc.fMaxSpeed = 15.f;
		Universal_PointDesc.fRange = _float3(10.f, 1.f, 10.f);
		Universal_PointDesc.fSize = 0.5f;
		Universal_PointDesc.pObject = this;
		Universal_PointDesc.dDuration = 99999;
		Universal_PointDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
			return E_FAIL;

		//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		//	return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		//FAILED_CHECK(GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_UI, 0.1f));
		FAILED_CHECK(GetSingle(CGameInstance)->PlaySound((L"Environment_Rain.mp3"), CHANNEL_UI));

		break;
	}
	case LEVEL_STAGE1_BOSSROOM:
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC Universal_PointDesc;

		Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::RAIN_PARTICLE;
		Universal_PointDesc.iParticleType = CVIBuffer_Point_Instance::PARTICLE_DOWN;
		Universal_PointDesc.fMinSpeed = 6.f;
		Universal_PointDesc.fMaxSpeed = 15.f;
		Universal_PointDesc.fRange = _float3(10.f, 1.f, 10.f);
		Universal_PointDesc.fSize = 0.5f;
		Universal_PointDesc.pObject = this;
		Universal_PointDesc.dDuration = 99999;
		Universal_PointDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
			return E_FAIL;

		//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		//	return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		//FAILED_CHECK(GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_UI, 0.1f));
		FAILED_CHECK(GetSingle(CGameInstance)->PlaySound((L"Environment_Rain.mp3"), CHANNEL_UI));

		break;
	}
	case LEVEL_STAGE2:
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC Universal_PointDesc;

		Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SNOW_PARTICLE;
		Universal_PointDesc.iParticleType = CVIBuffer_Point_Instance::PARTICLE_DIAGONAL_DOWN;
		Universal_PointDesc.fMinSpeed = 2.f;
		Universal_PointDesc.fMaxSpeed = 10.f;
		Universal_PointDesc.fRange = _float3(10.f, 1.f, 10.f);
		Universal_PointDesc.fSize = 0.5f;
		Universal_PointDesc.pObject = this;
		Universal_PointDesc.dDuration = 99999;
		Universal_PointDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
			return E_FAIL;

		//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		//	return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		//FAILED_CHECK(GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_UI, 0.1f));
		FAILED_CHECK(GetSingle(CGameInstance)->PlaySound((L"Environment_Snow.mp3"), CHANNEL_UI));

		break;
	}
	case LEVEL_STAGE2_BOSSROOM:
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC Universal_PointDesc;

		Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SNOW_PARTICLE;
		Universal_PointDesc.iParticleType = CVIBuffer_Point_Instance::PARTICLE_DIAGONAL_DOWN;
		Universal_PointDesc.fMinSpeed = 2.f;
		Universal_PointDesc.fMaxSpeed = 10.f;
		Universal_PointDesc.fRange = _float3(10.f, 1.f, 10.f);
		Universal_PointDesc.fSize = 0.5f;
		Universal_PointDesc.pObject = this;
		Universal_PointDesc.dDuration = 99999;
		Universal_PointDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
			return E_FAIL;

		//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		//	return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		//FAILED_CHECK(GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_UI, 0.1f));
		FAILED_CHECK(GetSingle(CGameInstance)->PlaySound((L"Environment_Snow.mp3"), CHANNEL_UI));


		break;
	}
	}


	//Mist_MeshEffect();

	return S_OK;
}

HRESULT CPlayer::Mist_MeshEffect()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_MeshEffect::UNIVERSAL_MESH_EFFECT_DESC Universal_Mesh_Effect_Desc;
	ZeroMemory(&Universal_Mesh_Effect_Desc, sizeof(CUniversal_MeshEffect::UNIVERSAL_MESH_EFFECT_DESC));

	Universal_Mesh_Effect_Desc.iMeshEffectNumber = CUniversal_MeshEffect::MESH_EFFECT_TYPE::MIST_EFFECT;

	Universal_Mesh_Effect_Desc.fSpeedPerSec = 10.f;
	Universal_Mesh_Effect_Desc.vScale = _float3(10.f, 10.f, 10.f);
	Universal_Mesh_Effect_Desc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_Mesh_Effect_Desc.dDuration = 100000;

	Universal_Mesh_Effect_Desc.pObject = this;
	Universal_Mesh_Effect_Desc.fPositioning = _float3(0.f,0.f, 0.f);

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Mesh_Effect"), TEXT("Prototype_GameObject_Universal_MeshEffect"), &Universal_Mesh_Effect_Desc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer::MaptoolNotPosition()
{
	//맵툴을 사용하지 않고 포지션을 다시 잡는함수

	switch (m_eNowLevelNum)
	{
	case LEVEL_STAGE1_BOSSROOM:
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&_float4(80.f, 1.f, 40.f, 1.f)));
		break;
	case LEVEL_STAGE2_BOSSROOM:
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&_float4(60.f, 1.f, 40.f, 1.f)));
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CPlayer::Key_Input(_double TimeDelta)
{

	if (LEVEL_MAPTOOL != m_eNowLevelNum)
	{
		Moving_Input(TimeDelta);
		Mouse_Input(TimeDelta);
		Event_Input(TimeDelta);
		Weapon_Slot(TimeDelta);
		Skill_Input(TimeDelta);
	}
	return S_OK;
}

HRESULT CPlayer::Moving_Input(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//평범하게 움직이기
	if (pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		m_pTransform->Go_Straight(TimeDelta, m_pNavigationCom);
	}
	if (pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		m_pTransform->Go_Right(TimeDelta, m_pNavigationCom);
	}
	if (pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		m_pTransform->Go_Left(TimeDelta, m_pNavigationCom);
	}
	if (pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		m_pTransform->Go_Backward(TimeDelta, m_pNavigationCom);
	}
	if (pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		if (false == m_JumpDesc.bJump)
		{
			m_JumpDesc.bJump = true;
		}
		//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_PLAYER, 1);
		GetSingle(CGameInstance)->PlaySound((L"Player_Jump.mp3"), CHANNEL_PLAYER);
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Mouse_Input(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_long		MouseMove = 0;
	if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
	{
		m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	{
		if (0 > MouseMove)
		{
			//if (true == Lock_Rotation(_float3(0.f, 1.f, 0.f), 30.f))
			//	m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove * 0.1f);
			if (true == Lock_Rotation(_float3(0.f, 1.f, 0.f), 10.f))
				m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove * 0.1f);
		}
		else
		{
			//if (true == Lock_Rotation(_float3(0.f, -1.f, 0.f), 50.f))
			//	m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove * 0.1f);
			if (true == Lock_Rotation(_float3(0.f, -1.f, 0.f), 10.f))
				m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove * 0.1f);
		}
	}

	//if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	//{
	//	m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove * 0.1f);
	//}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Fast_Moving_Input(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//빠르게 움직이기

	if (nullptr == m_pMpBar)
	{
		MSGBOX("스테미나에 있는 Bar가 NULL입니다.");
		return E_FAIL;
	}
	if (pGameInstance->Get_DIKeyState(DIK_LSHIFT) && pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press && true == m_bFastSwitch && true == m_pMpBar->Use_Mp(SHIFT_MP))
	{
		m_bGoInterpolationSwitch[0] = true;
		m_bFastSwitch = false;
		m_pMpBar->SetUp_MpBar(SHIFT_MP);

		CUI_Skill::UI_SKILLDESC UI_SkillDesc;
		//스킬들
		UI_SkillDesc.eType = CUI_Skill::ALPHA;
		UI_SkillDesc.fSizeX = 80;
		UI_SkillDesc.fSizeY = 80;
		UI_SkillDesc.fX = 580;
		UI_SkillDesc.fY = 620;
		UI_SkillDesc.fDepth = 2.f;
		UI_SkillDesc.dDuration = SHIFT_COOLTIME;
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
			return E_FAIL;

		DashStraight_Effect();

		//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_PLAYER, 1);
		GetSingle(CGameInstance)->PlaySound((L"Player_Dash.mp3"), CHANNEL_PLAYER);

	}else if (pGameInstance->Get_DIKeyState(DIK_LSHIFT) && pGameInstance->Get_DIKeyState(DIK_W) & DIS_Down && true == m_bFastSwitch && false == m_pMpBar->Use_Mp(SHIFT_MP))
	{
		//진동 우우웅~ 울리게 하자 + 브금
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(3, 0.3f);
		RELEASE_INSTANCE(CGameInstance);

		GetSingle(CGameInstance)->PlaySound((L"Embarrassment.mp3"), CHANNEL_PLAYER);

	}

	if (pGameInstance->Get_DIKeyState(DIK_LSHIFT) && pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press && true == m_bFastSwitch && true == m_pMpBar->Use_Mp(SHIFT_MP))
	{
		m_bGoInterpolationSwitch[1] = true;
		m_bFastSwitch = false;
		m_pMpBar->SetUp_MpBar(SHIFT_MP);

		CUI_Skill::UI_SKILLDESC UI_SkillDesc;
		//스킬들
		UI_SkillDesc.eType = CUI_Skill::ALPHA;
		UI_SkillDesc.fSizeX = 80;
		UI_SkillDesc.fSizeY = 80;
		UI_SkillDesc.fX = 580;
		UI_SkillDesc.fY = 620;
		UI_SkillDesc.fDepth = 2.f;
		UI_SkillDesc.dDuration = SHIFT_COOLTIME;
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
			return E_FAIL;

		//DashBackward_Effect();
		DashStraight_Effect();

		//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_PLAYER, 1);
		GetSingle(CGameInstance)->PlaySound((L"Player_Dash.mp3"), CHANNEL_PLAYER);

	}else if (pGameInstance->Get_DIKeyState(DIK_LSHIFT) && pGameInstance->Get_DIKeyState(DIK_S) & DIS_Down && true == m_bFastSwitch && false == m_pMpBar->Use_Mp(SHIFT_MP))
	{
		//진동 우우웅~ 울리게 하자 + 브금
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(3, 0.3f);
		RELEASE_INSTANCE(CGameInstance);

		GetSingle(CGameInstance)->PlaySound((L"Embarrassment.mp3"), CHANNEL_PLAYER);

	}

	if (pGameInstance->Get_DIKeyState(DIK_LSHIFT) && pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press && true == m_bFastSwitch && true == m_pMpBar->Use_Mp(SHIFT_MP))
	{
		m_bGoInterpolationSwitch[2] = true;
		m_bFastSwitch = false;
		m_pMpBar->SetUp_MpBar(SHIFT_MP);

		CUI_Skill::UI_SKILLDESC UI_SkillDesc;
		//스킬들
		UI_SkillDesc.eType = CUI_Skill::ALPHA;
		UI_SkillDesc.fSizeX = 80;
		UI_SkillDesc.fSizeY = 80;
		UI_SkillDesc.fX = 580;
		UI_SkillDesc.fY = 620;
		UI_SkillDesc.fDepth = 2.f;
		UI_SkillDesc.dDuration = SHIFT_COOLTIME;
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
			return E_FAIL;

		DashRight_Effect();

		//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_PLAYER, 1);
		GetSingle(CGameInstance)->PlaySound((L"Player_Dash.mp3"), CHANNEL_PLAYER);

	}else if (pGameInstance->Get_DIKeyState(DIK_LSHIFT) && pGameInstance->Get_DIKeyState(DIK_D) & DIS_Down && true == m_bFastSwitch && false == m_pMpBar->Use_Mp(SHIFT_MP))
	{
		//진동 우우웅~ 울리게 하자 + 브금
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(3, 0.3f);
		RELEASE_INSTANCE(CGameInstance);

		GetSingle(CGameInstance)->PlaySound((L"Embarrassment.mp3"), CHANNEL_PLAYER);

	}

	if (pGameInstance->Get_DIKeyState(DIK_LSHIFT) && pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press && true == m_bFastSwitch && true == m_pMpBar->Use_Mp(SHIFT_MP))
	{
		m_bGoInterpolationSwitch[3] = true;
		m_bFastSwitch = false;
		m_pMpBar->SetUp_MpBar(SHIFT_MP);

		CUI_Skill::UI_SKILLDESC UI_SkillDesc;
		//스킬들
		UI_SkillDesc.eType = CUI_Skill::ALPHA;
		UI_SkillDesc.fSizeX = 80;
		UI_SkillDesc.fSizeY = 80;
		UI_SkillDesc.fX = 580;
		UI_SkillDesc.fY = 620;
		UI_SkillDesc.fDepth = 2.f;
		UI_SkillDesc.dDuration = SHIFT_COOLTIME;
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
			return E_FAIL;

		DashLeft_Effect();

		//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_PLAYER, 1);
		GetSingle(CGameInstance)->PlaySound((L"Player_Dash.mp3"), CHANNEL_PLAYER);

	}else if (pGameInstance->Get_DIKeyState(DIK_LSHIFT) && pGameInstance->Get_DIKeyState(DIK_A) & DIS_Down && true == m_bFastSwitch && false == m_pMpBar->Use_Mp(SHIFT_MP))
	{
		//진동 우우웅~ 울리게 하자 + 브금
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(3, 0.3f);
		RELEASE_INSTANCE(CGameInstance);

		GetSingle(CGameInstance)->PlaySound((L"Embarrassment.mp3"), CHANNEL_PLAYER);
	}

	RELEASE_INSTANCE(CGameInstance);

	if (true == m_bGoInterpolationSwitch[0])
	{
		m_bGoInterpolationSwitch[0] = Go_Interpolation_Straight(0.3f, TimeDelta, 20);
	}
	if (true == m_bGoInterpolationSwitch[1])
	{
		m_bGoInterpolationSwitch[1] = Go_Interpolation_Backward(0.3f, TimeDelta, 20);
	}
	if (true == m_bGoInterpolationSwitch[2])
	{
		m_bGoInterpolationSwitch[2] = Go_Interpolation_Right(0.3f, TimeDelta, 20);
	}
	if (true == m_bGoInterpolationSwitch[3])
	{
		m_bGoInterpolationSwitch[3] = Go_Interpolation_Left(0.3f, TimeDelta, 20);
	}

	return S_OK;
}

HRESULT CPlayer::Event_Input(_double TimeDelta)
{
	Fast_Moving_Input(TimeDelta);
	Jumping(TimeDelta);
	return S_OK;
}

HRESULT CPlayer::Jumping(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == m_JumpDesc.bJump)
	{
		_vector	vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

		_float4	vTempPos;
		XMStoreFloat4(&vTempPos,vPosition);
		vTempPos.y = m_JumpDesc.fJumpY + (m_JumpDesc.fJumpPower * m_JumpDesc.dTime - 9.8f * m_JumpDesc.dTime * m_JumpDesc.dTime * 0.5f);
		m_JumpDesc.dTime += TimeDelta;

		if (m_JumpDesc.fJumpY > vTempPos.y)
		{
			m_JumpDesc.bJump = false;
			m_JumpDesc.dTime = 0;
		}

		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vTempPos));
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer::Skill_Input(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_Q) & DIS_Down && true == m_bReflectSwitch && true == m_pMpBar->Use_Mp(REFLECT_MP))
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::REFLECT_SKILL;
		Universal_BulletDesc.eType = PLAYER;

		Universal_BulletDesc.fSpeedPerSec = 10.f;
		Universal_BulletDesc.vScale = _float3(10.f, 10.f, 10.f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);

		_float4 fTempPos;
		_float3 fTempLook;
		XMStoreFloat4(&fTempPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		XMStoreFloat3(&fTempLook, m_pTransform->Get_State(CTransform::STATE_LOOK));
		//fTempPos = _float4(1.f,1.f,1.f,1.f);
		Universal_BulletDesc.vTranslation = fTempPos;
		Universal_BulletDesc.vLook = fTempLook;


		Universal_BulletDesc.dDuration = REFLECT_COOLTIME;

		Universal_BulletDesc.fColliderSphereScale = _float3(0.1f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(0.1f, 1.0f, 1.0f);

		Universal_BulletDesc.iTextureIndex = 0; //텍스쳐 인덱스 갯수

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Reflect_SKill"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bReflectSwitch = false;
		m_pMpBar->SetUp_MpBar(REFLECT_MP);


		CUI_Skill::UI_SKILLDESC UI_SkillDesc;
		//스킬들
		UI_SkillDesc.eType = CUI_Skill::ALPHA;
		UI_SkillDesc.fSizeX = 80;
		UI_SkillDesc.fSizeY = 80;
		UI_SkillDesc.fX = 680;
		UI_SkillDesc.fY = 620;
		UI_SkillDesc.fDepth = 2.f;
		UI_SkillDesc.dDuration = REFLECT_COOLTIME;
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
			return E_FAIL;

		GetSingle(CGameInstance)->PlaySound((L"Player_Reflect_Summons.mp3"), CHANNEL_PLAYER);

	}
	else if (pGameInstance->Get_DIKeyState(DIK_Q) & DIS_Down && true == m_bReflectSwitch && false == m_pMpBar->Use_Mp(REFLECT_MP))
	{
		//대충 마나가 없다는 사운드 내뿜으면 될 듯 + 진동
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(3, 0.3f);
		RELEASE_INSTANCE(CGameInstance);

		GetSingle(CGameInstance)->PlaySound((L"Embarrassment.mp3"), CHANNEL_PLAYER);
	}

	if (pGameInstance->Get_DIKeyState(DIK_E) & DIS_Down && true == m_bSlowTimeSwitch && true == m_pMpBar->Use_Mp(SLOWTIME_MP))
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::SLOWTIME_SKILL;
		Universal_BulletDesc.eType = BULLET;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(30.f, 30.f, 30.f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = SLOWTIME_COOLTIME;
		Universal_BulletDesc.fColliderSphereScale = _float3(0.75f, 0.75f, 0.75f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_SlowTime_SKill"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;
		Universal_BulletDesc.eType = PLAYER;
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_SlowTime_SKill"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bSlowTimeSwitch = false;
		m_pMpBar->SetUp_MpBar(SLOWTIME_MP);


		CUI_Skill::UI_SKILLDESC UI_SkillDesc;
		//스킬들
		UI_SkillDesc.eType = CUI_Skill::ALPHA;
		UI_SkillDesc.fSizeX = 80;
		UI_SkillDesc.fSizeY = 80;
		UI_SkillDesc.fX = 780;
		UI_SkillDesc.fY = 620;
		UI_SkillDesc.fDepth = 2.f;
		UI_SkillDesc.dDuration = SLOWTIME_COOLTIME;
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
			return E_FAIL;

		GetSingle(CGameInstance)->PlaySound((L"Player_Slow.mp3"), CHANNEL_PLAYER);

	}else if (pGameInstance->Get_DIKeyState(DIK_E) & DIS_Down && true == m_bSlowTimeSwitch && false == m_pMpBar->Use_Mp(SLOWTIME_MP))
	{
		//대충 마나가 없다는 사운드 내뿜으면 될 듯 + 진동
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(3, 0.3f);
		RELEASE_INSTANCE(CGameInstance);

		GetSingle(CGameInstance)->PlaySound((L"Embarrassment.mp3"), CHANNEL_PLAYER);
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_bool CPlayer::Lock_Rotation(_float3 vAxis, _float fDegree)
{
	_vector vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

	vLook = XMVector3Normalize(vLook);

	//_vector TempRadian = XMVector3Dot(vLook, XMLoadFloat3(&vAxis));

	_float TempRadian;

	XMStoreFloat(&TempRadian, XMVector3Dot(vLook, XMLoadFloat3(&vAxis)));

	_float fRadian = acosf(TempRadian);

	if (XMConvertToRadians(fDegree) < fRadian)
		return true;
	else
		return false;
}

_bool CPlayer::Go_Interpolation_Straight(_float fTime, _double TimeDelta, _float fAcceleration)
{
	_vector		vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

	if (false == m_bMovingSwitch)
	{
		XMStoreFloat3(&m_fFastMoving, m_pTransform->Get_State(CTransform::STATE_RIGHT));

		m_bMovingSwitch = true;
	}

	_vector vStraight = XMVector3Cross(XMLoadFloat3(&m_fFastMoving), XMLoadFloat3(&_float3(0.f, 1.f, 0.f)));

	m_dMovingTime += TimeDelta;

	if (m_dMovingTime > fTime)
	{
		m_dMovingTime = 0;
		m_bMovingSwitch = false;
		return false;
	}
	else {
		vPosition += XMVector3Normalize(vStraight) *  fAcceleration * (_float)TimeDelta;
		
		_float3 vDir;
		XMStoreFloat3(&vDir, XMVector3Normalize(vStraight) * fAcceleration * (_float)TimeDelta);

		if (true == m_pNavigationCom->Move_OnNavigation(m_pTransform->Get_State(CTransform::STATE_POSITION), &vDir))
		{
			m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		}
		else {
			m_pTransform->Set_State(CTransform::STATE_POSITION,m_pTransform->Get_State(CTransform::STATE_POSITION)+ XMLoadFloat3(&vDir));
		}

		//m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	}


	return true;
}

_bool CPlayer::Go_Interpolation_Right(_float fTime, _double TimeDelta, _float fAcceleration)
{
	_vector		vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

	if (false == m_bMovingSwitch)
	{
		XMStoreFloat3(&m_fFastMoving, m_pTransform->Get_State(CTransform::STATE_LOOK));

		m_bMovingSwitch = true;
	}

	_vector vRight = XMVector3Cross(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), XMLoadFloat3(&m_fFastMoving));

	m_dMovingTime += TimeDelta;

	if (m_dMovingTime > fTime)
	{
		m_dMovingTime = 0;
		m_bMovingSwitch = false;
		return false;
	}
	else {
		vPosition += XMVector3Normalize(vRight) * fAcceleration * (_float)TimeDelta;
		
		_float3 vDir;
		XMStoreFloat3(&vDir, XMVector3Normalize(vRight) * fAcceleration * (_float)TimeDelta);

		if (true == m_pNavigationCom->Move_OnNavigation(m_pTransform->Get_State(CTransform::STATE_POSITION), &vDir))
		{
			m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		}
		else {
			m_pTransform->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + XMLoadFloat3(&vDir));
		}
	}


	return true;
}

_bool CPlayer::Go_Interpolation_Left(_float fTime, _double TimeDelta, _float fAcceleration)
{
	_vector		vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

	if (false == m_bMovingSwitch)
	{
		XMStoreFloat3(&m_fFastMoving, m_pTransform->Get_State(CTransform::STATE_LOOK));

		m_bMovingSwitch = true;
	}

	_vector vLeft = XMVector3Cross(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), XMLoadFloat3(&m_fFastMoving));

	m_dMovingTime += TimeDelta;

	if (m_dMovingTime > fTime)
	{
		m_dMovingTime = 0;
		m_bMovingSwitch = false;
		return false;
	}
	else {
		vPosition -= XMVector3Normalize(vLeft) * fAcceleration * (_float)TimeDelta;
		
		_float3 vDir;
		XMStoreFloat3(&vDir, XMVector3Normalize(-vLeft) * fAcceleration * (_float)TimeDelta);

		if (true == m_pNavigationCom->Move_OnNavigation(m_pTransform->Get_State(CTransform::STATE_POSITION), &vDir))
		{
			m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		}
		else {
			m_pTransform->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + XMLoadFloat3(&vDir));
		}
	}


	return true;
}

_bool CPlayer::Go_Interpolation_Backward(_float fTime, _double TimeDelta, _float fAcceleration)
{
	_vector		vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

	if (false == m_bMovingSwitch)
	{
		XMStoreFloat3(&m_fFastMoving, m_pTransform->Get_State(CTransform::STATE_RIGHT));

		m_bMovingSwitch = true;
	}

	_vector vBackward = XMVector3Cross(XMLoadFloat3(&m_fFastMoving), XMLoadFloat3(&_float3(0.f, 1.f, 0.f)));

	m_dMovingTime += TimeDelta;

	if (m_dMovingTime > fTime)
	{
		m_dMovingTime = 0;
		m_bMovingSwitch = false;
		return false;
	}
	else {
		vPosition -= XMVector3Normalize(vBackward) * fAcceleration * (_float)TimeDelta;
		
		_float3 vDir;
		XMStoreFloat3(&vDir, XMVector3Normalize(-vBackward) * fAcceleration * (_float)TimeDelta);

		if (true == m_pNavigationCom->Move_OnNavigation(m_pTransform->Get_State(CTransform::STATE_POSITION), &vDir))
		{
			m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		}
		else {
			m_pTransform->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + XMLoadFloat3(&vDir));
		}
	}


	return true;
}

HRESULT CPlayer::CoolTime_Manager(_double TimeDelta)
{
	if (false == m_bFastSwitch)
	{
		m_dFastCoolTime += TimeDelta;

		if (m_dFastCoolTime >= SHIFT_COOLTIME)
		{
			m_bFastSwitch = true;
			m_dFastCoolTime = 0;
		}
	}

	if (false == m_bReflectSwitch)
	{
		m_dReflectCoolTime += TimeDelta;

		if (m_dReflectCoolTime >= REFLECT_COOLTIME)
		{
			m_bReflectSwitch = true;
			m_dReflectCoolTime = 0;
		}
	}

	if (false == m_bSlowTimeSwitch)
	{
		m_dSlowTimeCoolTime += TimeDelta;

		if (m_dSlowTimeCoolTime >= SLOWTIME_COOLTIME)
		{
			m_bSlowTimeSwitch = true;
			m_dSlowTimeCoolTime = 0;
		}
	}

	return S_OK;
}

HRESULT CPlayer::SetUp_Weapon()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	m_pWeapon[PISHSWORD] = static_cast<CWeapon*> (pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Weapon_PishSword")));
	m_pWeapon[PISTOL] = static_cast<CWeapon*> (pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Weapon_Pistol")));
	m_pWeapon[SNIPER] = static_cast<CWeapon*> (pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Weapon_Sniper")));
	m_pWeapon[SHOTGUN] = static_cast<CWeapon*> (pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Weapon_Shotgun")));
	m_pWeapon[LAUNCHER] = static_cast<CWeapon*> (pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Weapon_Launcher")));
	m_pWeapon[CANNON] = static_cast<CWeapon*> (pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Weapon_Cannon")));

	m_pUI_Weapon = static_cast<CUI_Weapon*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Weapon")));

	for (_uint i = 0; i < WEAPONEND; i++)
	{
		if (nullptr == m_pWeapon[i])
		{
			MSGBOX("현재 플레이어 내부에 무기가 존재하지 않습니다.");
			return E_FAIL;
		}
	}

	if (nullptr == m_pUI_Weapon)
	{
		MSGBOX("현재 플레이어 내부에 무기UI가 존재하지 않습니다.");
		return E_FAIL;
	}

	Weapon_SetUp = true;

	m_pWeapon[PISHSWORD]->SetUp_TransformOn(true);

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CPlayer::Weapon_Slot(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_1) & DIS_Down)
	{
		for (_uint i = 0; i < WEAPONEND; i++)
		{
			if (i == PISHSWORD)
				continue;
			m_pWeapon[i]->SetUp_TransformOn(false);
		}
		m_pWeapon[PISHSWORD]->SetUp_TransformOn(true);
		m_pWeapon[PISHSWORD]->Set_SlotNumber(PISHSWORD);
		m_pUI_Weapon->Set_Weapon_Slot(0);
	}

	if (pGameInstance->Get_DIKeyState(DIK_2) & DIS_Down)
	{
		for (_uint i = 0; i < WEAPONEND; i++)
		{
			if (i == PISTOL)
				continue;
			m_pWeapon[i]->SetUp_TransformOn(false);
		}
		m_pWeapon[PISTOL]->SetUp_TransformOn(true);
		m_pWeapon[PISTOL]->Set_SlotNumber(PISTOL);
		m_pUI_Weapon->Set_Weapon_Slot(1);
	}

	if (pGameInstance->Get_DIKeyState(DIK_3) & DIS_Down)
	{
		for (_uint i = 0; i < WEAPONEND; i++)
		{
			if (i == SNIPER)
				continue;
			m_pWeapon[i]->SetUp_TransformOn(false);
		}
		m_pWeapon[SNIPER]->SetUp_TransformOn(true);
		m_pWeapon[SNIPER]->Set_SlotNumber(SNIPER);
		m_pUI_Weapon->Set_Weapon_Slot(2);
	}

	if (pGameInstance->Get_DIKeyState(DIK_4) & DIS_Down)
	{
		for (_uint i = 0; i < WEAPONEND; i++)
		{
			if (i == SHOTGUN)
				continue;
			m_pWeapon[i]->SetUp_TransformOn(false);
		}
		m_pWeapon[SHOTGUN]->SetUp_TransformOn(true);
		m_pWeapon[SHOTGUN]->Set_SlotNumber(SHOTGUN);
		m_pUI_Weapon->Set_Weapon_Slot(3);
	}

	if (pGameInstance->Get_DIKeyState(DIK_5) & DIS_Down)
	{
		for (_uint i = 0; i < WEAPONEND; i++)
		{
			if (i == LAUNCHER)
				continue;
			m_pWeapon[i]->SetUp_TransformOn(false);
		}
		m_pWeapon[LAUNCHER]->SetUp_TransformOn(true);
		m_pWeapon[LAUNCHER]->Set_SlotNumber(LAUNCHER);
		m_pUI_Weapon->Set_Weapon_Slot(4);
	}

	if (pGameInstance->Get_DIKeyState(DIK_6) & DIS_Down)
	{
		for (_uint i = 0; i < WEAPONEND; i++)
		{
			if (i == CANNON)
				continue;
			m_pWeapon[i]->SetUp_TransformOn(false);
		}
		m_pWeapon[CANNON]->SetUp_TransformOn(true);
		m_pWeapon[CANNON]->Set_SlotNumber(CANNON);
		m_pUI_Weapon->Set_Weapon_Slot(5);
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer::SetUp_UI()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	CUI_Skill::UI_SKILLDESC UI_SkillDesc;
	//스킬들
	UI_SkillDesc.eType = CUI_Skill::SHIFT_SKILL;
	UI_SkillDesc.fSizeX = 80;
	UI_SkillDesc.fSizeY = 80;
	UI_SkillDesc.fX = 580;
	UI_SkillDesc.fY = 620;
	UI_SkillDesc.fDepth = 20.f;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"),&UI_SkillDesc)))
		return E_FAIL;


	UI_SkillDesc.eType = CUI_Skill::Q_SKILL;
	UI_SkillDesc.fSizeX = 80;
	UI_SkillDesc.fSizeY = 80;
	UI_SkillDesc.fX = 680;
	UI_SkillDesc.fY = 620;
	UI_SkillDesc.fDepth = 20.f;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
		return E_FAIL;


	UI_SkillDesc.eType = CUI_Skill::E_SKILL;
	UI_SkillDesc.fSizeX = 80;
	UI_SkillDesc.fSizeY = 80;
	UI_SkillDesc.fX = 780;
	UI_SkillDesc.fY = 620;
	UI_SkillDesc.fDepth = 20.f;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
		return E_FAIL;

	//폰트들
	UI_SkillDesc.eType = CUI_Skill::SHIFT;
	UI_SkillDesc.fSizeX = 30;
	UI_SkillDesc.fSizeY = 30;
	UI_SkillDesc.fX = 600;
	UI_SkillDesc.fY = 650;
	UI_SkillDesc.fDepth = 10.f;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
		return E_FAIL;

	UI_SkillDesc.eType = CUI_Skill::Q;
	UI_SkillDesc.fSizeX = 30;
	UI_SkillDesc.fSizeY = 30;
	UI_SkillDesc.fX = 700;
	UI_SkillDesc.fY = 650;
	UI_SkillDesc.fDepth = 10.f;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
		return E_FAIL;

	UI_SkillDesc.eType = CUI_Skill::E;
	UI_SkillDesc.fSizeX = 30;
	UI_SkillDesc.fSizeY = 30;
	UI_SkillDesc.fX = 800;
	UI_SkillDesc.fY = 650;
	UI_SkillDesc.fDepth = 10.f;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
		return E_FAIL;

	UI_SkillDesc.eType = CUI_Skill::UI_BULLETTYPE;
	UI_SkillDesc.fSizeX = 150;
	UI_SkillDesc.fSizeY = 100;
	UI_SkillDesc.fX = 1000;
	UI_SkillDesc.fY = 680;
	UI_SkillDesc.fDepth = 20.f;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Skill"), TEXT("Prototype_GameObject_UI_Skill"), &UI_SkillDesc)))
		return E_FAIL;


	//탄창
	CUI_Bullet_Number::UI_NUMBERDESC UI_NumberDesc;

	UI_NumberDesc.eType = GREENMAGAZINETENTH;
	UI_NumberDesc.fSizeX = 15;
	UI_NumberDesc.fSizeY = 15;
	UI_NumberDesc.fX = 990;
	UI_NumberDesc.fY = 670;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_GreenMagazineTenth"), TEXT("Prototype_GameObject_UI_Bullet_Number"), &UI_NumberDesc)))
		return E_FAIL;

	UI_NumberDesc.eType = GREENMAGAZINE;
	UI_NumberDesc.fSizeX = 15;
	UI_NumberDesc.fSizeY = 15;
	UI_NumberDesc.fX = 1005;
	UI_NumberDesc.fY = 670;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_GreenMagazine"), TEXT("Prototype_GameObject_UI_Bullet_Number"), &UI_NumberDesc)))
		return E_FAIL;

	UI_NumberDesc.eType = BLUEMAGAZINETENTH;
	UI_NumberDesc.fSizeX = 15;
	UI_NumberDesc.fSizeY = 15;
	UI_NumberDesc.fX = 1040;
	UI_NumberDesc.fY = 670;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_BlueMagazineTenth"), TEXT("Prototype_GameObject_UI_Bullet_Number"), &UI_NumberDesc)))
		return E_FAIL;

	UI_NumberDesc.eType = BLUEMAGAZINE;
	UI_NumberDesc.fSizeX = 15;
	UI_NumberDesc.fSizeY = 15;
	UI_NumberDesc.fX = 1055;
	UI_NumberDesc.fY = 670;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_BlueMagazine"), TEXT("Prototype_GameObject_UI_Bullet_Number"), &UI_NumberDesc)))
		return E_FAIL;

	//십의 자리와 일의 자리 총 2개씩 ㄱㄱㄱ

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CPlayer::Obsever_Hit(void * pArg)
{
	//CUniversal_Bullet_Mesh* SorceBullet = static_cast<CUniversal_Bullet_Mesh*>(pArg);

	//if (nullptr != SorceBullet)//몬스터들의 탄환은 이걸로 계산됨
	//{
	//	switch (SorceBullet->m_Universal_Bullet_MeshDesc.iMeshNumber)
	//	{
	//	case CUniversal_Bullet_Mesh::WARP:
	//		return _int();
	//	default:
	//		break;
	//	}
	//}

	m_Info.fHp *= -0.01;
	m_pHpBar->SetUp_HpBar(m_Info.fHp);

	m_Info.fHp = 0.f;


	Hit_Effect();


	return _int();
}



CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CPlayer*	pInstance = new CPlayer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	///////////////////////////셰이더에 쓰이지 않음 나중에 위치 확인할때만 확인하자
	Safe_Release(m_pRendererCom);
	//Safe_Release(m_pVIBufferCom);
	//Safe_Release(m_pTextureCom);
	//Safe_Release(m_pShaderCom);
	////////////////////////////////////////////////////
}
