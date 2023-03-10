# AviUtl プラグイン - レベルメーター

音声のレベルメーターを表示します。
[最新バージョンをダウンロード](../../releases/latest/)

## 導入方法

以下のファイルを AviUtl の Plugins フォルダに入れてください。
* LevelMeter.auf
* LevelMeter (フォルダ)

## 使用方法

メニューの「設定」→「レベルメーターの設定」を選択してレベルメーターウィンドウを表示します。

## UI 説明

棒グラフが RMS メーターです。水平線がピークメーターです。目盛りの単位は dB です。

## 操作方法

レベルメーターウィンドウの上半分をドラッグすると最大値が変動します。下半分をドラッグすると最小値が変動します。

## 設定方法

1. レベルメーターウィンドウを右クリックしてコンテキストメニューを表示します。
2. 「設定」を選択します。
3. ダイアログが表示されるので配色などを設定します。<br>
「縞模様ステップ」を 0 にすると縞模様は描画されなくなります。

## 再生が終了したのにメーターが動く

(1) 音声データが生成されてから実際に音が流れるまでタイムラグがある。<br>
(2) プラグインは再生が終了(中断)したことを知ることができない。(再生が開始したこともわからない)<br>
以上の理由で、再生が終わったことを知らずにタイムラグ分のメーターを表示してしまいます。

## 更新履歴

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
* (共存確認) patch.aul r42 https://scrapbox.io/ePi5131/patch.aul

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
