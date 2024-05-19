#include "stdafx.h"
#include "..\Public\Universal_Bullet.h"
#include "GameInstance.h"
#include "Universal_Bullet_Mesh.h"
#include "Universal_Point_Instance.h"
#include "Camera_Player.h"

const 	_tchar*	 m_pTextureTag[CUniversal_Bullet::BULLET_END] = {
	TEXT("Prototype_Component_Texture_Universal_Bullet_Reflect_SKill"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_SlowTime_SKill"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_Launcher_Boom"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_Lizard"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_GhostDeer"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_PowerLizard"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_Raven"),
	TEXT("Prototype_Component_Texture_Universal_Yellow_Potal"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_NonTexture"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_NonTexture"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_NonTexture"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_NonTexture"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_NonTexture"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_NonTexture"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_NonTexture"),
	TEXT("Prototype_Component_Texture_Universal_Bullet_NonTexture")
};

CUniversal_Bullet::CUniversal_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CUniversal_Bullet::CUniversal_Bullet(const CUniversal_Bullet & rhs)
	: CLandObject(rhs)
{
}

HRESULT CUniversal_Bullet::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUniversal_Bullet::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Universal_BulletDesc, pArg, sizeof(UNIVERSAL_BULLETDESC));
	}
	else
	{
		MSGBOX("Monster_Bullet에 pArg가 Nullptr입니다.");
	}

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = m_Universal_BulletDesc.fSpeedPerSec;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	SetUp_Info();

	m_pNavigationCom->Find_CellIndex(m_pTransform);

	return S_OK;
}

_int CUniversal_Bullet::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;
	if (m_iDead == OBJ_DEAD)
	{
		return m_iDead;
	}
	m_dTextureTime = TimeDelta;
	Slow_On();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance->GetInstance()->Add_ObseverGroup(m_Universal_BulletDesc.eType, this)))
	{
		MSGBOX("Monster_Bullet의 Add_ObseverGroup에 문제가 발생했습니다");
		return E_FAIL;
	}

	SetUp_Fire(TimeDelta * m_dSlowTime); //슬로우타임 때문에 여러가지가 꼬일 수 있으니 항상 체크할 것




	RELEASE_INSTANCE(CGameInstance);


	m_dDeltaTime += TimeDelta;

	if (m_dDeltaTime > m_Universal_BulletDesc.dDuration)
	{
		return OBJ_DEAD;
	}


	return m_iDead;
}

_int CUniversal_Bullet::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	switch (m_Universal_BulletDesc.iBulletNumber)
	{
	case SLOWTIME_SKILL:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
		break;
	}
	default:

		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		break;
	}

	RELEASE_INSTANCE(CGameInstance);

#ifdef _DEBUG	
	m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif // _DEBUG


	return _int();
}

HRESULT CUniversal_Bullet::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 1);


	return S_OK;
}

HRESULT CUniversal_Bullet::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXTEX"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_pTextureTag[m_Universal_BulletDesc.iBulletNumber], TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Navigation */
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.pParent = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_BackGround"), CGameObject::m_pTransformTag));;
	NaviDesc.iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	SetUp_Collider();

	return S_OK;
}

