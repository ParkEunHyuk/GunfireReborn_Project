#pragma once

#include "Client_Defines.h"
#include "UI.h"
#include "LandObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)

class CUniversal_TextureEffect final : public CLandObject
{
public:
	enum EFFECTTYPE {
		PISTOL_EFFECT,
		PISTOL_MUZZLE_EFFECT,
		SHOTGUN_EFFECT,
		SHOTGUN_MUZZLE_EFFECT,
		SNIPER_EFFECT,
		ENTER_EFFECT,
		TORCHLIGHT_EFFECT,
		SLOW_EFFECT,
		EFFECT_END
	};

public:
	typedef struct tagUniversal_TextureEffectDesc
	{
		_uint			iEffectNumber;   //_uint형으로 불렛 타입을 지정 중

		_float3			vScale;
		_float3			vLook;
		_float4			vRotation;
		_float4			vTranslation;

		CGameObject*	pObject = nullptr; //플레이어가 아닌 몬스터 총알을 만들 때 쓰이는 정보
		_float3			fPositioning;			//만약 총알을 쓰는게 몬스터일 경우 vTranslation을 사용하지 않고 fPositioning를 사용한다. (트랜스폼은 게임 오브젝트로 가져온다.)

		_uint			iTextureIndex = 0;

		_double			dDuration; //지속시간
	}UNIVERSAL_EFFECTDESC;
private:
	explicit CUniversal_TextureEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUniversal_TextureEffect(const CUniversal_TextureEffect& rhs);
	virtual ~CUniversal_TextureEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT	SetUp_Info();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	HRESULT	SetUp_Effect(_double TimeDelta);

private:
	HRESULT				Billboard(); //빌보드


private:
	HRESULT				Pistol_Effect(_double TimeDelta);
	HRESULT				Pistol_Muzzle(_double TimeDelta);
	HRESULT				Shotgun_Muzzle(_double TimeDelta);
	HRESULT				Enter_Effect(_double TimeDelta);





private: //데드 패턴
	virtual	_int		Dead_Trigger()override;

public:
	UNIVERSAL_EFFECTDESC	m_Universal_EffectDesc;

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	class CTransform*	m_pPlayerTransform = nullptr;

	_double				m_dDeltaTime = 0; //지속시간을 체크하기 위한 변수
	_double				m_dTextureTime = 0; //텍스쳐의 교체 타이밍을 위한 변수

	//거리
	_float				m_fDistance = 0;


public:
	static CUniversal_TextureEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END
