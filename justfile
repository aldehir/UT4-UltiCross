set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]

Platform := "Win64"
Version := "1.0-alpha.5"

BuildBat := "../../../Engine/Build/BatchFiles/Build.bat"
EditorExe := "../../../Engine/Binaries/Win64/UE4Editor-Win64-Debug.exe"
EditorArgs := "UnrealTournament"
GameArgs := "UnrealTournament -Game /Game/RestrictedAssets/Maps/Example_Map -ResX=1280 -ResY=768 -Windowed"

build:
  {{ BuildBat }} UnrealTournamentEditor {{ Platform }} Debug -waitmutex

debug:
  WinDbgX {{ EditorExe }} {{ GameArgs }}

edit:
  {{ EditorExe }} {{ EditorArgs }}

play:
  {{ EditorExe }} {{ GameArgs }}

build-editor:
  {{ BuildBat }} UnrealTournamentEditor {{ Platform }} Development -waitmutex

build-game:
  {{ BuildBat }} UnrealTournament {{ Platform }} Shipping -waitmutex

package: build-editor build-game
  If (Test-Path Release/{{ Version }}) { Remove-Item Release/{{ Version }} -Recurse | Out-Null }
  If (Test-Path Release/UT4-UltiCross-{{ Version }}.zip) { Remove-Item Release/UT4-UltiCross-{{ Version }}.zip | Out-Null }
  New-Item Release/{{ Version }} -ItemType Directory | Out-Null
  New-Item Release/{{ Version }}/UnrealTournament -ItemType Directory | Out-Null
  New-Item Release/{{ Version }}/UnrealTournament/Plugins -ItemType Directory | Out-Null
  New-Item Release/{{ Version }}/UnrealTournament/Plugins/UltiCross -ItemType Directory | Out-Null
  New-Item Release/{{ Version }}/UnrealTournament/Plugins/UltiCross/Binaries -ItemType Directory | Out-Null
  New-Item Release/{{ Version }}/UnrealTournament/Plugins/UltiCross/Binaries/{{ Platform }} -ItemType Directory | Out-Null
  New-Item Release/{{ Version }}/UnrealTournament/Content -ItemType Directory | Out-Null
  New-Item Release/{{ Version }}/UnrealTournament/Content/Paks -ItemType Directory | Out-Null
  New-Item Release/{{ Version }}/UnrealTournament/Content/Paks/UltiCross -ItemType Directory | Out-Null
  Copy-Item UltiCross.uplugin Release/{{ Version }}/UnrealTournament/Plugins/UltiCross/
  Copy-Item Binaries/Win64/UE4-UltiCross-{{ Platform }}-Shipping.dll Release/{{ Version }}/UnrealTournament/Plugins/UltiCross/Binaries/{{ Platform }}/
  Copy-Item Binaries/Win64/UE4Editor-UltiCross.dll Release/{{ Version }}/UnrealTournament/Plugins/UltiCross/Binaries/{{ Platform }}/
  Copy-Item Content/Paks/*.pak Release/{{ Version }}/UnrealTournament/Content/Paks/UltiCross/
  cd Release/{{ Version }}; Compress-Archive UnrealTournament ../UT4-UltiCross-{{ Version }}.zip
