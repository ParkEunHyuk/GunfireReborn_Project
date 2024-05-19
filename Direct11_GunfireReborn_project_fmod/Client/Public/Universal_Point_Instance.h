#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

class CUniversal_Point_Instance final : public CGameObject
{
public:
	enum TEXTURETYPE {
		RAIN_PARTICLE,   //레인 파티클
		SNOW_PARTICLE,   //스노우 파티클
		GLOW_RED_PARTICLE, //글로우 레드 파티클
		SMALL_GLOW_RED_PARTICLE, //스몰 글로우 레드 파티클
		SMALL_GLOW_BLUE_PARTICLE, //스몰 글로우 블루 파티클
		SMALL_GLOW_PURPLE_PARTICLE, //스몰 글로우 퍼플 파티클
		SMALL_GLOW_GREEN_PARTICLE, //스몰 글로우 그린 파티클
		SMALL_GLOW_YELLOW_PARTICLE, //스몰 글로우 옐로우 파티클
		EMBERS_PARTICLE, //불씨 파티클
		FIRE_PARTICLE, //불 파티클
		FIRE_ROCK_PARTICLE, //불 바위 파티클
		GREEN_SHOTGUN_PARTICLE,//초록색 파티클
		SLOW_PARTICLE,
		BLUE_ROCK_PARTICLE_0,
		BLUE_ROCK_PARTICLE_1,
		BLUE_ROCK_PARTICLE_2,
		BLUE_ROCK_PARTICLE_3,
		DEVIL_ROCK_PARTICLE_1,
		BLACK_ROCK_PARTICLE_0,
		WHITE_ROCK_PARTICLE_0,
		PARTICEL_END
	};

public:
	typedef struct tagUniversal_Point_InstanceDesc
	{
		_uint		iParticleTexture; //파티클 사진
		_uint		iParticleType; //파티클 타입

		_float		fMinSpeed; //최소 시간
		_float		fMaxSpeed; //최대 시간

		_float3		fRange; //범위
		_float3		fDirection; //방향
		_float3		fPositioning; //위치

		_float		fSize = 1; //사이즈
		_uint		iTextureIndex; //인덱스를 넣어야한다. 즉,텍스쳐의 갯수가 10개 있으면 인덱스 9

		void*		pObject = nullptr;

		_double		dDuration; //지속시간
	}UNIVERSAL_POINT_INSTANCEDESC;

private:
	CUniversal_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUniversal_Point_Instance(const CUniversal_Point_Instance& rhs);
	virtual ~CUniversal_Point_Instance() = default;

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
	HRESULT SetUp_Info(); //Universal_Point_Instance를 설정해주고 있음
	HRESULT SetUp_Instance(); //인스턴스 버퍼를 설정중
	HRESULT	SetUp_Texture(); //텍스쳐를 설정중

	HRESULT Play_Particle(_double TimeDelta); //파티클의 포지션을 잡아주고 있음

private://각각 파티클들
	HRESULT	Rain_Particle(); //레인 파티클
	HRESULT	Snow_Particle(); //스노우 파티클
	HRESULT	Boom_Red_Particle(); //레드 파티클 나머지 글로우 파티클들은 만들기 귀찮아서 switch 디폴트에 넣어둠
	HRESULT	Green_Shotgun_Particle(); //초록색 샷건을 위한 파티클
	HRESULT	Slow_Particle(_double TimeDelta); //슬로우 파티클을 위한 파티클
	HRESULT	Devil_Die_Particle();
	HRESULT	Devil_Eye_Die_Particle();
	HRESULT	BlackAndWhite_Particle(_double TimeDelta);


private:
	UNIVERSAL_POINT_INSTANCEDESC m_Universal_Point_InstanceDesc;

	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Point_Instance*	m_pVIBufferCom = nullptr;

	CTransform*					m_pParticleTransform = nullptr;

	_float4						m_fTempPos;


	_double						m_dDurationTime = 0;
	_double						m_dTextureTime = 0;


public:
	static CUniversal_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END