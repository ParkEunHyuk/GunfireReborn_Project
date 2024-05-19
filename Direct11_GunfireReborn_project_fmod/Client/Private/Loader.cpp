#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "Sky.h"
#include "Camera_Dynamic.h"
#include "Terrain.h"
#include "Main_Logo.h"
#include "Sword.h"
#include "BlueEffect.h"
#include "RedEffect.h"
#include "UI_Start_Button.h"
#include "UI_Cancel_Button.h"
#include "UI_Stamina.h"
#include "UI_HpBar.h"
#include "UI_MpBar.h"
#include "UI_DotSight.h"
#include "UI_MonsterBar.h"
#include "UI_Monster_HpBar.h"
#include "UI_Weapon.h"
#include "UI_Bullet_Number.h"
#include "UI_Skill.h"
#include "UI_Universal.h"
#include "Universal_Bullet.h"
#include "Universal_Bullet_Mesh.h"
#include "Item_Universal.h"
#include "Weapon_Cannon.h"
#include "Weapon_Sniper.h"
#include "Weapon_Launcher.h"
#include "Weapon_Pistol.h"
#include "Weapon_PishSword.h"
#include "Weapon_Shotgun.h"
#include "Bullet_Pistol.h"
#include "Bullet_Sniper.h"
#include "Bullet_Shotgun.h"
#include "Bullet_Energy.h"
#include "Bullet_Launcher.h"
#include "NonAnimModel.h"
#include "Monster_Lizard.h"
#include "Monster_GhostDeer.h"
#include "Monster_Horse.h"
#include "Monster_Cube.h"
#include "Monster_PowerLizard.h"
#include "Monster_Raven.h"
#include "Monster_Lamp.h"
#include "Monster_Catfish.h"
#include "Monster_Diego.h"
#include "Monster_Boss_Dokkaebi.h"
#include "Monster_Boss_Griffin.h"
#include "Monster_Boss_Bear.h"
#include "Monster_Npc.h"

#include "Monster_Shield.h"
#include "Monster_Lazer.h"

#include "Universal_TextureEffect.h"
#include "Universal_Point_Instance.h"
#include "Universal_MeshEffect.h"

#include "Trail.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

unsigned int APIENTRY ThreadFunction(void* pArg)
{	
	/* CLoader객체안에 로딩에 필요한 데이터들을 담아놨는데 그걸 써야혀? */
	/* CLoader객체의 주소를 가지고있으면 좋을것같다. */
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	switch (pLoader->Get_LevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_ForLogo();
		break;
	case LEVEL_STAGE1:
		pLoader->Loading_ForLevel_Stage1();
		break;
	case LEVEL_STAGE1_BOSSROOM:
		pLoader->Loading_ForLevel_Stage1_BossRoom();
		break;
	case LEVEL_STAGE2:
		pLoader->Loading_ForLevel_Stage2();
		break;
	case LEVEL_STAGE2_BOSSROOM:
		pLoader->Loading_ForLevel_Stage2_BossRoom();
		break;
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlay();
		break;
	case LEVEL_BOSS:
		pLoader->Loading_ForBoss();
		break;
	case LEVEL_MAPTOOL:
		pLoader->Loading_ForMapTool();
		break;
	}	

	LeaveCriticalSection(&pLoader->Get_CS());	

	return 0;
}



