#include "stdafx.h"
#include "..\Public\Bullet.h"
#include "GameInstance.h"

CBullet::CBullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CBullet::CBullet(const CLandObject & rhs)
	: CLandObject(rhs)
{

}

HRESULT CBullet::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBullet::NativeConstruct(const CTransform::TRANSFORMDESC * pTransformDesc)
{
	if (FAILED(__super::NativeConstruct(pTransformDesc)))
		return E_FAIL;

	return S_OK;
}

_int CBullet::Tick(_double TimeDelta)
{
	return _int();
}

_int CBullet::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CBullet::Render()
{
	return S_OK;
}

void CBullet::Free()
{
	__super::Free();

}
