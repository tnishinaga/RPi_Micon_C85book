# 仲良く切り替わるGPIO

### 問題概要
GPIOに出力するプログラムを動かしたところ、1つのポートに対する制御が、複数のポートに影響を与えてしまう問題が起きた。

### 経緯とバグの確認
I2C通信を行うプログラムを書くにあたって、ブレッドボードに指している2つのピンがSDAなのかSCLなのか区別がつかなくなってしまった。
そこで両方をアウトプットにし、不論理でLEDをつなぎ、片方をLOW、もう片方をHIGHにする次のようなプログラムを走らせた。
```C
	pinMode(2, OUTPUT); // SDA
	pinMode(3, OUTPUT); // SCL
	
	digitalWrite(2,HIGH);
	digitalWrite(3,LOW);

	while(1);
```
正常動作すれば、どちらか片方のみのLEDが点灯し、SDAかSCLなのか区別が付く……はずだった。

このプログラムを動作させると、どちらのLEDも点灯してしまうという問題が起こった。
試しにHIGHとLOWを入れ替えてみると、今度はLEDがすべて消灯してしまった。

調査のためコードを次のように変更して動作を確認した。
```C
	pinMode(2, OUTPUT); // SDA
	pinMode(3, OUTPUT); // SCL
	
	digitalWrite(2,LOW);

	while(1);
```

結果として、LEDがどちらも仲良く点灯した。

これらの検証より、次のバグが存在することを認識した。

 * 1ポート限定の操作が、他のポートに対しても影響を与える

これは……だいぶまずいバグである……。
 
### 原因調査
JTAGデバッガを接続して、コードを追った。

まずmain関数のdigitalWrite関数にブレックポイントを設定し、この関数を超えた時にGPIOが両方一般に制御されてしまうことを確認した。
```
(gdb) display/i $pc
(gdb) disas main
〜略〜
807c:	eb0002ca 	bl	8bac <digitalWrite>
〜略〜
8088:	eb0002c7 	bl	8bac <digitalWrite>
〜略〜
(gdb) b *0x807c
Breakpoint 1 at 0x807c
(gdb) b *0x8088
Breakpoint 2 at 0x8088
(gdb) c
Continuing.

Breakpoint 1, 0x0000807c in main ()
1: x/i $pc
=> 0x807c <main+104>:	bl	0x8bac <digitalWrite>
(gdb) ni
0x00008080 in main ()
1: x/i $pc
=> 0x8080 <main+108>:	mov	r0, #3
(gdb) c
Continuing.

Breakpoint 2, 0x00008088 in main ()
1: x/i $pc
=> 0x8088 <main+116>:	bl	0x8bac <digitalWrite>
(gdb) ni

Breakpoint 2, 0x00008088 in main ()
1: x/i $pc
=> 0x8088 <main+116>:	bl	0x8bac <digitalWrite>
(gdb) ni
0x0000808c in main ()
1: x/i $pc
=> 0x808c <main+120>:	b	0x808c <main+120>
```
よってバグがdigitalWrite関数内にあると断定した。

#### digitalWrite関数
digitalWrite関数を読んでいくと、値を代入している部分が怪しい気がした。
```
// output
*res |= (0x01 << (pin % 32));
```
この部分はレジスタの値と、更新したいビット（ポート）のORを取って、レジスタを更新するようになっている。
普通のレジスタなら特にこれで問題ない……はずだが、念のためこの部分の前後でレジスタがどう書き換わるかを見てみた。

この部分に該当する逆アセンブル結果を次に示す。（剰余演算とシフト演算は省略）
```
    8c84:	e1812003 	orr	r2, r1, r3
    8c88:	e51b3008 	ldr	r3, [fp, #-8]
    8c8c:	e5832000 	str	r2, [r3]
```
この部分は次の処理を行っている。

 * r3に入った右辺の演算結果と、r1に入った現在のGPSETまたはGPCLRの値でORを取り、r2に格納。
 * r3にレジスタアドレスを入れて、r2の値でレジスタを更新

つまりORを取る前後の、r1,r2,r3の値を見れば何かがわかるはず。

ブレックポイントを挟んで調べてみると、OR演算の前後でレジスタはこのようになっていた。
```
(gdb) b *0x8c84
Breakpoint 3 at 0x8c84
(gdb) c
Continuing.

Breakpoint 3, 0x00008c84 in digitalWrite ()
1: x/i $pc
=> 0x8c84 <digitalWrite+216>:	orr	r2, r1, r3
(gdb) i r
r1             0x6770696f	1735420271
r2             0x1	1
r3             0x4	4
(gdb) ni
0x00008c88 in digitalWrite ()
1: x/i $pc
=> 0x8c88 <digitalWrite+220>:	ldr	r3, [r11, #-8]
(gdb) i r
r0             0x2	2
r1             0x6770696f	1735420271
r2             0x6770696f	1735420271
r3             0x4	4
```
r1レジスタの前後を見るに、どうやらこのレジスタは読み込むと現在の出力状況ではなく、謎の値が帰ってくるらしい。
OR演算を行うのは、やめたほうが良さそうだ。

### bug fix
OR演算をやめて、バグを含むコードを次のように修正した。
```
// output
*res |= (0x01 << (pin % 32));
```
このコードを実行したところ、今度は問題なくGPIO2とGPIO3で別々の出力が得られた。

よってこのバグfixをすべてのコードに対して行い、修正を完了した。


### 教訓
GPSETとGPCLRレジスタは、ORで制御しちゃだめ。絶対。

