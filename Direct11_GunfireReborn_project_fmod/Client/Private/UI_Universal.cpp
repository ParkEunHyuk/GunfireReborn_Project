#include "stdafx.h"
#include "..\Public\UI_Universal.h"
#include "GameInstance.h"
#include "Level.h"

const	_tchar* m_pTexture_UI_Tag[CUI_Universal::UI_END] = {
	TEXT("Prototype_Component_Texture_UI_Universal_Boss_Dokkaebi_Stamina"),
	TEXT("Prototype_Component_Texture_UI_Universal_Boss_Griffin_Stamina"),
	TEXT("Prototype_Component_Texture_UI_Universal_Boss_Bear_Stamina"),
	TEXT("Prototype_Component_Texture_UI_Universal_Boss_Hp"),
	TEXT("Prototype_Component_Texture_UI_Universal_Boss_Shield"),
	TEXT("Prototype_Component_Texture_UI_Universal_Hit_Player"),
	TEXT("Prototype_Component_Texture_UI_Universal_Dash_Straight"),
	TEXT("Prototype_Component_Texture_UI_Universal_Dash_Right"),
	TEXT("Prototype_Component_Texture_UI_Universal_Dash_Left"),
	TEXT("Prototype_Component_Texture_UI_Universal_Victory"),
	TEXT("Prototype_Component_Texture_UI_Universal_Talk_Box"),
	TEXT("Prototype_Component_Texture_UI_Universal_EndingCredits")
};


CUI_Universal::CUI_Universal(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{

}

CUI_Universal::CUI_Universal(const CUI_Universal & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Universal::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Universal::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_UI_UniversalDesc, pArg, sizeof(UI_Universal_DESC));
	}
	else
	{
		MSGBOX("UI_Universal에 pArg가 Nullptr입니다.");
	}

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//m_fX와 m_fY로 위치 조정하기
	m_fSizeX = m_UI_UniversalDesc.fSizeX;
	m_fSizeY = m_UI_UniversalDesc.fSizeY;
	m_fX = m_UI_UniversalDesc.fX;	//(g_iWinCX * 0.5f + 525) = 1165;
	m_fY = m_UI_UniversalDesc.fY;	//(g_iWinCY * 0.5f + 250) = 610;


	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	m_fDepth = m_UI_UniversalDesc.fDepth;

	Initialize();

	//깊이 조절
	return S_OK;
}

_int CUI_Universal::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	Frame_Speed(TimeDelta);


	Play_UI(TimeDelta);

	return m_iDead;
}

_int CUI_Universal::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (VICTORY	!= m_UI_UniversalDesc.iUI_Number
		&& TALK_BOX != m_UI_UniversalDesc.iUI_Number)
	{
		m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f)); //Z로 앞 뒤 조절하는듯?
	}																																 //m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(200, -200, 0.f, 1.f));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI_Universal::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return S_OK;
}

HRESULT CUI_Universal::Initialize()
{
	switch (m_UI_UniversalDesc.iUI_Number)
	{
	case UI_BOSS_HP:
	{
		CLandObject* pObject = static_cast<CLandObject*>(m_UI_UniversalDesc.pGameObject);

		m_fTemp = pObject->Get_Info().fHp;

		m_fMaxSizeX = m_fSizeX;
		m_fMaxSizeY = m_fSizeY;
		break;
	}
	case UI_BOSS_SHIELD:
	{
		CLandObject* pObject = static_cast<CLandObject*>(m_UI_UniversalDesc.pGameObject);

		m_fTemp = pObject->Get_Info().fShield;

		m_fMaxSizeX = m_fSizeX;
		m_fMaxSizeY = m_fSizeY;
		break;
	}
	case TALK_BOX:
	{
		m_fSizeX = m_UI_UniversalDesc.fSizeX + 200;
		m_fSizeY = m_UI_UniversalDesc.fSizeY;
		break;
	}
	default:
		break;
	}
	return S_OK;
}

