#include "stdafx.h"
#include "..\Public\Monster_Boss_Bear.h"
#include "GameInstance.h"
#include "Item_Universal.h"
#include "UI_Universal.h"
#include "Camera_Player.h"
#include "Universal_Point_Instance.h"


CMonster_Boss_Bear::CMonster_Boss_Bear(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CMonster_Boss_Bear::CMonster_Boss_Bear(const CMonster_Boss_Bear & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Boss_Bear::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Boss_Bear::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.0f)))))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		memcpy(&m_tObjectDataDesc, pArg, sizeof(OBJECTDATADESC));

		m_pTransform->Set_WorldFloat4x4(XMLoadFloat4x4(&m_tObjectDataDesc.TransformMatrix));
	}

	//몬스터 정보
	m_Info.fHp = 1500;
	m_Info.fMaxHp = m_Info.fHp;
	m_Info.fShield = 1000;
	m_Info.fMaxShield = m_Info.fShield;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	m_pPlayer = static_cast<CGameObject*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Player")));
	m_pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")));

	m_pModelCom->SetUp_AnimIndex(0);

	RELEASE_INSTANCE(CGameInstance);

	m_pTransform->Scaled(XMVectorSet(1.5f, 1.5f, 1.5f, 0.f));

	SetUp_UI();

	m_pNavigationCom->Find_CellIndex(m_pTransform);


	SetUp_Trail();

	m_pTrail->Set_NotRendering(true);

	return S_OK;
}

_int CMonster_Boss_Bear::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	if (m_Info.fHp <= 0)
	{
		m_iOncePattern = 98;
		m_iInfinityPattern = 98;

		m_bComboSwtich = false;

		m_iCurrentAnim = 7;
		m_iCurrentAnim = 7;

		if (m_dAnimCutTime > 6)
		{
			m_bDissolveOn = true;
			m_fDissolveTime += (_float)TimeDelta * 0.3;
		}
	}

	if (true == m_bPhase_One)
	{
		SetUp_Shield();
	}

	Slow_On();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->GetInstance()->Add_ObseverGroup(MONSTER, this)))
	{
		MSGBOX("CMonster_Boss_Bear의 Add_ObseverGroup에 문제가 발생했습니다");
		return E_FAIL;
	}



	SetUp_Fight(TimeDelta * m_dSlowTime);

	PlayAnim(TimeDelta * m_dSlowTime);



	RELEASE_INSTANCE(CGameInstance);

	return m_iDead;
}

_int CMonster_Boss_Bear::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//__super::SetUp_OnTerrain(m_eNowLevelNum, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	SetUp_OnNavigation(m_pNavigationCom, m_eNowLevelNum, TEXT("Layer_BackGround"), 0);


	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);


	RELEASE_INSTANCE(CGameInstance);

#ifdef _DEBUG	
	m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif // _DEBUG

	return _int();
}

HRESULT CMonster_Boss_Bear::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");

		//m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");

		m_pModelCom->Render(m_pShaderCom, (false == m_bDissolveOn) ? 0 : 1, i, "g_BoneMatrices");

	}

	return S_OK;
}

HRESULT CMonster_Boss_Bear::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Model_Boss_Bear"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve_2"), TEXT("Com_Texture"), (CComponent**)&m_pDissolveTextrueCom)))
		return E_FAIL;

	/* For.Com_Sphere */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 1.f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;


	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 1.2f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.pParent = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_BackGround"), CGameObject::m_pTransformTag));;
	NaviDesc.iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;

}

HRESULT CMonster_Boss_Bear::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pDissolveTextrueCom->SetUp_OnShader(m_pShaderCom, "g_DissolveTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fDissolveTime", &m_fDissolveTime, sizeof(_float))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::SetUp_UI()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUI_Universal::UI_Universal_DESC UI_UniversalDesc;

	UI_UniversalDesc.iUI_Number = CUI_Universal::UI_BEAR_STAMINA;
	UI_UniversalDesc.fSizeX = 800;
	UI_UniversalDesc.fSizeY = 170;
	UI_UniversalDesc.fX = 640;
	UI_UniversalDesc.fY = 100;
	UI_UniversalDesc.fDepth = 11.f;
	UI_UniversalDesc.iTextureIndex = 0;
	UI_UniversalDesc.pGameObject = this;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Stamina"), TEXT("Prototype_GameObject_UI_Universal"), &UI_UniversalDesc)))
		return E_FAIL;

	UI_UniversalDesc.iUI_Number = CUI_Universal::UI_BOSS_HP;
	UI_UniversalDesc.fSizeX = 550;
	UI_UniversalDesc.fSizeY = 20;
	UI_UniversalDesc.fX = 700;
	UI_UniversalDesc.fY = 110;
	UI_UniversalDesc.fDepth = 5.f;
	UI_UniversalDesc.iTextureIndex = 0;
	UI_UniversalDesc.pGameObject = this;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Hp"), TEXT("Prototype_GameObject_UI_Universal"), &UI_UniversalDesc)))
		return E_FAIL;

	UI_UniversalDesc.iUI_Number = CUI_Universal::UI_BOSS_SHIELD;
	UI_UniversalDesc.fSizeX = 550;
	UI_UniversalDesc.fSizeY = 10;
	UI_UniversalDesc.fX = 700;
	UI_UniversalDesc.fY = 90;
	UI_UniversalDesc.fDepth = 5.f;
	UI_UniversalDesc.iTextureIndex = 0;
	UI_UniversalDesc.pGameObject = this;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Shield"), TEXT("Prototype_GameObject_UI_Universal"), &UI_UniversalDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMonster_Boss_Bear::SetUp_Event_Tirgger(_double TimeDelta)
{
	return S_OK;
}