HRESULT CUniversal_Bullet::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
	//	return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_OnShader_AutoFrame(m_pShaderCom, "g_DiffuseTexture", m_dTextureTime, m_Universal_BulletDesc.iTextureIndex))) //최대 갯수의 인덱스
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUniversal_Bullet::SetUp_Info()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));


	if (nullptr != m_Universal_BulletDesc.pObject)
	{
		CTransform* pMonsterTransform = static_cast<CTransform*>(m_Universal_BulletDesc.pObject->Get_Component(TEXT("Com_Transform")));

		m_pTransform->Scaled(XMLoadFloat3(&m_Universal_BulletDesc.vScale));

		//////////이거 나중에 확인해보자 이거 없어도 잘 날라가긴 함 빌보드할 때 따라왔던 이유가 이거인듯
		_vector vRight = pMonsterTransform->Get_State(CTransform::STATE_RIGHT);
		_vector vUp = pMonsterTransform->Get_State(CTransform::STATE_UP);
		_vector vLook = pMonsterTransform->Get_State(CTransform::STATE_LOOK);

		vRight = m_pTransform->Get_Scale(CTransform::STATE_RIGHT) * XMVector3Normalize(vRight);
		vUp = m_pTransform->Get_Scale(CTransform::STATE_UP) * XMVector3Normalize(vUp);
		vLook = m_pTransform->Get_Scale(CTransform::STATE_LOOK) * XMVector3Normalize(vLook);

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
		////////////////

		_vector		vPosition = pMonsterTransform->Get_State(CTransform::STATE_POSITION);

		vPosition += XMVector3Normalize(pMonsterTransform->Get_State(CTransform::STATE_RIGHT)) *m_Universal_BulletDesc.fPositioning.x;
		vPosition += XMVector3Normalize(pMonsterTransform->Get_State(CTransform::STATE_UP)) * m_Universal_BulletDesc.fPositioning.y;
		vPosition += XMVector3Normalize(pMonsterTransform->Get_State(CTransform::STATE_LOOK)) * m_Universal_BulletDesc.fPositioning.z;

		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

		//이거 왜 그러는지는 모르겠지만 밑에 플레이어 전용 else에는 사용할 수 없음
		m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), 1);

		XMStoreFloat4(&m_fTempPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		XMStoreFloat3(&m_fTempLook, m_pTransform->Get_State(CTransform::STATE_LOOK));

		//SetUp_Info에 몬스터의 룩을 넣어서 따라왔던거 같음
	}
	else {

		m_pTransform->Scaled(XMLoadFloat3(&m_Universal_BulletDesc.vScale));
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_Universal_BulletDesc.vTranslation));

		m_fTempPos = m_Universal_BulletDesc.vTranslation;
		m_fTempLook = m_Universal_BulletDesc.vLook;

	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUniversal_Bullet::SetUp_Fire(_double TimeDelta)
{
	switch (m_Universal_BulletDesc.iBulletNumber)
	{
	case REFLECT_SKILL:
		Billboard();
		Reflect_Skill(TimeDelta*0.1);
		break;
	case SLOWTIME_SKILL:
		SlowTime_Skill(TimeDelta);
		break;
	case LAUNCHER_BOOM:
		Launcher_Boom(TimeDelta); //나중에 이펙트 작업을 위해 원형만 만들어둠
		break;
	case LIZARD:
		NavigationOn(TimeDelta);
		Billboard();
		Lizard(TimeDelta);
		break;
	case GHOSTDEER:
		NavigationOn(TimeDelta);
		GhostDeer(TimeDelta);
		Billboard();
		break;
	case POWERLIZARD:
		NavigationOn(TimeDelta);
		PowerLizard(TimeDelta);
		Billboard();
		break;
	case RAVEN:
		NavigationOn(TimeDelta);
		Raven(TimeDelta);
		Billboard();
		break;
	case YELLOW_POTAL:
		Yellow_Potal(TimeDelta);
		Billboard();
		break;
	case MELEE_ATTACK:
		//근접공격만 있기 때문에 굳이 설정을 해줄 필요 없음
		break;
	case DOKKAEBI_ATTACK:
		Dokkaebi_Attack(TimeDelta);
		break;
	case DOKKAEBI_STRIKEDOWN:
		Dokkaebi_StrikeDown(TimeDelta);
		break;
	case DOKKAEBI_TERRAIN_ATTACK_GUIDED:
		NavigationOn(TimeDelta);
		Dokkaebi_Terrain_Guided(TimeDelta);
		break;
	case BEAR_ATTACK:
		Bear_Attack(TimeDelta);
		break;
	case CUBE_BOOM:
		Cube_Boom(TimeDelta); //나중에 이펙트 작업을 위해 원형만 만들어둠
		break;
	case NONTEXTURE:
		NonTexture(TimeDelta);
		break;
	case NOTICE:
		break;
	default:
		MSGBOX("BulletType이 존재하지 않습니다");
		break;
	}

	///////////////////////
	//_vector		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	//_vector		vUp = m_pTransform->Get_State(CTransform::STATE_UP);
	//_vector		vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

	//_vector vAxis = XMLoadFloat4(&_float4(0.f, 0.f, 1.f, 0.f));


	//i += 1;
	//_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, i * (_float)TimeDelta);

	//vRight = XMVector4Transform(vRight, RotationMatrix);
	//vUp = XMVector4Transform(vUp, RotationMatrix);
	//vLook = XMVector4Transform(vLook, RotationMatrix);

	//m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	//m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	//m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

	//_vector		Tlqkf = m_pTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		glaemfek = m_pTransform->Get_State(CTransform::STATE_UP);

	//Tlqkf += XMVector3Normalize(glaemfek) *50 * (_float)TimeDelta;

	//m_pTransform->Set_State(CTransform::STATE_POSITION, Tlqkf);
	//////////////////////////

	return S_OK;
}

