#pragma once



#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define D3D11COLOR_ARGB(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

#define GetSingle(SINGLETON) SINGLETON::GetInstance()

#define DIS_Up				0b00000001
#define DIS_Down			0b00000110
#define DIS_DoubleDown		0b00000100
#define DIS_Press			0b11111111

#define	OBJ_NOEVENT			0
#define OBJ_DEAD			1

#define GET_INSTANCE(CLASSNAME)	[](){											\
	CLASSNAME*	pInstance = CLASSNAME::GetInstance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->AddRef();}														\
	return pInstance;															\
	}();

#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
	CLASSNAME*	pInstance = CLASSNAME::GetInstance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->Release();}														\
	}();

#define BEGIN(NAMESPACE) namespace NAMESPACE { 

#define END }

#define MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK);

#define NO_COPY(CLASSNAME)										\
		private:												\
		CLASSNAME(const CLASSNAME&);							\
		CLASSNAME& operator = (const CLASSNAME&);				

#define DECLARE_SINGLETON(CLASSNAME)							\
		NO_COPY(CLASSNAME)										\
		private:												\
		static CLASSNAME*	m_pInstance;						\
		public:													\
		static CLASSNAME*	GetInstance( void );				\
		static unsigned long DestroyInstance( void );					

#define IMPLEMENT_SINGLETON(CLASSNAME)							\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;				\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
			if(nullptr == m_pInstance) {						\
				m_pInstance = new CLASSNAME;					\
			}													\
			return m_pInstance;									\
		}														\
		unsigned long CLASSNAME::DestroyInstance( void ) {		\
			unsigned long dwRefCnt = 0;							\
			if(nullptr != m_pInstance)	{						\
				dwRefCnt = m_pInstance->Release();				\
				if(0 == dwRefCnt)								\
				m_pInstance = nullptr;							\
			}													\
			return dwRefCnt;									\
		}


// ����� üũ ��ũ��
// {__asm { int 3; }; �ڵ� �������� Break ����Ʈ �ɱ� 86��Ʈ ����
// __debugbreak(); �ڵ� �������� Break ����Ʈ �ɱ� 64��Ʈ ����

// HRESULT ����üũ1 / �⺻
#define FAILED_CHECK(_hr) \
if(((HRESULT)(_hr)) < 0) { MSGBOX("Fail"); __debugbreak();\
return E_FAIL;}

// NULL üũ4 / �޼����ڽ�
#define NULL_CHECK_MSG( _ptr, _message) \
if( _ptr == 0){MessageBox(NULL, _message, L"System Message", MB_OK);__debugbreak();}