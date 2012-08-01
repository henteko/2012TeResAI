// ============================================================================
//
//			ＤＸＡファイル作成ソフト
//
//	Creator			: 山田　巧
//	Creation Date	: 2003/09/13
//	Version			: 1.02
//
// ============================================================================

// include ----------------------------
#include "DxArchive\\DXArchive.h"
#include "FileLib\\FileLib.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>

// define -----------------------------
#define VERSION			"1.03"				// バージョン
#define FILETYPE		"dxa"				// 拡張子

// code -------------------------------

// メイン関数
int main( int argc, char *argv[] )
{
	char SrcPath[256], DestPath[256], *KeyString, *SrcDirectoryPath, *DestFilePath ;

	printf( "ＤＸＡファイル簡易作成ソフト  Ver%s\n", VERSION ) ;

	// 引数が一個だったらヘルプを出す
	if( argc == 1 ) goto ERR ;

	// 鍵文字列があるかどうかを調べる
	if( strnicmp( argv[1], "-K:", 3 ) == 0 )
	{
		// 鍵文字列がある場合
		if( argc == 2 ) goto ERR ;

		KeyString        = argv[1] + 3 ;
		SrcDirectoryPath = argv[2] ;
		DestFilePath     = argc == 3 ? NULL : argv[3] ;
	}
	else
	{
		// 鍵文字列が無い場合

		KeyString        = NULL ;
		SrcDirectoryPath = argv[1] ;
		DestFilePath     = argc == 2 ? NULL : argv[2] ;
	}

	// ソースパスのフルパスを作成する
	ConvertFullPath( SrcDirectoryPath, SrcPath ) ;

	// 出力ファイルパスの作成
	{
		if( DestFilePath != NULL )
		{
			char temp[256] ;

			strcpy( temp, DestFilePath ) ;
			ConvertFullPath( temp, DestPath ) ;
		}
		else
		{
			// ソースパスから出力ファイル名を作成する
			sprintf( DestPath, "%s.%s", SrcPath, FILETYPE ) ;
		}
	}

	// アーカイブファイルを作成する
	DXArchive::EncodeArchiveOneDirectory( DestPath, SrcPath, true, KeyString ) ;

	// 終了
	return 0 ;

ERR:

	printf( "DxaEncode.exe [-K:KeyString] SrcDirectoryPath  [DestFilePath]\n\n" ) ;
	printf( "・ＤＸＡファイルにしたいフォルダをドラッグ＆ドロップするだけでも作成可能です\n" ) ; 
	printf( "・-K オプションで鍵を設定出来ます(最大１２文字) 　例  -K:KeyCode \n" ) ; 
	return -1; 
}