HRESULT CMonster_Boss_Bear::SetUp_Fight(_double TimeDelta)
{
	Fight_On();

	if (true == m_bFight)
	{
		if (false == m_bLookAtOff)
		{
			m_pTransform->LookAtNOTAxisY(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
		}

		if (m_fDistance < 5)
		{
			m_pTransform->Go_Backward(TimeDelta * 3, m_pNavigationCom);
		}


		CoolTime_Manager(TimeDelta);

	}

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Fight_On()
{

	_vector vDis = m_pTransform->Get_Distance(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	XMStoreFloat(&m_fDistance, vDis);

	if (false == m_bOneTimeSwith)
	{
		m_bFight = true;
	}


	return S_OK;
}


HRESULT CMonster_Boss_Bear::PlayAnim(_double TimeDelta)
{

	if (false == m_bOneTimeSwith && false == m_bComboSwtich) //이건 블렌딩되어 있으면서 한번만 실행시키는 변수 tmi)웨폰에 있는 애니메이션 제어는 블렌딩하지 않음
	{
		Infinity_AnimMotion(TimeDelta);
		m_pModelCom->SetUp_AnimIndex(m_iInfinityAnimNumber); //만약 특정 변수로 계속 반복적으로 움직임
	}
	else if (true == m_bComboSwtich)
	{
		Combo_AnimMotion(TimeDelta);
		if (true == m_bOneTimeSwith)
		{
			m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_iCurrentAnim, m_iNextAnim, m_bOneTimeSwith);
		}
		else if (false == m_bOneTimeSwith) {
			Infinity_AnimMotion(TimeDelta);
			m_pModelCom->SetUp_AnimIndex(m_iInfinityAnimNumber);
		}
	}
	else {
		Once_AnimMotion(TimeDelta); //이거 어택이랑 무빙으로 되어 있는거 한번만 실행과 무한실행으로 바꾸고 그 함수 여기에 박으면 될 듯?
		m_pModelCom->SetUp_AnimIndex(m_iOnceAnimNumber); //한번만 돌릴려면 일단 m_bOneTimeSwith를 true로 바꿔줘야함
	}

	if (false == m_bAnimCutOn && false == m_bComboSwtich)
	{
		m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_bOneTimeSwith);
	}
	else if(true == m_bAnimCutOn && false == m_bComboSwtich) {
		Infinity_AnimMotion(TimeDelta);
		m_pModelCom->SetUp_AnimIndex(m_iInfinityAnimNumber);
		m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_bOneTimeSwith);
	}

	return S_OK;
}

