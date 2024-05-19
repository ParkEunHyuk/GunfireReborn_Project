#include "stdafx.h"
#include "..\Public\Universal_TextureEffect.h"
#include "GameInstance.h"

const 	_tchar*	 m_pTextureEffectTag[CUniversal_TextureEffect::EFFECT_END] = {
	TEXT("Prototype_Component_Texture_Universal_TextureEffect_Pistol"),
	TEXT("Prototype_Component_Texture_Universal_TextureEffect_Pistol_Muzzle"),
	TEXT("Prototype_Component_Texture_Universal_TextureEffect_Shotgun"),
	TEXT("Prototype_Component_Texture_Universal_TextureEffect_Shotgun_Muzzle"),
	TEXT("Prototype_Component_Texture_Universal_TextureEffect_Sniper"),
	TEXT("Prototype_Component_Texture_Universal_TextureEffect_Enter"),
	TEXT("Prototype_Component_Texture_Universal_TextureEffect_Torchlight"),
	TEXT("Prototype_Component_Texture_Universal_TextureEffect_Slow")
};


CUniversal_TextureEffect::CUniversal_TextureEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CUniversal_TextureEffect::CUniversal_TextureEffect(const CUniversal_TextureEffect & rhs)
	: CLandObject(rhs)
{
}

HRESULT CUniversal_TextureEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUniversal_TextureEffect::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Universal_EffectDesc, pArg, sizeof(UNIVERSAL_EFFECTDESC));
	}
	else
	{
		MSGBOX("CUniversal_TextureEffect에 pArg가 Nullptr입니다.");
	}

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	SetUp_Info();

	return S_OK;
}

_int CUniversal_TextureEffect::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;
	if (m_iDead == OBJ_DEAD)
	{
		return m_iDead;
	}
	m_dTextureTime = TimeDelta;

	SetUp_Effect(TimeDelta);





	m_dDeltaTime += TimeDelta;

	if (m_dDeltaTime > m_Universal_EffectDesc.dDuration)
	{
		return OBJ_DEAD;
	}


	return m_iDead;
}

_int CUniversal_TextureEffect::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CUniversal_TextureEffect::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 1);


	return S_OK;
}

HRESULT CUniversal_TextureEffect::SetUp_Info()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));
	
	if (nullptr != m_Universal_EffectDesc.pObject)
	{
		CTransform* pEffectTransform = static_cast<CTransform*>(m_Universal_EffectDesc.pObject->Get_Component(TEXT("Com_Transform")));

		m_pTransform->Scaled(XMLoadFloat3(&m_Universal_EffectDesc.vScale));

		_vector vRight = pEffectTransform->Get_State(CTransform::STATE_RIGHT);
		_vector vUp = pEffectTransform->Get_State(CTransform::STATE_UP);
		_vector vLook = pEffectTransform->Get_State(CTransform::STATE_LOOK);

		vRight = m_pTransform->Get_Scale(CTransform::STATE_RIGHT) * XMVector3Normalize(vRight);
		vUp = m_pTransform->Get_Scale(CTransform::STATE_UP) * XMVector3Normalize(vUp);
		vLook = m_pTransform->Get_Scale(CTransform::STATE_LOOK) * XMVector3Normalize(vLook);

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

		_vector		vPosition = pEffectTransform->Get_State(CTransform::STATE_POSITION);

		vPosition += XMVector3Normalize(pEffectTransform->Get_State(CTransform::STATE_RIGHT)) *m_Universal_EffectDesc.fPositioning.x;
		vPosition += XMVector3Normalize(pEffectTransform->Get_State(CTransform::STATE_UP)) * m_Universal_EffectDesc.fPositioning.y;
		vPosition += XMVector3Normalize(pEffectTransform->Get_State(CTransform::STATE_LOOK)) * m_Universal_EffectDesc.fPositioning.z;

		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	}
	else {
		m_pTransform->Scaled(XMLoadFloat3(&m_Universal_EffectDesc.vScale));

		m_pTransform->Set_State(CTransform::STATE_POSITION,XMVectorSet(m_Universal_EffectDesc.fPositioning.x, m_Universal_EffectDesc.fPositioning.y, m_Universal_EffectDesc.fPositioning.z,1.f));
	}
	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

