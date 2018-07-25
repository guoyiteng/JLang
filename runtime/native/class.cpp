// This file defines important data structures associated with
// Java class objects. These data structures are used to support
// JVM/JNI functionality, such as reflection.
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <string>
#include <dlfcn.h>
#include <stdio.h>
#include "jni.h"
#include "class.h"
#include "rep.h"


#define MEMCPY(a,b,c) memcpy((void *) a, (void *) b, c)
static constexpr bool kDebug = false;

static JClassRep* intKlass = (JClassRep*)malloc(sizeof(JClassRep));
jclass initArrayKlass();
jclass globalArrayKlass = NULL;

// For simplicity we store class information in a map.
// If we find this to be too slow, we could allocate extra memory for
// class objects and store the information inline with each instance.
static std::unordered_map<jclass, const JavaClassInfo*> classes;
static std::unordered_map<std::string, const jclass> cnames;

extern "C" {

extern void Polyglot_polyllvm_runtime_ObjectArray_load_class();
extern jclass Polyglot_polyllvm_runtime_ObjectArray_class;


void RegisterJavaClass(jclass cls, const JavaClassInfo* info) {

    if (kDebug) {

        printf("loading %s %s with super class %s\n",
	       (info->isIntf ? "interface" : "class"),
            info->name,
            info->super_ptr
                ? GetJavaClassInfo(*info->super_ptr)->name
                : "[none]");

	for (int32_t i = 0; i < info->num_intfs; ++i) {
	  jclass* intf = info->intfs[i];
	  printf("  implements interface %p\n", *intf);
	}
        for (int32_t i = 0; i < info->num_fields; ++i) {
            auto* f = &info->fields[i];
            printf("  found field %s with offset %d\n", f->name, f->offset);
        }

        for (int32_t i = 0; i < info->num_static_fields; ++i) {
            auto* f = &info->static_fields[i];
            printf("  found static field %s with sig %s and ptr %p\n", f->name, f->sig, f->ptr);
        }

        for (int32_t i = 0; i < info->num_methods; ++i) {
            auto* m = &info->methods[i];
            printf(
                "  found method %s%s\n"
                "    offset %d\n"
                "    function pointer %p\n"
                "    trampoline pointer %p\n"
                , m->name, m->sig, m->offset, m->fnPtr, m->trampoline);
        }
    }

    assert(classes.count(cls) == 0 && "Java class was loaded twice!");
    classes.emplace(cls, info);
    std::string cname(info->name);
    cnames.emplace(cname, cls);
}

} // extern "C"

//Force this class load function to be called at initialization
jclass initArrayKlass() {
  Polyglot_polyllvm_runtime_ObjectArray_load_class();
  return Polyglot_polyllvm_runtime_ObjectArray_class;
}        

//This assumes char* is non-null, C-string with len > 0
bool isArrayClassName(const char* name) {
  return name[0] == '[';
}

//This assumes char* is non-null, C-string with len > 0
//It also assumes that it has not been initialized
const jclass initArrayClass(const char* name) {
  if (globalArrayKlass == NULL) {
    globalArrayKlass = initArrayKlass();
  }
  jclass newKlazz = (jclass)malloc(sizeof(JClassRep));
  memcpy(newKlazz, globalArrayKlass, sizeof(JClassRep));
  JavaClassInfo* newInfo = (JavaClassInfo*)malloc(sizeof(JavaClassInfo));
  memcpy(newInfo, GetJavaClassInfo(globalArrayKlass), sizeof(JavaClassInfo));
  int nameLen = strlen(name) + 1; //add the '\0' terminator
  char* newName = (char*)malloc(nameLen);
  memcpy(newName, name, nameLen);
  newInfo->name = newName;
  RegisterJavaClass(newKlazz, newInfo);
  return newKlazz;
}

const JavaClassInfo*
GetJavaClassInfo(jclass cls) {
  try {
    return classes.at(cls);
  } catch (const std::out_of_range& oor) {
    return NULL;
  }
}

const jclass
GetPrimitiveClass(const char* name) {
  if (strcmp(name, "int") == 0) {
    return intKlass->Wrap();
  } else {
    return NULL;
  }
}