HRESULT CMonster_Boss_Bear::CoolTime_Manager(_double TimeDelta)
{
	m_dOnceCoolTime += TimeDelta;

	if (true == m_bPhase_One)
	{
		if (m_dOnceCoolTime > 3 && m_fDistance < 20 && false == m_bComboSwtich )
		{

			m_bOneTimeSwith = true;
			m_dOnceCoolTime = 0;
			m_dInfinityCoolTime = 0;

			m_bFire = true;
			m_dFireTime = 0;

			m_iOncePattern += 1;

			if (m_iOncePattern > 19) //지금 현재 보스는 랜덤이 아닌 순서대로 출력 시켜야함 그래서 m_iOncePattern의 최대값이 되면 0으로 초기화 시켜주는중
			{						//또한 현재 m_iOncePattern는 99부터 시작하니 0으로 만들어주는중
				m_iOncePattern = 0;
			}
		}
		else if(m_fDistance >= 20){
			m_iInfinityPattern = 4;

			m_dOnceCoolTime = 0;
		}
	}
	else {
		if (m_dOnceCoolTime > 2 && m_fDistance < 20 && false == m_bComboSwtich) //여기서 부터 페이즈2
		{

			m_bOneTimeSwith = true;
			m_dOnceCoolTime = 0;
			m_dInfinityCoolTime = 0;

			m_bFire = true;
			m_dFireTime = 0;

			m_iOncePattern += 1;

			if (m_iOncePattern > 45) 
			{					
				m_iOncePattern = 20;
			}
		}
		else if (m_fDistance >= 20) {
			m_iInfinityPattern = 4;

			m_dOnceCoolTime = 0;
		}
	}

	m_dInfinityCoolTime += TimeDelta;
	if (m_dInfinityCoolTime > 2)
	{
		m_iInfinityPattern = rand() % 6;
		m_dInfinityCoolTime = 0;
		m_bLookAtOff = false;

	}

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Once_AnimMotion(_double TimeDelta) //무빙넘버와 공격넘버로 바꿔도 괜찮을듯?
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	switch (m_iOncePattern)
	{
	case 0:
	{
		m_iOnceAnimNumber = 10; //플레이어를 향해 점프
		Jumping(TimeDelta);
		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 10;
		m_iNextAnim = 31;
		m_pTrail->Set_NotRendering(false);
		break;
	}
	case 1:
	{
		m_iCurrentAnim = 26;
		m_iNextAnim = 1;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_dFireTime += TimeDelta;

		if (m_dFireTime > 1)
		{
			m_bLookAtOff = false;
		}

		m_pTrail->Set_NotRendering(true);

		break;
	}
	case 2:
	{
		m_pTransform->Go_Straight(TimeDelta*2.5);
		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 2;
		m_iNextAnim = 2;
		break;
	}
	case 3:
	{
		m_pTransform->Go_Straight(TimeDelta*2.5);
		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 2;
		m_iNextAnim = 2;
		break;
	}
	case 4:
	{

		m_iOnceAnimNumber = 8; //발도 시작
		Universal_Sword(TimeDelta);

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 8;
		m_iNextAnim = 36;
		
		m_pTrail->Set_NotRendering(false);
		break;
	}
	case 5:
	{
		m_iOnceAnimNumber = 36; //발도 끝
		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 36;
		m_iNextAnim = 2;

		m_pTrail->Set_NotRendering(true);
		break;
	}
	case 6:
	{
		m_pTransform->Go_Straight(TimeDelta * 2.5);
		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 2;
		m_iNextAnim = 2;
		break;
	}
	case 7:
	{
		m_pTransform->Go_Straight(TimeDelta * 2.5);
		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 2;
		m_iNextAnim = 2;
		break;
	}
	case 8:
	{
		m_iOnceAnimNumber = 11; //돌려치기 시작
		Universal_Turn(TimeDelta);

		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 11;
		m_iNextAnim = 24;

		m_pTrail->Set_NotRendering(false);
		break;
	}
	case 9:
	{
		m_iOnceAnimNumber = 24; //돌려치기 끝
		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 24;
		m_iNextAnim = 2;

		m_pTrail->Set_NotRendering(true);
		break;
	}
	case 10:
	{
		m_pTransform->Go_Straight(TimeDelta * 2.5);
		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 2;
		m_iNextAnim = 2;
		break;
	}
	case 11:
	{
		m_pTransform->Go_Straight(TimeDelta * 2.5);
		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 2;
		m_iNextAnim = 2;
		break;
	}
	case 12:
	{
		m_iOnceAnimNumber = 7; //내려찍기 시작
		Universal_StrikeDown(TimeDelta);

		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 7;
		m_iNextAnim = 25;

		m_pTrail->Set_NotRendering(false);
		break;
	}
	case 13:
	{
		m_iOnceAnimNumber = 25; //내려찍기 끝
		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 25;
		m_iNextAnim = 9;

		m_pTrail->Set_NotRendering(true);
		break;
	}
	case 14:
	{
		m_pTransform->Go_Straight(TimeDelta * 2.5);
		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 2;
		m_iNextAnim = 2;
		break;
	}
	case 15:
	{
		m_pTransform->Go_Straight(TimeDelta * 2.5);
		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 2;
		m_iNextAnim = 2;
		break;
	}
	case 16:
	{
		m_iOnceAnimNumber = 9; //3연속 첫번째 베기

		m_dFireTime += TimeDelta;

		if (m_dFireTime < 0.4)
		{
			m_pTransform->Go_Straight(TimeDelta * 3, m_pNavigationCom);
		}


		if (true == m_bFire && m_dFireTime > 0.6)
		{
			Universal_TripleAttack(TimeDelta);
		}

		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 9;
		m_iNextAnim = 41;

		if (false == m_bSoundSwitch && m_dFireTime > 0.2)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_TripleAttack1.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = true;
		}

		m_pTrail->Set_NotRendering(false);
		break;
	}
	case 17:
	{
		m_iOnceAnimNumber = 41; //3연속 두번째 베기
		
		m_dFireTime += TimeDelta;

		if (m_dFireTime < 0.4)
		{
			m_pTransform->Go_Straight(TimeDelta * 3, m_pNavigationCom);
		}

		if (true == m_bFire && m_dFireTime > 0.7)
		{
			Universal_TripleAttack(TimeDelta);
		}

		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 41;
		m_iNextAnim = 11;

		if (true == m_bSoundSwitch && m_dFireTime > 0.2)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_TripleAttack2.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}
		break;
	}
	case 18:
	{
		m_iOnceAnimNumber = 10; //3연속 세번째 베기

		m_dFireTime += TimeDelta;

		if (m_dFireTime < 0.4)
		{
			m_pTransform->Go_Straight(TimeDelta * 3, m_pNavigationCom);
		}


		if (true == m_bFire && m_dFireTime > 0.7)
		{
			Universal_TripleAttack(TimeDelta);

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(3, 0.5f);
			RELEASE_INSTANCE(CGameInstance);
		}

		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 10;
		m_iNextAnim = 26;

		if (false == m_bSoundSwitch && m_dFireTime > 0.1)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_Bear_TripleAttack3.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = true;
		}
		break;
	}
	case 19:
	{
		m_iOnceAnimNumber = 26; //3연속 공격이 멈춤
		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 26;
		m_iNextAnim = 2;

		m_pTrail->Set_NotRendering(true);

		break;
	}
	case 20:
	{
		m_iOnceAnimNumber = 10; //플레이어를 향해 점프 메쉬 구해서 바꿀 예정
		Jumping_PhaseTwo(TimeDelta);
		m_bOneTimeSwith = true;

		//m_dAnimCutTime += TimeDelta;
		//if (m_dAnimCutTime > 1.6)
		//{
		//	m_bAnimCutOn = true;
		//	m_bFight = true;
		//	m_bOneTimeSwith = false;

		//	m_dAnimCutTime = 0;
		//	m_bAnimCutOn = false;
		//}

		m_bComboSwtich = true;

		m_iCurrentAnim = 10;
		m_iNextAnim = 31;

		m_pTrail->Set_NotRendering(false);

		break;
	}
	case 21: //점프 공격 종료
	{
		m_iCurrentAnim = 26;
		m_iNextAnim = 1;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_dFireTime += TimeDelta;
		
		if (m_dFireTime > 1)
		{
			m_bLookAtOff = false;
		}

		m_pTrail->Set_NotRendering(true);

		break;
	}
	case 22:
	{
		m_iOnceAnimNumber = 31; //러쉬 준비
		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 31;
		m_iNextAnim = 32;

		m_pTrail->Set_NotRendering(false);

		m_dFireTime += TimeDelta;

		if (true == m_bSoundSwitch && m_dFireTime < 1.5)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_Dash.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}

		break;
	}
	case 23:
	{
		m_iOnceAnimNumber = 32; //러쉬

		if (m_fDistance > 8)
		{
			m_pTransform->Go_Straight(TimeDelta * 5, m_pNavigationCom);
		}
		else {
			m_pTransform->Go_Straight(TimeDelta * 3, m_pNavigationCom);
		}

		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 32;
		m_iNextAnim = 35;
		break;
	}
	case 24://횡베기
	{
		m_iOnceAnimNumber = 33;
		m_bOneTimeSwith = true;

		Rush_Attack_One(TimeDelta);

		m_bComboSwtich = true;

		m_iCurrentAnim = 33;
		m_iNextAnim = 23;

		break;
	}
	case 25://횡베기 끝
	{
		m_iOnceAnimNumber = 23;
		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 23;
		m_iNextAnim = 27;

		m_pTrail->Set_NotRendering(true);
		break;
	}
	case 26: //충전 공격 준비
	{
		Tornado_Shield(TimeDelta);

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 27;
		m_iNextAnim = 28;
		break;
	}
	case 27://충전 공격 충전중
	{
		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 28;
		m_iNextAnim = 19;
		break;
	}
	case 28://충전 공격중 //공격 모션 ㄱㄱ
	{
		Tornado_Bullet(TimeDelta);

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 19;
		m_iNextAnim = 20;

		m_pTrail->Set_NotRendering(false);
		break;
	}
	case 29://충전 공격 후 딜레이 //이거 빼더라도 자연스러울듯?
	{
		m_bOneTimeSwith = true;

		m_bComboSwtich = true;

		m_iCurrentAnim = 20;
		m_iNextAnim = 22;

		m_pTrail->Set_NotRendering(true);
		break;
	}
	case 30://충전 공격 끝
	{
		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_iCurrentAnim = 22;
		m_iNextAnim = 32;

		GetSingle(CGameInstance)->Stop_ChannelSound(CHANNEL_OBJECT);
		break;
	}
	case 31://러쉬 준비
	{
		m_iCurrentAnim = 31;
		m_iNextAnim = 32;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_pTrail->Set_NotRendering(false);

		if (true == m_bSoundSwitch && m_dFireTime < 0.5)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_Dash.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}

		break;
	}
	case 32: //러쉬 중
	{
		m_iCurrentAnim = 32;
		m_iNextAnim = 34;

		if (m_fDistance > 8)
		{
			m_pTransform->Go_Straight(TimeDelta * 5, m_pNavigationCom);
		}
		else {
			m_pTransform->Go_Straight(TimeDelta * 3, m_pNavigationCom);
		}

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;
		break;
	}
	case 33:  //올려 베기
	{
		m_bOneTimeSwith = true;

		Rush_Attack_Two(TimeDelta);

		m_iCurrentAnim = 34;
		m_iNextAnim = 37;

		m_bComboSwtich = true;
		break;
	}
	case 34: //올려 베기 종료
	{
		m_iCurrentAnim = 37;
		m_iNextAnim = 15;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_pTrail->Set_NotRendering(true);
		break;
	}
	case 35: //불렛 공격 준비
	{
		Sphere_Potal(TimeDelta);
		m_iCurrentAnim = 15;
		m_iNextAnim = 16;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;
		break;
	}
	case 36: //불렛 끌어모으는 중
	{

		m_iCurrentAnim = 16;
		m_iNextAnim = 17;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_dFireTime += TimeDelta;

		if (true == m_bSoundSwitch && m_dFireTime > 1)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_Sphere_Potal2.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}

		break;
	}
	case 37://불렛 발사
	{
		m_iCurrentAnim = 17;
		m_iNextAnim = 1;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;
		break;
	}
	case 38://러쉬 준비
	{
		m_iCurrentAnim = 31;
		m_iNextAnim = 32;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_pTrail->Set_NotRendering(false);

		if (false == m_bSoundSwitch && m_dFireTime < 0.5)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_Dash.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = true;
		}
		break;
	}
	case 39: //러쉬 중
	{
		m_iCurrentAnim = 32;
		m_iNextAnim = 35;

		if (m_fDistance > 8)
		{
			m_pTransform->Go_Straight(TimeDelta * 5, m_pNavigationCom);
		}
		else {
			m_pTransform->Go_Straight(TimeDelta * 3, m_pNavigationCom);
		}

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;
		break;
	}
	case 40:  //종 베기
	{
		Rush_Attack_Three(TimeDelta);

		m_iCurrentAnim = 35;
		m_iNextAnim = 39;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		break;
	}
	case 41: //종 베기 종료
	{
		m_iCurrentAnim = 39;
		m_iNextAnim = 1;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_pTrail->Set_NotRendering(true);
		break;
	}
	case 42: //첫번째 트리플 어택
	{
		m_dFireTime += TimeDelta;

		if (m_dFireTime < 0.4)
		{
			if (m_fDistance > 8)
			{
				m_pTransform->Go_Straight(TimeDelta * 10, m_pNavigationCom);
			}
			else {
				m_pTransform->Go_Straight(TimeDelta, m_pNavigationCom);
			}
		}

		if (true == m_bFire && m_dFireTime > 0.6)
		{
			Universal_TripleAttack(TimeDelta);
		}

		m_iCurrentAnim = 9;
		m_iNextAnim = 41;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_pTrail->Set_NotRendering(false);

		if (true == m_bSoundSwitch && m_dFireTime < 0.5)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_TripleAttack1.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}

		break;
	}
	case 43: //두번째 트리플 어택
	{
		m_dFireTime += TimeDelta;

		if (m_dFireTime < 0.4)
		{
			if (m_fDistance > 8)
			{
				m_pTransform->Go_Straight(TimeDelta * 10, m_pNavigationCom);
			}
			else {
				m_pTransform->Go_Straight(TimeDelta, m_pNavigationCom);
			}
		}

		if (true == m_bFire && m_dFireTime > 0.7)
		{
			Universal_TripleAttack(TimeDelta);
		}

		m_iCurrentAnim = 41;
		m_iNextAnim = 10;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		if (false == m_bSoundSwitch && m_dFireTime < 0.5)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_TripleAttack2.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = true;
		}
		break;
	}
	case 44: //세번째 트리플 어택
	{
		m_dFireTime += TimeDelta;

		if (m_dFireTime < 0.4)
		{
			m_pTransform->Go_Straight(TimeDelta * 3, m_pNavigationCom);
		}

		TripleAttack_SwordWave(TimeDelta);

		m_iCurrentAnim = 10;
		m_iNextAnim = 26;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		if (true == m_bSoundSwitch && m_dFireTime < 0.5)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_TripleAttack3.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}

		break;
	}
	case 45: //트리플 어택 종료
	{
		m_iCurrentAnim = 26;
		m_iNextAnim = 1;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_pTrail->Set_NotRendering(true);
		break;
	}
	case 95:
	{
		m_iCurrentAnim = 18;
		m_iNextAnim = 21;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;
		break;
	}
	case 96:
	{
		if (false == m_bDevilOn)
		{
			Devil_Summons(TimeDelta);
			Devil_Summons_Particle();
			m_bDevilOn = true;


		}
		//Mace_Summons(TimeDelta);

		m_iCurrentAnim = 21;
		m_iNextAnim = 21;

		m_bOneTimeSwith = true;
		m_bComboSwtich = true;

		m_bLookAtOff = true;

		m_pTrail->Set_NotRendering(true);

		m_dSoundTime += TimeDelta;
		if (m_dSoundTime >= 2)
		{
			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
			GetSingle(CGameInstance)->PlaySound((L"Bear_Sword_Shake.mp3"), CHANNEL_OBJECT);

			m_dSoundTime = 0;
		}

		break;
	}
	case 97:
	{
		Red_Sphere(TimeDelta);
		//Phase_Two(TimeDelta);

		m_iOnceAnimNumber = 15; //페이즈2 준비 모션
		m_bOneTimeSwith = true;
		m_dOnceCoolTime = 0;
		m_dInfinityCoolTime = 0;
		m_bLookAtOff = false;


		m_bComboSwtich = false;

		break;
	}
	case 98:
	{
		m_iOnceAnimNumber = 6;
		m_bOneTimeSwith = true;
		m_dOnceCoolTime = 0;
		m_dInfinityCoolTime = 0;

		m_bLookAtOff = false;

		m_dFireTime += TimeDelta;
		if (true == m_bSoundSwitch && m_dFireTime > 0 && m_dFireTime < 6.5)
		{
			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
			GetSingle(CGameInstance)->PlaySound((L"Bear_KnowDown.mp3"), CHANNEL_OBJECT);
			m_bSoundSwitch = false;
		}

		if (false == m_bSoundSwitch && m_dFireTime > 7 && m_dFireTime < 9.5)
		{
			GetSingle(CGameInstance)->Stop_AllChannel();

			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
			GetSingle(CGameInstance)->PlaySound((L"Bear_Dead.mp3"), CHANNEL_OBJECT);


			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 0.2f);
			GetSingle(CGameInstance)->PlayBGM((L"Bear_Dead.mp3"), CHANNEL_OBJECT);
			m_bSoundSwitch = true;
		}


		m_dAnimCutTime += TimeDelta;
		if (m_dAnimCutTime > 9)
		{
			m_iDead = OBJ_DEAD;
		}

		break;
	}
	case 99:
	{
		Appear(TimeDelta);
		m_iOnceAnimNumber = 12;
		m_bOneTimeSwith = true;
		m_dOnceCoolTime = 0;
		m_dInfinityCoolTime = 0;

		m_bComboSwtich = false;


		//m_dAnimCutTime += TimeDelta;
		//if (m_dAnimCutTime > 7.0)
		//{
		//	m_bAnimCutOn = true;
		//	m_bFight = true;
		//	m_bOneTimeSwith = false;

		//	m_dAnimCutTime = 0;
		//	m_bAnimCutOn = false;
		//}

		break;
	}
	default:
		break;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Infinity_AnimMotion(_double TimeDelta)
{
	if (true == m_bFight)
	{
		switch (m_iInfinityPattern)
		{
		case 0: //앞으로 움직임
			m_iInfinityAnimNumber = 2;
			m_pTransform->Go_Straight(TimeDelta, m_pNavigationCom);
			break;
		case 1: //왼쪽으로 움직임
			if (true == m_bPhase_One)
			{
				m_iInfinityAnimNumber = 4;
				m_pTransform->Go_Left(TimeDelta, m_pNavigationCom);
			}
			else {
				m_iInfinityAnimNumber = 2;
				m_pTransform->Go_Straight(TimeDelta * 2, m_pNavigationCom);
			}
			break;
		case 2: //오른쪽으로 움직임
			if (true == m_bPhase_One)
			{
				m_iInfinityAnimNumber = 5;
				m_pTransform->Go_Right(TimeDelta, m_pNavigationCom);
			}
			else {
				m_iInfinityAnimNumber = 2;
				m_pTransform->Go_Straight(TimeDelta * 2, m_pNavigationCom);
			}
			break;
		case 3: //앞으로 뛰어감
			m_iInfinityAnimNumber = 2;
			m_pTransform->Go_Straight(TimeDelta*2, m_pNavigationCom);
			break;
		case 4: //앞으로 뛰어감
			m_iInfinityAnimNumber = 2;
			m_pTransform->Go_Straight(TimeDelta*2, m_pNavigationCom);
			break;
		case 5: //앞으로 뛰어감
			m_iInfinityAnimNumber = 2;
			m_pTransform->Go_Straight(TimeDelta*2, m_pNavigationCom);
			break;
		case 98://사망씬 m_iOncePattern에도 존재한다.
			m_iInfinityAnimNumber = 6;
			m_dOnceCoolTime = 0;
			m_dInfinityCoolTime = 0;

			m_dFireTime += TimeDelta;
			if (true == m_bSoundSwitch && m_dFireTime > 0 && m_dFireTime < 6.5)
			{
				//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
				GetSingle(CGameInstance)->PlaySound((L"Bear_KnowDown.mp3"), CHANNEL_OBJECT);
				m_bSoundSwitch = false;
			}

			if (false == m_bSoundSwitch && m_dFireTime > 7 && m_dFireTime < 9.5)
			{
				GetSingle(CGameInstance)->Stop_AllChannel();

				//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
				GetSingle(CGameInstance)->PlaySound((L"Bear_Dead.mp3"), CHANNEL_OBJECT);


				//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 0.2f);
				GetSingle(CGameInstance)->PlayBGM((L"Bear_Dead.mp3"), CHANNEL_OBJECT);
				m_bSoundSwitch = true;
			}



			m_dAnimCutTime += TimeDelta;
			if (m_dAnimCutTime > 9)
			{
				m_iDead = OBJ_DEAD;
			}

			break;
		default:
			break;
		}
	}
	return S_OK;
}
HRESULT CMonster_Boss_Bear::Combo_AnimMotion(_double TimeDelta)
{
	m_dOnceCoolTime = 0;
	m_dInfinityCoolTime = 0;

	if (false == m_bOneTimeSwith)
	{
		if (true == m_bPhase_One)
		{
			m_dFireTime = 0;
			m_bFire = true;

			if (true == m_bComboEnd)
			{
				m_bComboEnd = false;
				m_bComboSwtich = false;

				m_iInfinityPattern = 4;

				return S_OK;
			}
			else {
				m_iOncePattern += 1;
			}

			if (m_iOncePattern > 19)
			{
				m_iOncePattern = 0;
			}
		}
		else if(false == m_bPhase_One)
		{
			m_dFireTime = 0;
			m_bFire = true;


			m_iInfinityPattern = 4;

			m_iOncePattern += 1;

			if (m_iOncePattern > 45)
			{
				m_iOncePattern = 20;

				m_bComboSwtich = false;
			}

			if (m_Info.fHp >= 1 && m_Info.fHp <= 300 && m_iFinalAttack == 0)
			{
				m_iOncePattern = 95;
				m_iFinalAttack += 1;
			}
			else if (m_Info.fHp >= 1 && m_Info.fHp <= 300 && m_iFinalAttack == 1)
			{
				m_iOncePattern = 96;
			}

		}

	}

	Once_AnimMotion(TimeDelta);

	return S_OK;
}
_int CMonster_Boss_Bear::Obsever_Hit(void* pArg)
{


	return _int();
}