HRESULT CUniversal_Bullet::SetUp_Collider()
{
	switch (m_Universal_BulletDesc.iBulletNumber)
	{
	case SLOWTIME_SKILL:
	{
		/* For.Com_Sphere */
		CCollider::COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

		ColliderDesc.vScale = m_Universal_BulletDesc.fColliderSphereScale;
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
			return E_FAIL;
	}
	default:
	{
		/* For.Com_Sphere */
		CCollider::COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

		ColliderDesc.vScale = m_Universal_BulletDesc.fColliderSphereScale;
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
			return E_FAIL;

		/* For.Com_OBB */
		ColliderDesc.vScale = m_Universal_BulletDesc.fColliderOBBScale;
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
			return E_FAIL;
		break;
	}
	}

	return S_OK;
}

_int CUniversal_Bullet::Obsever_Hit(void * pArg)
{
	CLandObject* SorceObject = static_cast<CLandObject*>(pArg);

	if (nullptr != SorceObject)//몬스터들의 탄환은 이걸로 계산됨
	{
		switch (m_Universal_BulletDesc.iBulletNumber)
		{
		case LIZARD:
			SorceObject->m_Info.fHp -= 10;
			break;
		case MELEE_ATTACK:
			SorceObject->m_Info.fHp -= 10;
			break;
		case GHOSTDEER:
			SorceObject->m_Info.fHp -= 20;
			break;
		case POWERLIZARD:
			SorceObject->m_Info.fHp -= 10;
			break;
		case RAVEN:
			SorceObject->m_Info.fHp -= 30;
			break;
		case DOKKAEBI_ATTACK:
			SorceObject->m_Info.fHp -= 30;
			break;
		case DOKKAEBI_STRIKEDOWN:
			SorceObject->m_Info.fHp -= 20;
			break;
		case BEAR_ATTACK:
			SorceObject->m_Info.fHp -= 20;
			break;
		case CUBE_BOOM:
			SorceObject->m_Info.fHp -= 50;
			break;
		}
	}

	switch (m_Universal_BulletDesc.iBulletNumber) //자기 자신의 이름을 찾고 자신을 기준으로 스킬을 발동시킴
	{
	case REFLECT_SKILL:
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Bullet* SorceBullet = static_cast<CUniversal_Bullet*>(pArg);
		
		if (true == SorceBullet->Get_NotMesh())
		{
			//이렇게 예외처리를 해야 큐브의 자폭 공격이 REFLECT_SKILL로 통해 몬스터를 공격하지 않음
			if (CUBE_BOOM == SorceBullet->m_Universal_BulletDesc.iBulletNumber || DOKKAEBI_TERRAIN_ATTACK_GUIDED == SorceBullet->m_Universal_BulletDesc.iBulletNumber)
			{
				RELEASE_INSTANCE(CGameInstance);
				break;
			}

			UNIVERSAL_BULLETDESC Universal_BulletDesc;
			ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

			Universal_BulletDesc = SorceBullet->m_Universal_BulletDesc;
			Universal_BulletDesc.pObject = nullptr;
			Universal_BulletDesc.eType = BULLET;

			_float4 fTempPos;
			_float3 fTempLook;
			CTransform* SorceTransform = static_cast<CTransform*>(SorceBullet->Get_Component(TEXT("Com_Transform")));
			XMStoreFloat4(&fTempPos, SorceTransform->Get_State(CTransform::STATE_POSITION));
			XMStoreFloat3(&fTempLook, SorceTransform->Get_State(CTransform::STATE_LOOK));

			Universal_BulletDesc.vTranslation = fTempPos;
			Universal_BulletDesc.vLook = fTempLook;

			if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Reflect_SKill"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
				return E_FAIL;

			GetSingle(CGameInstance)->PlaySound((L"Player_Reflect.mp3"), CHANNEL_EFFECT);
		}
		else {
			//이건 메쉬 불렛에 관한 내용
			CUniversal_Bullet_Mesh* MeshBullet = static_cast<CUniversal_Bullet_Mesh*>(pArg);
			if (nullptr != MeshBullet)
			{
				if (CUniversal_Bullet_Mesh::METEORITE_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
					|| CUniversal_Bullet_Mesh::TRIANGLESWORD_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
					|| CUniversal_Bullet_Mesh::BLUNT_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
					|| CUniversal_Bullet_Mesh::BIDENT_SPEAR == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
					|| CUniversal_Bullet_Mesh::BIG_SWORD == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
					|| CUniversal_Bullet_Mesh::FATAL_RAPIER == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
					|| CUniversal_Bullet_Mesh::SWORDWAVE_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
					|| CUniversal_Bullet_Mesh::LATERAL_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
					|| CUniversal_Bullet_Mesh::UPPER_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
					|| CUniversal_Bullet_Mesh::VERTICAL_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber)
				{
					if (CUniversal_Bullet_Mesh::METEORITE_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
						|| CUniversal_Bullet_Mesh::TRIANGLESWORD_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
						|| CUniversal_Bullet_Mesh::BLUNT_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
						|| CUniversal_Bullet_Mesh::BIDENT_SPEAR == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
						|| CUniversal_Bullet_Mesh::BIG_SWORD == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
						|| CUniversal_Bullet_Mesh::FATAL_RAPIER == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
						|| CUniversal_Bullet_Mesh::SWORDWAVE_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
						|| CUniversal_Bullet_Mesh::LATERAL_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
						|| CUniversal_Bullet_Mesh::UPPER_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber
						|| CUniversal_Bullet_Mesh::VERTICAL_BULLET == MeshBullet->m_Universal_Bullet_MeshDesc.iMeshNumber)
					{
						RELEASE_INSTANCE(CGameInstance);
						m_iDead = OBJ_DEAD;
						break;
					}

					RELEASE_INSTANCE(CGameInstance);
					break;
				}

				CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_Bullet_MeshDesc;
				ZeroMemory(&Universal_Bullet_MeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

				Universal_Bullet_MeshDesc = MeshBullet->m_Universal_Bullet_MeshDesc;
				Universal_Bullet_MeshDesc.pObject = nullptr;
				Universal_Bullet_MeshDesc.eType = BULLET;

				_float4 fTempPos;
				_float3 fTempLook;
				CTransform* SorceMeshTransform = static_cast<CTransform*>(MeshBullet->Get_Component(TEXT("Com_Transform")));
				XMStoreFloat4(&fTempPos, SorceMeshTransform->Get_State(CTransform::STATE_POSITION));
				//XMStoreFloat3(&fTempLook, -SorceMeshTransform->Get_State(CTransform::STATE_LOOK)); 불렛의 방향대로 감
				XMStoreFloat3(&fTempLook, m_pTransform->Get_State(CTransform::STATE_LOOK)); //거울의 룩 방향으로 감(거울은 빌보드 상태이기 때문에 뒷면이 룩인 상태이므로 알아서 잘 날라감)

				Universal_Bullet_MeshDesc.vTranslation = fTempPos;
				Universal_Bullet_MeshDesc.vLook = fTempLook;

				if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Reflect_SKill"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_Bullet_MeshDesc)))
					return E_FAIL;

				GetSingle(CGameInstance)->PlaySound((L"Player_Reflect.mp3"), CHANNEL_EFFECT);
			}
		}

		RELEASE_INSTANCE(CGameInstance);
		break;
	}
	case LAUNCHER_BOOM:
	{
		CLandObject* DestObject = static_cast<CLandObject*>(pArg);

		if (m_Universal_BulletDesc.eType == BULLET)
		{
			DestObject->m_Info.fHp -= LAUNCHER_BOOM_DAMAGE;
		}
		break;
	}
	case NONTEXTURE:
	{
		CLandObject* DestObject = static_cast<CLandObject*>(pArg);

		if (m_Universal_BulletDesc.eType == BULLET)
		{
			DestObject->m_Info.fHp -= 10;
			GetSingle(CGameInstance)->PlaySound((L"PishSwordShot.mp3"), CHANNEL_EFFECT);
		}
		//else {
		//	여기다가 몬스터 근접 공격 넣으면 될 듯
		//}
	}
	default:
		break;
	}


	return _int();
}

