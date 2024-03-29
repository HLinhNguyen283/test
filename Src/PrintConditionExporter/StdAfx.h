﻿/**
 * @file	stdafx.h
 * @brief	標準のシステム インクルード ファイル
 *
 *          または参照回数が多く、かつあまり変更されない
 *          プロジェクト専用のインクルード ファイルを記述します。
**/

#pragma once

// この位置にヘッダーを挿入してください
#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します

#include <windows.h>
#include <shlwapi.h>

#include "PrintNavBase.h"

// C ランタイムヘッダーファイル。
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <crtdbg.h>
#include <time.h>
#include <io.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

