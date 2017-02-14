all: quip launcher

clean:
	rm -rf build

quip: build/Release/Quip.app

launcher: build/Release/quip

build/Release/Quip.app:
	xcodebuild build -project Quip.xcodeproj -target Quip -configuration Release

build/Release/quip:
	xcodebuild build -project Quip.xcodeproj -target Quip.Launcher -configuration Release

install: build/Release/Quip.app
	cp -R build/Release/Quip.app /Applications
	cp -R build/Release/quip ~/.bin

.PHONY: all clean
