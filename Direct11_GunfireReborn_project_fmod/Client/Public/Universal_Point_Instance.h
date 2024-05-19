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
		RAIN_PARTICLE,   //���� ��ƼŬ
		SNOW_PARTICLE,   //����� ��ƼŬ
		GLOW_RED_PARTICLE, //�۷ο� ���� ��ƼŬ
		SMALL_GLOW_RED_PARTICLE, //���� �۷ο� ���� ��ƼŬ
		SMALL_GLOW_BLUE_PARTICLE, //���� �۷ο� ��� ��ƼŬ
		SMALL_GLOW_PURPLE_PARTICLE, //���� �۷ο� ���� ��ƼŬ
		SMALL_GLOW_GREEN_PARTICLE, //���� �۷ο� �׸� ��ƼŬ
		SMALL_GLOW_YELLOW_PARTICLE, //���� �۷ο� ���ο� ��ƼŬ
		EMBERS_PARTICLE, //�Ҿ� ��ƼŬ
		FIRE_PARTICLE, //�� ��ƼŬ
		FIRE_ROCK_PARTICLE, //�� ���� ��ƼŬ
		GREEN_SHOTGUN_PARTICLE,//�ʷϻ� ��ƼŬ
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
		_uint		iParticleTexture; //��ƼŬ ����
		_uint		iParticleType; //��ƼŬ Ÿ��

		_float		fMinSpeed; //�ּ� �ð�
		_float		fMaxSpeed; //�ִ� �ð�

		_float3		fRange; //����
		_float3		fDirection; //����
		_float3		fPositioning; //��ġ

		_float		fSize = 1; //������
		_uint		iTextureIndex; //�ε����� �־���Ѵ�. ��,�ؽ����� ������ 10�� ������ �ε��� 9

		void*		pObject = nullptr;

		_double		dDuration; //���ӽð�
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
	HRESULT SetUp_Info(); //Universal_Point_Instance�� �������ְ� ����
	HRESULT SetUp_Instance(); //�ν��Ͻ� ���۸� ������
	HRESULT	SetUp_Texture(); //�ؽ��ĸ� ������

	HRESULT Play_Particle(_double TimeDelta); //��ƼŬ�� �������� ����ְ� ����

private://���� ��ƼŬ��
	HRESULT	Rain_Particle(); //���� ��ƼŬ
	HRESULT	Snow_Particle(); //����� ��ƼŬ
	HRESULT	Boom_Red_Particle(); //���� ��ƼŬ ������ �۷ο� ��ƼŬ���� ����� �����Ƽ� switch ����Ʈ�� �־��
	HRESULT	Green_Shotgun_Particle(); //�ʷϻ� ������ ���� ��ƼŬ
	HRESULT	Slow_Particle(_double TimeDelta); //���ο� ��ƼŬ�� ���� ��ƼŬ
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