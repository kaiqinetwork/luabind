set BOOST_ROOT=E:\OpenSource\boost_1_47_0
set LUA_PATH=E:\OpenSource\lua5.1
call "D:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat"
bjam --toolset=msvc-9.0 --stagedir=libs link=shared runtime-link=shared threading=multi debug
bjam --toolset=msvc-9.0 --stagedir=libs link=shared runtime-link=shared threading=multi release