#include "stdafx.h"
#include "..\Public\Universal_Point_Instance.h"
#include "GameInstance.h"
#include "UI_Universal.h"

const 	_tchar*	 m_pParticleTag[CUniversal_Point_Instance::PARTICEL_END] = {
	TEXT("Prototype_Component_Texture_Universal_Particle_Rain"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Snow"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Glow_Red"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Small_Glow_Red"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Small_Glow_Blue"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Small_Glow_Purple"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Small_Glow_Green"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Small_Glow_Yellow"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Embers"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Fire"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Fire_Rock"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Green_Rock"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Slow"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Blue_Rock_0"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Blue_Rock_1"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Blue_Rock_2"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Blue_Rock_3"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Fire_Rock"),
	TEXT("Prototype_Component_Texture_Universal_Particle_Black_Rock_0"),
	TEXT("Prototype_Component_Texture_Universal_Particle_White_Rock_0")
};


CUniversal_Point_Instance::CUniversal_Point_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CUniversal_Point_Instance::CUniversal_Point_Instance(const CUniversal_Point_Instance & rhs)
	: CGameObject(rhs)
{

}

HRESULT CUniversal_Point_Instance::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUniversal_Point_Instance::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Universal_Point_InstanceDesc, pArg, sizeof(UNIVERSAL_POINT_INSTANCEDESC));
	}
	else
	{
		MSGBOX("CUniversal_Point_Instance에 pArg가 Nullptr입니다.");
	}

	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.0f)))))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pParticleTransform = static_cast<CTransform*>(static_cast<CGameObject*>(m_Universal_Point_InstanceDesc.pObject)->Get_Component(TEXT("Com_Transform")));


	SetUp_Info();

	return S_OK;
}

_int CUniversal_Point_Instance::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_dTextureTime = TimeDelta;

	Play_Particle(TimeDelta);


	return m_iDead;
}

_int CUniversal_Point_Instance::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pVIBufferCom->Update(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

	return _int();
}

HRESULT CUniversal_Point_Instance::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return S_OK;
}

