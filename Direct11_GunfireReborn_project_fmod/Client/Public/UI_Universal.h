#pragma once

#include "Client_Defines.h"
#include "UI.h"
#include "LandObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Universal final : public CUI
{
public:
	enum UITYPE {
		UI_DOKKAEBI_STAMINA,
		UI_GRIFFIN_STAMINA,
		UI_BEAR_STAMINA,
		UI_BOSS_HP,
		UI_BOSS_SHIELD,
		HIT_PLAYER,
		DASH_STRAIGHT,
		DASH_RIGHT,
		DASH_LEFT,
		VICTORY,
		TALK_BOX,
		ENDINGCREDITS,
		UI_END
	};
public:
	typedef struct tagUI_UniversalDesc
	{
		_uint		iUI_Number;
		_float		fSizeX;
		_float		fSizeY;
		_float		fX;
		_float		fY;
		_float		fDepth; //���ڰ� ū �� �ڿ� ��
		_uint		iTextureIndex; //�ؽ����� �ε����� �� ����)�ؽ��İ� 2�� �����ϸ� �ε���1
		_double		dDuration;
		void*		pGameObject = nullptr;
	}UI_Universal_DESC;
private:
	explicit CUI_Universal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Universal(const CUI_Universal& rhs);
	virtual ~CUI_Universal() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	_float	Get_SizeX() { return m_fSizeX; }

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	HRESULT				Initialize();

private:
	HRESULT				Play_UI(_double TimeDelta);

private:
	HRESULT				Boss_Stamina(); //������ ü�¹� ���(������)
	HRESULT				Boss_Shield(); //������ �ǵ忡 ���� �Լ�
	HRESULT				Boss_Hp(); //������ ü�¹�
	HRESULT				Hit_Player(_double TimeDelta); // �÷��̾� �ǰݽ� ����Ʈ
	HRESULT				Dash_Straight(_double TimeDelta); //������ ������ ����Ʈ
	HRESULT				Dash_Right(_double TimeDelta); //���������� ������ ����Ʈ
	HRESULT				Dash_Left(_double TimeDelta); //�������� ������ ����Ʈ
	HRESULT				Victory(_double TimeDelta); //���丮 ����Ʈ
	HRESULT				Talk_Box(_double TimeDelta);

private:
	HRESULT				Frame_Speed(_double TimeDelta);

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	UI_Universal_DESC	m_UI_UniversalDesc;

	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

	//�������� �ƽ�
	_float				m_fMaxSizeX;
	_float				m_fMaxSizeY;

	_float				m_fTemp;


	_double				m_dDeltaTime = 0;
	_double				m_AccumulationTime = 0;

	//Easing
	_double				m_dEasingTime = 0;

	_bool				m_bLevelChangeOn = false;

	//����
	_double				m_SoundTime = 0;
	_bool				m_SoundSwitch = false;


public:
	static CUI_Universal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END