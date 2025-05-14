#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "typedefs.h"

// A class which contains all available il2cpp functions
// Created by zoller27osu
class il2cpp_functions {
   public:
    // note: the "inline" just make these definitions as well as declarations, so they don't have to be defined in the .cpp
    inline static void (*init)(char const* domain_name);
    inline static void (*init_utf16)(Il2CppChar const* domain_name);
    inline static void (*shutdown)();
    inline static void (*set_config_dir)(char const* config_path);
    inline static void (*set_data_dir)(char const* data_path);
    inline static void (*set_temp_dir)(char const* temp_path);
    inline static void (*set_commandline_arguments)(int argc, char const* const argv[], char const* basedir);
    inline static void (*set_commandline_arguments_utf16)(int argc, Il2CppChar const* const argv[], char const* basedir);
    inline static void (*set_config_utf16)(Il2CppChar const* executablePath);
    inline static void (*set_config)(char const* executablePath);
    inline static void (*set_memory_callbacks)(Il2CppMemoryCallbacks* callbacks);
    inline static Il2CppImage const* (*get_corlib)();
    inline static void (*add_internal_call)(char const* name, Il2CppMethodPointer method);
    inline static Il2CppMethodPointer (*resolve_icall)(char const* name);
    inline static void* (*alloc)(size_t size);
    inline static void (*free)(void* ptr);
    inline static Il2CppClass* (*array_class_get)(Il2CppClass* element_class, uint32_t rank);
    inline static uint32_t (*array_length)(Il2CppArray* array);
    inline static uint32_t (*array_get_byte_length)(Il2CppArray* array);
    inline static Il2CppArray* (*array_new)(Il2CppClass* elementTypeInfo, il2cpp_array_size_t length);
    inline static Il2CppArray* (*array_new_specific)(Il2CppClass* arrayTypeInfo, il2cpp_array_size_t length);
    inline static Il2CppArray* (*array_new_full)(Il2CppClass* array_class, il2cpp_array_size_t* lengths, il2cpp_array_size_t* lower_bounds);
    inline static Il2CppClass* (*bounded_array_class_get)(Il2CppClass* element_class, uint32_t rank, bool bounded);
    inline static int (*array_element_size)(Il2CppClass const* array_class);
    inline static Il2CppImage const* (*assembly_get_image)(Il2CppAssembly const* assembly);
    inline static Il2CppType const* (*class_enum_basetype)(Il2CppClass* klass);
    inline static bool (*class_is_generic)(Il2CppClass const* klass);
    inline static bool (*class_is_inflated)(Il2CppClass const* klass);
    inline static bool (*class_is_assignable_from)(Il2CppClass* klass, Il2CppClass* oklass);
    inline static bool (*class_is_subclass_of)(Il2CppClass* klass, Il2CppClass* klassc, bool check_interfaces);
    inline static bool (*class_has_parent)(Il2CppClass* klass, Il2CppClass* klassc);
    inline static Il2CppClass* (*class_from_il2cpp_type)(Il2CppType const* type);
    inline static Il2CppClass* (*class_from_name)(Il2CppImage const* image, char const* namespaze, char const* name);
    inline static Il2CppClass* (*class_from_system_type)(Il2CppReflectionType* type);
    inline static Il2CppClass* (*class_get_element_class)(Il2CppClass* klass);
    inline static EventInfo const* (*class_get_events)(Il2CppClass* klass, void** iter);
    inline static FieldInfo* (*class_get_fields)(Il2CppClass* klass, void** iter);
    inline static Il2CppClass* (*class_get_nested_types)(Il2CppClass* klass, void** iter);
    inline static Il2CppClass* (*class_get_interfaces)(Il2CppClass* klass, void** iter);
    inline static PropertyInfo const* (*class_get_properties)(Il2CppClass* klass, void** iter);
    inline static PropertyInfo const* (*class_get_property_from_name)(Il2CppClass* klass, char const* name);
    inline static FieldInfo* (*class_get_field_from_name)(Il2CppClass* klass, char const* name);
    inline static MethodInfo const* (*class_get_methods)(Il2CppClass* klass, void** iter);
    inline static MethodInfo const* (*class_get_method_from_name)(Il2CppClass* klass, char const* name, int argsCount);
    inline static char const* (*class_get_name)(Il2CppClass* klass);
    inline static char const* (*class_get_namespace)(Il2CppClass* klass);
    inline static Il2CppClass* (*class_get_parent)(Il2CppClass* klass);
    inline static Il2CppClass* (*class_get_declaring_type)(Il2CppClass* klass);
    inline static int32_t (*class_instance_size)(Il2CppClass* klass);
    inline static size_t (*class_num_fields)(Il2CppClass const* enumKlass);
    inline static bool (*class_is_valuetype)(Il2CppClass const* klass);
    inline static int32_t (*class_value_size)(Il2CppClass* klass, uint32_t* align);
    inline static bool (*class_is_blittable)(Il2CppClass const* klass);
    inline static int (*class_get_flags)(Il2CppClass const* klass);
    inline static bool (*class_is_abstract)(Il2CppClass const* klass);
    inline static bool (*class_is_interface)(Il2CppClass const* klass);
    inline static int (*class_array_element_size)(Il2CppClass const* klass);
    inline static Il2CppClass* (*class_from_type)(Il2CppType const* type);
    inline static Il2CppType const* (*class_get_type)(Il2CppClass* klass);
    inline static uint32_t (*class_get_type_token)(Il2CppClass* klass);
    inline static bool (*class_has_attribute)(Il2CppClass* klass, Il2CppClass* attr_class);
    inline static bool (*class_has_references)(Il2CppClass* klass);
    inline static bool (*class_is_enum)(Il2CppClass const* klass);
    inline static Il2CppImage const* (*class_get_image)(Il2CppClass* klass);
    inline static char const* (*class_get_assemblyname)(Il2CppClass const* klass);
    inline static int (*class_get_rank)(Il2CppClass const* klass);
    inline static size_t (*class_get_bitmap_size)(Il2CppClass const* klass);
    inline static void (*class_get_bitmap)(Il2CppClass* klass, size_t* bitmap);
    inline static bool (*stats_dump_to_file)(char const* path);
    inline static uint64_t (*stats_get_value)(Il2CppStat stat);
    inline static Il2CppDomain* (*domain_get)();
    inline static Il2CppAssembly const* (*domain_assembly_open)(Il2CppDomain* domain, char const* name);
    inline static Il2CppAssembly const** (*domain_get_assemblies)(Il2CppDomain const* domain, size_t* size);
    inline static Il2CppException* (*exception_from_name_msg)(Il2CppImage const* image, char const* name_space, char const* name, char const* msg);
    inline static Il2CppException* (*get_exception_argument_null)(char const* arg);
    inline static void (*format_exception)(Il2CppException const* ex, char* message, int message_size);
    inline static void (*format_stack_trace)(Il2CppException const* ex, char* output, int output_size);
    inline static void (*unhandled_exception)(Il2CppException*);
    inline static int (*field_get_flags)(FieldInfo* field);
    inline static char const* (*field_get_name)(FieldInfo* field);
    inline static Il2CppClass* (*field_get_parent)(FieldInfo* field);
    inline static size_t (*field_get_offset)(FieldInfo* field);
    inline static Il2CppType const* (*field_get_type)(FieldInfo* field);
    inline static void (*field_get_value)(Il2CppObject* obj, FieldInfo* field, void* value);
    inline static Il2CppObject* (*field_get_value_object)(FieldInfo* field, Il2CppObject* obj);
    inline static bool (*field_has_attribute)(FieldInfo* field, Il2CppClass* attr_class);
    inline static void (*field_set_value)(Il2CppObject* obj, FieldInfo* field, void* value);
    inline static void (*field_static_get_value)(FieldInfo* field, void* value);
    inline static void (*field_static_set_value)(FieldInfo* field, void* value);
    inline static void (*field_set_value_object)(Il2CppObject* instance, FieldInfo* field, Il2CppObject* value);
    inline static void (*gc_collect)(int maxGenerations);
    inline static int32_t (*gc_collect_a_little)();
    inline static void (*gc_disable)();
    inline static void (*gc_enable)();
    inline static bool (*gc_is_disabled)();
    inline static int64_t (*gc_get_used_size)();
    inline static int64_t (*gc_get_heap_size)();
    inline static void (*gc_wbarrier_set_field)(Il2CppObject* obj, void** targetAddress, void* object);
    inline static uint32_t (*gchandle_new)(Il2CppObject* obj, bool pinned);
    inline static uint32_t (*gchandle_new_weakref)(Il2CppObject* obj, bool track_resurrection);
    inline static Il2CppObject* (*gchandle_get_target)(uint32_t gchandle);
    inline static void (*gchandle_free)(uint32_t gchandle);
    inline static void* (*unity_liveness_calculation_begin)(
        Il2CppClass* filter,
        int max_object_count,
        il2cpp_register_object_callback callback,
        void* userdata,
        il2cpp_WorldChangedCallback onWorldStarted,
        il2cpp_WorldChangedCallback onWorldStopped
    );
    inline static void (*unity_liveness_calculation_end)(void* state);
    inline static void (*unity_liveness_calculation_from_root)(Il2CppObject* root, void* state);
    inline static void (*unity_liveness_calculation_from_statics)(void* state);
    inline static Il2CppType const* (*method_get_return_type)(MethodInfo const* method);
    inline static Il2CppClass* (*method_get_declaring_type)(MethodInfo const* method);
    inline static char const* (*method_get_name)(MethodInfo const* method);
    inline static MethodInfo const* (*method_get_from_reflection)(Il2CppReflectionMethod const* method);
    inline static Il2CppReflectionMethod* (*method_get_object)(MethodInfo const* method, Il2CppClass* refclass);
    inline static bool (*method_is_generic)(MethodInfo const* method);
    inline static bool (*method_is_inflated)(MethodInfo const* method);
    inline static bool (*method_is_instance)(MethodInfo const* method);
    inline static uint32_t (*method_get_param_count)(MethodInfo const* method);
    inline static Il2CppType const* (*method_get_param)(MethodInfo const* method, uint32_t index);
    inline static Il2CppClass* (*method_get_class)(MethodInfo const* method);
    inline static bool (*method_has_attribute)(MethodInfo const* method, Il2CppClass* attr_class);
    inline static uint32_t (*method_get_flags)(MethodInfo const* method, uint32_t* iflags);
    inline static uint32_t (*method_get_token)(MethodInfo const* method);
    inline static char const* (*method_get_param_name)(MethodInfo const* method, uint32_t index);
    inline static void (*profiler_install)(Il2CppProfiler* prof, Il2CppProfileFunc shutdown_callback);
    inline static void (*profiler_set_events)(Il2CppProfileFlags events);
    inline static void (*profiler_install_enter_leave)(Il2CppProfileMethodFunc enter, Il2CppProfileMethodFunc fleave);
    inline static void (*profiler_install_allocation)(Il2CppProfileAllocFunc callback);
    inline static void (*profiler_install_gc)(Il2CppProfileGCFunc callback, Il2CppProfileGCResizeFunc heap_resize_callback);
    inline static void (*profiler_install_fileio)(Il2CppProfileFileIOFunc callback);
    inline static void (*profiler_install_thread)(Il2CppProfileThreadFunc start, Il2CppProfileThreadFunc end);
    inline static uint32_t (*property_get_flags)(PropertyInfo* prop);
    inline static MethodInfo const* (*property_get_get_method)(PropertyInfo* prop);
    inline static MethodInfo const* (*property_get_set_method)(PropertyInfo* prop);
    inline static char const* (*property_get_name)(PropertyInfo* prop);
    inline static Il2CppClass* (*property_get_parent)(PropertyInfo* prop);
    inline static Il2CppClass* (*object_get_class)(Il2CppObject* obj);
    inline static uint32_t (*object_get_size)(Il2CppObject* obj);
    inline static MethodInfo const* (*object_get_virtual_method)(Il2CppObject* obj, MethodInfo const* method);
    inline static Il2CppObject* (*object_new)(Il2CppClass const* klass);
    inline static void* (*object_unbox)(Il2CppObject* obj);
    inline static Il2CppObject* (*value_box)(Il2CppClass* klass, void* data);
    inline static void (*monitor_enter)(Il2CppObject* obj);
    inline static bool (*monitor_try_enter)(Il2CppObject* obj, uint32_t timeout);
    inline static void (*monitor_exit)(Il2CppObject* obj);
    inline static void (*monitor_pulse)(Il2CppObject* obj);
    inline static void (*monitor_pulse_all)(Il2CppObject* obj);
    inline static void (*monitor_wait)(Il2CppObject* obj);
    inline static bool (*monitor_try_wait)(Il2CppObject* obj, uint32_t timeout);
    inline static Il2CppObject* (*runtime_invoke)(MethodInfo const* method, void* obj, void** params, Il2CppException** exc);
    inline static Il2CppObject* (*runtime_invoke_convert_args)(
        MethodInfo const* method, void* obj, Il2CppObject** params, int paramCount, Il2CppException** exc
    );
    inline static void (*runtime_class_init)(Il2CppClass* klass);
    inline static void (*runtime_object_init)(Il2CppObject* obj);
    inline static void (*runtime_object_init_exception)(Il2CppObject* obj, Il2CppException** exc);
    inline static void (*runtime_unhandled_exception_policy_set)(Il2CppRuntimeUnhandledExceptionPolicy value);
    inline static int32_t (*string_length)(Il2CppString* str);
    inline static Il2CppChar* (*string_chars)(Il2CppString* str);
    inline static Il2CppString* (*string_new)(char const* str);
    inline static Il2CppString* (*string_new_len)(char const* str, uint32_t length);
    inline static Il2CppString* (*string_new_utf16)(Il2CppChar const* text, int32_t len);
    inline static Il2CppString* (*string_new_wrapper)(char const* str);
    inline static Il2CppString* (*string_intern)(Il2CppString* str);
    inline static Il2CppString* (*string_is_interned)(Il2CppString* str);
    inline static Il2CppThread* (*thread_current)();
    inline static Il2CppThread* (*thread_attach)(Il2CppDomain* domain);
    inline static void (*thread_detach)(Il2CppThread* thread);
    inline static Il2CppThread** (*thread_get_all_attached_threads)(size_t* size);
    inline static bool (*is_vm_thread)(Il2CppThread* thread);
    inline static void (*current_thread_walk_frame_stack)(Il2CppFrameWalkFunc func, void* user_data);
    inline static void (*thread_walk_frame_stack)(Il2CppThread* thread, Il2CppFrameWalkFunc func, void* user_data);
    inline static bool (*current_thread_get_top_frame)(Il2CppStackFrameInfo* frame);
    inline static bool (*thread_get_top_frame)(Il2CppThread* thread, Il2CppStackFrameInfo* frame);
    inline static bool (*current_thread_get_frame_at)(int32_t offset, Il2CppStackFrameInfo* frame);
    inline static bool (*thread_get_frame_at)(Il2CppThread* thread, int32_t offset, Il2CppStackFrameInfo* frame);
    inline static int32_t (*current_thread_get_stack_depth)();
    inline static int32_t (*thread_get_stack_depth)(Il2CppThread* thread);
    inline static Il2CppObject* (*type_get_object)(Il2CppType const* type);
    inline static int (*type_get_type)(Il2CppType const* type);
    inline static Il2CppClass* (*type_get_class_or_element_class)(Il2CppType const* type);
    inline static char* (*type_get_name)(Il2CppType const* type);
    inline static bool (*type_is_byref)(Il2CppType const* type);
    inline static uint32_t (*type_get_attrs)(Il2CppType const* type);
    inline static bool (*type_equals)(Il2CppType const* type, Il2CppType const* otherType);
    inline static char* (*type_get_assembly_qualified_name)(Il2CppType const* type);
    inline static Il2CppAssembly const* (*image_get_assembly)(Il2CppImage const* image);
    inline static char const* (*image_get_name)(Il2CppImage const* image);
    inline static char const* (*image_get_filename)(Il2CppImage const* image);
    inline static MethodInfo const* (*image_get_entry_point)(Il2CppImage const* image);
    inline static size_t (*image_get_class_count)(Il2CppImage const* image);
    inline static Il2CppClass const* (*image_get_class)(Il2CppImage const* image, size_t index);
    inline static Il2CppManagedMemorySnapshot* (*capture_memory_snapshot)();
    inline static void (*free_captured_memory_snapshot)(Il2CppManagedMemorySnapshot* snapshot);
    inline static void (*set_find_plugin_callback)(Il2CppSetFindPlugInCallback method);
    inline static void (*register_log_callback)(Il2CppLogCallback method);
    inline static void (*debugger_set_agent_options)(char const* options);
    inline static bool (*is_debugger_attached)();
    inline static void (*unity_install_unitytls_interface)(void const* unitytlsInterfaceStruct);
    inline static Il2CppCustomAttrInfo* (*custom_attrs_from_class)(Il2CppClass* klass);
    inline static Il2CppCustomAttrInfo* (*custom_attrs_from_method)(MethodInfo const* method);
    inline static Il2CppObject* (*custom_attrs_get_attr)(Il2CppCustomAttrInfo* ainfo, Il2CppClass* attr_klass);
    inline static bool (*custom_attrs_has_attr)(Il2CppCustomAttrInfo* ainfo, Il2CppClass* attr_klass);
    inline static Il2CppArray* (*custom_attrs_construct)(Il2CppCustomAttrInfo* cinfo);
    inline static void (*custom_attrs_free)(Il2CppCustomAttrInfo* ainfo);

    // MANUALLY DEFINED CONST DEFINITIONS
    inline static Il2CppType const* (*class_get_type_const)(Il2CppClass const* klass);
    inline static char const* (*class_get_name_const)(Il2CppClass const* klass);

    // Whether all of the il2cpp functions have been initialized or not
    inline static bool initialized = false;
    // Initializes all of the IL2CPP functions via dlopen and dlsym for use.
    static void Init();
};