_int CUniversal_Bullet::Obsever_Event(void * pArg)
{
	CUniversal_Bullet* SorceBullet = static_cast<CUniversal_Bullet*>(pArg);

	if (nullptr != SorceBullet)
	{
		switch (SorceBullet->m_Universal_BulletDesc.iBulletNumber)
		{
		case SLOWTIME_SKILL:
		{
			m_pTempSlow_Bullet = SorceBullet;
			m_dSlowTime = 0.05;
			break;
		}
		default:
			break;
		}
	}

	return _int();
}

_int CUniversal_Bullet::Dead_Trigger()
{
	return _int();
}

HRESULT CUniversal_Bullet::Slow_Particle()
{
	if (false == m_bSlowParticleOn)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC Universal_PointDesc;

		Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SLOW_PARTICLE;
		Universal_PointDesc.iParticleType = CVIBuffer_Point_Instance::PARTICLE_UP;
		Universal_PointDesc.fMinSpeed = 1.f;
		Universal_PointDesc.fMaxSpeed = 10.f;
		Universal_PointDesc.fRange = _float3(9.f, 1.f, 9.f);
		Universal_PointDesc.fSize = 2.5f;
		Universal_PointDesc.pObject = this;
		Universal_PointDesc.dDuration = SLOWTIME_COOLTIME;
		Universal_PointDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		m_bSlowParticleOn = true;
	}
	return S_OK;
}