_int CMonster_Boss_Bear::Obsever_Event(void * pArg)
{
	CUniversal_Bullet* SorceBullet = static_cast<CUniversal_Bullet*>(pArg);

	if (nullptr != SorceBullet)
	{
		switch (SorceBullet->m_Universal_BulletDesc.iBulletNumber)
		{
		case CUniversal_Bullet::SLOWTIME_SKILL:
		{
			m_pTempSlow_Bullet = SorceBullet;
			m_dSlowTime = 0.1;
			break;
		}
		default:
			break;
		}
	}

	return _int();
}

_int CMonster_Boss_Bear::Dead_Trigger()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CItem_Universal::ITEM_UNIVERSALDESC Item_UniversalDesc;
	//스킬들
	Item_UniversalDesc.vScale = _float3(1.f, 1.f, 1.f);
	Item_UniversalDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);

	_float4 fTempPos;

	XMStoreFloat4(&fTempPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	fTempPos.y += 0.5f;
	Item_UniversalDesc.vTranslation = fTempPos;

	for (_int i = 0; i < 5; i++)
	{
		_int Rand = rand() % 4;
		Item_UniversalDesc.iItemNumber = Rand;
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Item_Universal"), &Item_UniversalDesc)))
			return E_FAIL;
	}



	RELEASE_INSTANCE(CGameInstance);

	Victory();

	Npc();

	m_pTrail->Stop_ComputeTrail();

	return _int();
}

