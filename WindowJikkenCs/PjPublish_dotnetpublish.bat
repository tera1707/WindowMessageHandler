cd %~dp0
dotnet publish -c Release -r win-x64 --output .\publish_10 -p:RuntimeIdentifier=win-x64 -p:Platform=x64
dotnet publish -c Release -r win-x64 --output .\publish_11 -p:RuntimeIdentifier=win-x64 -p:Platform=x64 -p:IncludeNativeLibrariesForSelfExtract=true

rem dotnet publish -c Release -r win-x64 --output .\publish_5 -p:RuntimeIdentifier=win-x64 -p:Platform=x64 -p:PublishSingleFile=true -p:IncludeNativeLibrariesForSelfExtract=true
rem dotnet publish -c Release -r win-x64 --output .\publish_4 -p:RuntimeIdentifier=win-x64 -p:Platform=x64 -p:IncludeNativeLibrariesForSelfExtract=true
rem dotnet publish -c Release -r win-x64 --output .\publish_3 -p:RuntimeIdentifier=win-x64 -p:Platform=x64 -p:PublishSingleFile=true 
rem dotnet publish -c Release -r win-x64 --output .\publish_2 --self-contained -p:RuntimeIdentifier=win-x64 -p:Platform=x64 -p:PublishSingleFile=true 
rem dotnet publish -c Release -r win-x64 --output .\publish_1 --self-contained -p:RuntimeIdentifier=win-x64 -p:Platform=x64

pause

rem https://learn.microsoft.com/ja-jp/dotnet/core/tools/dotnet-publish