HRESULT CLoader::NativeConstruct(LEVEL eNextLevel)
{
	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunction, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	m_eLevel = eNextLevel;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogo()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 원형컴포넌트를 생성한다. */
#pragma region PROTOTYPE_COMPONENT
	/* For.Prototype_Component_Texture_Player */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Player/Player_0.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Main_Logo"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Logo/Main_Logo%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_UI_Start_Button"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Start_Button_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_UI_Cancel_Button"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Cancel_Button_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Stamina */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Stamina"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Stamina_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HpBar */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_HpBar"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_HpBar_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MpBar */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MpBar"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_MpBar_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_DotSight */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DotSight"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/DotSight/UI_DotSight_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MonsterBar */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterBar"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Monster/UI_MonsterBar_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Monster_HpBar */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_HpBar"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Monster/UI_Monster_HpBar_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Weapon */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Weapon"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Weapon/UI_Weapon_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Bullet_Number */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Bullet_Number"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Bullet_Number/UI_Bullet_Number_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Skill */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Skill"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Skill/UI_Skill_%d.png"), 8))))
		return E_FAIL;

	//==============================유니버셜 불릿들
	/* For.Prototype_Component_Texture_Universal_Bullet_Lizard */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Bullet_Lizard"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Universal_Bullet/Lizard/Monster_Bullet_Lizard_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Bullet_Reflect_Skill */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Bullet_Reflect_SKill"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Universal_Bullet/Reflect_Skill/Reflect_SKill_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Bullet_SlowTime_Skill */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Bullet_SlowTime_SKill"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Universal_Bullet/SlowTime_Skill/SlowTime_SKill_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Bullet_Launcher_Boom */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Bullet_Launcher_Boom"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Universal_Bullet/Launcher_Boom/Launcher_Boom_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Bullet_NonTexture */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Bullet_NonTexture"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Universal_Bullet/NonTexture/NonTexture_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Bullet_GhostDeer */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Bullet_GhostDeer"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Universal_Bullet/GhostDeer/Monster_Bullet_GhostDeer_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Bullet_PowerLizard */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Bullet_PowerLizard"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Universal_Bullet/PowerLizard/Monster_Bullet_PowerLizard_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Bullet_Raven */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Bullet_Raven"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Universal_Bullet/Raven/Monster_Bullet_Raven_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Bullet_Raven_Lazer */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Raven_Lazer"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Universal_Bullet/Raven_Lazer/Monster_Bullet_Raven_Lazer_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Bullet_Yellow_Potal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Yellow_Potal"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Universal_Bullet/Yellow_Potal/Yellow_Potal_%d.png"), 1))))
		return E_FAIL;

	//================================================================

	/* For.Prototype_Component_Texture_Item_Universal*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Universal"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Item/Item_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Boss_Dokkaebi_Stamina*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Boss_Dokkaebi_Stamina"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Boss_Stamina/Boss_Stamina_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Boss_Griffin_Stamina*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Boss_Griffin_Stamina"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Boss_Stamina/Boss_Stamina_1.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Boss_Bear_Stamina*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Boss_Bear_Stamina"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Boss_Stamina/Boss_Stamina_2.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Boss_Hp*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Boss_Hp"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Boss_Stamina/Boss_Hp_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Boss_Shield*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Boss_Shield"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Boss_Stamina/Boss_Shield_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Hit_Player*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Hit_Player"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Player/Hit_Player_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Dash_Straight*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Dash_Straight"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Player/Dash_Straight_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Dash_Right*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Dash_Right"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Player/Dash_Right_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Dash_Left*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Dash_Left"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Player/Dash_Left_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Victory*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Victory"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Victory/Victory_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_Talk_Box*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_Talk_Box"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/Talk_Box/Talk_Box_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Universal_EndingCredits*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Universal_EndingCredits"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Universal/EndingCredits/EndingCredits_0.png"), 1))))
		return E_FAIL;

	//이펙트 텍스쳐

	/* For.Prototype_Component_Texture_Universal_TextureEffect_Pistol*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_TextureEffect_Pistol"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Pistol/Effect_Pistol_%d.png"), 16))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_TextureEffect_Shotgun*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_TextureEffect_Shotgun"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Shotgun/Effect_Shotgun_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_TextureEffect_Sniper*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_TextureEffect_Sniper"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Sniper/Effect_Sniper_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_TextureEffect_Enter*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_TextureEffect_Enter"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Enter/Enter_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_TextureEffect_Torchlight*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_TextureEffect_Torchlight"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Torchlight/Torchlight_%d.png"), 19))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_TextureEffect_Pistol_Muzzle*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_TextureEffect_Pistol_Muzzle"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Pistol/Effect_Pistol_Muzzle_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_TextureEffect_Shotgun_Muzzle*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_TextureEffect_Shotgun_Muzzle"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Shotgun/Effect_Shotgun_Muzzle_0.png"), 1))))
		return E_FAIL;


	//파티클 텍스쳐
	/* For.Prototype_Component_Texture_Universal_Particle_Rain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Rain"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Rain/Rain_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Snow*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Snow"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Snow/Snow_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Glow_Red*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Glow_Red"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Glow/Glow_Red_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Small_Glow_Red*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Small_Glow_Red"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Glow/Small_Glow_Red_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Small_Glow_Yellow*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Small_Glow_Yellow"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Glow/Small_Glow_Yellow_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Small_Glow_Purple*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Small_Glow_Purple"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Glow/Small_Glow_Purple_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Small_Glow_Green*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Small_Glow_Green"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Glow/Small_Glow_Green_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Small_Glow_Blue*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Small_Glow_Blue"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Glow/Small_Glow_Blue_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Embers*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Embers"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Embers/Embers_%d.png"), 16))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Fire*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Fire"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Embers/Fire_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Fire_Rock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Fire_Rock"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Embers/Fire_Rock_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Green_Rock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Green_Rock"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Green_Rock/Green_Rock_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Slow*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Slow"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Slow/Slow_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Blue_Rock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Blue_Rock_0"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Blue_Rock/Blue_Rock_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Blue_Rock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Blue_Rock_1"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Blue_Rock/Blue_Rock_1.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Blue_Rock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Blue_Rock_2"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Blue_Rock/Blue_Rock_2.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Blue_Rock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Blue_Rock_3"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Blue_Rock/Blue_Rock_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_Black_Rock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_Black_Rock_0"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/Black_Rock/Black_Rock_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Universal_Particle_White_Rock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Particle_White_Rock_0"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Particle/White_Rock/White_Rock_%d.png"), 1))))
		return E_FAIL;



	//디졸브 텍스쳐
	/* For.Prototype_Component_Texture_Dissolve*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve_1"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Dissolve/Dissolve_0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Dissolve_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve_2"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Dissolve/Dissolve_1.png"), 1))))
		return E_FAIL;

	//트레일 텍스쳐
	/* For.Prototype_Component_Texture_Red_Trail*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Universal_Trail"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Trail/Universal_Trail_0.png"), 1))))
		return E_FAIL;

#pragma region PROTOTYPE_COMPONENT_WEAPON
	//총 모델들
	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	/* For.Prototype_Component_Model_Cannon*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Cannon"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Weapon/Cannon/", "1409_Right.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Sniper*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sniper"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Weapon/Sniper/", "1501_Right.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Launcher*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Launcher"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Weapon/Launcher/", "1401_Right.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Pistol*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Pistol"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Weapon/Pistol/", "1205_Right.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Shotgun*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shotgun"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Weapon/Shotgun/", "1305_Right.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_PishSword*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_PishSword"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Weapon/PishSword/", "1901_Right.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	//===============================총알들

	/* For.Prototype_Component_Model_Bullet_Launcher*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bullet_Launcher"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Launcher/", "1004_Right.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Bullet_Energy*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bullet_Energy"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/", "Bullet_Energy.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Bullet_Pistol*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bullet_Pistol"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/", "Bullet_Pistol.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Bullet_Sniper*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bullet_Sniper"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/", "Bullet_Sniper.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Bullet_Shutgun*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bullet_Shotgun"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/", "Bullet_Shotgun.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Item_Hp*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Item_Hp"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Item/Item_Hp/", "Dropltem_Bullet_Gren_205.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Item_Mp*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Item_Mp"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Item/Item_Mp/", "Dropltem_Bullet_Gren_201.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Item_GreenMagazine*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Item_GreenMagazine"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Item/Item_GreenMagazine/", "Dropltem_Bullet_Shrapnel.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Item_BlueMagazine*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Item_BlueMagazine"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Item/Item_BlueMagazine/", "Dropltem_Bullet_Standard.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Burn_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Burn_Bullet"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Burn_Bullet/", "BFGBullet.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Supplement_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Supplement_Bullet"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Supplement_Bullet/", "Supplement_Bullet.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Meteorite_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Meteorite_Bullet"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Meteorite_Bullet/", "Meteorite_Bullet.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Terrain_Bullet"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Terrain_Bullet/", "Terrain_Bullet.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_ShockWave_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_ShockWave_Bullet"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/ShockWave_Bullet/", "Static_Mesh_ShockWave_1.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_SwordWave_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordWave_Bullet"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/SwordWave_Bullet/", "SM_Stone_01.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Rock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rock"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Rock/", "Crack_1.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Tornado_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Tornado_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Tornado/", "Tornado_1.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Tornado_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Tornado_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Tornado/", "Tornado_2.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Triangle_Sword*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Triangle_Sword"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Weapon_Bullet/", "Triangle_Sword.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Blunt_Sword*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Blunt_Sword"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Weapon_Bullet/", "Blunt_Sword.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Bident_Spear*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bident_Spear"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Weapon_Bullet/", "Bident_Spear.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Big_Sword*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Big_Sword"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Weapon_Bullet/", "Big_Sword.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fatal_Rapier*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Fatal_Rapier"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Weapon_Bullet/", "Fatal_Rapier.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Sphere*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sphere"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Weapon_Bullet/", "Sphere.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Lateral_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Lateral_Bullet"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Sword_Bullet/", "Lateral_Bullet.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Upper_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Upper_Bullet"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Sword_Bullet/", "Upper_Bullet.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Vertical_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Vertical_Bullet"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Sword_Bullet/", "Vertical_Bullet.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Phase_Two*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Phase_Two"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Phase_Two/", "Phase_Two.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Sauron*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sauron"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Sauron/", "Sauron.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Devil*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Devil"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Devil/", "Devil.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Awl*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Awl"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Awl/", "Awl.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Mace_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Mace_Bullet"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Mace_Bullet/", "Mace_Bullet.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Mace_Bullet_Right*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Mace_Bullet_Right"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Mace_Bullet_Right/", "Mace_Bullet_Right.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Warp*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Warp"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bullet/Universal_Bullet_Mesh/Warp/", "Warp.fbx", TransformMatrix))))
		return E_FAIL;


	/* For.Prototype_Component_Model_MeshEffect_Lightning*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_MeshEffect_Lightning"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Effect/Lightning/", "Lightning.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_MeshEffect_Mist*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_MeshEffect_Mist"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Effect/Mist/", "Mist.fbx", TransformMatrix))))
		return E_FAIL;




#pragma endregion

#pragma endregion



#pragma  region PROTOTYPE_GAMEOBJECT
	//일단 배웠기 때문에 넣어둔 더미 데이터들
	/* For.Prototype_GameObject_Sword */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sword"),
		CSword::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BlueEffect */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BlueEffect"),
		CBlueEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_RedEffect */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RedEffect"),
		CRedEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


#pragma  region PROTOTYPE_UI
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Main_Logo"),
		CMain_Logo::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Start_Button"),
		CUI_Start_Button::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Button */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Cancel_Button"),
		CUI_Cancel_Button::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_UI_Stamina */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Stamina"), CUI_Stamina::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_UI_Stamina");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_UI_HpBar */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HpBar"), CUI_HpBar::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_UI_HpBar");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_UI_MpBar */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MpBar"), CUI_MpBar::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_UI_MpBar");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_UI_DotSight */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_DotSight"), CUI_DotSight::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_UI_DotSightr");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_UI_Monster */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MonsterBar"), CUI_MonsterBar::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_UI_Monster");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_UI_Monster_HpBar */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Monster_HpBar"), CUI_Monster_HpBar::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_UI_Monster_HpBar");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_UI_Weapon */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Weapon"), CUI_Weapon::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_UI_Weapon");
			return E_FAIL;
	}

	/* For.Prototype_GameObject_UI_Bullet_Number */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Bullet_Number"), CUI_Bullet_Number::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_UI_Bullet_Number");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_UI_Skill */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Skill"), CUI_Skill::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_UI_Skill");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_UI_Universal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Universal"), CUI_Universal::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_UI_Universal");
		return E_FAIL;
	}

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT_MODEL
	/* For.Prototype_GameObject_NonAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NonAnimModel"), CNonAnimModel::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CNonNonAnimModel 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Lizard */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Lizard"), CMonster_Lizard::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CMonster_Lizard 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_GostDeer */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GostDeer"), CMonster_GhostDeer::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CMonster_GhostDeer 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Horse */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Horse"), CMonster_Horse::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CMonster_Horse 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Cube */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"), CMonster_Cube::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CMonster_Cube 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_PowerLizard */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PowerLizard"), CMonster_PowerLizard::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Prototype_GameObject_PowerLizard 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Raven */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Raven"), CMonster_Raven::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Prototype_GameObject_Raven 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Lamp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Lamp"), CMonster_Lamp::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Prototype_GameObject_Lamp 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Catfish */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Catfish"), CMonster_Catfish::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Prototype_GameObject_Catfish 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Diego */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Diego"), CMonster_Diego::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Prototype_GameObject_Diego 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Npc */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Npc"), CMonster_Npc::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Prototype_GameObject_Npc 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Boss_Dokkaebi */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Dokkaebi"), CMonster_Boss_Dokkaebi::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Prototype_GameObject_Boss_Dokkaebi 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Boss_Griffin */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Griffin"), CMonster_Boss_Griffin::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Prototype_GameObject_Boss_Griffin 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Boss_Bear */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Bear"), CMonster_Boss_Bear::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Prototype_GameObject_Boss_Bear 생성 실패");
		return E_FAIL;
	}


	//총들

	/* For.Prototype_GameObject_Cannon*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cannon"), CWeapon_Cannon::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CWeapon_Cannon 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Sniper*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sniper"), CWeapon_Sniper::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CWeapon_Cannon 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Launcher*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Launcher"), CWeapon_Launcher::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CWeapon_Launcher 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Pistol*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Pistol"), CWeapon_Pistol::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CWeapon_Pistol 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Shotgun*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shotgun"), CWeapon_Shotgun::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CWeapon_Shotgun 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_PishSword*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PishSword"), CWeapon_PishSword::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CWeapon_PishSword 생성 실패");
		return E_FAIL;
	}

	//총알들================================

	/* For.Prototype_GameObject_Bullet_Pistol*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet_Pistol"), CBullet_Pistol::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CBullet_Pistol 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Bullet_Sniper*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet_Sniper"), CBullet_Sniper::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CBullet_Sniper 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Bullet_ShotGun*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet_Shotgun"), CBullet_Shotgun::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CBullet_ShotGun 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Bullet_Launcher*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet_Launcher"), CBullet_Launcher::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CBullet_Launcher 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Bullet_Energy*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bullet_Energy"), CBullet_Energy::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CBullet_Energy 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Universal_Bullet*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Universal_Bullet"), CUniversal_Bullet::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CUniversal_Bullet 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Universal_Bullet_Mesh*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), CUniversal_Bullet_Mesh::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CUniversal_Bullet_Mesh 생성 실패");
		return E_FAIL;
	}

	//아이템
	/* For.Prototype_GameObject_Item_Universal*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item_Universal"), CItem_Universal::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CItem_Universal 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Monster_Shield*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Shield"), CMonster_Shield::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CMonster_Shield 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Monster_Lazer*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Lazer"), CMonster_Lazer::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("CMonster_Lazer 생성 실패");
		return E_FAIL;
	}

#pragma endregion


#pragma  region PROTOTYPE_EFFECT


	/* For.Prototype_GameObject_Universal_Point_Instance */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Universal_Point_Instance"),
		CUniversal_Point_Instance::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Universal_TextureEffect */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Universal_TextureEffect"), CUniversal_TextureEffect::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_Universal_TextureEffect");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Trail */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Trail"), CTrail::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_Trail");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Universal_MeshEffect */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Universal_MeshEffect"), CUniversal_MeshEffect::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_Universal_MeshEffect");
		return E_FAIL;
	}

