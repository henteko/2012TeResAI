// ============================================================================
//
//		ファイル関係のライブラリ
//
//		Creator			: 山田　巧
//		Creation Data	: 09/17/2004
//
// ============================================================================

#ifndef __FILELIB
#define __FILELIB

// include --------------------------------------

// define ---------------------------------------

// data type ------------------------------------

// data -----------------------------------------

// function proto type --------------------------

// フルパスではないパス文字列をフルパスに変換する
// ( CurrentDir はフルパスである必要がある(語尾に『\』があっても無くても良い) )
// ( CurrentDir が 0 の場合は実際のカレントディレクトリを使用する )
extern int ConvertFullPath( const char *Src, char *Dest, const char *CurrentDir = 0 ) ;

// ファイル名も一緒になっていると分かっているパス中からファイル名とディレクトリパスを分割する
// フルパスである必要は無い、ファイル名だけでも良い
extern int AnalysisFileNameAndDirPath( const char *Src, char *FileName = 0, char *DirPath = 0 ) ;

// ファイルパスからファイル名と拡張子を取得する
extern int AnalysisFileNameAndExeName( const char *Src, char *Name = 0, char *ExeName = 0 ) ;

#endif
