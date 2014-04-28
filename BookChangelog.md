「BareMetalで遊ぶ　Raspberry Pi」更新履歴
=================

# 概要
同人サークル迷惑研がコミックマーケット85にて販売した同人誌、および達人出版会さまより出版された「BareMetalで遊ぶ　Raspberry Pi」の変更履歴をここに記します。

# 変更履歴

## 達人出版会β(ver0.9.0)
 * mainから戻った後の動作が不定だった問題
 * _sbrk実装上の問題で無限にmallocできてしまう問題

の2つを修正しました。
これに伴い一部の文章も修正されています。

また、JTAGデバッガとOpenOCD、gdbを使ったハードウェアデバッグの方法も追加しました。


## 達人出版会β(ver0.9.1)
SPIとI2Cを扱う記事を追加し、以下の問題を修正しました。

 * crosstool-ngにてbinutilsのバージョンをあげ忘れていた問題
 * crosstool-ngにてgdbのバージョンをあげ忘れていた問題
 * crosstool-ngにてCPUとアーキテクチャ両方に対し最適化をかけていた問題（一般的に片方しか指定しない）
 * crosstool-ng及びサンプルプログラムにて浮動小数点演算を行うとハングする問題	
 * printfでfloatやdoubleの値が表示できなかった問題

binutilsのバージョンアップに伴いコンパイル時に「Unknown EABI object attribute 34」が出る問題も解決されました。

浮動小数点演算ができない問題は、開発環境及びサンプルプログラムにてsoft-floatを利用するように変更して対処しました。
詳しくはgithubの「failtureproject」内を参照してください。

### 環境のアップデート方法
以前のバージョンをお持ちの方は、以降の作業を行ってクロスコンパイル環境を再構築してください。

まず次のコマンドより、新しいバージョンのgdbに必要なパッケージを追加してください。
```bash
$ sudo aptitude -y install python-dev
```

次にcrosstool-ngのコンパイル設定を書き換えます。
本書の「2.3開発用マシンの構築」を開き、14ページ目付近の
```bash
$ ~/cross/bin/ct-ng arm-unknown-eabi
```
を行ったディレクトリに移動してください（すでに削除してしまった方は、ここまでの手順をやり直してください）。
移動したら既存の「.config」ファイルを削除し、本リポジトリの「crosstool-ng/dot_sf_config」をコピーしてきて、「.config」にリネームしてください。

最後に次のコマンドを実行して、環境の再構築を行ってください。
```bash
$ ~/cross/bin/ct-ng build
```
正常にコンパイルが完了すれば、アップデートは完了です。


## 達人出版会β(ver0.9.2)
U-Bootを使ったネットワークブート方法の記事を追加し、以下の修正を行いました。

 * JTAGデバッガとP1コネクタの対応表が間違っていた問題の修正
 * uartechoTest02のコードの修正
 * I2CキャラクタLCD説明の補足


### JTAGデバッガとP1コネクタの対応表が間違っていた問題
「表 B.3 ARM-USB-TINY-H と Raspberry Pi の対応表」のピン対応表が間違っていました。
正しくは以下のようになります。


|	機能		|	ピン番号（P1）	|	ピン番号（ARM JTAG 20）	|
|:---------:|:-----------------:|:-------------------------:|
|	VTREF	|	1				|	1 						|
|	TRST	|	15				|	3 						|
|	TDI		|	7				|	5 						|
|	TMS		|	13				|	7 						|
|	TCK		|	22				|	9						|
|	TDO		|	18				|	13 						|
|	GND		|	25				|	4,6,..,20 				|


### uartechoTest02のコードの修正
uartechoTest02のコードに、ループ終了条件のミスがあったので、修正しました。

また改行コードについての質問を頂いたため、改行コードについてのコメントを追記しました。
本書ではLFのみをサポートとします。

```C
// 文字列を受け取る(改行がくるまで読み込む)
		do{
			// 受信FIFOが空でなくなるのを待つ
			while (*UART0_FR & (1 << 4));
			// 受信データを読み込む
			c = *UART0_DR;
			echo[length] = c & 0xff;
			length++;
		}while(c != '\n' && i < 256);
```

```C
// 255文字以下で改行（LF）が来るまで読み込む
        // （255文字以下の文字列を読み込む）
		do{
			// 受信FIFOにデータが来るのを待つ
			while (*UART0_FR & (1 << 4));
			// 受信データを読み込む
			c = *UART0_DR;
			echo[length] = c & 0xff;
			length++;
		}while(c != '\n' && length < 256);
```

### I2CキャラクタLCD説明の補足
I2CキャラクタLCDの説明に「コマンドがHD44780互換」であることを追記しました。



# 連絡先
作者：[@tnishinaga](https://twitter.com/tnishinaga)

達人出版会： [http://tatsu-zine.com/](http://tatsu-zine.com/)

同人サークル迷惑研： [@carlo_yama](https://twitter.com/carlo_yama)