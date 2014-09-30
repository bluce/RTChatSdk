
#include "jni_helpers.h"

#include <limits>
#include <android/log.h>

jmethodID GetMethodID(JNIEnv* jni, jclass c, const std::string& name,
                      const char* signature) {
  jmethodID m = jni->GetMethodID(c, name.c_str(), signature);
  CHECK_EXCEPTION(jni, "error during GetMethodID");
  return m;
}

jlong jlongFromPointer(void* ptr) {
  CHECK(sizeof(intptr_t) <= sizeof(jlong), "Time to rethink the use of jlongs");
  // Going through intptr_t to be obvious about the definedness of the
  // conversion from pointer to integral type.  intptr_t to jlong is a standard
  // widening by the COMPILE_ASSERT above.
  jlong ret = reinterpret_cast<intptr_t>(ptr);
  CHECK(reinterpret_cast<void*>(ret) == ptr,
        "jlong does not convert back to pointer");
  return ret;
}

// Given a (UTF-16) jstring return a new UTF-8 native string.
std::string JavaToStdString(JNIEnv* jni, const jstring& j_string) {
    char* rtn = NULL;  
    jclass clsstring = jni->FindClass("java/lang/String");  
    jstring strencode = jni->NewStringUTF("utf-8");  
    jmethodID mid = jni->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");  
    jbyteArray barr = (jbyteArray)jni->CallObjectMethod(j_string, mid, strencode);  
    jsize alen = jni->GetArrayLength(barr);  
    jbyte* ba = jni->GetByteArrayElements(barr, JNI_FALSE);  
    if (alen > 0)  
    {  
        rtn = (char*)malloc(alen + 1);  
        memcpy(rtn, ba, alen);  
        rtn[alen] = 0;  
    }  
    jni->ReleaseByteArrayElements(barr, ba, 0);
    std::string stemp(rtn);
    free(rtn); 
    return stemp;  
}

ClassReferenceHolder::ClassReferenceHolder(JNIEnv* jni, const char** classes,
                                           int size) {
  for (int i = 0; i < size; ++i) {
    LoadClass(jni, classes[i]);
  }
}
ClassReferenceHolder::~ClassReferenceHolder() {
  CHECK(classes_.empty(), "Must call FreeReferences() before dtor!");
}

void ClassReferenceHolder::FreeReferences(JNIEnv* jni) {
  for (std::map<std::string, jclass>::const_iterator it = classes_.begin();
       it != classes_.end(); ++it) {
    jni->DeleteGlobalRef(it->second);
  }
  classes_.clear();
}

jclass ClassReferenceHolder::GetClass(const std::string& name) {
  std::map<std::string, jclass>::iterator it = classes_.find(name);
  CHECK(it != classes_.end(), "Could not find class");
  return it->second;
}

void ClassReferenceHolder::LoadClass(JNIEnv* jni, const std::string& name) {
  jclass localRef = jni->FindClass(name.c_str());
  CHECK_EXCEPTION(jni, "Could not load class");
  CHECK(localRef, name.c_str());
  jclass globalRef = reinterpret_cast<jclass>(jni->NewGlobalRef(localRef));
  CHECK_EXCEPTION(jni, "error during NewGlobalRef");
  CHECK(globalRef, name.c_str());
  bool inserted = classes_.insert(std::make_pair(name, globalRef)).second;
  CHECK(inserted, "Duplicate class name");
}