HRESULT CUniversal_Bullet::Billboard()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vRight = m_pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
	_vector vUp = m_pPlayerTransform->Get_State(CTransform::STATE_UP);
	_vector vLook = m_pPlayerTransform->Get_State(CTransform::STATE_LOOK);

	vRight = m_pTransform->Get_Scale(CTransform::STATE_RIGHT) * XMVector3Normalize(vRight);
	vUp = m_pTransform->Get_Scale(CTransform::STATE_UP) * XMVector3Normalize(vUp);
	vLook = m_pTransform->Get_Scale(CTransform::STATE_LOOK) * XMVector3Normalize(vLook);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

	//SetUp_Info에 몬스터의 룩을 넣어서 따라왔던거 같음


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUniversal_Bullet::Lizard(_double TimeDelta)
{

	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook= XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_BulletDesc.fSpeedPerSec * TimeDelta;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);


	return S_OK;
}

HRESULT CUniversal_Bullet::GhostDeer(_double TimeDelta)//룩만 냅두고 돌리면 될 듯?
{
	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	m_fDegrees += 12; //회전 각

	m_pTransform->Rotation(XMVectorSet(0.f,0.f,1.f,0.f), XMConvertToRadians(m_fDegrees));
	//m_pTransform->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(m_fDegrees));//***

	vPosition += XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_UP)) * m_Universal_BulletDesc.fSpeedPerSec * TimeDelta *0.8; //값이 커질수록 크게 회전
	//vPosition += XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * m_Universal_BulletDesc.fSpeedPerSec * TimeDelta *0.8;//***

	vPosition += XMVector3Normalize(vLook) * m_Universal_BulletDesc.fSpeedPerSec * TimeDelta;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	//////////////////////////////위에 코드가 쓰던거


	return S_OK;
}

HRESULT CUniversal_Bullet::PowerLizard(_double TimeDelta)
{
	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_BulletDesc.fSpeedPerSec * TimeDelta;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	return S_OK;
}