HRESULT CMonster_Boss_Bear::Appear(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::APPEAR;
		Universal_BulletMeshDesc.eType = MONSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 20.f;
		Universal_BulletMeshDesc.vScale = _float3(8.f, 8.f, 8.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 5;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.8f, 0.7f, 1.f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMonster_Boss_Bear::Jumping(_double TimeDelta)
{
	if (m_dFireTime>0.2 && m_dFireTime < 0.8)
	{
		m_pTransform->Go_Straight(TimeDelta*7.0, m_pNavigationCom);
		m_bLookAtOff = true;

		_vector	vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

		_float4	vTempPos;
		XMStoreFloat4(&vTempPos, vPosition);

		vTempPos.y += 1.5f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vTempPos));

		if (true == m_bSoundSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_Jumping.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.8)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::BEAR_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(6.5f, 6.5f, 6.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 2.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;


		//==============================균열 메쉬
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::CRACK;
		Universal_BulletMeshDesc.eType = MONSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, -2.f, 2.5f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.7f, 0.7f, 0.7f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(7, 1.f);
		RELEASE_INSTANCE(CGameInstance);

		if (false == m_bSoundSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_Landing.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = true;
		}
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Universal_Sword(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.8)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::BEAR_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(6.5f, 6.5f, 6.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 2.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;
	}

	if (true == m_bSoundSwitch && m_dFireTime > 0.5)
	{
		GetSingle(CGameInstance)->PlaySound((L"Bear_Sword.mp3"), CHANNEL_OBJECT);

		m_bSoundSwitch = false;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Universal_Turn(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 1)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::BEAR_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(6.5f, 6.5f, 6.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 2.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;
	}

	if (false == m_bSoundSwitch && m_dFireTime > 0.5)
	{
		GetSingle(CGameInstance)->PlaySound((L"Bear_Turn.mp3"), CHANNEL_OBJECT);

		m_bSoundSwitch = true;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Phase_Two(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (false == m_bSkyBoxOn && m_dFireTime > 1.5)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::PHASE_TWO;
		Universal_BulletMeshDesc.eType = BULLET;

		Universal_BulletMeshDesc.fSpeedPerSec = 20.f;
		Universal_BulletMeshDesc.vScale = _float3(1.f, 1.f, 1.f);
		//Universal_BulletMeshDesc.vScale = _float3(0.004f, 0.004f, 0.004f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 1000000;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 50.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(4.f, 4.f, 4.f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(3.5f, 3.5f, 3.5f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Phase_Two"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bSkyBoxOn = true;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Red_Sphere(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::REDSPHERE;
		Universal_BulletMeshDesc.eType = MONSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(1.f, 1.f, 1.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 5;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 2.5f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(1.f, 1.f, 1.f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;
	}

	if (false == m_bSoundSwitch && m_dFireTime > 1.5)
	{
		GetSingle(CGameInstance)->PlaySound((L"Bear_Red_Eyes.mp3"), CHANNEL_OBJECT);

		m_bSoundSwitch = true;
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMonster_Boss_Bear::EyeOfSauron(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::SAURON;
		Universal_BulletMeshDesc.eType = MONSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(10.f, 10.f, 10.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 5;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 0.f, -40.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(0.2f, 0.2f, 0.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.14f, 0.14f, 0.14f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMonster_Boss_Bear::Devil_Summons(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::DEVIL;
		Universal_BulletMeshDesc.eType = MONSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(0.007f, 0.007f, 0.007f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 100000;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, -35.f, -8.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(0.1f, 0.1f, 0.1f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.07f, 0.07f, 0.07f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Mace_Summons(_double TimeDelta)
{
	//여기 말고 유니버설로 가자
	if (true == m_bMace_BulletOn)
	{
		//철퇴
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::MACE_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(1.f, 1.f, 1.f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bMace_BulletOn = false;

		RELEASE_INSTANCE(CGameInstance);
	}

	if (false == m_bMace_BulletOn)
	{
		m_dMaceSummons_Time += TimeDelta;

		if (m_dMaceSummons_Time > 7)
		{
			m_dMaceSummons_Time = 0;
			m_bMace_BulletOn = true;
		}
	}

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Jumping_PhaseTwo(_double TimeDelta)
{
	if (m_dFireTime>0.2 && m_dFireTime < 0.8)
	{
		m_pTransform->Go_Straight(TimeDelta*7.0, m_pNavigationCom);
		m_bLookAtOff = true;

		_vector	vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

		_float4	vTempPos;
		XMStoreFloat4(&vTempPos, vPosition);

		vTempPos.y += 1.5f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vTempPos));

		if (true == m_bSoundSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_Jumping.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.8)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::BEAR_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(6.5f, 6.5f, 6.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 2.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;


		//==============================균열 메쉬
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::CRACK;
		Universal_BulletMeshDesc.eType = MONSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, -2.f, 2.5f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.7f, 0.7f, 0.7f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(7, 1.f);
		RELEASE_INSTANCE(CGameInstance);


		if (false == m_bSoundSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_Landing.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = true;
		}
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Tornado_Shield(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.3)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::TORNADO_1;
		Universal_BulletMeshDesc.eType = MONSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(1.f, 1.f, 1.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 1.8;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(0.2f, 0.2f, 0.20f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.14f, 0.14f, 0.14f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;

		GetSingle(CGameInstance)->PlaySound((L"Bear_Tornado_Shield.mp3"), CHANNEL_OBJECT);
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Tornado_Bullet(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.4)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::TORNADO_2;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 20.f;
		Universal_BulletMeshDesc.vScale = _float3(15.f, 15.f, 15.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 0.f, -5.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.8f, 0.7f, 1.f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;

		GetSingle(CGameInstance)->PlaySound((L"Bear_TornadoShot.mp3"), CHANNEL_OBJECT);

		GetSingle(CGameInstance)->PlaySound((L"Bear_Tornado_Bullet.mp3"), CHANNEL_OBJECT);
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Sphere_Potal(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.3)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::SPHERE;
		Universal_BulletMeshDesc.eType = MONSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(10.f, 10.f, 10.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 5;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 0.f, -40.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(0.2f, 0.2f, 0.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.14f, 0.14f, 0.14f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;


		GetSingle(CGameInstance)->PlaySound((L"Bear_Sphere_Potal.mp3"), CHANNEL_OBJECT);


	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CMonster_Boss_Bear::Rush_Attack_One(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.7)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::BEAR_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(6.5f, 6.5f, 6.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 2.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::LATERAL_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 30.f;
		Universal_BulletMeshDesc.vScale = _float3(1.f, 2.f, 1.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 1.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.7f, 1.7f, 1.7f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;

	}

	if (false == m_bSoundSwitch && m_dFireTime < 0.5)
	{
		GetSingle(CGameInstance)->PlaySound((L"Bear_Rush_Attack_1.mp3"), CHANNEL_OBJECT);

		m_bSoundSwitch = true;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Rush_Attack_Two(_double TimeDelta)
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.7)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::BEAR_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(6.5f, 6.5f, 6.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 2.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::UPPER_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(1.f, 2.f, 1.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 5.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 2.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.7f, 1.7f, 1.7f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;


	}
	if (false == m_bSoundSwitch && m_dFireTime > 0.8)
	{
		GetSingle(CGameInstance)->PlaySound((L"Bear_Rush_Attack_2.mp3"), CHANNEL_OBJECT);

		m_bSoundSwitch = true;
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMonster_Boss_Bear::Rush_Attack_Three(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.8)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::VERTICAL_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(1.f, 2.f, 1.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.8f, 1.8f, 1.8f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(1.8f, 1.8f, 1.8f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::BEAR_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(6.5f, 6.5f, 6.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 2.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;

		GetSingle(CGameInstance)->PlaySound((L"Bear_Rush_Attack_3.mp3"), CHANNEL_OBJECT);


		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(3, 0.5f);
		RELEASE_INSTANCE(CGameInstance);
	}


	RELEASE_INSTANCE(CGameInstance);

	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//m_dFireTime += TimeDelta;
	//if (true == m_bFire && m_dFireTime > 0.4)
	//{
	//	CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
	//	ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

	//	Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::VERTICAL_BULLET;
	//	Universal_BulletMeshDesc.eType = BULLET_MOSTER;

	//	Universal_BulletMeshDesc.fSpeedPerSec = 20.f;
	//	Universal_BulletMeshDesc.vScale = _float3(8.f, 8.f, 8.f);
	//	Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	//	Universal_BulletMeshDesc.dDuration = 10;

	//	Universal_BulletMeshDesc.pObject = this;
	//	Universal_BulletMeshDesc.fPositioning = _float3(0.f, 0.f, 0.f);


	//	Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
	//	Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.8f, 0.7f, 1.f);

	//	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
	//		return E_FAIL;

	//	m_bFire = false;
	//}


	//RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::TripleAttack_SwordWave(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (true == m_bFire && m_dFireTime > 0.7)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::SWORDWAVE_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 20.f;
		Universal_BulletMeshDesc.vScale = _float3(1.f, 1.f, 1.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(4.f, 4.f, 4.f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(3.5f, 3.5f, 3.5f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(7, 1.f);
		RELEASE_INSTANCE(CGameInstance);
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CMonster_Boss_Bear::Universal_StrikeDown(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.5)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::BEAR_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(6.5f, 6.5f, 6.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 2.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(3, 0.5f);
		RELEASE_INSTANCE(CGameInstance);
	}


	RELEASE_INSTANCE(CGameInstance);

	if (true == m_bSoundSwitch && m_dFireTime > 0.2)
	{
		GetSingle(CGameInstance)->PlaySound((L"Bear_StrikeDown.mp3"), CHANNEL_OBJECT);

		m_bSoundSwitch = false;
	}

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Universal_TripleAttack(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
	ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

	Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::BEAR_ATTACK;
	Universal_BulletDesc.eType = BULLET_MOSTER;

	Universal_BulletDesc.fSpeedPerSec = 0.f;
	Universal_BulletDesc.vScale = _float3(6.5f, 6.5f, 6.5f);
	Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_BulletDesc.dDuration = 0.1;

	Universal_BulletDesc.pObject = this;
	Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 2.f);


	Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
	Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

	Universal_BulletDesc.iTextureIndex = 0;

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
		return E_FAIL;

	m_bFire = false;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::SetUp_Trail()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTrail::TRAILDESC TrailDesc;
	TrailDesc.iTrailTextureIndex = CTrail::UNIVERSAL_TRAIL;
	//TrailDesc.pTransformMatrix = &m_TransformMatrix;

	TrailDesc.pModel = m_pModelCom;
	TrailDesc.pSocketName = "3924_weapon01";
	TrailDesc.pTransform = m_pTransform;
	TrailDesc.pObject = this;


	//TrailDesc.vHighPos = _float3(0.f, 0.f, 1.05f);
	//TrailDesc.vLowPos = _float3(0.0f, 0.f, 0.15f);
	//TrailDesc.vColor = _float4(138.f / 255.f, 3.f / 255.f, 3.f / 255.f, 1.f);


	TrailDesc.vHighPos = _float3(0.f, 1.2f, -0.2f);
	TrailDesc.vLowPos = _float3(0.0f, -0.8f, 0.1f);

	TrailDesc.vColor = _float4(1.f / 255.f, 255.f / 175.f, 255.f / 255.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Trail_Bear"), TEXT("Prototype_GameObject_Trail"), &TrailDesc)))
		return E_FAIL;

	m_pTrail = static_cast<CTrail*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Trail_Bear"))); //이거 레이어 이름 다 다르게 해야함 ㅋㅋㅋ

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Devil_Summons_Particle()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC Universal_PointDesc;
	ZeroMemory(&Universal_PointDesc, sizeof(CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC));

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::BLACK_ROCK_PARTICLE_0;
	Universal_PointDesc.iParticleType = CVIBuffer_Point_Instance::PARTICLE_UP;
	Universal_PointDesc.fMinSpeed = 4.f;
	Universal_PointDesc.fMaxSpeed = 15.f;
	Universal_PointDesc.fRange = _float3(10.f, 1.f, 10.f);
	Universal_PointDesc.fSize = 2.f;
	Universal_PointDesc.pObject = this;
	Universal_PointDesc.dDuration = 99999;
	Universal_PointDesc.iTextureIndex = 0;
	Universal_PointDesc.fPositioning = _float3(-5.f, 0.f, -5.f);

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::WHITE_ROCK_PARTICLE_0;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::SetUp_Shield()
{
	if (m_Info.fShield > 0.1)
	{
		_int Damage = m_Info.fMaxHp - m_Info.fHp;

		m_Info.fShield -= Damage;

		m_Info.fHp = m_Info.fMaxHp;
	}
	else if (m_Info.fShield <= 0.1)
	{
		m_Info.fShield = 0;

		//페이즈 2를 위한 함수들
		m_bPhase_One = false;
		m_bOneTimeSwith = true;
		m_iOncePattern = 97;

		m_bFire = true;

		m_bLookAtOff = false; ////이거 조심하셈~!@~@~!@~@! 나중에 룩엣오프에 문제 생기면 이거 한번 없애보자

		m_bSoundSwitch = false;

		m_dInfinityCoolTime = 0;
		m_dOnceCoolTime = 0;
		m_dFireTime = 0;
	}


	return S_OK;
}

HRESULT CMonster_Boss_Bear::SetUp_Hp()
{
	return S_OK;
}

HRESULT CMonster_Boss_Bear::Slow_On()
{
	if (nullptr != m_pTempSlow_Bullet && m_fSlowBulletDistance < 200)
	{
		CTransform* Slow_BulletTransform = static_cast<CTransform*>(m_pTempSlow_Bullet->Get_Component(TEXT("Com_Transform")));
		_vector vDis = m_pTransform->Get_Distance(Slow_BulletTransform->Get_State(CTransform::STATE_POSITION));

		XMStoreFloat(&m_fSlowBulletDistance, vDis);
	}


	if (m_fSlowBulletDistance < 100)
	{
		if (m_pSphereCom->Collision_Sphere(m_pTempSlow_Bullet->m_pSphereCom))
		{
			m_dSlowTime = 0.2;
		}
		else {
			//m_dSlowTime = 1;
			if (m_dSlowTime <= 1)
			{
				m_dSlowTime += 0.01;
			}
		}
	}
	else {
		if (m_dSlowTime <= 1)
		{
			m_dSlowTime += 0.01;
		}
	}

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Victory()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUI_Universal::UI_Universal_DESC UI_UniversalDesc;

	UI_UniversalDesc.iUI_Number = CUI_Universal::VICTORY;
	UI_UniversalDesc.fSizeX = 640;
	UI_UniversalDesc.fSizeY = 360;
	UI_UniversalDesc.fX = 700;
	UI_UniversalDesc.fY = 350;
	UI_UniversalDesc.fDepth = 11.f;
	UI_UniversalDesc.iTextureIndex = 0;
	UI_UniversalDesc.pGameObject = this;
	UI_UniversalDesc.dDuration = 4;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Victory"), TEXT("Prototype_GameObject_UI_Universal"), &UI_UniversalDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Bear::Npc()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Npc"), TEXT("Prototype_GameObject_Npc"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CMonster_Boss_Bear * CMonster_Boss_Bear::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Boss_Bear*	pInstance = new CMonster_Boss_Bear(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMonster_Boss_Bear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Boss_Bear::Clone(void * pArg)
{
	CMonster_Boss_Bear*	pInstance = new CMonster_Boss_Bear(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Boss_Bear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Boss_Bear::Free()
{
	__super::Free();
	Safe_Release(m_pDissolveTextrueCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}