HRESULT CUniversal_Point_Instance::SetUp_Info()
{
	XMStoreFloat4(&m_fTempPos,m_pParticleTransform->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

HRESULT CUniversal_Point_Instance::SetUp_Instance()
{
	/* For.Com_VIBuffer */
	CVIBuffer_Point_Instance::PARTICLEDESC		ParticleDesc;
	ParticleDesc.ParticleType = m_Universal_Point_InstanceDesc.iParticleType;
	ParticleDesc.fMinSpeed = m_Universal_Point_InstanceDesc.fMinSpeed;
	ParticleDesc.fMaxSpeed = m_Universal_Point_InstanceDesc.fMaxSpeed;
	ParticleDesc.fRange = m_Universal_Point_InstanceDesc.fRange;
	ParticleDesc.fDirection = m_Universal_Point_InstanceDesc.fDirection;
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_VIBuffer_Point_Instance"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, &ParticleDesc)))
		return E_FAIL;

	return S_OK;

}

HRESULT CUniversal_Point_Instance::SetUp_Texture()
{
	switch (m_Universal_Point_InstanceDesc.iParticleTexture)
	{
	case EMBERS_PARTICLE:
	{
		FAILED_CHECK(m_pTextureCom->SetUp_OnShader_AutoFrame(m_pShaderCom, "g_DiffuseTexture",m_dTextureTime, m_Universal_Point_InstanceDesc.iTextureIndex));
		break;
	}
	default:
	{
		if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
		break;
	}
	}

	return S_OK;
}

HRESULT CUniversal_Point_Instance::Play_Particle(_double TimeDelta)
{

	switch (m_Universal_Point_InstanceDesc.iParticleTexture)
	{
	case RAIN_PARTICLE:
	{
		Rain_Particle();
		break;
	}
	case SNOW_PARTICLE:
	{
		Snow_Particle();
		break;
	}
	case GLOW_RED_PARTICLE:
	{
		Boom_Red_Particle();
		m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta*10);
		break;
	}
	case SMALL_GLOW_RED_PARTICLE:
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fTempPos));
		m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * 10);
		break;
	}
	case SMALL_GLOW_BLUE_PARTICLE:
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fTempPos));
		m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * 10);
		break;
	}
	case SMALL_GLOW_PURPLE_PARTICLE:
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fTempPos));
		m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * 10);
		break;
	}
	case SMALL_GLOW_GREEN_PARTICLE:
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fTempPos));
		m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * 10);
		break;
	}
	case SMALL_GLOW_YELLOW_PARTICLE:
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fTempPos));
		m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * 10);
		break;
	}
	case GREEN_SHOTGUN_PARTICLE:
	{
		Green_Shotgun_Particle();
		break;
	}
	case SLOW_PARTICLE:
	{
		Slow_Particle(TimeDelta);
		break;
	}
	case BLUE_ROCK_PARTICLE_0:
	{
		Devil_Die_Particle();
		break;
	}
	case BLUE_ROCK_PARTICLE_1:
	{
		Devil_Die_Particle();
		break;
	}
	case BLUE_ROCK_PARTICLE_2:
	{
		Devil_Die_Particle();
		break;
	}
	case BLUE_ROCK_PARTICLE_3:
	{
		Devil_Die_Particle();
		break;
	}
	case DEVIL_ROCK_PARTICLE_1:
	{
		Devil_Eye_Die_Particle();
		break;
	}
	case BLACK_ROCK_PARTICLE_0:
	{
		BlackAndWhite_Particle(TimeDelta);
		break;
	}
	case WHITE_ROCK_PARTICLE_0:
	{
		BlackAndWhite_Particle(TimeDelta);
		break;
	}
	default:
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fTempPos));
		break;
	}
	}

	m_dDurationTime += TimeDelta;

	if (m_dDurationTime > m_Universal_Point_InstanceDesc.dDuration)
	{
		m_iDead = OBJ_DEAD;
	}

	return S_OK;
}

HRESULT CUniversal_Point_Instance::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_PointInstance"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/*For.Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_pParticleTag[m_Universal_Point_InstanceDesc.iParticleTexture], TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	SetUp_Instance();

	return S_OK;
}

HRESULT CUniversal_Point_Instance::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	FAILED_CHECK(m_pTextureCom->SetUp_OnShader_AutoFrame(m_pShaderCom, "g_DiffuseTexture", m_dTextureTime*10, m_Universal_Point_InstanceDesc.iTextureIndex));
	//SetUp_Texture();

	_float4		vCamPosition;
	XMStoreFloat4(&vCamPosition, pGameInstance->Get_CamPosition());

	m_pShaderCom->Set_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4));

	m_pShaderCom->Set_RawValue("g_fSize", &m_Universal_Point_InstanceDesc.fSize,sizeof(_float) );

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUniversal_Point_Instance::Rain_Particle()
{
	_vector vPos = m_pParticleTransform->Get_State(CTransform::STATE_POSITION);

	_float4 fTemPos;

	XMStoreFloat4(&fTemPos, vPos);

	fTemPos.x -= 5;
	fTemPos.y = 10;
	fTemPos.z -= 5;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&fTemPos));

	return S_OK;
}

HRESULT CUniversal_Point_Instance::Snow_Particle()
{
	_vector vPos = m_pParticleTransform->Get_State(CTransform::STATE_POSITION);

	_float4 fTemPos;

	XMStoreFloat4(&fTemPos,vPos);

	fTemPos.x -= 2;
	fTemPos.y = 10;
	fTemPos.z -= 2;

	m_pTransform->Set_State(CTransform::STATE_POSITION,XMLoadFloat4(&fTemPos));

	return S_OK;
}

HRESULT CUniversal_Point_Instance::Boom_Red_Particle()
{

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fTempPos));

	return S_OK;
}

