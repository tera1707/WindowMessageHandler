# 概要

ウインドウメッセージで検知できるイベントをログに残して、
どういうタイミングで何のウインドウメッセージがくるかを試すためのツール。

# 使い方

- ビルドする
- ビルド出力フォルダに、下記のfileがある。
  - DisplayingLogStart.bat
  - DisplayngLogRegister.bat
  - DisplayngLogUnregister.bat
  - WindowJikken.exe
  - WindowJikken.pdb
- まず、WindowJikken.exe をダブルクリックして実行する  
  →Consoleが起動し、デスクトップに「mylog.log」が生成される。ここに各種ウインドウメッセージ到来時にログが残る。
- DisplayingLogStart.bat を実行する  
  →ログのモニターが始まるので、ここで何のメッセージがきたかを見る。
- スタートアップ時に自動でログのモニタを始めたい場合は、DisplayngLogRegister.bat を実行すると、次回ログイン時から自動でモニタが始まる。  
  （バッチがstartupにはいる）
- 自動のモニタをやめたい場合は、「DisplayngLogUnregister.bat」を実行する。  
  （バッチがstartupから削除される）

# コードについて

とにかく、WndProcにごちゃごちゃ処理を書きたくない。  
また、各イベントが「ウインドウ」から来るメッセージで使って検知していることを意識したくないので、
イベント(ウインドウメッセージ)の意味ごとにfunctionを登録できるようにラップした。  
（各種イベントが「ウインドウメッセージ」で来るがゆえに、それを受けるためのWndProcの中にUIの処理、UIと関係なくできる処理が一緒に書かれて、読みづらいコードになるのを避けたい）
