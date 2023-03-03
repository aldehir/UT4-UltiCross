set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]

BuildBat := "../../../Engine/Build/BatchFiles/Build.bat"
Platform := "Win64"

build:
  & {{ BuildBat }} UnrealTournamentEditor {{ Platform }} Debug -waitmutex
