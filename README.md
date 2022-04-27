# AviutlProjectCreatorGUI
「AviutlProjectCreatorGUI」は Aviutlで使うプロジェクト(？)を作るためのプログラムです。

## 環境構築
---
ビルドには「Visual Studio 2022」が必要です。  
  
言語バージョンは 「プレビュー - 最新の C++ Working Draft からの機能 (/std:c++latest)」です。
(std::formatを使うため。地雷を踏むかもしれませんがご了承ください。)  

### 注意
プログラミング初心者(？)が作ったため、スパゲッティです。  
許してください。  

## 使い方
---
そのまま立ち上げて
- プロジェクトを作る場所 (パス)
- シリーズ名 (なくてもOK)
- プロジェクト名
を入力するだけです。  

![screenshot_0.png](./screenshot/screenshot_0.png)  

あとは勝手にフォルダを作成してくれます。

### フォルダ構成
---
フォルダ構造は、シリーズ名が入力されていると、
プロジェクトを作る場所のフォルダのなかに、シリーズ名の フォルダができて、さらにその中に、プロジェクト名のフォルダという感じでできます。  
シリーズ名が空の場合は、直接、プロジェクトを作る場所に、プロジェクト名のフォルダができます。

### その他の機能
---
AviutlProjectCreatorGUI には、Markdownを自動生成する機能と、
追加でフォルダを生成する機能があります。  
  
まず、追加でフォルダを生成する機能についてですが、こちらは、「config」というフォルダの中にある「adddir.md」という、テキストファイルに、
作りたいフォルダを、改行区切りで書いてください。  
  
つぎに、Markdownですが、「config」というフォルダの中にある「template.md」という Markdownが最初から用意されています。  
なので、そのファイルを編集してみてください。  
  
そして、この2つのファイルには、プロジェクト名などを自動で置換する機能があります。  

置換前文字列 | 置換後文字列
--- | ---
%DATE% | プロジェクトを作った日付
%PROJPATH% | プロジェクトのあるパス
%PROJNAME% | プロジェクトの名前
%SERIESNAME% | シリーズの名前 (入力しなかったときは空)

この機能を利用して、シリーズや、プロジェクトごとに、フォルダを作ることができます。

## ライセンス
このプログラムは MIT LICENSE で提供されます。  
[LICENSE](./LICENSE)
