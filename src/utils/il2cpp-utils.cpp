#include "audica-hook/utils/il2cpp-utils.hpp"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <string_view>

#include "audica-hook/utils/utils.hpp"
#include "utils/export.hpp"

using namespace AudicaHook::Logging;

// Please see comments in il2cpp-utils.hpp
// TODO: Make this into a static class
namespace il2cpp_utils {
// Maximum length of characters of an exception message - 1
#define EXCEPTION_MESSAGE_SIZE 4096

    // Returns a legible string from an Il2CppException*
    HOOK_EXPORT std::string ExceptionToString(Il2CppException* exp) {
        char msg[EXCEPTION_MESSAGE_SIZE];
        il2cpp_functions::format_exception(exp, msg, EXCEPTION_MESSAGE_SIZE);
        // auto exception_message = csstrtostr(exp->message);
        // return to_utf8(exception_message);
        return msg;
    }

    HOOK_EXPORT bool ParameterMatch(MethodInfo const* method, Il2CppType** type_arr, int count) {
        if (method->parameters_count != count) {
            return false;
        }
        for (int i = 0; i < method->parameters_count; i++) {
            if (!il2cpp_functions::type_equals(method->parameters[i].parameter_type, type_arr[i])) {
                return false;
            }
        }
        return true;
    }

    HOOK_EXPORT Il2CppClass* GetClassFromName(char const* name_space, char const* type_name) {
        il2cpp_functions::Init();

        auto dom = il2cpp_functions::domain_get();
        if (!dom) {
            Logger.error("GetClassFromName: Could not get domain!");
            return nullptr;
        }
        size_t assemb_count;
        Il2CppAssembly const** allAssemb = il2cpp_functions::domain_get_assemblies(dom, &assemb_count);

        for (int i = 0; i < assemb_count; i++) {
            auto assemb = allAssemb[i];
            auto img = il2cpp_functions::assembly_get_image(assemb);
            if (!img) {
                Logger.error("Assembly with name: {} has a null image!", assemb->aname.name);
                continue;
            }
            auto klass = il2cpp_functions::class_from_name(img, name_space, type_name);
            if (klass) {
                return klass;
            }
        }
        Logger.error("il2cpp_utils: GetClassFromName: Could not find class with namepace: {} and name: {}", name_space, type_name);
        return nullptr;
    }

    HOOK_EXPORT MethodInfo const* GetMethod(Il2CppClass* klass, std::string_view methodName, int argsCount) {
        if (!klass) {
            return nullptr;
        }
        auto methodInfo = il2cpp_functions::class_get_method_from_name(klass, methodName.data(), argsCount);
        if (!methodInfo) {
            Logger.error(
                "could not find method {} with {} parameters in class {} (namespace '{}')!",
                methodName.data(),
                argsCount,
                il2cpp_functions::class_get_name(klass),
                il2cpp_functions::class_get_namespace(klass)
            );
            return nullptr;
        }
        return methodInfo;
    }

    HOOK_EXPORT MethodInfo const* GetMethod(std::string_view nameSpace, std::string_view className, std::string_view methodName, int argsCount) {
        return GetMethod(GetClassFromName(nameSpace.data(), className.data()), methodName, argsCount);
    }

    HOOK_EXPORT FieldInfo* FindField(Il2CppClass* klass, std::string_view fieldName) {
        if (!klass) {
            return nullptr;
        }
        auto field = il2cpp_functions::class_get_field_from_name(klass, fieldName.data());
        if (!field) {
            Logger.error(
                "could not find field {} in class {} (namespace '{}')!",
                fieldName.data(),
                il2cpp_functions::class_get_name(klass),
                il2cpp_functions::class_get_namespace(klass)
            );
        }
        return field;
    }

    HOOK_EXPORT bool SetFieldValue(Il2CppObject* instance, FieldInfo* field, void* value) {
        il2cpp_functions::Init();
        if (!field) {
            Logger.error("il2cpp_utils: SetFieldValue: Null field parameter!");
            return false;
        }
        if (instance) {
            il2cpp_functions::field_set_value(instance, field, value);
        } else {  // Fallback to perform a static field set
            il2cpp_functions::field_static_set_value(field, value);
        }
        return true;
    }

