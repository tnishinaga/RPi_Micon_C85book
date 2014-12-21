# 64bit Debian Linux 環境への CoolTerm インストール方法

<!-- TODO: ia32-libsインストール方法の修正 -->
<!-- TODO: 詳細説明の追記 -->

CoolTermは32bitライブラリを必要とするので、64bit Debian Linuxで動かすためには以下の作業を行ってください。

## ライブラリのインストール

```
$ aptitude -y install ia32-libs libgtk-2.0
```

## パーミッションの再設定

```
$ chmod 755 CoolTerm
```

## 使用デバイスの設定

```
$ echo "/dev/ttyUSB0" > ports.ini
```

## 起動

```
$ sudo ./CoolTerm
```


32bit環境用のライブラリと32bit用のlibgtk-2.0をインストールして、パーミッション755に設定し、ports.iniというファイルに「/dev/ttyUSB」と記入してCoolTermと同じ場所に保存し、管理者権限で起動する必要があります。ライブラリのインストールはすでに[subsec:cross]小節で行っているので、上手く動かなかった場合は32bitライブラリがきちんとインストールされているかを確認して下さい。パーミッションは、

//cmd{
$ chmod 755 CoolTerm
//}

として設定してください。そして最後にシリアルデバイス名を追加して管理者権限で起動します。

//cmd{
$ echo "/dev/ttyUSB0" > ports.ini
$ sudo ./CoolTerm
//}