HRESULT CUniversal_Point_Instance::Green_Shotgun_Particle()
{

	_vector vPosition = m_pParticleTransform->Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(m_pParticleTransform->Get_State(CTransform::STATE_RIGHT)) *m_Universal_Point_InstanceDesc.fPositioning.x;
	vPosition += XMVector3Normalize(m_pParticleTransform->Get_State(CTransform::STATE_UP)) * m_Universal_Point_InstanceDesc.fPositioning.y;
	vPosition += XMVector3Normalize(m_pParticleTransform->Get_State(CTransform::STATE_LOOK)) * m_Universal_Point_InstanceDesc.fPositioning.z;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CUniversal_Point_Instance::Slow_Particle(_double TimeDelta)
{
	_vector vPos = m_pParticleTransform->Get_State(CTransform::STATE_POSITION);

	_float4 fTemPos;

	XMStoreFloat4(&fTemPos, vPos);

	fTemPos.x -= 3.8;
	fTemPos.y = 0;
	fTemPos.z -= 3.8f;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&fTemPos));

	//m_pTransform->Turn(XMVectorSet(0.f,1.f,0.f,0.f), TimeDelta);

	return S_OK;
}

HRESULT CUniversal_Point_Instance::Devil_Die_Particle()
{
	_float4 fNewPosition;

	fNewPosition.x = m_fTempPos.x + m_Universal_Point_InstanceDesc.fPositioning.x;
	fNewPosition.y = m_fTempPos.y + m_Universal_Point_InstanceDesc.fPositioning.y;
	fNewPosition.z = m_fTempPos.z + m_Universal_Point_InstanceDesc.fPositioning.z;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&fNewPosition));

	return S_OK;
}

HRESULT CUniversal_Point_Instance::Devil_Eye_Die_Particle()
{
	_float4 fNewPosition;

	fNewPosition.x = m_fTempPos.x + m_Universal_Point_InstanceDesc.fPositioning.x;
	fNewPosition.y = m_fTempPos.y + m_Universal_Point_InstanceDesc.fPositioning.y;
	fNewPosition.z = m_fTempPos.z + m_Universal_Point_InstanceDesc.fPositioning.z;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&fNewPosition));

	return S_OK;
}

HRESULT CUniversal_Point_Instance::BlackAndWhite_Particle(_double TimeDelta)
{
	//_float4 fNewPosition;

	//fNewPosition.x = m_fTempPos.x; + m_Universal_Point_InstanceDesc.fPositioning.x;
	//fNewPosition.y = m_fTempPos.y; + m_Universal_Point_InstanceDesc.fPositioning.y;
	//fNewPosition.z = m_fTempPos.z; + m_Universal_Point_InstanceDesc.fPositioning.z;

	//m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&fNewPosition));

	_vector vPos = m_pParticleTransform->Get_State(CTransform::STATE_POSITION);

	_float4 fTemPos;

	XMStoreFloat4(&fTemPos, vPos);

	fTemPos.x += m_Universal_Point_InstanceDesc.fPositioning.x;
	fTemPos.y += m_Universal_Point_InstanceDesc.fPositioning.y;
	fTemPos.z += m_Universal_Point_InstanceDesc.fPositioning.z;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&fTemPos));


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUI_Universal* Hp = static_cast<CUI_Universal*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Hp")));

	if (0 >= Hp->Get_SizeX()) //이거 겟 사이즈 X 받아올 때 ui 체력바 삭제 시키는바람에 호출이 안됨 그래서 체력바 m_idead를 없애버림 즉, 없어진척함
	{
		m_iDead = OBJ_DEAD;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CUniversal_Point_Instance * CUniversal_Point_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUniversal_Point_Instance*	pInstance = new CUniversal_Point_Instance(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUniversal_Point_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUniversal_Point_Instance::Clone(void * pArg)
{
	CUniversal_Point_Instance*	pInstance = new CUniversal_Point_Instance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUniversal_Point_Instance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUniversal_Point_Instance::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}