HRESULT CUniversal_Bullet::Raven(_double TimeDelta)
{
	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_BulletDesc.fSpeedPerSec * TimeDelta;

	vPosition = XMVectorSetY(vPosition, 0.5f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	return S_OK;
}

HRESULT CUniversal_Bullet::Cube_Boom(_double TimeDelta)
{
	return S_OK;
}

HRESULT CUniversal_Bullet::Dokkaebi_Attack(_double TimeDelta)
{
	return S_OK;
}

HRESULT CUniversal_Bullet::Dokkaebi_StrikeDown(_double TimeDelta)
{
	return S_OK;
}

HRESULT CUniversal_Bullet::Dokkaebi_Terrain_Guided(_double TimeDelta)
{
	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_BulletDesc.fSpeedPerSec * TimeDelta;

	vPosition = XMVectorSetY(vPosition, 0.5f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);


	m_dFireTime += TimeDelta;

	if (m_dFireTime > 0.1)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::TERRAIN_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 7.f;
		Universal_BulletMeshDesc.vScale = _float3(10.0f, 10.0f, 10.0f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 5;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, -5.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(0.7f, 0.7f, 0.7f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.5f, 0.5f, 0.5f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_dFireTime = 0;

		//진동
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(3, 0.1f);

		RELEASE_INSTANCE(CGameInstance);

		//_float fSound_Dis = this->Get_CamDistance();
		//fSound_Dis = fSound_Dis / 100;  //거리(소리나는위치 - 현재위치) / 최대거리
		//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1 - fSound_Dis);
		//GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Terrain.mp3"), CHANNEL_OBJECT);
	}

	return S_OK;
}

HRESULT CUniversal_Bullet::Bear_Attack(_double TimeDelta)
{
	return S_OK;
}

HRESULT CUniversal_Bullet::Yellow_Potal(_double TimeDelta)
{

	m_pTransform->Scaling(3,30,TimeDelta);

	m_fDegrees += 12; //회전 각

	m_pTransform->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(m_fDegrees));
	//m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta);

	return S_OK;
}

HRESULT CUniversal_Bullet::Launcher_Boom(_double TimeDelta)
{
	return S_OK;
}

HRESULT CUniversal_Bullet::NonTexture(_double TimeDelta)
{
	//if (nullptr == m_Universal_BulletDesc.pObject)
	//{
	//	CTransform* DescTransform = static_cast<CTransform*>(m_Universal_BulletDesc.pObject->Get_Component(TEXT("Com_Transform")));
	//	m_pTransform->Set_State(CTransform::STATE_POSITION, DescTransform->Get_State(CTransform::STATE_POSITION));
	//}

	return S_OK;
}

HRESULT CUniversal_Bullet::Slow_On()
{
	if (nullptr != m_pTempSlow_Bullet && m_fSlowBulletDistance < 200)
	{
		_vector vDis = m_pTransform->Get_Distance(m_pTempSlow_Bullet->m_pTransform->Get_State(CTransform::STATE_POSITION));

		XMStoreFloat(&m_fSlowBulletDistance, vDis);
	}


	if (m_fSlowBulletDistance < 100)
	{
		if (m_pSphereCom->Collision_Sphere(m_pTempSlow_Bullet->m_pSphereCom))
		{
			m_dSlowTime = 0.1;
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

HRESULT CUniversal_Bullet::NavigationOn(_double TimeDelta)
{
	_float3 vDir;
	XMStoreFloat3(&vDir, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * m_pTransform->Get_TransformDesc().fSpeedPerSec * (_float)TimeDelta);

	if (false == m_pNavigationCom->Move_OnNavigation(m_pTransform->Get_State(CTransform::STATE_POSITION), &vDir))
	{
		m_iDead = OBJ_DEAD;
	}

	return S_OK;
}

HRESULT CUniversal_Bullet::Reflect_Skill(_double TimeDelta)
{
	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_BulletDesc.fSpeedPerSec * TimeDelta;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	return S_OK;
}

HRESULT CUniversal_Bullet::SlowTime_Skill(_double TimeDelta)
{
	_vector vAxis = XMLoadFloat4(&_float4(1.f, 0.f, 0.f, 0.f));
	m_pTransform->Rotation(vAxis, XMConvertToRadians(90));

	_vector vPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	vPos = XMVectorSetY(vPos,0.2f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	if (m_dDeltaTime >= m_Universal_BulletDesc.dDuration - 0.5)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, OUT_POSITION);

	}

	Slow_Particle();

	return S_OK;
}



CUniversal_Bullet * CUniversal_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUniversal_Bullet*	pInstance = new CUniversal_Bullet(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUniversal_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUniversal_Bullet::Clone(void * pArg)
{
	CUniversal_Bullet*	pInstance = new CUniversal_Bullet(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CUniversal_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUniversal_Bullet::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
