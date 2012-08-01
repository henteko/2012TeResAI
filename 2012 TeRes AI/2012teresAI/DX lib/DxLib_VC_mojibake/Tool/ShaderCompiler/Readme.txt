
  簡易シェーダーコンパイルソフト


　コマンドプロンプトで使用するシェーダープログラムをコンパイルするソフトです。
　コンパイルには DirectX の機能を使用しますので、なるべく新しい DirectX End-User Runtime をインストールしておいてください。
　あと、シェーダーを使うには最低でもグラフィックスデバイスがシェーダーモデル２．０に対応している必要があります。


  ShaderCompiler.exe <option> <shader file>

	オプション

	/T<profile>	作成するシェーダーのターゲットの指定
			( vs_1_1 vs_2_0 vs_3_0 ps_2_0 ps_3_0 等 )

	/Fo<file path>	出力先のファイルパスの指定

	/E<name>	エントリーポイントの指定
			( 作成するシェーダーのプログラム開始関数の指定
			  デフォルトでは main )

	/D<id>=<text>	#define <id> <text> のマクロ定義



　使用例は Sample フォルダの中をご覧ください。
　Sample フォルダの中のファイルの解説↓


　PixelShaderTest.exe
　　サンプルプログラムを管理人がコンパイルしたもの

　PixelShaderTest.cpp   
　　サンプルシェーダープログラムを実際に使うＤＸライブラリを使用したプログラム

　SamplePS.fx
　　サンプルピクセルシェーダープログラム
　　テクスチャの色成分の r と b を入れ替えて描画するためのピクセルシェーダープログラムです

　SamplePS.pso
　　SamplePS.fx を ShaderCompiler.exe でバイナリコードにコンパイルしたファイル
　　ＤＸライブラリではこのファイルを読み込みます

　SamplePSCompile.bat
　　SamplePS.fx をコンパイルして SamplePS.pso を作成するためのバッチファイル
　　テキストエディタで開くと中はこんな風になっています

ShaderCompiler.exe /Tps_2_0 SamplePS.fx
pause

　　一行目は SamplePS.fx をピクセルシェーダーモデル２．０のシェーダープログラムとして
　　コンパイルするコマンドです
　　２行目は何かキーが押されるまでバッチ処理をとめるコマンドです

　Test1.bmp
　　サンプルプログラム( PixelShaderText.cpp )で使う画像ファイル

　ShaderCompiler.exe
　　一つ上のフォルダにあるシェーダーコンパイルソフトと同じものです