HRESULT CUniversal_TextureEffect::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_pTextureEffectTag[m_Universal_EffectDesc.iEffectNumber], TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUniversal_TextureEffect::SetUp_ConstantTable()
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


	if (FAILED(m_pTextureCom->SetUp_OnShader_AutoFrame(m_pShaderCom, "g_DiffuseTexture", m_dTextureTime*10, m_Universal_EffectDesc.iTextureIndex))) //최대 갯수의 인덱스
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUniversal_TextureEffect::SetUp_Effect(_double TimeDelta)
{

	switch (m_Universal_EffectDesc.iEffectNumber)
	{
	case PISTOL_EFFECT:
	{
		Pistol_Effect(TimeDelta);
		Billboard();
		break;
	}
	case PISTOL_MUZZLE_EFFECT:
	{
		Pistol_Muzzle(TimeDelta);
		Billboard();
		break;
	}
	case SHOTGUN_EFFECT:
	{
		Billboard();
		break;
	}
	case SHOTGUN_MUZZLE_EFFECT:
	{
		Shotgun_Muzzle(TimeDelta);
		Billboard();
		break;
	}
	case SNIPER_EFFECT:
	{
		m_pTransform->Scaling(1,70, TimeDelta);
		Billboard();
		break;
	}
	case ENTER_EFFECT:
	{
		Enter_Effect(TimeDelta);
		break;
	}
	default:
		Billboard();
		break;
	}
	return S_OK;
}

_int CUniversal_TextureEffect::Dead_Trigger()
{
	return _int();
}

HRESULT CUniversal_TextureEffect::Billboard()
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

HRESULT CUniversal_TextureEffect::Pistol_Effect(_double TimeDelta)
{
	m_pTransform->Scaling(2,10, TimeDelta);
	return S_OK;
}

HRESULT CUniversal_TextureEffect::Pistol_Muzzle(_double TimeDelta)
{
	CTransform* pEffectTransform = static_cast<CTransform*>(m_Universal_EffectDesc.pObject->Get_Component(TEXT("Com_Transform")));

	m_pTransform->Scaled(XMLoadFloat3(&m_Universal_EffectDesc.vScale));

	_vector vRight = pEffectTransform->Get_State(CTransform::STATE_RIGHT);
	_vector vUp = pEffectTransform->Get_State(CTransform::STATE_UP);
	_vector vLook = pEffectTransform->Get_State(CTransform::STATE_LOOK);

	vRight = m_pTransform->Get_Scale(CTransform::STATE_RIGHT) * XMVector3Normalize(vRight);
	vUp = m_pTransform->Get_Scale(CTransform::STATE_UP) * XMVector3Normalize(vUp);
	vLook = m_pTransform->Get_Scale(CTransform::STATE_LOOK) * XMVector3Normalize(vLook);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

	_vector		vPosition = pEffectTransform->Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(pEffectTransform->Get_State(CTransform::STATE_RIGHT)) *m_Universal_EffectDesc.fPositioning.x;
	vPosition += XMVector3Normalize(pEffectTransform->Get_State(CTransform::STATE_UP)) * m_Universal_EffectDesc.fPositioning.y;
	vPosition += XMVector3Normalize(pEffectTransform->Get_State(CTransform::STATE_LOOK)) * m_Universal_EffectDesc.fPositioning.z;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CUniversal_TextureEffect::Shotgun_Muzzle(_double TimeDelta)
{
	CTransform* pEffectTransform = static_cast<CTransform*>(m_Universal_EffectDesc.pObject->Get_Component(TEXT("Com_Transform")));

	m_pTransform->Scaled(XMLoadFloat3(&m_Universal_EffectDesc.vScale));

	_vector vRight = pEffectTransform->Get_State(CTransform::STATE_RIGHT);
	_vector vUp = pEffectTransform->Get_State(CTransform::STATE_UP);
	_vector vLook = pEffectTransform->Get_State(CTransform::STATE_LOOK);

	vRight = m_pTransform->Get_Scale(CTransform::STATE_RIGHT) * XMVector3Normalize(vRight);
	vUp = m_pTransform->Get_Scale(CTransform::STATE_UP) * XMVector3Normalize(vUp);
	vLook = m_pTransform->Get_Scale(CTransform::STATE_LOOK) * XMVector3Normalize(vLook);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

	_vector		vPosition = pEffectTransform->Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(pEffectTransform->Get_State(CTransform::STATE_RIGHT)) *m_Universal_EffectDesc.fPositioning.x;
	vPosition += XMVector3Normalize(pEffectTransform->Get_State(CTransform::STATE_UP)) * m_Universal_EffectDesc.fPositioning.y;
	vPosition += XMVector3Normalize(pEffectTransform->Get_State(CTransform::STATE_LOOK)) * m_Universal_EffectDesc.fPositioning.z;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CUniversal_TextureEffect::Enter_Effect(_double TimeDelta)
{
	_vector vDis = m_pTransform->Get_Distance(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	m_fDistance;

	XMStoreFloat(&m_fDistance, vDis);
	if (m_fDistance > 11)
	{
		m_iDead = OBJ_DEAD;
	}

	Billboard();

	m_pTransform->Scaling(1,1, TimeDelta);

	return S_OK;
}

CUniversal_TextureEffect * CUniversal_TextureEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUniversal_TextureEffect*	pInstance = new CUniversal_TextureEffect(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUniversal_TextureEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUniversal_TextureEffect::Clone(void * pArg)
{
	CUniversal_TextureEffect*	pInstance = new CUniversal_TextureEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CUniversal_TextureEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUniversal_TextureEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
