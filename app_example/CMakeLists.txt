# Please refer to path/to/your/ameba_sdk/cmake/CMakeLists-template.cmake
set(private_sources)
ameba_list_append(private_sources app_main.c pwm_timer_mbed_RGB.c)
ameba_add_internal_library(app_example pwm_timer_mbed_RGB
    p_SOURCES
	    ${private_sources}
)
    