    HOOK_EXPORT bool SetFieldValue(Il2CppClass* klass, std::string_view fieldName, void* value) {
        il2cpp_functions::Init();
        if (!klass) {
            Logger.error("il2cpp_utils: SetFieldValue: Null klass parameter!");
            return false;
        }
        auto field = FindField(klass, fieldName);
        if (!field) {
            return false;
        }
        return SetFieldValue(nullptr, field, value);
    }

    HOOK_EXPORT bool SetFieldValue(Il2CppObject* instance, std::string_view fieldName, void* value) {
        il2cpp_functions::Init();
        if (!instance) {
            Logger.error("il2cpp_utils: SetFieldValue: Null instance parameter!");
            return false;
        }
        auto klass = il2cpp_functions::object_get_class(instance);
        if (!klass) {
            Logger.error("il2cpp_utils: SetFieldValue: Could not find object class!");
            return false;
        }
        auto field = FindField(klass, fieldName);
        if (!field) {
            return false;
        }
        return SetFieldValue(instance, field, value);
    }

    HOOK_EXPORT Il2CppReflectionType* MakeGenericType(Il2CppReflectionType* gt, Il2CppArray* types) {
        il2cpp_functions::Init();

        auto runtimeType = GetClassFromName("System", "RuntimeType");
        if (!runtimeType) {
            Logger.error("il2cpp_utils: MakeGenericType: Failed to get System.RuntimeType!");
            return nullptr;
        }
        auto makeGenericMethod = il2cpp_functions::class_get_method_from_name(runtimeType, "MakeGenericType", 2);
        if (!makeGenericMethod) {
            Logger.error("il2cpp_utils: MakeGenericType: Failed to get RuntimeType.MakeGenericType(param1, param2) method!");
            return nullptr;
        }

        Il2CppException* exp = nullptr;
        void* params[] = {reinterpret_cast<void*>(gt), reinterpret_cast<void*>(types)};
        auto genericType = il2cpp_functions::runtime_invoke(makeGenericMethod, nullptr, params, &exp);
        if (exp) {
            Logger.error("il2cpp_utils: MakeGenericType: Failed with exception: {}", ExceptionToString(exp).c_str());
            return nullptr;
        }
        return reinterpret_cast<Il2CppReflectionType*>(genericType);
    }

    HOOK_EXPORT Il2CppClass* MakeGeneric(Il2CppClass const* klass, std::initializer_list<Il2CppClass const*> args) {
        il2cpp_functions::Init();

        auto typ = GetClassFromName("System", "Type");
        if (!typ) {
            return nullptr;
        }
        auto getType = il2cpp_functions::class_get_method_from_name(typ, "GetType", 1);
        if (!getType) {
            Logger.error("il2cpp_utils: MakeGeneric: Failed to get System.Type.GetType(param1) method!");
            return nullptr;
        }

        auto klassType = il2cpp_functions::type_get_object(il2cpp_functions::class_get_type_const(klass));
        if (!klassType) {
            Logger.error("il2cpp_utils: MakeGeneric: Failed to get class type object!");
            return nullptr;
        }

        // Call Type.MakeGenericType on it
        auto a = il2cpp_functions::array_new_specific(typ, args.size());
        if (!a) {
            Logger.error("il2cpp_utils: MakeGeneric: Failed to make new array with length: {}", args.size());
            return nullptr;
        }

        int i = 0;
        for (auto arg : args) {
            auto t = il2cpp_functions::class_get_type_const(arg);
            auto o = il2cpp_functions::type_get_object(t);
            if (!o) {
                Logger.error("il2cpp_utils: MakeGeneric: Failed to get type for {}", il2cpp_functions::class_get_name_const(arg));
                return nullptr;
            }
            il2cpp_array_set(a, void*, i, reinterpret_cast<void*>(o));
            i++;
        }

        auto reflection_type = MakeGenericType(reinterpret_cast<Il2CppReflectionType*>(klassType), a);
        if (!reflection_type) {
            Logger.error("il2cpp_utils: MakeGeneric: Failed to MakeGenericType from Il2CppReflectionType and Il2CppArray!");
            return nullptr;
        }

        auto ret = il2cpp_functions::class_from_system_type(reinterpret_cast<Il2CppReflectionType*>(reflection_type));
        if (!ret) {
            Logger.error("il2cpp_utils: MakeGeneric: Failed to get class from Il2CppReflectionType!");
            return nullptr;
        }
        Logger.debug("il2cpp_utils: MakeGeneric: returning {}", il2cpp_functions::class_get_name(ret));
        return ret;
    }

