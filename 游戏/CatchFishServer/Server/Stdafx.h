#pragma once

//////////////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//开发环境

//平台环境
#include "../../../系统模块/服务器组件/游戏服务/GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

#define ERROR_LOG do { printf("error %s %d\n",__FUNCTION__,__LINE__); } while (0);

#define D3DX_PI    ((FLOAT)  3.141592654f)
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))

#include <string>
#include <vector>
#include <algorithm>
#include "math.h"

#include "../tinyxml/tinyxml.h"

#include "../消息定义/CMD_Game.h"
#include "Resource.h"

#include "Array.h"
#include "factory.h"

#include "GameServiceManager.h"
#include "MatchManager.h"
#include "TableFrameSink.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////////////
