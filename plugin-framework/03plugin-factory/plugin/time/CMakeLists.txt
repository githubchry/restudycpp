
# 生产类插件
set(output_name TimeCollector)
set(plugin_type Collector)
aux_source_directory(. source_files)

# 分别编译release和debug版本，仅存在-g参数区别
set(build_types debug;release)
foreach (build_type IN LISTS build_types)
    if (STATIC_LINK_MODE MATCHES OFF)
        add_library(${output_name}_${build_type} SHARED ${source_files})
        target_link_libraries(${output_name}_${build_type} PRIVATE -fPIC)
    else ()
        add_library(${output_name}_${build_type} STATIC ${source_files})
    endif ()

    set_target_properties(${output_name}_${build_type} PROPERTIES OUTPUT_NAME ${output_name})
    if (${build_type} MATCHES release)
        set_target_properties(${output_name}_${build_type} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../${plugin_type})
    else ()
        set_target_properties(${output_name}_${build_type} PROPERTIES COMPILE_FLAGS -g)
        set_target_properties(${output_name}_${build_type} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../${build_type}/${plugin_type})
    endif ()
endforeach ()