HRESULT CUI_Universal::Play_UI(_double TimeDelta)
{
	switch (m_UI_UniversalDesc.iUI_Number)
	{
	case UI_DOKKAEBI_STAMINA:
		Boss_Stamina();
		break;
	case UI_BEAR_STAMINA:
		Boss_Stamina();
		break;
	case UI_BOSS_HP:
		Boss_Hp();
		break;
	case UI_BOSS_SHIELD:
		Boss_Shield();
		break;
	case HIT_PLAYER:
		Hit_Player(TimeDelta);
		break;
	case DASH_STRAIGHT:
		Dash_Straight(TimeDelta);
		break;
	case DASH_RIGHT:
		Dash_Right(TimeDelta);
		break;
	case DASH_LEFT:
		Dash_Left(TimeDelta);
		break;
	case VICTORY:
		Victory(TimeDelta);
		break;
	case TALK_BOX:
		Talk_Box(TimeDelta);
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CUI_Universal::Boss_Stamina()
{
	CLandObject* pObject = static_cast<CLandObject*>(m_UI_UniversalDesc.pGameObject);
	if (pObject->Get_Info().fHp <= 0)
	{
		m_iDead = OBJ_DEAD;
	}
	return S_OK;
}

HRESULT CUI_Universal::Boss_Shield()
{
	CLandObject* pObject = static_cast<CLandObject*>(m_UI_UniversalDesc.pGameObject);

	if (m_fTemp != pObject->Get_Info().fShield)
	{
		if (pObject->Get_Info().fShield <= 0)
		{
			m_fSizeX = 0;
			m_fTemp = pObject->Get_Info().fShield;

			m_iDead = OBJ_DEAD;
		}
		else {
			_float fShieldDamage = m_fTemp - pObject->Get_Info().fShield;

			_float	fAttenuation = m_fMaxSizeX / pObject->Get_Info().fMaxShield;

			m_fSizeX -= fShieldDamage * fAttenuation;

			m_fX -= ((fShieldDamage* fAttenuation)*0.5);

			m_fTemp = pObject->Get_Info().fShield;

		}

	}


	return S_OK;
}

HRESULT CUI_Universal::Boss_Hp()
{
	CLandObject* pObject = static_cast<CLandObject*>(m_UI_UniversalDesc.pGameObject);

	if (m_fTemp != pObject->Get_Info().fHp)
	{
		if (pObject->Get_Info().fHp <= 0)
		{
			m_fSizeX = 0;
			m_fTemp = pObject->Get_Info().fHp;

			//체력바는 지금 사이즈가 0이라서 안보이는것이지 객체를 삭제시키고 있지 않음 나중에 조심할 것
		}
		else {
			if (0.1 >= pObject->Get_Info().fShield)
			{
				_float fHpDamage = m_fTemp - pObject->Get_Info().fHp;

				_float	fAttenuation = m_fMaxSizeX / pObject->Get_Info().fMaxHp;

				m_fSizeX -= fHpDamage * fAttenuation;

				m_fX -= ((fHpDamage* fAttenuation)*0.5);

				m_fTemp = pObject->Get_Info().fHp;
			}

		}

	}
	return S_OK;
}

HRESULT CUI_Universal::Hit_Player(_double TimeDelta)
{
	m_AccumulationTime += TimeDelta;

	if (m_AccumulationTime > m_UI_UniversalDesc.dDuration)
	{
		m_iDead = OBJ_DEAD;
	}

	return S_OK;
}

HRESULT CUI_Universal::Dash_Straight(_double TimeDelta)
{
	m_AccumulationTime += TimeDelta;

	if (m_AccumulationTime > m_UI_UniversalDesc.dDuration)
	{
		m_iDead = OBJ_DEAD;
	}

	return S_OK;
}

HRESULT CUI_Universal::Dash_Right(_double TimeDelta)
{
	m_AccumulationTime += TimeDelta;

	if (m_AccumulationTime > m_UI_UniversalDesc.dDuration)
	{
		m_iDead = OBJ_DEAD;
	}
	return S_OK;
}

HRESULT CUI_Universal::Dash_Left(_double TimeDelta)
{
	m_AccumulationTime += TimeDelta;

	if (m_AccumulationTime > m_UI_UniversalDesc.dDuration)
	{
		m_iDead = OBJ_DEAD;
	}
	return S_OK;
}

HRESULT CUI_Universal::Victory(_double TimeDelta)
{
	_vector TempPos;

	m_AccumulationTime += TimeDelta;
	m_dEasingTime += TimeDelta;
	m_SoundTime += TimeDelta;

	if ( m_AccumulationTime <= 2)
	{

		TempPos = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, XMVectorSet(0.f, 500.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), m_dEasingTime, 2.0f);
		if (m_dEasingTime >= 2)
		{
			TempPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}
		m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
		m_pTransform->Set_State(CTransform::STATE_POSITION, TempPos);

	}

	if (m_SoundTime >= 2 && false == m_SoundSwitch)
	{
		GetSingle(CGameInstance)->Stop_AllChannel();

		GetSingle(CGameInstance)->PlayBGM((L"Clear.mp3"), 0.7);

		//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_UI, 1);
		GetSingle(CGameInstance)->PlaySound((L"Victory.wav"), CHANNEL_UI);

		m_SoundSwitch = true;
	}

	if (m_AccumulationTime > m_UI_UniversalDesc.dDuration)
	{
		m_iDead = OBJ_DEAD;
	}

	return S_OK;
}

HRESULT CUI_Universal::Talk_Box(_double TimeDelta)
{
	_vector TempPos;

	m_AccumulationTime += TimeDelta;
	m_dEasingTime += TimeDelta;
	if (m_AccumulationTime < 2)
	{
		TempPos = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, XMVectorSet(700.f, -150.f, 0.f, 1.f), XMVectorSet(0.f, -150.f, 0.f, 1.f), m_dEasingTime, 2.0f);

		m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
		m_pTransform->Set_State(CTransform::STATE_POSITION, TempPos);
	}
	else {
		TempPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		m_bLevelChangeOn = true;
	}

	if (m_AccumulationTime > m_UI_UniversalDesc.dDuration)
	{
		m_iDead = OBJ_DEAD;
	}


	if (true == m_bLevelChangeOn)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		if (pGameInstance->Get_DIKeyState(DIK_RETURN) & DIS_Down)
		{
			switch (m_eNowLevelNum)
			{
			case LEVEL_STAGE1:
			{
				CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

				CLevel* TempLevel = pGameInstance->Get_Level();
				TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE1_BOSSROOM);

				pGameInstance->Stop_AllChannel();
				pGameInstance->Release_Light();

				RELEASE_INSTANCE(CGameInstance);
				break;
			}
			case LEVEL_STAGE1_BOSSROOM:
			{
				CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

				CLevel* TempLevel = pGameInstance->Get_Level();
				TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE2);

				pGameInstance->Stop_AllChannel();
				pGameInstance->Release_Light();

				RELEASE_INSTANCE(CGameInstance);
				break;
			}
			case LEVEL_STAGE2:
			{
				CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

				CLevel* TempLevel = pGameInstance->Get_Level();
				TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE2_BOSSROOM);

				pGameInstance->Stop_AllChannel();
				pGameInstance->Release_Light();

				RELEASE_INSTANCE(CGameInstance);
				break;
			}
			case LEVEL_STAGE2_BOSSROOM:
			{
				CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

				CUI_Universal::UI_Universal_DESC UI_UniversalDesc;

				UI_UniversalDesc.iUI_Number = CUI_Universal::ENDINGCREDITS;
				UI_UniversalDesc.fSizeX = g_iWinCX;
				UI_UniversalDesc.fSizeY = g_iWinCY;
				UI_UniversalDesc.fX = g_iWinCX/2;
				UI_UniversalDesc.fY = g_iWinCY/2;
				UI_UniversalDesc.fDepth = 1.f;
				UI_UniversalDesc.iTextureIndex = 0;
				UI_UniversalDesc.pGameObject = this;
				UI_UniversalDesc.dDuration = 4;


				if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_EndingCredits"), TEXT("Prototype_GameObject_UI_Universal"), &UI_UniversalDesc)))
					return E_FAIL;

				pGameInstance->Stop_AllChannel();
				pGameInstance->Release_Light();

				RELEASE_INSTANCE(CGameInstance);

				m_iDead = OBJ_DEAD;
				break;
			}
			default:
				break;
			}
		}
		RELEASE_INSTANCE(CGameInstance);
	}

	return S_OK;
}