#pragma endregion


#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_bIsFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_ForLevel_Stage1()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT

#pragma region Buffer

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_Stage1/Terrain.bin"), 129, 129))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect_Instance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pDeviceContext, 30))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point_Instance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 100))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_RectTrail */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_RectTrail"),
		CVIBuffer_RectTrail::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

#pragma endregion

#pragma region TEXTURE

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Terrain_0.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain1 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain1"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Terrain_0.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/SkyBox_0.dds"), 5))))
		return E_FAIL;

#pragma endregion



#pragma region Shader
	/* For.Prototype_Component_Shader_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_Cube"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_NonAnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_NonAnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_NonAnimModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/*For.Prototype_Component_Shader_AnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_AnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_AnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxINstance.hlsl"), VTXMATRIX_DECLARATION::Elements, VTXMATRIX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_PointInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_PointInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointINstance.hlsl"), VTXPOINT_MATRIX_DECLARATION::Elements, VTXPOINT_MATRIX_DECLARATION::iNumElements))))
		return E_FAIL;
#pragma endregion

#pragma region Navigation
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_Stage1/Navigation.dat")))))
		return E_FAIL;
#pragma endregion


#pragma region Model
	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	//스테이지1@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_10*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_10.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_10 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_11*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_11"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_11.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_11 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_12*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_12"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_12.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_12 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_13*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_13"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_13.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_13 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_14*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_14"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_14.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_14 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Ship_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Ship_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Ship_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Ship_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Ship_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Ship_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Ship_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Ship_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_5 생성 실패");
		return E_FAIL;
	}

	//추가 데코
	/* For.Prototype_Component_Model_Terrain_SubDecoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_SubDecoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_SubDecoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_SubDecoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_SubDecoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_SubDecoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_SubDecoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_SubDecoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_SubDecoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Terrain_SubDecoration_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_9 생성 실패");
		return E_FAIL;
	}



	//===============================여기부턴 애니메이션 모델

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	/* For.Prototype_Component_Model_Lizard*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Lizard"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Lizard/", "2122_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_GostDeer*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_GostDeer"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/GostDeer/", "2164_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Horse*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Horse"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Horse/", "3123_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Cube"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Cube/", "2224_model (merge).fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Lamp*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Lamp"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Lamp/", "2003.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Npc*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Npc"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Npc/", "Npc_Smith_Skin.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

#pragma endregion


#pragma endregion




#pragma region PROTOTYPE_COMPONENT
#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_bIsFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLevel_Stage1_BossRoom()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT

#pragma region Buffer

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_Stage1_BossRoom/Terrain.bin"), 129, 129))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect_Instance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pDeviceContext, 30))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point_Instance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 100))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_RectTrail */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_VIBuffer_RectTrail"),
		CVIBuffer_RectTrail::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

