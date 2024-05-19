#pragma once

#pragma warning (disable : 4251)

#include <d3d11.h>
#include <DirectXMath.h>
#include <typeinfo>

#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include <DirectXCollision.h>

#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/Effects.h>

#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/ScreenGrab.h> //��ũ�������
#include <wincodec.h> //�̰� �־�� ��ũ���� ����

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

using namespace DirectX;
using namespace Assimp;


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <process.h>

#include <string>
#include <fstream>
#include <tchar.h>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

/////////���� �������
#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

////////���� ���� ���/////////////
#include <io.h>
#include "fmod.h"
//#include "fmod.hpp"
#pragma comment(lib, "fmod_vc.lib")

//IMGUI�� ���� �ּ� == // Clinet_Define���� �ű�

// �޸� ������ üũ�Ϸ��� #define USE_IMGUI ������ �ּ�ó���ϱ�
#ifdef _DEBUG
#define USE_IMGUI
#endif

// USE_IMGUI �� define �Ǿ����� ���� ������ �޸� ���� üũ
#if !defined(USE_IMGUI)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif // _DEBUG
#endif // DBG_NEW
#endif // !defined(USE_IMGUI)



//#ifdef _DEBUG
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#ifndef DBG_NEW 
//
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//
//#endif
//
//#endif // _DEBUG


#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Engine_Enum.h"
#include "Engine_Easing.h"

using namespace Engine;
