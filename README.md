# AviUtl プラグイン - レベルメーター

音量のレベルメーターを表示します。
[最新バージョンをダウンロード](../../releases/latest/)

## 導入方法

以下のファイルを AviUtl の Plugins フォルダに入れてください。
* LevelMeter.auf
* LevelMeter (フォルダ)

## 使用方法

メニューの『設定』→『レベルメーターの設定』を選択してレベルメーターウィンドウを表示します。

## UI 説明

棒グラフが音量レベルです。水平線が音量ピークです。目盛りの単位は dB です。

## 操作方法

レベルメーターウィンドウの上半分をドラッグすると最大値が変動します。下半分をドラッグすると最小値が変動します。

## 設定方法

1. レベルメーターウィンドウを右クリックしてコンテキストメニューを表示します。
2. 「設定」を選択します。
3. ダイアログが表示されるので配色などを設定します。<br>
『縞模様』の『ステップ数』を 0 にすると縞模様は描画されなくなります。

* ※一部の設定はダイアログからは変更できません。(色のアルファなど)

## 設定方法 2

1. LevelMeter/LevelMeter.ini をテキストエディタで編集します。
2. 編集したあとファイルを保存すると即座に変更が反映されます。

```
[Config]
enableMode=1
leftName=左レベ
rightName=右レベ
minRange=-40
maxRange=10

[Design]
; 使用するフォント
fontDefault=C:\Windows\Fonts\segoeui.ttf
fontDefault2=C:\Windows\Fonts\meiryo.ttc

; 背景画像の設定
image.fileName=
image.offset.x=0
image.offset.y=0
image.align.x=center
image.align.y=center
image.scaleMode=normal
image.scale=1.000000
image.angle=0.000000
image.alpha=1.000000

; 左レベの設定
left.image.fileName=
left.image.offset.x=0
left.image.offset.y=0
left.image.align.x=center
left.image.align.y=center
left.image.scaleMode=normal
left.image.scale=1.000000
left.image.angle=0.000000
left.image.alpha=1.000000

; 右レベの設定
right.image.fileName=
right.image.offset.x=0
right.image.offset.y=0
right.image.align.x=center
right.image.align.y=center
right.image.scaleMode=normal
right.image.scale=1.000000
right.image.angle=0.000000
right.image.alpha=1.000000

background.fill.start=26, 26, 26, 255 ; 背景のグラデーション開始色
background.fill.end=26, 26, 26, 255 ; 背景のグラデーション終了色
scale.stroke.style=solid ; 目盛りの線のスタイル
scale.stroke.width=1 ; 目盛りの線の幅
scale.stroke.color=204, 204, 204, 230 ; 目盛りの線の色
scale.text.height=16 ; 目盛りの文字のサイズ
scale.text.dilate=0.000000 ; 目盛りの文字の縁取り量。通常は使用しない
scale.text.blur=0.000000 ; 目盛りの文字のブラー量。通常は使用しない
scale.text.color=204, 204, 204, 255 ; 目盛りの文字の色
scale.text.shadow.dilate=1.000000 ; 影の縁取り量
scale.text.shadow.blur=1.000000 ; 影のブラー量
scale.text.shadow.offset.x=0 ; 影の位置
scale.text.shadow.offset.y=0 ; 影の位置
scale.text.shadow.color=0, 0, 0, 102 ; 影の色
volume.level.fill.start=204, 204, 0, 230 ; 音量レベルのグラデーション開始色
volume.level.fill.end=204, 102, 0, 230 ; 音量レベルのグラデーション終了色
volume.peak.stroke.style=solid ; 音量ピークの線のスタイル
volume.peak.stroke.width=2 ; 音量ピークの線の幅
volume.peak.stroke.color=204, 0, 0, 230 ; 音量ピークの線の色
volume.zebra.step=3 ; 縞模様のステップ数
volume.zebra.stroke.style=solid ; 縞模様の線のスタイル
volume.zebra.stroke.width=1 ; 縞模様の線の幅
volume.zebra.stroke.color=0, 0, 0, 230 ; 縞模様の線の色
```

## 再生が終了したのにメーターが動く

(1) 音声データが生成されてから実際に音が流れるまでタイムラグがある。<br>
(2) プラグインは再生が終了(中断)したことを知ることができない。(再生が開始したこともわからない)<br>
以上の理由で、再生が終わったことを知らずにタイムラグ分のメーターを表示してしまいます。

## 更新履歴

* 2.2.0 - 2023/07/23 左右別々のウィンドウを追加
* 2.1.1 - 2023/03/10 ファイル名に日本語が使えない問題を修正
* 2.1.0 - 2023/02/21 背景画像などの描画オプションを追加
* 2.0.0 - 2023/02/20 マルチスレッド/マルチプロセスにして高速化
* 1.4.0 - 2022/12/20 有効/無効を切り替えられるように修正
* 1.3.0 - 2022/07/16 ほんの少し最適化
* 1.2.0 - 2022/07/16 縞模様にできるように修正
* 1.1.0 - 2022/07/14 テーマで描画できるように修正
* 1.0.0 - 2022/07/14 初版

## 動作確認

* (必須) AviUtl 1.10 & 拡張編集 0.92 http://spring-fragrance.mints.ne.jp/aviutl/
* (共存確認) patch.aul r43 謎さうなフォーク版29 https://scrapbox.io/nazosauna/patch.aul

## クレジット

* NanoVG https://github.com/memononen/nanovg
* The OpenGL Extension Wrangler Library (GLEW) https://glew.sourceforge.net/
* Microsoft Research Detours Package https://github.com/microsoft/Detours
* aviutl_exedit_sdk https://github.com/ePi5131/aviutl_exedit_sdk
* Common Library https://github.com/hebiiro/Common-Library

## 作成者情報

* 作成者 - 蛇色 (へびいろ)
* GitHub - https://github.com/hebiiro
* Twitter - https://twitter.com/io_hebiiro

## 免責事項

この作成物および同梱物を使用したことによって生じたすべての障害・損害・不具合等に関しては、私と私の関係者および私の所属するいかなる団体・組織とも、一切の責任を負いません。各自の責任においてご使用ください。
