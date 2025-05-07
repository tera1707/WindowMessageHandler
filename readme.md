# 概要

ウインドウメッセージで検知できるイベントをログに残して、
どういうタイミングで何のウインドウメッセージがくるかを試すためのツール。

# 検知対象イベント/メッセージ

検知するイベントと、そのイベントとして受けるウインドウメッセージを下記に挙げる。
(どのウインドウメッセージが来たら、どういうイベントとして取るか)


| イベント           | ウインドウメッセージ | パラメータ(wp)             | パラメータ(lp)                    | パラメータ(lp)の値と意味                                                                                                                                                                                                                                                                                                                                                  |
| :----------------- | :------------------- | :------------------------- | :-------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| ディスプレイON/OFF | WM_POWERBROADCAST    | PBT_POWERSETTINGCHANGE     | GUID_CONSOLE_DISPLAY_STATE        | 0:ディスプレイOFF<br/>1:ディスプレイON                                                                                                                                                                                                                                                                                                                                    |
|                    |                      |                            | GUID_BATTERY_PERCENTAGE_REMAINING | バッテリー残量(%)                                                                                                                                                                                                                                                                                                                                                         |
|                    |                      |                            | GUID_ACDC_POWER_SOURCE            | 0:AC<br/>1:DC<br/>2:UPS等の短期電源                                                                                                                                                                                                                                                                                                                                       |
|                    |                      |                            | GUID_SESSION_USER_PRESENCE        | 0:PowerUserPresent<br/>1:PowerUserNotPresent<br/>2:PowerUserInactive                                                                                                                                                                                                                                                                                                      |
|                    |                      |                            | GUID_LIDSWITCH_STATE_CHANGE       | 0:蓋が閉じた<br/>1:蓋が開いた                                                                                                                                                                                                                                                                                                                                             |
|                    |                      |                            | GUID_POWER_SAVING_STATUS          | 0:PowerSavingがOFF<br/>1:PowerSavingがON<br/>※24H2より前のOSVer                                                                                                                                                                                                                                                                                                           |
|                    |                      |                            | GUID_ENERGY_SAVER_STATUS          | 0:EnergySaverがOFF<br/>1:EnergySaverがスタンダード(ユーザーに影響が小さいときだけ省電力) <br/>2:高節約(出来るだけ節約)<br/>※「常に省エネ機能を使用する」をON or 省エネ閾値を下回るとこれになる                                                                                                                                                                            |
|                    |                      | PBT_APMSUSPEND             | -                                 | スリープに入る（モダンスタンバイ時には来ない）                                                                                                                                                                                                                                                                                                                            |
|                    |                      | PBT_APMRESUMESUSPEND       | -                                 | 通常のスリープから復帰（モダンスタンバイ復帰時には来ない）                                                                                                                                                                                                                                                                                                                |
|                    |                      | PBT_APMRESUMEAUTOMATIC     | -                                 | リモートウェイクによるスリープから復帰                                                                                                                                                                                                                                                                                                                                    |
|                    |                      | PBT_APMBATTERYLOW          | -                                 | バッテリ残量低下（BIOSの実装によっては来ない場合がある）                                                                                                                                                                                                                                                                                                                  |
|                    |                      | PBT_APMPOWERSTATUSCHANGE   | -                                 | 電源状態の変化（AC/DC、バッテリ残量、バッテリ充電状態など。[GetSystemPowerStatus関数で取れる情報の変化時](https://learn.microsoft.com/ja-jp/windows/win32/api/winbase/ns-winbase-system_power_status)と思われる）                                                                                                                                                         |
|                    | WM_QUERYENDSESSION   |                            |                                   | セッション終了（ログオフもしくはシャットダウン）の前段階に来る。このMsgに対して0を返すと、シャットダウンを中断させるという意味になり、Windowsが「・・妨げています」というUIを出すことになる                                                                                                                                                                               |
|                    | WM_ENDSESSION        |                            |                                   | セッション終了（ログオフもしくはシャットダウン）。ログオフorシャットダウン時に時間のかかる保存処理等をするときはここでやるのが推奨。<br/>[参考1：MS公式](https://learn.microsoft.com/en-us/windows/win32/shutdown/shutdown-changes-for-windows-vista)、[参考2：その他サイト](https://kodamadeveloped.main.jp/lets-programming/details_wxwidgets_and_system_shutdown.html) |
|                    | WM_WTSSESSION_CHANGE | WTS_CONSOLE_CONNECT        | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |
|                    |                      | WTS_CONSOLE_DISCONNECT     | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |
|                    |                      | WTS_REMOTE_CONNECT         | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |
|                    |                      | WTS_REMOTE_DISCONNECT      | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |
|                    |                      | WTS_SESSION_LOGON          | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |
|                    |                      | WTS_SESSION_LOGOFF         | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |
|                    |                      | WTS_SESSION_LOCK           | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |
|                    |                      | WTS_SESSION_UNLOCK         | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |
|                    |                      | WTS_SESSION_REMOTE_CONTROL | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |
|                    |                      | WTS_SESSION_CREATE         | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |
|                    |                      | WTS_SESSION_TERMINATE      | セッション番号                    |                                                                                                                                                                                                                                                                                                                                                                           |

※WM_POWERBROADCAST/PBT_POWERSETTINGCHANGE到来時の「パラメータ」は、`((PPOWERBROADCAST_SETTING)lp)->Data[0]`を見ることとする。




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