#pragma endregion

#pragma region TEXTURE

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Terrain_0.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain1 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Texture_Terrain1"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Terrain_0.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/SkyBox_0.dds"), 5))))
		return E_FAIL;

#pragma endregion



#pragma region Shader
	/* For.Prototype_Component_Shader_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Shader_Cube"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_NonAnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Shader_NonAnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_NonAnimModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/*For.Prototype_Component_Shader_AnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Shader_AnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_AnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Shader_VtxInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxINstance.hlsl"), VTXMATRIX_DECLARATION::Elements, VTXMATRIX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_PointInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Shader_PointInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointINstance.hlsl"), VTXPOINT_MATRIX_DECLARATION::Elements, VTXPOINT_MATRIX_DECLARATION::iNumElements))))
		return E_FAIL;
#pragma endregion

#pragma region Navigation
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_Stage1_BossRoom/Navigation.dat")))))
		return E_FAIL;
#pragma endregion


#pragma region Model
	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_10*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_10.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_10 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_11*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_11"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_11.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_11 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_12*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_12"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_12.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_12 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_13*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_13"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_13.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_13 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_14*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_14"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_14.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_14 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Ship_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Ship_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Ship_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Ship_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Ship_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Ship_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Ship_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Ship_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_5 생성 실패");
		return E_FAIL;
	}

	//추가 데코
	/* For.Prototype_Component_Model_Terrain_SubDecoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_9 생성 실패");
		return E_FAIL;
	}

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	/* For.Prototype_Component_Model_Boss_Dokkaebi*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Boss_Dokkaebi"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Boss_Dokkaebi/", "3901_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Npc*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1_BOSSROOM, TEXT("Prototype_Component_Model_Npc"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Npc/", "Npc_Smith_Skin.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

#pragma endregion


#pragma endregion




#pragma region PROTOTYPE_COMPONENT
#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_bIsFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLevel_Stage2()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT

#pragma region Buffer

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_Stage2/Terrain.bin"), 129, 129))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect_Instance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pDeviceContext, 30))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point_Instance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 100))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_RectTrail */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_VIBuffer_RectTrail"),
		CVIBuffer_RectTrail::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

