#pragma once

#include "Client_Defines.h"
#include "UI.h"
#include "LandObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CItem_Universal final : public CLandObject
{
public:
	enum ITEMTYPE {
		ITEM_HP,
		ITEM_MP,
		ITEM_GREENBULLET,
		ITEM_BLUEBULLET,
		ITEM_END
	};
public:
	typedef struct tagItem_UniversalDesc {
		_uint	iItemNumber;
		_float3	vScale;
		_float4	vRotation;
		_float4	vTranslation;
	}ITEM_UNIVERSALDESC;
private:
	CItem_Universal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CItem_Universal(const CItem_Universal& rhs);
	virtual ~CItem_Universal() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	HRESULT				Trace(_double TimeDelta); //���� ����
	HRESULT				FollowMe(_double TimeDelta); // �� ���� �޷���

private:
	HRESULT				Random_Pos();
	HRESULT				Easing(_double TimeDelta);
	HRESULT				Animation(_double TimeDelta);

private:
	virtual	_int Dead_Trigger()override;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;

	class CTransform*	m_pPlayerTransform = nullptr;

	ITEM_UNIVERSALDESC	m_Item_UniversalDesc;

	_double				m_dDeltaTime = 0; //�������� �����ִ� ���ӽð�

	_float				m_fDistance = 0; //�÷��̾���� �Ÿ�
	_bool				m_bTraceOn = false; //���� ������ ������ On��
	_bool				m_bDisappearOn = false; //�������� �Ծ����� �ƴϸ� �ð��� ���� ����������� Ȯ���ϴ� �Ұ�


	//������ ���õ� �����
	_float4				fTargetFirstPos;
	_float4				fTargetSecondPos;
	_double				m_dTimer = 0;
	_double				m_dEasingTime = 0;
	_bool				m_bEasingFirst = true; //������ ù��°�� ������ false�� �ι�° ���� �غ�

	_bool				m_bEasingOff = false; //������ �������� �˸�

	//�ִϸ��̼� ����
	_bool				m_bOneTimeSwith = true;
	_uint				m_iOnceAnimNumber = 0;
	_uint				m_iInfinityAnimNumber = 1;

public:
	static CItem_Universal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END