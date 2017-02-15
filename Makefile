all: quip launcher

quip:
	xcodebuild build -project Quip.xcodeproj -target Quip -configuration Release

launcher:
	xcodebuild build -project Quip.xcodeproj -target Quip.Launcher -configuration Release

install: quip launcher
	cp -R build/Release/Quip.app /Applications
	cp -R build/Release/quip ~/.bin

clean:
	rm -rf build

.PHONY: all quip launcher install clean
