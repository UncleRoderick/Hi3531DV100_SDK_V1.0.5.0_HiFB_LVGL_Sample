aux_source_directory (. SRC_LIST)

INCLUDE_DIRECTORIES(./lvgl/)

aux_source_directory (./lvgl SRC_LIST)

aux_source_directory (./lvgl/lvgl/src/core SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra SRC_LIST)

aux_source_directory (./lvgl/lvgl/src/extra/layouts/flex SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/layouts/grid SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/themes/basic SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/themes/default SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/themes/mono SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/animimg SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/calendar SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/chart SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/colorwheel SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/imgbtn SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/keyboard SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/led SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/list SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/meter SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/msgbox SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/span SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/spinbox SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/spinner SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/tabview SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/tileview SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/extra/widgets/win SRC_LIST) 

aux_source_directory (./lvgl/lvgl/src/gpu SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/misc SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/draw SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/font SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/hal SRC_LIST) 
aux_source_directory (./lvgl/lvgl/src/widgets SRC_LIST) 

aux_source_directory (./lvgl/lv_drivers/gtkdrv SRC_LIST) 
aux_source_directory (./lvgl/lv_drivers/win32drv SRC_LIST) 
aux_source_directory (./lvgl/lv_drivers/display SRC_LIST) 
aux_source_directory (./lvgl/lv_drivers/indev SRC_LIST) 
aux_source_directory (./lvgl/lv_drivers/sdl SRC_LIST) 
aux_source_directory (./lvgl/lv_drivers/wayland SRC_LIST) 

aux_source_directory (./lvgl/lv_demos/src/lv_demo_music SRC_LIST) 
aux_source_directory (./lvgl/lv_demos/src/lv_demo_music/assets SRC_LIST) 

aux_source_directory (./lvgl/lv_demos/src/lv_demo_widgets SRC_LIST) 
aux_source_directory (./lvgl/lv_demos/src/lv_demo_widgets/assets SRC_LIST) 

aux_source_directory (./lvgl/lv_demos/src/lv_demo_benchmark SRC_LIST) 
aux_source_directory (./lvgl/lv_demos/src/lv_demo_benchmark/assets SRC_LIST) 

aux_source_directory (./lvgl/lv_demos/src/lv_demo_keypad_encoder SRC_LIST) 

aux_source_directory (./lvgl/lv_demos/src/lv_demo_stress SRC_LIST) 
aux_source_directory (./lvgl/lv_demos/src/lv_demo_stress/assets SRC_LIST) 

add_executable (EXEC_SAMPLE_MPP ${SRC_LIST})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(EXEC_SAMPLE_MPP pthread dl m
	${CMAKE_SOURCE_DIR}/lib/libmpi.a
	${CMAKE_SOURCE_DIR}/lib/libhdmi.a
	${CMAKE_SOURCE_DIR}/lib/libjpeg.a
	${CMAKE_SOURCE_DIR}/lib/libVoiceEngine.a
	${CMAKE_SOURCE_DIR}/lib/libupvqe.a
	${CMAKE_SOURCE_DIR}/lib/libdnvqe.a
	${CMAKE_SOURCE_DIR}/lib/libaacdec.a
	${CMAKE_SOURCE_DIR}/lib/libaacenc.a
	${CMAKE_SOURCE_DIR}/lib/libtde.a)
