// ============================================================================
//
//			ＤＸＡファイルデコードソフト
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

// define -----------------------------
#define VERSION			"1.03"				// バージョン
#define FILETYPE		"dxa"				// 拡張子

// data type --------------------------

// data -------------------------------

// function proto type ----------------

// function code ----------------------

// メイン関数
int main( int argc, char *argv[ ] )
{
	char FullPath[MAX_PATH], FilePath[MAX_PATH], DirectoryPath[MAX_PATH], FileName[MAX_PATH] ;
	char *DxaFilePath, *KeyString ;

	printf( "ＤＸＡファイル簡易デコードソフト  Ver%s\n\n", VERSION ) ;

	// 引数が一個だったらヘルプを出す
	if( argc == 1 ) goto ERR ;

	// 鍵文字列があるかどうかで処理を分岐
	if( strnicmp( argv[1], "-K:", 3 ) == 0 )
	{
		// 鍵文字列がある場合
		if( argc == 2 ) goto ERR ;

		KeyString   = argv[1] + 3 ;
		DxaFilePath = argv[2] ;
	}
	else
	{
		// 鍵文字列が無い場合
		KeyString   = NULL ;
		DxaFilePath = argv[1] ;
	}

	// ＤＸＡファイルのフルパスを取得
	ConvertFullPath( DxaFilePath, FullPath ) ;

	// ディレクトリとファイル名に分解
	AnalysisFileNameAndDirPath( FullPath, FilePath, DirectoryPath ) ;

	// カレントディレクトリを変更
	SetCurrentDirectory( DirectoryPath ) ;

	// ファイル名から拡張子を取ったものを作成
	AnalysisFileNameAndExeName( FilePath, FileName, NULL ) ;

	// ファイル名と同じディレクトリの作成
	CreateDirectory( FileName, NULL ) ;

	// カレントディレクトリを変更
	SetCurrentDirectory( FileName ) ;

	// アーカイブファイルを展開する
	DXArchive::DecodeArchive( FullPath, "", KeyString ) ;

	// 終了
	return 0 ;

ERR:
	printf( "DxaDecode.exe [-K:KeyString] DxArchiveFilePath\n" ) ;
	printf( "・ＤＸＡファイルをドラッグ＆ドロップするだけでも展開出来ます\n" ) ;
	printf( "・鍵が設定されている場合は -K オプションで鍵を指定する必要があります\n" ) ;
	return -1 ;
}

	

