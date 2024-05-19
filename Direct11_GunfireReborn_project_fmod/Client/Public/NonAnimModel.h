#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CRenderer;
END

BEGIN(Client)
class CNonAnimModel final : public CLandObject
{
private:
	explicit CNonAnimModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CNonAnimModel(const CNonAnimModel& rhs);
	virtual ~CNonAnimModel() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT				Culling_Range();

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	static CNonAnimModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject * Clone(void * pArg) override;
	virtual void Free() override;
};

END