#pragma region Buffer

#pragma region TEXTURE

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Terrain_1.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain1 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Terrain1"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Terrain_1.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/SkyBox_2.dds"), 5))))
		return E_FAIL;

#pragma endregion



#pragma region Shader
	/* For.Prototype_Component_Shader_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Shader_Cube"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_NonAnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Shader_NonAnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_NonAnimModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/*For.Prototype_Component_Shader_AnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Shader_AnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_AnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Shader_VtxInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxINstance.hlsl"), VTXMATRIX_DECLARATION::Elements, VTXMATRIX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_PointInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Shader_PointInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointINstance.hlsl"), VTXPOINT_MATRIX_DECLARATION::Elements, VTXPOINT_MATRIX_DECLARATION::iNumElements))))
		return E_FAIL;
#pragma endregion

#pragma region Navigation
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_Stage2/Navigation.dat")))))
		return E_FAIL;
#pragma endregion


#pragma region Model

	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	//스테이지2@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_10*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_10.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_10 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_11*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_11"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_11.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_11 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_12*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_12"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_12.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_12 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_10*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_10.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_10 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_11*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_11"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_11.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_11 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_12*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_12"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_12.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_12 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Snow_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Snow_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Snow_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Snow_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Snow_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Snow_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Snow_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Snow_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Snow_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_5 생성 실패");
		return E_FAIL;
	}

	//추가 데코
	/* For.Prototype_Component_Model_Terrain_SubDecoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_SubDecoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_SubDecoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_SubDecoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_SubDecoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_SubDecoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_SubDecoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_SubDecoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_SubDecoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_SubDecoration_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_5 생성 실패");
		return E_FAIL;
	}

	//추가 애니메이션

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	/* For.Prototype_Component_Model_Lizard*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Lizard"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Lizard/", "2122_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_GostDeer*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_GostDeer"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/GostDeer/", "2164_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Horse*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Horse"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Horse/", "3123_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Cube"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Cube/", "2224_model (merge).fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_PowerLizard*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_PowerLizard"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/PowerLizard/", "3125_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Raven*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Raven"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Raven/", "2163_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Lamp*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Lamp"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Lamp/", "2003.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Catfish*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Catfish"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Catfish/", "2087_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Diego*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Diego"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Diego/", "2021_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Npc*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Model_Npc"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Npc/", "Npc_Smith_Skin.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

#pragma endregion


#pragma endregion




#pragma region PROTOTYPE_COMPONENT
#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_bIsFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLevel_Stage2_BossRoom()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT

#pragma region Buffer

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_Stage2_BossRoom/Terrain.bin"), 129, 129))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect_Instance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pDeviceContext, 30))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point_Instance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 100))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_RectTrail */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_VIBuffer_RectTrail"),
		CVIBuffer_RectTrail::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

