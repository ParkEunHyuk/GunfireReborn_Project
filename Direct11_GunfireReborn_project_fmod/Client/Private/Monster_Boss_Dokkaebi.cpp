#include "stdafx.h"
#include "..\Public\Monster_Boss_Dokkaebi.h"
#include "GameInstance.h"
#include "Item_Universal.h"
#include "UI_Universal.h"
#include "Camera_Player.h"


CMonster_Boss_Dokkaebi::CMonster_Boss_Dokkaebi(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CMonster_Boss_Dokkaebi::CMonster_Boss_Dokkaebi(const CMonster_Boss_Dokkaebi & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Boss_Dokkaebi::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::NativeConstruct(void * pArg)
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
	else {
		MaptoolNotPosition();
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

	m_pTransform->Scaled(XMVectorSet(1.5f,1.5f,1.5f,0.f));

	SetUp_UI();

	m_pNavigationCom->Find_CellIndex(m_pTransform);

	return S_OK;
}

_int CMonster_Boss_Dokkaebi::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	if (m_Info.fHp <= 0 )
	{
		m_iOncePattern = 98;
		m_iInfinityPattern = 98;

		if (m_dAnimCutTime > 5)
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
		MSGBOX("CMonster_Boss_Dokkaebi의 Add_ObseverGroup에 문제가 발생했습니다");
		return E_FAIL;
	}



	SetUp_Fight(TimeDelta * m_dSlowTime);

	PlayAnim(TimeDelta * m_dSlowTime);



	RELEASE_INSTANCE(CGameInstance);

	return m_iDead;
}

_int CMonster_Boss_Dokkaebi::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//__super::SetUp_OnTerrain(m_eNowLevelNum, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	SetUp_OnNavigation(m_pNavigationCom, m_eNowLevelNum, TEXT("Layer_BackGround"), 0);


	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);


	RELEASE_INSTANCE(CGameInstance);

#ifdef _DEBUG	
	m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif // _DEBUG

	return _int();
}

HRESULT CMonster_Boss_Dokkaebi::Render()
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

HRESULT CMonster_Boss_Dokkaebi::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Model_Boss_Dokkaebi"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve_1"), TEXT("Com_Texture"), (CComponent**)&m_pDissolveTextrueCom)))
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

HRESULT CMonster_Boss_Dokkaebi::SetUp_ConstantTable()
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

HRESULT CMonster_Boss_Dokkaebi::SetUp_UI()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUI_Universal::UI_Universal_DESC UI_UniversalDesc;

	UI_UniversalDesc.iUI_Number = CUI_Universal::UI_DOKKAEBI_STAMINA;
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