    // Gets the type enum of a given type
    // TODO Remove this method! Replace with default typesystem
    int GetTypeEnum(char const* name_space, char const* type_name) {
        auto klass = GetClassFromName(name_space, type_name);
        auto typ = il2cpp_functions::class_get_type(klass);
        return il2cpp_functions::type_get_type(typ);
    }

    static std::unordered_map<int, char const*> typeMap;

    HOOK_EXPORT char const* TypeGetSimpleName(Il2CppType const* type) {
        if (typeMap.empty()) {
            typeMap[GetTypeEnum("System", "Boolean")] = "bool";
            typeMap[GetTypeEnum("System", "Byte")] = "byte";
            typeMap[GetTypeEnum("System", "SByte")] = "sbyte";
            typeMap[GetTypeEnum("System", "Char")] = "char";
            typeMap[GetTypeEnum("System", "Single")] = "float";
            typeMap[GetTypeEnum("System", "Double")] = "double";
            typeMap[GetTypeEnum("System", "Int16")] = "short";
            typeMap[GetTypeEnum("System", "UInt16")] = "ushort";
            typeMap[GetTypeEnum("System", "Int32")] = "int";
            typeMap[GetTypeEnum("System", "UInt32")] = "uint";
            typeMap[GetTypeEnum("System", "Int64")] = "long";
            typeMap[GetTypeEnum("System", "UInt64")] = "ulong";
            typeMap[GetTypeEnum("System", "Object")] = "object";
            typeMap[GetTypeEnum("System", "String")] = "string";
            typeMap[GetTypeEnum("System", "Void")] = "void";
        }
        auto p = typeMap.find(il2cpp_functions::type_get_type(type));
        if (p != typeMap.end()) {
            return p->second;
        } else {
            return il2cpp_functions::type_get_name(type);
        }
    }

