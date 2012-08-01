// ============================================================================
//
//		ファイル関係のライブラリ
//
//		Creator			: 山田　巧
//		Creation Data	: 09/17/2004
//
// ============================================================================

// include --------------------------------------
#include <string.h>
#include <stdio.h>
#include <mbstring.h>
#include <windows.h>
#include "FileLib.h"

// define ---------------------------------------

// data type ------------------------------------

// data -----------------------------------------

// function proto type --------------------------

// function code --------------------------------

// フルパスではないパス文字列をフルパスに変換する
// ( CurrentDir はフルパスである必要がある(語尾に『\』があっても無くても良い) )
// ( CurrentDir が NULL の場合は実際のカレントディレクトリを使用する )
extern int ConvertFullPath( const char *Src, char *Dest, const char *CurrentDir )
{
	int i, j, k ;
	char iden[256], cur[MAX_PATH] ;

	if( CurrentDir == NULL )
	{
		GetCurrentDirectory( MAX_PATH, cur ) ;
		CurrentDir = cur ;
	}

	if( Src == NULL )
	{
		strcpy( Dest, CurrentDir ) ;
		goto END ;
	}

	i = 0 ;
	j = 0 ;
	k = 0 ;
	
	// 最初が『\』又は『/』の場合はカレントドライブのルートディレクトリまで落ちる
	if( Src[0] == '\\' )
	{
		Dest[0] = CurrentDir[0] ;
		Dest[1] = CurrentDir[1] ;
		Dest[2] = '\0' ;

		i ++ ;
		j = 2 ;
	}
	else
	// ドライブ名が書かれていたらそのドライブへ
	if( Src[1] == ':' )
	{
		Dest[0] = Src[0] ;
		Dest[1] = Src[1] ;
		Dest[2] = '\0' ;

		i = 2 ;
		j = 2 ;

		if( Src[i] == '\\' ) i ++ ;
	}
	else
	// それ以外の場合はカレントディレクトリ
	{
		strcpy( Dest, CurrentDir ) ;
		j = strlen( Dest ) ;
		if( Dest[j-1] == '\\' || Dest[j-1] == '/' )
		{
			Dest[j-1] = '\0' ;
			j -- ;
		}
	}

	for(;;)
	{
		switch( Src[i] )
		{
		case '\0' :
			if( k != 0 )
			{
				Dest[j] = '\\' ; j ++ ;
				strcpy( &Dest[j], iden ) ;
				j += k ;
				k = 0 ;
			}
			goto END ;

		case '\\' :
		case '/' :
			if( k == 0 ) return -1 ;		// 文字列が無かったらエラー
			if( strcmp( iden, "." ) == 0 )
			{
				// なにもしない
			}
			else
			if( strcmp( iden, ".." ) == 0 )
			{
				// 一つ下のディレクトリへ
				j -- ;
				while( Dest[j] != '\\' && Dest[j] != '/' && Dest[j] != ':' ) j -- ;
				if( Dest[j] != ':' ) Dest[j] = '\0' ;
				else j ++ ;
			}
			else
			{
				Dest[j] = '\\' ; j ++ ;
				strcpy( &Dest[j], iden ) ;
				j += k ;
			}

			k = 0 ;
			i ++ ;
			break ;
		
		default :
			if( _mbsbtype( ( const unsigned char * )&Src[i], 0 ) == 0/*(_MBC_SINGLE)*/ )
			{
				iden[k] = Src[i] ;
				iden[k+1] = 0 ; 
				k ++ ;
				i ++ ;
			}
			else
			{
				*(( unsigned short * )&iden[k] ) = *(( unsigned short * )&Src[i] ) ;
				iden[k+2] = '\0' ;
				k += 2 ;
				i += 2 ;
			}
			break ;
		}
	}
	
END :
	// 正常終了
	return 0 ;
}

// ファイル名も一緒になっていると分かっているパス中からファイル名とディレクトリパスを分割する
// フルパスである必要は無い、ファイル名だけでも良い
extern int AnalysisFileNameAndDirPath( const char *Src, char *FileName, char *DirPath )
{
	int i, Last ;
	
	// ファイル名を抜き出す
	i = 0 ;
	Last = -1 ;
	while( Src[i] != '\0' )
	{
		if( _mbsbtype( ( const unsigned char * )&Src[i], 0 ) == 0/*(_MBC_SINGLE)*/ )
		{
			if( Src[i] == '\\' || Src[i] == '/' || Src[i] == '\0' || Src[i] == ':' ) Last = i ;
			i ++ ;
		}
		else
		{
			i += 2 ;
		}
	}
	if( FileName != NULL )
	{
		if( Last != -1 ) strcpy( FileName, &Src[Last+1] ) ;
		else strcpy( FileName, Src ) ;
	}
	
	// ディレクトリパスを抜き出す
	if( DirPath != NULL )
	{
		if( Last != -1 )
		{
			strncpy( DirPath, Src, Last ) ;
			DirPath[Last] = '\0' ;
		}
		else
		{
			DirPath[0] = '\0' ;
		}
	}
	
	// 終了
	return 0 ;
}

// ファイルパスからファイル名と拡張子を取得する
extern int AnalysisFileNameAndExeName( const char *Src, char *Name, char *ExeName )
{
	char FileName[256], *p, ename[128], name[128] ;

	// ファイル名のみを取得
	AnalysisFileNameAndDirPath( Src, FileName, 0 ) ;

	// 『.』があるかどうかで処理を分岐
	if( ( p = strchr( FileName, '.' ) ) == NULL )
	{
		strcpy( name, FileName ) ;
		ename[0] = '\0' ;
	}
	else
	{
		strncpy( name, FileName, p - FileName ) ;
		name[p - FileName] = '\0' ;
		strcpy( ename, p + 1 ) ;
	}

	if( Name != NULL ) strcpy( Name, name ) ;
	if( ExeName != NULL ) strcpy( ExeName, ename ) ;

	// 終了
	return 0 ;
}