HRESULT CMonster_Boss_Dokkaebi::SetUp_Event_Tirgger(_double TimeDelta)
{
	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::SetUp_Fight(_double TimeDelta)
{
	Fight_On();

	if (true == m_bFight)
	{
		if (false == m_bJumpingOn)
		{
			m_pTransform->LookAtNOTAxisY(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
		}

		if (m_fDistance < 4)
		{
			m_pTransform->Go_Backward(TimeDelta * 1.4, m_pNavigationCom);
		}


		CoolTime_Manager(TimeDelta);

	}

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Fight_On()
{

	_vector vDis = m_pTransform->Get_Distance(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	XMStoreFloat(&m_fDistance, vDis);

	if (false == m_bOneTimeSwith)
	{
		m_bFight = true;
	}


	return S_OK;
}


HRESULT CMonster_Boss_Dokkaebi::PlayAnim(_double TimeDelta)
{
	if (false == m_bOneTimeSwith) //이건 블렌딩되어 있으면서 한번만 실행시키는 변수 tmi)웨폰에 있는 애니메이션 제어는 블렌딩하지 않음
	{
		Infinity_AnimMotion(TimeDelta);
		m_pModelCom->SetUp_AnimIndex(m_iInfinityAnimNumber); //만약 특정 변수로 계속 반복적으로 움직임
	}
	else {
		Once_AnimMotion(TimeDelta); //이거 어택이랑 무빙으로 되어 있는거 한번만 실행과 무한실행으로 바꾸고 그 함수 여기에 박으면 될 듯?
		m_pModelCom->SetUp_AnimIndex(m_iOnceAnimNumber); //한번만 돌릴려면 일단 m_bOneTimeSwith를 true로 바꿔줘야함
	}

	if (false == m_bAnimCutOn)
	{
		m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_bOneTimeSwith);
	}
	else {
		Infinity_AnimMotion(TimeDelta);
		m_pModelCom->SetUp_AnimIndex(m_iInfinityAnimNumber);
		m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_bOneTimeSwith);
	}

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::CoolTime_Manager(_double TimeDelta)
{
	m_dOnceCoolTime += TimeDelta;

	if (true == m_bPhase_One)
	{
		if (m_dOnceCoolTime > 5)
		{
			m_bOneTimeSwith = true;
			m_dOnceCoolTime = 0;
			m_dInfinityCoolTime = 0;

			m_bFire = true;
			m_dFireTime = 0;

			m_iOncePattern += 1;

			if (m_iOncePattern > 2) //지금 현재 보스는 랜덤이 아닌 순서대로 출력 시켜야함 그래서 m_iOncePattern의 최대값이 되면 0으로 초기화 시켜주는중
			{						//또한 현재 m_iOncePattern는 99부터 시작하니 0으로 만들어주는중
				m_iOncePattern = 0;
			}
		}
	}
	else {
		if (m_dOnceCoolTime > 5)
		{
			m_bOneTimeSwith = true;
			m_dOnceCoolTime = 0;
			m_dInfinityCoolTime = 0;

			m_bFire = true;
			m_dFireTime = 0;

			m_iOncePattern += 1;

			//3부터 시작하면 됨 3~ 6
			if (m_iOncePattern > 6) //지금 현재 보스는 랜덤이 아닌 순서대로 출력 시켜야함 그래서 m_iOncePattern의 최대값이 되면 0으로 초기화 시켜주는중
			{						//또한 현재 m_iOncePattern는 99부터 시작하니 0으로 만들어주는중
				m_iOncePattern = 3;
				m_iSupplementAttack = 0;
			}
		}
	}

	m_dInfinityCoolTime += TimeDelta;
	if (m_dInfinityCoolTime > 3)
	{
		m_iInfinityPattern = rand() % 6;
		m_dInfinityCoolTime = 0;

	}

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Once_AnimMotion(_double TimeDelta) //무빙넘버와 공격넘버로 바꿔도 괜찮을듯?
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	switch (m_iOncePattern)
	{
	case 0:
	{
		m_iOnceAnimNumber = 13; //플레이어를 향해 점프
		Jumping(TimeDelta);
		m_bOneTimeSwith = true;

		m_iInfinityPattern = 4;
		m_dInfinityCoolTime = 0;
		break;
	}
	case 1:
	{
		m_iOnceAnimNumber = 15; //팔을 땅에 꽂음
		m_bOneTimeSwith = true;
		Universal_StrikeDown(TimeDelta);
		break;
	}
	case 2:
	{
		m_iOnceAnimNumber = 16; //총 발사 준비
		m_bOneTimeSwith = true;
		Universal_Bullet(TimeDelta);

		break;
	}
	case 3:
	{
		m_iOnceAnimNumber = 13; //페이즈 2, 플레이어를 향해 점프
		Jumping_PhaseTwo(TimeDelta);
		m_bOneTimeSwith = true;

		m_iInfinityPattern = 4;
		m_dInfinityCoolTime = 0;
		break;
	}
	case 4:
	{
		m_iOnceAnimNumber = 16; //페이즈 2, 총 발사
		m_bOneTimeSwith = true;
		Universal_Bullet_PhaseTwo(TimeDelta);

		m_iInfinityPattern = 10;
		break;
	}
	case 5:
	{
		m_iOnceAnimNumber = 14; //페이즈 2, 메테오 발사 준비
		Meteorite_Bullet(TimeDelta);
		m_bOneTimeSwith = true;
		break;
	}
	case 6:
	{
		m_iOnceAnimNumber = 15; //페이즈 2, 팔을 땅에 내려쳐서 기둥을 소환함
		Terrain_Attack(TimeDelta);
		m_bOneTimeSwith = true;
		break;
	}
	case 97:
	{
		m_iOnceAnimNumber = 12; //페이즈2 준비 모션
		m_bOneTimeSwith = true;
		m_dOnceCoolTime = 0;
		m_dInfinityCoolTime = 0;

		if (true == m_bSoundSwitch && m_dFireTime > 0.5 && m_dFireTime < 1)
		{
			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1.f);
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Phase1_KnockDown.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}

		m_dFireTime += TimeDelta;
		if ( m_dFireTime > 1.5 )
		{
			m_pTransform->Scaling(2, 1, TimeDelta);

			if (false == m_bSoundSwitch && m_dFireTime > 3.0 && m_dFireTime < 3.5)
			{
				GetSingle(CGameInstance)->PlaySound((L"DokkaebiRoar2.mp3"), CHANNEL_OBJECT);

				m_bSoundSwitch = true;
			}
		}
		break;
	}
	case 98:
	{
		m_iOnceAnimNumber = 7;
		m_bOneTimeSwith = true;
		m_dOnceCoolTime = 0;
		m_dInfinityCoolTime = 0;

		m_dFireTime += TimeDelta;
		if (true == m_bSoundSwitch && m_dFireTime > 0 && m_dFireTime < 6.5)
		{
			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_KnockDown.mp3"), CHANNEL_OBJECT);
			m_bSoundSwitch = false;
		}

		if (false == m_bSoundSwitch && m_dFireTime > 7 && m_dFireTime < 9.5)
		{
			GetSingle(CGameInstance)->Stop_AllChannel();

			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Dead.mp3"), CHANNEL_OBJECT);


			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 0.2f);
			GetSingle(CGameInstance)->PlayBGM((L"Dokkaebi_Dead.mp3"), CHANNEL_OBJECT);
			m_bSoundSwitch = true;
		}


		m_dAnimCutTime += TimeDelta;
		if (m_dAnimCutTime > 8)
		{
			m_iDead = OBJ_DEAD;
		}

		break;
	}
	case 99:
	{
		m_iOnceAnimNumber = 6;
		m_bOneTimeSwith = true;
		m_dAnimCutTime += TimeDelta;

		if (m_dAnimCutTime > 7.0)
		{
			m_bAnimCutOn = true;
			m_bFight = true;
			m_bOneTimeSwith = false;

			m_dAnimCutTime = 0;
			m_bAnimCutOn = false;
		}

		if (m_dAnimCutTime > 1.5 && false == m_bSoundSwitch && m_dAnimCutTime <2.0)
		{
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Start.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = true;
		}
		if (m_dAnimCutTime > 3.3 && m_dAnimCutTime < 4.9 && true == m_bSoundSwitch)
		{
			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 0.3f);
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Hand.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}

		if (m_dAnimCutTime > 5.8 && m_dAnimCutTime < 6.5 && false == m_bSoundSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"DokkaebiRoar.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = true;
		}

		break;
	}
	default:
		break;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Infinity_AnimMotion(_double TimeDelta)
{
	if (true == m_bFight)
	{
		switch (m_iInfinityPattern)
		{
		case 0: //앞으로 움직임
			m_iInfinityAnimNumber = 1;
			m_pTransform->Go_Straight(TimeDelta, m_pNavigationCom);
			break;
		case 1: //왼쪽으로 움직임
			m_iInfinityAnimNumber = 4;
			m_pTransform->Go_Left(TimeDelta, m_pNavigationCom);
			break;
		case 2: //오른쪽으로 움직임
			m_iInfinityAnimNumber = 2;
			m_pTransform->Go_Right(TimeDelta, m_pNavigationCom);
			break;
		case 3: //앞으로 뛰어감
			m_iInfinityAnimNumber = 5;
			m_pTransform->Go_Straight(TimeDelta*1.4, m_pNavigationCom);
			break;
		case 4: //앞으로 뛰어감
			m_iInfinityAnimNumber = 5;
			m_pTransform->Go_Straight(TimeDelta*1.4, m_pNavigationCom);
			break;
		case 5: //앞으로 뛰어감
			m_iInfinityAnimNumber = 5;
			m_pTransform->Go_Straight(TimeDelta*1.4, m_pNavigationCom);
			break;
		case 10:
			m_iInfinityAnimNumber = 17;
			Universal_Bullet_SupplementAttack(TimeDelta);
			break;
		case 98://사망씬 m_iOncePattern에도 존재한다.
			m_iInfinityAnimNumber = 7;
			m_dOnceCoolTime = 0;
			m_dInfinityCoolTime = 0;

			m_dFireTime += TimeDelta;
			if (true == m_bSoundSwitch && m_dFireTime > 0 && m_dFireTime < 6.5)
			{
				//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
				GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_KnockDown.mp3"), CHANNEL_OBJECT);
				m_bSoundSwitch = false;
			}

			if (false == m_bSoundSwitch && m_dFireTime > 7 && m_dFireTime < 8.5)
			{
				//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
				GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Dead.mp3"), CHANNEL_OBJECT);
				m_bSoundSwitch = true;
			}

			m_dAnimCutTime += TimeDelta;
			if (m_dAnimCutTime > 8)
			{

				m_iDead = OBJ_DEAD;
			}

			break;
			break;
		default:
			break;
		}
	}
	return S_OK;
}
_int CMonster_Boss_Dokkaebi::Obsever_Hit(void* pArg)
{


	return _int();
}

_int CMonster_Boss_Dokkaebi::Obsever_Event(void * pArg)
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

_int CMonster_Boss_Dokkaebi::Dead_Trigger()
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


	Warp();

	Victory();

	return _int();
}

HRESULT CMonster_Boss_Dokkaebi::Jumping(_double TimeDelta)
{
	if (m_dFireTime>1 && m_dFireTime < 1.7)
	{
		m_pTransform->Go_Straight(TimeDelta*5.0, m_pNavigationCom);
		m_bJumpingOn = true;

		_vector	vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

		_float4	vTempPos;
		XMStoreFloat4(&vTempPos, vPosition);

		vTempPos.y += 2.5;
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vTempPos));

		if (true == m_bSoundSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Jumping.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}
	}
	else {
		m_bJumpingOn = false;
	}

	//이거 키고 위에 else를 지우면 날 바라보지 않고 쭉 날라감
	//if(m_dFireTime > 3.5)
	//{
	//	m_bJumpingOn = false;
	//}



	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 1.7)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::DOKKAEBI_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(12.5f, 12.5f, 12.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;


		//진동
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(7, 1.f);
		RELEASE_INSTANCE(CGameInstance);


		if (false == m_bSoundSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Landing.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = true;
		}
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Jumping_PhaseTwo(_double TimeDelta)
{
	if (m_dFireTime>1 && m_dFireTime < 1.7)
	{
		m_pTransform->Go_Straight(TimeDelta*5.0, m_pNavigationCom);
		m_bJumpingOn = true;

		_vector	vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

		_float4	vTempPos;
		XMStoreFloat4(&vTempPos, vPosition);

		vTempPos.y += 2.5;
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vTempPos));

		if (true == m_bSoundSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Jumping.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = false;
		}
	}
	else {
		m_bJumpingOn = false;
	}

	//이거 키고 위에 else를 지우면 날 바라보지 않고 쭉 날라감
	//if(m_dFireTime > 3.5)
	//{
	//	m_bJumpingOn = false;
	//}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 1.7)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::DOKKAEBI_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(18.5f, 18.5f, 18.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.5f, 1.5f, 1.5f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;

		//진동
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(7, 1.f);
		RELEASE_INSTANCE(CGameInstance);



		if (false == m_bSoundSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Landing.mp3"), CHANNEL_OBJECT);

			m_bSoundSwitch = true;
		}

	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Universal_Bullet_PhaseTwo(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime>0.7)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::BURN_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(3.f, 3.f, 3.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(1.5, 3.5, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.7f, 0.7f, 0.7f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;

		GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Bullet.mp3"), CHANNEL_OBJECT);
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Universal_Bullet_SupplementAttack(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;

	if (m_dFireTime > 1.1 && 0 == m_iSupplementAttack)
	{
		m_bFire = true;
		m_iSupplementAttack += 1;
	}
	else if (m_dFireTime > 1.8 && 1 == m_iSupplementAttack)
	{
		m_bFire = true;
		m_iSupplementAttack += 1;
	}
	else if (m_dFireTime > 2.5 && 2 == m_iSupplementAttack)
	{
		m_bFire = true;
		m_iSupplementAttack += 1;
	}

	if (true == m_bFire)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::SUPPLEMENT_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 30.f;
		Universal_BulletMeshDesc.vScale = _float3(3.f, 3.f, 3.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(1.5, 3.5, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.7f, 0.7f, 0.7f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;

		GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Bullet.mp3"), CHANNEL_OBJECT);
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Meteorite_Bullet(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime>0.7)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::METEORITE_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(2.0f, 2.0f, 2.0f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 5;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(3.f, 3.5f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(0.4f, 0.4f, 0.4f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.18f, 0.18f, 0.18f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Terrain_Attack(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime>1.5)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::DOKKAEBI_TERRAIN_ATTACK_GUIDED;
		Universal_BulletDesc.eType = PLAYER;

		Universal_BulletDesc.fSpeedPerSec = 30.f;
		Universal_BulletDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 10;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0, 1.5, 0.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(0.7f, 0.7f, 0.7f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Terrain_Attack"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;

		GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Terrain.mp3"), CHANNEL_OBJECT);

	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Universal_Bullet(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime>0.7)
	{
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::BURN_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(2.0f, 2.0f, 2.0f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 10;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(1.25, 1.5, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.7f, 0.7f, 0.7f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;

		GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Bullet.mp3"), CHANNEL_OBJECT);
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Universal_StrikeDown(_double TimeDelta)
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 1.2)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::DOKKAEBI_STRIKEDOWN;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(12.5f, 12.5f, 12.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 0.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletDesc.fColliderOBBScale = _float3(1.2f, 1.2f, 1.2f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;

		//진동
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(5, 0.2f);
		RELEASE_INSTANCE(CGameInstance);


		GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Terrainpunch.mp3"), CHANNEL_OBJECT);
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::SetUp_Shield()
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

		m_dInfinityCoolTime = 0;
		m_dOnceCoolTime = 0;
		m_dFireTime = 0;
	}


	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::SetUp_Hp()
{
	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Slow_On()
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

HRESULT CMonster_Boss_Dokkaebi::Warp()
{
	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
	//ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

	//Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::WARP;
	//Universal_BulletMeshDesc.eType = BULLET_MOSTER;

	//Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
	//Universal_BulletMeshDesc.vScale = _float3(7.5f, 7.5f, 7.5f);
	//Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	//Universal_BulletMeshDesc.dDuration = 1000000;

	//Universal_BulletMeshDesc.pObject = this;
	//Universal_BulletMeshDesc.fPositioning = _float3(9999.f, 9999.f, 9999.f);


	//Universal_BulletMeshDesc.fColliderSphereScale = _float3(0.7f, 0.7f, 0.7f);
	//Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.5f, 0.5f, 0.5f);

	//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
	//	return E_FAIL;

	//RELEASE_INSTANCE(CGameInstance);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Npc"), TEXT("Prototype_GameObject_Npc"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Boss_Dokkaebi::Victory()
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

HRESULT CMonster_Boss_Dokkaebi::MaptoolNotPosition()
{
	_float TimeDelta = 0.f;
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&_float4(80.f, 1.f, 80.f, 1.f)));
	return S_OK;
}

CMonster_Boss_Dokkaebi * CMonster_Boss_Dokkaebi::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Boss_Dokkaebi*	pInstance = new CMonster_Boss_Dokkaebi(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMonster_Boss_Dokkaebi");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Boss_Dokkaebi::Clone(void * pArg)
{
	CMonster_Boss_Dokkaebi*	pInstance = new CMonster_Boss_Dokkaebi(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Boss_Dokkaebi");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Boss_Dokkaebi::Free()
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