#pragma endregion

#pragma region TEXTURE

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Terrain_1.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain1 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Texture_Terrain1"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Terrain_1.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/SkyBox_3.dds"), 5))))
		return E_FAIL;

#pragma endregion



#pragma region Shader
	/* For.Prototype_Component_Shader_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Shader_Cube"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_NonAnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Shader_NonAnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_NonAnimModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/*For.Prototype_Component_Shader_AnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Shader_AnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_AnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Shader_VtxInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxINstance.hlsl"), VTXMATRIX_DECLARATION::Elements, VTXMATRIX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_PointInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Shader_PointInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointINstance.hlsl"), VTXPOINT_MATRIX_DECLARATION::Elements, VTXPOINT_MATRIX_DECLARATION::iNumElements))))
		return E_FAIL;
#pragma endregion

#pragma region Navigation
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_Stage2_BossRoom/Navigation.dat")))))
		return E_FAIL;
#pragma endregion


#pragma region Model
	//스테이지2@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_10*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_10.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_10 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_11*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_11"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_11.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_11 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_12*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_12"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_12.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_12 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_10*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_10.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_10 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_11*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_11"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_11.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_11 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_12*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_12"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_12.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_12 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Snow_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Snow_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Snow_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Snow_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Snow_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Snow_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Snow_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Snow_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Snow_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_5 생성 실패");
		return E_FAIL;
	}

	//추가 데코
	/* For.Prototype_Component_Model_Terrain_SubDecoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_SubDecoration_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Snow_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Snow_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Snow_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Snow_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Snow_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Terrain_Snow_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	//===============================여기부턴 애니메이션 모델

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));


	/* For.Prototype_Component_Model_Boss_Bear*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Boss_Bear"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Boss_Bear/", "3924_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Npc*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2_BOSSROOM, TEXT("Prototype_Component_Model_Npc"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Npc/", "Npc_Smith_Skin.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

#pragma endregion


#pragma endregion




#pragma region PROTOTYPE_COMPONENT
#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_bIsFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlay()
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 원형컴포넌트를 생성한다. */
#pragma region PROTOTYPE_COMPONENT

	///* For.Prototype_Component_VIBuffer_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
	//	return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_GamePlay/Terrain.bin"),129,129))))
		return E_FAIL;


	///* For.Prototype_Component_VIBuffer_Terrain2 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain2"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, 129, 129))))
	//	return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect_Instance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pDeviceContext, 30))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_RectTrail */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_RectTrail"),
		CVIBuffer_RectTrail::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point_Instance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pDeviceContext, 100))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Cube"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_NonAnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_NonAnimModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_AnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_AnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_AnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxINstance.hlsl"), VTXMATRIX_DECLARATION::Elements, VTXMATRIX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_PointInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_PointInstance"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointINstance.hlsl"), VTXPOINT_MATRIX_DECLARATION::Elements, VTXPOINT_MATRIX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_GamePlay/Navigation.dat")))))
		return E_FAIL;




	/* For.Prototype_Component_Texture_Snow*/ //!@#!@#@!#!@#@#!@#!@#!@ 렉트 인스턴싱
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
		return E_FAIL;



	/* For.Prototype_Component_Texture_Terrain1 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain1"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds"), 1))))
		return E_FAIL;
//@@@@@@@@@@@@@@@@
	/* For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/SkyBox_4.dds"), 5))))
		return E_FAIL;


	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	///* For.Prototype_Component_Model_Fiona */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Fiona/", "Fiona.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Fork */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fork"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ForkLift/", "ForkLift.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Volcano_1 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Volcano_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Volcano_2 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Volcano_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Volcano_3 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Volcano_3"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Volcano_Rock_1 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Volcano_Rock_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_Rock_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Volcano_Rock_2 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Volcano_Rock_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_Rock_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Mountain_1 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Mountain_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Mountain_2 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Mountain_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Mountain_3 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Mountain_3"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Mountain_4 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Mountain_4"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Mountain_5 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Mountain_5"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Mountain_6 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Mountain_6"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Snow_Rock_1 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Snow_Rock_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_Rock_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Snow_Rock_2 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Snow_Rock_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_Rock_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Snow_Rock_3 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Snow_Rock_3"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_Rock_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Rock_1 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Rock_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Rock_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Rock_2 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Rock_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Rock_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Rock_3 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Rock_3"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Rock_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Rock_4 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Rock_4"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Rock_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Snow_1 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Snow_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Snow_2 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Snow_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Snow_3 */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Snow_3"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Door_1*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Door_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Door_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Door_2*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Door_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Door_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Decoration_1*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Decoration_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Decoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Decoration_2*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Decoration_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Decoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Decoration_3*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Decoration_3"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Decoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Decoration_4*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Decoration_4"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Decoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Volcano_Far_1*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Volcano_Far_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_Far_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_Volcano_Far_2*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Volcano_Far_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_Far_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_SnowMountain_Far_1*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_SnowMountain_Far_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_SnowMountain_Far_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Terrain_SnowMountain_Far_2*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_SnowMountain_Far_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_SnowMountain_Far_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;


	////스테이지1@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	///* For.Prototype_Component_Model_Terrain_Stage1_Decoration_1*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//{
	//	MSGBOX("로더에서 Stage1_Decoration_1 생성 실패");
	//	return E_FAIL;
	//}

	///* For.Prototype_Component_Model_Terrain_Stage1_Ship_1*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Stage1_Ship_1"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Ship_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//{
	//	MSGBOX("로더에서 Terrain_Stage1_Ship_1 생성 실패");
	//	return E_FAIL;
	//}
	///* For.Prototype_Component_Model_Terrain_Stage1_Ship_2*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Terrain_Stage1_Ship_2"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Ship_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//{
	//	MSGBOX("로더에서 Terrain_Stage1_Ship_2 생성 실패");
	//	return E_FAIL;
	//}




	////===============================여기부턴 애니메이션 모델

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	/* For.Prototype_Component_Model_Lizard*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Lizard"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Lizard/", "2122_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_GostDeer*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_GostDeer"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/GostDeer/", "2164_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Horse*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Horse"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Horse/", "3123_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cube"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Cube/", "2224_model (merge).fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_PowerLizard*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PowerLizard"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/PowerLizard/", "3125_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Raven*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Raven"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Raven/", "2163_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Lamp*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Lamp"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Lamp/", "2003.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Catfish*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Catfish"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Catfish/", "2087_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Diego*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Diego"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Diego/", "2021_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	///* For.Prototype_Component_Model_Boss_Dokkaebi*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Dokkaebi"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Boss_Dokkaebi/", "3901_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Boss_Bear*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Bear"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Boss_Bear/", "3924_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

	//TransformMatrix = XMMatrixRotationY(XMConvertToRadians(220.0f))*XMMatrixTranslation(-192.25f, -7.5f, -3.f); //그리핀을 위한 행렬 변환

	///* For.Prototype_Component_Model_Boss_Griffin*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Griffin"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Boss_Griffin/", "3915_Skin.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	//	return E_FAIL;

//@@@@@@@@@@@@@@@@@@


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	/* 원형객체를 생성한다. */

	/* For.Prototype_GameObject_Player */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
	//	CTerrain::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Sky */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
	//	CSky::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Camera_Dynamic */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
	//	CCamera_Dynamic::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_UI */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"), CUI::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"), CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_BlueEffect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BlueEffect"),
	//	CBlueEffect::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;


