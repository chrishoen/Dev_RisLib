pushd %1

del /s *.ncb
del /s *.sdf
del /s /ah *.suo
rmdir /s /q ipch
rmdir /s /q Debug
rmdir /s /q Release

popd
