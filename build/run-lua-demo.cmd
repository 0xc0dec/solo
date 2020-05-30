set BUILD_DIR=%1
set DEMO_NUMBER=%2
set RENDER_BACKEND=%3

pushd "bin/%BUILD_DIR%"

Solr.exe "../../../src/demos/lua/demo%DEMO_NUMBER%/demo_%RENDER_BACKEND%.lua"

popd