#pragma endregion

	RELEASE_INSTANCE(CGameInstance);


	m_bIsFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForBoss()
{

	m_bIsFinished = true;
	
	return S_OK;
}

HRESULT CLoader::Loading_ForMapTool()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 원형컴포넌트를 생성한다. */
#pragma region PROTOTYPE_COMPONENT

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	///* For.Prototype_Component_VIBuffer_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
	//	return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Terrain_Dynamic */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_VIBuffer_Terrain_Dynamic"),
		CVIBuffer_Terrain_Dynamic::Create(m_pDevice, m_pDeviceContext, 129,129))))
		return E_FAIL;


	///* For.Prototype_Component_VIBuffer_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_VIBuffer_Terrain2"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, 129, 129))))
	//	return E_FAIL;

	/*텍스쳐*/
#pragma region TEXTURE


	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain1 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Terrain1"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/SkyBox_4.dds"), 5))))
		return E_FAIL;
#pragma endregion

#pragma region Shader
	/*셰이더*/

	/* For.Prototype_Component_Shader_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Shader_Cube"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_NonAnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Shader_NonAnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_NonAnimModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/*For.Prototype_Component_Shader_AnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Shader_AnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_AnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */ //맵툴에선 몬스터들을 움직일 필요가 없으므로 맵툴의 Navigation.dat에 신경쓰지말자
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Save/Level_GamePlay/Navigation.dat")))))
		return E_FAIL;

