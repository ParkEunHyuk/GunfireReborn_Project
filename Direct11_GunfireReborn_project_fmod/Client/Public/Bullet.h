#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
BEGIN(Client)

class CBullet abstract : public CLandObject
{
protected:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CBullet(const CLandObject& rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc = nullptr);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END