const jclass
GetJavaClassFromPathName(const char* name) {
  int name_len = strlen(name);
  char path_name[name_len + 1];
  for (int i = 0; i <= name_len; i++) {
    char c = name[i];
    if (c == '/') {
      c = '.';
    }
    path_name[i] = c;
  }
  return GetJavaClassFromName(path_name);
}


const jclass
GetJavaClassFromName(const char* name) {
  try {
    return cnames.at(std::string(name));
  } catch (const std::out_of_range& oor) {
    if (isArrayClassName(name)) {
      return initArrayClass(name);
    } else {
      return NULL;
    }
  }
}

const JavaStaticFieldInfo*
GetJavaStaticFieldInfo(jclass cls, const char* name, const char* sig) {
    auto* clazz = classes.at(cls);
    auto* fields = clazz->static_fields;
    for (int32_t i = 0, e = clazz->num_static_fields; i < e; ++i) {
        auto* f = &fields[i];
        if (strcmp(name, f->name) == 0 && strcmp(sig, f->sig) == 0) {
            return f;
        }
    }

    // TODO: Should technically throw NoSuchFieldError.
    fprintf(stderr,
        "Could not find static field %s in class %s. Aborting.\n",
        name, clazz->name);
    abort();
}

const JavaFieldInfo*
GetJavaFieldInfo(jclass cls, const char* name) {
    auto* clazz = classes.at(cls);
    auto* fields = clazz->fields;
    for (int32_t i = 0, e = clazz->num_fields; i < e; ++i) {
        auto* f = &fields[i];
        if (strcmp(name, f->name) == 0) {
            return f;
        }
    }

    // TODO: Should technically throw NoSuchFieldError.
    fprintf(stderr,
        "Could not find field %s in class %s. Aborting.\n",
        name, clazz->name);
    abort();
}

const std::pair<JavaMethodInfo*,int32_t>
TryGetJavaMethodInfo(jclass cls, const char* name, const char* sig, bool search_super) {
    auto* clazz = classes.at(cls);
    auto* methods = clazz->methods;
    for (int32_t i = 0, e = clazz->num_methods; i < e; ++i) {
        auto* m = &methods[i];
        if (strcmp(name, m->name) == 0 && strcmp(sig, m->sig) == 0) {
	  return std::pair<JavaMethodInfo*,int32_t>(m,i);
        }
    }

    // Recurse to super class.
    if (search_super) {
      if  (auto super_ptr = clazz->super_ptr) {
        // TODO: Technically might not want to recurse for 'private' methods.
	return TryGetJavaMethodInfo(*super_ptr, name, sig, true);
      }
    }
    return std::pair<JavaMethodInfo*,int32_t>(nullptr,-1);
}

const std::pair<JavaMethodInfo*,int32_t>
GetJavaMethodInfo(jclass cls, const char* name, const char* sig) {
  return TryGetJavaMethodInfo(cls, name, sig, true);
}

const std::pair<JavaMethodInfo*, int32_t>
GetJavaStaticMethodInfo(jclass cls, const char* name, const char* sig) {
  return TryGetJavaMethodInfo(cls, name, sig, false);
}

//java.lang.Object -> Polyglot_java_lang_Object_load_class
#define LOADER_PREFIX "Polyglot_"
#define LOADER_PREFIX_LEN 9
#define LOADER_SUFFIX "_load_class"
#define LOADER_SUFFIX_LEN 10
#define LOADER_NAME_CHARS (LOADER_PREFIX_LEN + LOADER_SUFFIX_LEN)

typedef jclass (*class_loader)();

jclass
LoadJavaClassFromLib(const char* name) {
 int name_len = strlen(name);
 int new_len = name_len  + LOADER_NAME_CHARS + 1;
 char class_load_name[new_len];
 strcpy(class_load_name, LOADER_PREFIX);
 int i = 0;
 for (; i < name_len; i++) {
   char next_char = name[i];
   class_load_name[LOADER_PREFIX_LEN + i] = (next_char == '.' || next_char == '/') ? '_' : next_char;
 }
 class_load_name[LOADER_PREFIX_LEN + i] = '\0';
 strcat(class_load_name, LOADER_SUFFIX);
 auto class_load_func = reinterpret_cast<class_loader>(dlsym(RTLD_DEFAULT, class_load_name));
 if (class_load_func != NULL) {
   return class_load_func();
 } else {
   return NULL;
 }
}