HRESULT CUI_Universal::Frame_Speed(_double TimeDelta)
{
	switch (m_UI_UniversalDesc.iUI_Number)
	{
	case DASH_STRAIGHT:
		m_dDeltaTime = TimeDelta * 3;
		break;
	case DASH_RIGHT:
		m_dDeltaTime = TimeDelta * 3;
		break;
	case DASH_LEFT:
		m_dDeltaTime = TimeDelta * 3;
		break;
	default:
		m_dDeltaTime = TimeDelta;
		break;
	}
	return S_OK;
}

HRESULT CUI_Universal::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_pTexture_UI_Tag[m_UI_UniversalDesc.iUI_Number], TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	return S_OK;
}

HRESULT CUI_Universal::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	switch (m_UI_UniversalDesc.iUI_Number)
	{
	case TALK_BOX:
		if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", m_UI_UniversalDesc.iTextureIndex)))
			return E_FAIL;
		break;
	default:
		if (FAILED(m_pTextureCom->SetUp_OnShader_AutoFrame(m_pShaderCom, "g_DiffuseTexture", m_dDeltaTime, m_UI_UniversalDesc.iTextureIndex)))
			return E_FAIL;
		break;
	}



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CUI_Universal * CUI_Universal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_Universal*	pInstance = new CUI_Universal(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUI_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Universal::Clone(void * pArg)
{
	CUI_Universal*	pInstance = new CUI_Universal(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUI_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Universal::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