    HOOK_EXPORT void LogMethod(MethodInfo const* method) {
        il2cpp_functions::Init();

        auto flags = il2cpp_functions::method_get_flags(method, nullptr);
        std::stringstream flagStream;
        if (flags & METHOD_ATTRIBUTE_STATIC) {
            flagStream << "static ";
        }
        if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
            flagStream << "virtual ";
        }
        if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
            flagStream << "abstract ";
        }
        auto const& flagStrRef = flagStream.str();
        char const* flagStr = flagStrRef.c_str();
        auto retType = il2cpp_functions::method_get_return_type(method);
        auto retTypeStr = TypeGetSimpleName(retType);
        auto methodName = il2cpp_functions::method_get_name(method);
        methodName = methodName ? methodName : "__noname__";
        std::stringstream paramStream;
        for (int i = 0; i < il2cpp_functions::method_get_param_count(method); i++) {
            if (i > 0) {
                paramStream << ", ";
            }
            auto paramType = il2cpp_functions::method_get_param(method, i);
            if (il2cpp_functions::type_is_byref(paramType)) {
                paramStream << "out/ref ";
            }
            paramStream << TypeGetSimpleName(paramType) << " ";
            auto name = il2cpp_functions::method_get_param_name(method, i);
            paramStream << (name ? name : "__noname__");
        }
        auto const& paramStrRef = paramStream.str();
        char const* paramStr = paramStrRef.c_str();
        Logger.debug("{}{} {}({});", flagStr, retTypeStr, methodName, paramStr);
    }

    HOOK_EXPORT void LogField(FieldInfo* field) {
        il2cpp_functions::Init();

        auto flags = il2cpp_functions::field_get_flags(field);
        char const* flagStr = (flags & FIELD_ATTRIBUTE_STATIC) ? "static " : "";
        auto type = il2cpp_functions::field_get_type(field);
        auto typeStr = TypeGetSimpleName(type);
        auto name = il2cpp_functions::field_get_name(field);
        name = name ? name : "__noname__";
        auto offset = il2cpp_functions::field_get_offset(field);

        Logger.debug("{}{} {}; // 0x{:x}, flags: 0x{:#04X}", flagStr, typeStr, name, offset, flags);
    }

    HOOK_EXPORT void LogClass(Il2CppClass const* klass, bool logParents) {
        il2cpp_functions::Init();

        auto unconst = const_cast<Il2CppClass*>(klass);
        Logger.debug(
            "======================CLASS INFO FOR CLASS: {}::{}======================",
            il2cpp_functions::class_get_namespace(unconst),
            il2cpp_functions::class_get_name(unconst)
        );
        Logger.debug("Assembly Name: {}", il2cpp_functions::class_get_assemblyname(klass));
        Logger.debug("Rank: {}", il2cpp_functions::class_get_rank(klass));
        Logger.debug("Type Token: {}", il2cpp_functions::class_get_type_token(unconst));
        Logger.debug("Flags: 0x{:#08X}", il2cpp_functions::class_get_flags(klass));
        Logger.debug("Event Count: {}", klass->event_count);
        Logger.debug("Field Count: {}", klass->field_count);
        Logger.debug("Method Count: {}", klass->method_count);
        Logger.debug("Property Count: {}", klass->property_count);
        Logger.debug("Is Generic: {}", il2cpp_functions::class_is_generic(klass));
        Logger.debug("Is Abstract: {}", il2cpp_functions::class_is_abstract(klass));
        Logger.debug("=========METHODS=========");
        void* myIter = nullptr;
        for (int i = 0; i < unconst->method_count; i++) {
            if (unconst->methods[i]) {
                Logger.debug("Method {}:", i);
                Logger.debug("Name: {} Params: {}", unconst->methods[i]->name, unconst->methods[i]->parameters_count);
            } else {
                Logger.debug("Method: {} Does not exist!", i);
            }
        }
        auto genClass = klass->generic_class;
        if (genClass) {
            auto genContext = &genClass->context;
            auto genInst = genContext->class_inst;
            if (genInst) {
                for (int i = 0; i < genInst->type_argc; i++) {
                    auto typ = genInst->type_argv[i];
                    Logger.debug(" generic type {}: {}", i + 1, TypeGetSimpleName(typ));
                }
            }
        }
        auto declaring = il2cpp_functions::class_get_declaring_type(unconst);
        Logger.debug("declaring type: {}", reinterpret_cast<uint64_t>(declaring));
        if (declaring) {
            LogClass(declaring);
        }
        auto element = il2cpp_functions::class_get_element_class(unconst);
        Logger.debug("element class: {} (self = {})", reinterpret_cast<uint64_t>(element), reinterpret_cast<uint64_t>(klass));
        if (element && element != klass) {
            LogClass(element);
        }

        Logger.debug("=========FIELDS=========");
        myIter = nullptr;
        FieldInfo* field;
        while ((field = il2cpp_functions::class_get_fields(unconst, &myIter))) {
            LogField(field);
        }
        Logger.debug("=========END FIELDS=========");

        auto parent = il2cpp_functions::class_get_parent(unconst);
        Logger.debug("parent: {}", reinterpret_cast<uint64_t>(parent));
        if (parent && logParents) {
            LogClass(parent);
        }
        Logger.debug("==================================================================================");
    }

    HOOK_EXPORT Il2CppString* createcsstr(std::string_view inp) {
        il2cpp_functions::Init();
        return il2cpp_functions::string_new_len(inp.data(), (uint32_t) inp.length());
    }

    [[nodiscard]] HOOK_EXPORT bool Match(Il2CppObject const* source, Il2CppClass const* klass) noexcept {
        return (source->klass == klass);
    }

    HOOK_EXPORT bool AssertMatch(Il2CppObject const* source, Il2CppClass const* klass) {
        il2cpp_functions::Init();
        if (!Match(source, klass)) {
            Logger.critical(
                "il2cpp_utils: AssertMatch: Unhandled subtype: namespace {}, class {}",
                il2cpp_functions::class_get_namespace(source->klass),
                il2cpp_functions::class_get_name(source->klass)
            );
            std::terminate();
        }
        return true;
    }
}  // namespace il2cpp_utils
