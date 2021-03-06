cmake_minimum_required ( VERSION 2.8.8 )
project ( moai-native-events )

set (PLUGIN_TARGET  moai-native-events )

set (PLUGIN_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")


if(BUILD_IOS)
	file( GLOB PLUGIN_SRC 
		"${PLUGIN_ROOT}/MoaiNativeEventsIOS/moai-native-events/*.mm"  
		"${PLUGIN_ROOT}/MoaiNativeEventsIOS/moai-native-events/*.cpp" 
		"${PLUGIN_ROOT}/moai-native-events/*.cpp")
	set (PLUGIN_INCLUDES 
		${PLUGIN_ROOT}
		${PLUGIN_ROOT}/MoaiNativeEventsIOS
	)
elseif(BUILD_ANDROID)
	file( GLOB PLUGIN_SRC 
		"${PLUGIN_ROOT}/MoaiNativeEventsAndroid/moai-native-events/*.cpp" 
		"${PLUGIN_ROOT}/moai-native-events/*.cpp")
	set (PLUGIN_INCLUDES 
		${PLUGIN_ROOT}
	)
endif()


add_library(${PLUGIN_TARGET} STATIC ${PLUGIN_SRC} )
target_include_directories(${PLUGIN_TARGET} PUBLIC ${PLUGIN_INCLUDES})
target_link_libraries(${PLUGIN_TARGET} moai-core moai-util moai-sim zlcore)