#pragma endregion

#pragma region Model
	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_Fiona */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Fiona/", "Fiona.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Fork */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Fork"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ForkLift/", "ForkLift.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Volcano_1 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Volcano_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Volcano_2 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Volcano_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Volcano_3 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Volcano_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Volcano_Rock_1 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Volcano_Rock_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_Rock_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Volcano_Rock_2 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Volcano_Rock_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_Rock_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Mountain_1 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Mountain_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Mountain_2 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Mountain_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Mountain_3 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Mountain_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Mountain_4 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Mountain_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Mountain_5 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Mountain_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Mountain_6 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Mountain_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Mountain_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Snow_Rock_1 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Snow_Rock_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_Rock_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Snow_Rock_2 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Snow_Rock_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_Rock_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Snow_Rock_3 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Snow_Rock_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_Rock_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Rock_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Rock_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Rock_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Rock_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Rock_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Rock_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Rock_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Rock_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Rock_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Rock_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Rock_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Rock_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Snow_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Snow_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Snow_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Snow_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Snow_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Snow_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Snow_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Door_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Door_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Door_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Door_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Door_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Door_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Decoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Decoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Decoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Decoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Decoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Decoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Decoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Decoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Decoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Decoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Decoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Decoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Volcano_Far_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Volcano_Far_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_Far_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_Volcano_Far_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Volcano_Far_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Volcano_Far_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_SnowMountain_Far_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SnowMountain_Far_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_SnowMountain_Far_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Terrain_SnowMountain_Far_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SnowMountain_Far_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_SnowMountain_Far_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	//스테이지1@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Decoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Decoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Decoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Decoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_10*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_10.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_10 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_11*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_11"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_11.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_11 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_12*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_12"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_12.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_12 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_13*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_13"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_13.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_13 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Mountain_14*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Mountain_14"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Mountain_14.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Mountain_14 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Ship_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Ship_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Ship_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Ship_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Ship_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Ship_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Ship_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Ship_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage1_Wall_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage1_Wall_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage1_Wall_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage1_Wall_5 생성 실패");
		return E_FAIL;
	}

	//스테이지2@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_10*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_10.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_10 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_11*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_11"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_11.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_11 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Decoration_12*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Decoration_12"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Decoration_12.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Decoration_12 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_9 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_10*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_10"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_10.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_10 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_11*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_11"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_11.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_11 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Mountain_12*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Mountain_12"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Mountain_12.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Mountain_12 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Snow_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Snow_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Snow_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Snow_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Snow_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Snow_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Snow_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Snow_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Snow_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Snow_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_Stage2_Wall_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_Stage2_Wall_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "Terrain_Stage2_Wall_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_Stage2_Wall_5 생성 실패");
		return E_FAIL;
	}

	//추가 데코
	/* For.Prototype_Component_Model_Terrain_SubDecoration_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SubDecoration_1"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_1.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_1 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SubDecoration_2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_2.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_2 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_3*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SubDecoration_3"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_3.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_3 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_4*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SubDecoration_4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_4.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_4 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_5*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SubDecoration_5"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_5.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_5 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_6*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SubDecoration_6"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_6.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_6 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_7*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SubDecoration_7"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_7.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_7 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_8*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SubDecoration_8"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_8.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_8 생성 실패");
		return E_FAIL;
	}

	/* For.Prototype_Component_Model_Terrain_SubDecoration_9*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Terrain_SubDecoration_9"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Static/", "SubDecoration_9.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
	{
		MSGBOX("로더에서 Terrain_SubDecoration_9 생성 실패");
		return E_FAIL;
	}


	//===============================여기부턴 애니메이션 모델

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	/* For.Prototype_Component_Model_Lizard*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Lizard"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Lizard/", "2122_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_GostDeer*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_GostDeer"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/GostDeer/", "2164_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Horse*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Horse"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Horse/", "3123_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Cube"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Cube/", "2224_model (merge).fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_PowerLizard*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_PowerLizard"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/PowerLizard/", "3125_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Raven*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Raven"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Raven/", "2163_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Lamp*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Lamp"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Lamp/", "2003.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Catfish*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Catfish"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Catfish/", "2087_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Diego*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Diego"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Diego/", "2021_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Boss_Dokkaebi*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Boss_Dokkaebi"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Boss_Dokkaebi/", "3901_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	/* For.Prototype_Component_Model_Boss_Bear*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Boss_Bear"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Boss_Bear/", "3924_model.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(220.0f))*XMMatrixTranslation(-192.25f, -7.5f, -3.f); //그리핀을 위한 행렬 변환

	/* For.Prototype_Component_Model_Boss_Griffin*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_Boss_Griffin"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Monster/Boss_Griffin/", "3915_Skin.fbx", TransformMatrix)))) // 받아올 때 컴객체들과 타입, 경로, 이름과 확장자, 그리고 트랜스폼 행렬을 보내주고 있다.
		return E_FAIL;

#pragma endregion



#pragma endregion



#pragma region PROTOTYPE_GAMEOBJECT



#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_bIsFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevel)
{
	CLoader*	pInstance = new CLoader(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSGBOX("Failed to Creating CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteObject(m_hThread);

	DeleteCriticalSection(&m_CS);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);


}
