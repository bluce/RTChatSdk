// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: game.proto

#ifndef PROTOBUF_game_2eproto__INCLUDED
#define PROTOBUF_game_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "public.pb.h"
// @@protoc_insertion_point(includes)

namespace Cmd {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_game_2eproto();
void protobuf_AssignDesc_game_2eproto();
void protobuf_ShutdownFile_game_2eproto();

class cmdNotifyLogicInfo;
class cmdLogin;
class cmdquestCreateRoom;
class cmdReturnCreateResult;
class cmdRequestEnterRoom;
class cmdReturnEnterResult;
class cmdNotifyLeaveRoom;

// ===================================================================

class cmdNotifyLogicInfo : public ::google::protobuf::Message {
 public:
  cmdNotifyLogicInfo();
  virtual ~cmdNotifyLogicInfo();
  
  cmdNotifyLogicInfo(const cmdNotifyLogicInfo& from);
  
  inline cmdNotifyLogicInfo& operator=(const cmdNotifyLogicInfo& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const cmdNotifyLogicInfo& default_instance();
  
  void Swap(cmdNotifyLogicInfo* other);
  
  // implements Message ----------------------------------------------
  
  cmdNotifyLogicInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdNotifyLogicInfo& from);
  void MergeFrom(const cmdNotifyLogicInfo& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required string ip = 1;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 1;
  inline const ::std::string& ip() const;
  inline void set_ip(const ::std::string& value);
  inline void set_ip(const char* value);
  inline void set_ip(const char* value, size_t size);
  inline ::std::string* mutable_ip();
  inline ::std::string* release_ip();
  
  // required uint32 port = 2;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 2;
  inline ::google::protobuf::uint32 port() const;
  inline void set_port(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdNotifyLogicInfo)
 private:
  inline void set_has_ip();
  inline void clear_has_ip();
  inline void set_has_port();
  inline void clear_has_port();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* ip_;
  ::google::protobuf::uint32 port_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_game_2eproto();
  friend void protobuf_AssignDesc_game_2eproto();
  friend void protobuf_ShutdownFile_game_2eproto();
  
  void InitAsDefaultInstance();
  static cmdNotifyLogicInfo* default_instance_;
};
// -------------------------------------------------------------------

class cmdLogin : public ::google::protobuf::Message {
 public:
  cmdLogin();
  virtual ~cmdLogin();
  
  cmdLogin(const cmdLogin& from);
  
  inline cmdLogin& operator=(const cmdLogin& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const cmdLogin& default_instance();
  
  void Swap(cmdLogin* other);
  
  // implements Message ----------------------------------------------
  
  cmdLogin* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdLogin& from);
  void MergeFrom(const cmdLogin& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required string id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline const ::std::string& id() const;
  inline void set_id(const ::std::string& value);
  inline void set_id(const char* value);
  inline void set_id(const char* value, size_t size);
  inline ::std::string* mutable_id();
  inline ::std::string* release_id();
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdLogin)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_game_2eproto();
  friend void protobuf_AssignDesc_game_2eproto();
  friend void protobuf_ShutdownFile_game_2eproto();
  
  void InitAsDefaultInstance();
  static cmdLogin* default_instance_;
};
// -------------------------------------------------------------------

class cmdquestCreateRoom : public ::google::protobuf::Message {
 public:
  cmdquestCreateRoom();
  virtual ~cmdquestCreateRoom();
  
  cmdquestCreateRoom(const cmdquestCreateRoom& from);
  
  inline cmdquestCreateRoom& operator=(const cmdquestCreateRoom& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const cmdquestCreateRoom& default_instance();
  
  void Swap(cmdquestCreateRoom* other);
  
  // implements Message ----------------------------------------------
  
  cmdquestCreateRoom* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdquestCreateRoom& from);
  void MergeFrom(const cmdquestCreateRoom& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 ctype = 1;
  inline bool has_ctype() const;
  inline void clear_ctype();
  static const int kCtypeFieldNumber = 1;
  inline ::google::protobuf::uint32 ctype() const;
  inline void set_ctype(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdquestCreateRoom)
 private:
  inline void set_has_ctype();
  inline void clear_has_ctype();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint32 ctype_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_game_2eproto();
  friend void protobuf_AssignDesc_game_2eproto();
  friend void protobuf_ShutdownFile_game_2eproto();
  
  void InitAsDefaultInstance();
  static cmdquestCreateRoom* default_instance_;
};
// -------------------------------------------------------------------

class cmdReturnCreateResult : public ::google::protobuf::Message {
 public:
  cmdReturnCreateResult();
  virtual ~cmdReturnCreateResult();
  
  cmdReturnCreateResult(const cmdReturnCreateResult& from);
  
  inline cmdReturnCreateResult& operator=(const cmdReturnCreateResult& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const cmdReturnCreateResult& default_instance();
  
  void Swap(cmdReturnCreateResult* other);
  
  // implements Message ----------------------------------------------
  
  cmdReturnCreateResult* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdReturnCreateResult& from);
  void MergeFrom(const cmdReturnCreateResult& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint64 roomid = 1;
  inline bool has_roomid() const;
  inline void clear_roomid();
  static const int kRoomidFieldNumber = 1;
  inline ::google::protobuf::uint64 roomid() const;
  inline void set_roomid(::google::protobuf::uint64 value);
  
  // required string ip = 2;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 2;
  inline const ::std::string& ip() const;
  inline void set_ip(const ::std::string& value);
  inline void set_ip(const char* value);
  inline void set_ip(const char* value, size_t size);
  inline ::std::string* mutable_ip();
  inline ::std::string* release_ip();
  
  // required uint32 port = 3;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 3;
  inline ::google::protobuf::uint32 port() const;
  inline void set_port(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdReturnCreateResult)
 private:
  inline void set_has_roomid();
  inline void clear_has_roomid();
  inline void set_has_ip();
  inline void clear_has_ip();
  inline void set_has_port();
  inline void clear_has_port();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint64 roomid_;
  ::std::string* ip_;
  ::google::protobuf::uint32 port_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_game_2eproto();
  friend void protobuf_AssignDesc_game_2eproto();
  friend void protobuf_ShutdownFile_game_2eproto();
  
  void InitAsDefaultInstance();
  static cmdReturnCreateResult* default_instance_;
};
// -------------------------------------------------------------------

class cmdRequestEnterRoom : public ::google::protobuf::Message {
 public:
  cmdRequestEnterRoom();
  virtual ~cmdRequestEnterRoom();
  
  cmdRequestEnterRoom(const cmdRequestEnterRoom& from);
  
  inline cmdRequestEnterRoom& operator=(const cmdRequestEnterRoom& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const cmdRequestEnterRoom& default_instance();
  
  void Swap(cmdRequestEnterRoom* other);
  
  // implements Message ----------------------------------------------
  
  cmdRequestEnterRoom* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdRequestEnterRoom& from);
  void MergeFrom(const cmdRequestEnterRoom& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint64 roomid = 1;
  inline bool has_roomid() const;
  inline void clear_roomid();
  static const int kRoomidFieldNumber = 1;
  inline ::google::protobuf::uint64 roomid() const;
  inline void set_roomid(::google::protobuf::uint64 value);
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdRequestEnterRoom)
 private:
  inline void set_has_roomid();
  inline void clear_has_roomid();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint64 roomid_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_game_2eproto();
  friend void protobuf_AssignDesc_game_2eproto();
  friend void protobuf_ShutdownFile_game_2eproto();
  
  void InitAsDefaultInstance();
  static cmdRequestEnterRoom* default_instance_;
};
// -------------------------------------------------------------------

class cmdReturnEnterResult : public ::google::protobuf::Message {
 public:
  cmdReturnEnterResult();
  virtual ~cmdReturnEnterResult();
  
  cmdReturnEnterResult(const cmdReturnEnterResult& from);
  
  inline cmdReturnEnterResult& operator=(const cmdReturnEnterResult& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const cmdReturnEnterResult& default_instance();
  
  void Swap(cmdReturnEnterResult* other);
  
  // implements Message ----------------------------------------------
  
  cmdReturnEnterResult* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdReturnEnterResult& from);
  void MergeFrom(const cmdReturnEnterResult& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required bool isok = 1;
  inline bool has_isok() const;
  inline void clear_isok();
  static const int kIsokFieldNumber = 1;
  inline bool isok() const;
  inline void set_isok(bool value);
  
  // optional string ip = 2;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 2;
  inline const ::std::string& ip() const;
  inline void set_ip(const ::std::string& value);
  inline void set_ip(const char* value);
  inline void set_ip(const char* value, size_t size);
  inline ::std::string* mutable_ip();
  inline ::std::string* release_ip();
  
  // optional uint32 port = 3;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 3;
  inline ::google::protobuf::uint32 port() const;
  inline void set_port(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdReturnEnterResult)
 private:
  inline void set_has_isok();
  inline void clear_has_isok();
  inline void set_has_ip();
  inline void clear_has_ip();
  inline void set_has_port();
  inline void clear_has_port();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* ip_;
  bool isok_;
  ::google::protobuf::uint32 port_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_game_2eproto();
  friend void protobuf_AssignDesc_game_2eproto();
  friend void protobuf_ShutdownFile_game_2eproto();
  
  void InitAsDefaultInstance();
  static cmdReturnEnterResult* default_instance_;
};
// -------------------------------------------------------------------

class cmdNotifyLeaveRoom : public ::google::protobuf::Message {
 public:
  cmdNotifyLeaveRoom();
  virtual ~cmdNotifyLeaveRoom();
  
  cmdNotifyLeaveRoom(const cmdNotifyLeaveRoom& from);
  
  inline cmdNotifyLeaveRoom& operator=(const cmdNotifyLeaveRoom& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const cmdNotifyLeaveRoom& default_instance();
  
  void Swap(cmdNotifyLeaveRoom* other);
  
  // implements Message ----------------------------------------------
  
  cmdNotifyLeaveRoom* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdNotifyLeaveRoom& from);
  void MergeFrom(const cmdNotifyLeaveRoom& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::uint32 id() const;
  inline void set_id(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdNotifyLeaveRoom)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint32 id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_game_2eproto();
  friend void protobuf_AssignDesc_game_2eproto();
  friend void protobuf_ShutdownFile_game_2eproto();
  
  void InitAsDefaultInstance();
  static cmdNotifyLeaveRoom* default_instance_;
};
// ===================================================================


// ===================================================================

// cmdNotifyLogicInfo

// required string ip = 1;
inline bool cmdNotifyLogicInfo::has_ip() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdNotifyLogicInfo::set_has_ip() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdNotifyLogicInfo::clear_has_ip() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdNotifyLogicInfo::clear_ip() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& cmdNotifyLogicInfo::ip() const {
  return *ip_;
}
inline void cmdNotifyLogicInfo::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void cmdNotifyLogicInfo::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void cmdNotifyLogicInfo::set_ip(const char* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* cmdNotifyLogicInfo::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  return ip_;
}
inline ::std::string* cmdNotifyLogicInfo::release_ip() {
  clear_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = ip_;
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required uint32 port = 2;
inline bool cmdNotifyLogicInfo::has_port() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void cmdNotifyLogicInfo::set_has_port() {
  _has_bits_[0] |= 0x00000002u;
}
inline void cmdNotifyLogicInfo::clear_has_port() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void cmdNotifyLogicInfo::clear_port() {
  port_ = 0u;
  clear_has_port();
}
inline ::google::protobuf::uint32 cmdNotifyLogicInfo::port() const {
  return port_;
}
inline void cmdNotifyLogicInfo::set_port(::google::protobuf::uint32 value) {
  set_has_port();
  port_ = value;
}

// -------------------------------------------------------------------

// cmdLogin

// required string id = 1;
inline bool cmdLogin::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdLogin::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdLogin::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdLogin::clear_id() {
  if (id_ != &::google::protobuf::internal::kEmptyString) {
    id_->clear();
  }
  clear_has_id();
}
inline const ::std::string& cmdLogin::id() const {
  return *id_;
}
inline void cmdLogin::set_id(const ::std::string& value) {
  set_has_id();
  if (id_ == &::google::protobuf::internal::kEmptyString) {
    id_ = new ::std::string;
  }
  id_->assign(value);
}
inline void cmdLogin::set_id(const char* value) {
  set_has_id();
  if (id_ == &::google::protobuf::internal::kEmptyString) {
    id_ = new ::std::string;
  }
  id_->assign(value);
}
inline void cmdLogin::set_id(const char* value, size_t size) {
  set_has_id();
  if (id_ == &::google::protobuf::internal::kEmptyString) {
    id_ = new ::std::string;
  }
  id_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* cmdLogin::mutable_id() {
  set_has_id();
  if (id_ == &::google::protobuf::internal::kEmptyString) {
    id_ = new ::std::string;
  }
  return id_;
}
inline ::std::string* cmdLogin::release_id() {
  clear_has_id();
  if (id_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = id_;
    id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// cmdquestCreateRoom

// required uint32 ctype = 1;
inline bool cmdquestCreateRoom::has_ctype() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdquestCreateRoom::set_has_ctype() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdquestCreateRoom::clear_has_ctype() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdquestCreateRoom::clear_ctype() {
  ctype_ = 0u;
  clear_has_ctype();
}
inline ::google::protobuf::uint32 cmdquestCreateRoom::ctype() const {
  return ctype_;
}
inline void cmdquestCreateRoom::set_ctype(::google::protobuf::uint32 value) {
  set_has_ctype();
  ctype_ = value;
}

// -------------------------------------------------------------------

// cmdReturnCreateResult

// required uint64 roomid = 1;
inline bool cmdReturnCreateResult::has_roomid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdReturnCreateResult::set_has_roomid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdReturnCreateResult::clear_has_roomid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdReturnCreateResult::clear_roomid() {
  roomid_ = GOOGLE_ULONGLONG(0);
  clear_has_roomid();
}
inline ::google::protobuf::uint64 cmdReturnCreateResult::roomid() const {
  return roomid_;
}
inline void cmdReturnCreateResult::set_roomid(::google::protobuf::uint64 value) {
  set_has_roomid();
  roomid_ = value;
}

// required string ip = 2;
inline bool cmdReturnCreateResult::has_ip() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void cmdReturnCreateResult::set_has_ip() {
  _has_bits_[0] |= 0x00000002u;
}
inline void cmdReturnCreateResult::clear_has_ip() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void cmdReturnCreateResult::clear_ip() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& cmdReturnCreateResult::ip() const {
  return *ip_;
}
inline void cmdReturnCreateResult::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void cmdReturnCreateResult::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void cmdReturnCreateResult::set_ip(const char* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* cmdReturnCreateResult::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  return ip_;
}
inline ::std::string* cmdReturnCreateResult::release_ip() {
  clear_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = ip_;
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required uint32 port = 3;
inline bool cmdReturnCreateResult::has_port() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void cmdReturnCreateResult::set_has_port() {
  _has_bits_[0] |= 0x00000004u;
}
inline void cmdReturnCreateResult::clear_has_port() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void cmdReturnCreateResult::clear_port() {
  port_ = 0u;
  clear_has_port();
}
inline ::google::protobuf::uint32 cmdReturnCreateResult::port() const {
  return port_;
}
inline void cmdReturnCreateResult::set_port(::google::protobuf::uint32 value) {
  set_has_port();
  port_ = value;
}

// -------------------------------------------------------------------

// cmdRequestEnterRoom

// required uint64 roomid = 1;
inline bool cmdRequestEnterRoom::has_roomid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdRequestEnterRoom::set_has_roomid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdRequestEnterRoom::clear_has_roomid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdRequestEnterRoom::clear_roomid() {
  roomid_ = GOOGLE_ULONGLONG(0);
  clear_has_roomid();
}
inline ::google::protobuf::uint64 cmdRequestEnterRoom::roomid() const {
  return roomid_;
}
inline void cmdRequestEnterRoom::set_roomid(::google::protobuf::uint64 value) {
  set_has_roomid();
  roomid_ = value;
}

// -------------------------------------------------------------------

// cmdReturnEnterResult

// required bool isok = 1;
inline bool cmdReturnEnterResult::has_isok() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdReturnEnterResult::set_has_isok() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdReturnEnterResult::clear_has_isok() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdReturnEnterResult::clear_isok() {
  isok_ = false;
  clear_has_isok();
}
inline bool cmdReturnEnterResult::isok() const {
  return isok_;
}
inline void cmdReturnEnterResult::set_isok(bool value) {
  set_has_isok();
  isok_ = value;
}

// optional string ip = 2;
inline bool cmdReturnEnterResult::has_ip() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void cmdReturnEnterResult::set_has_ip() {
  _has_bits_[0] |= 0x00000002u;
}
inline void cmdReturnEnterResult::clear_has_ip() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void cmdReturnEnterResult::clear_ip() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& cmdReturnEnterResult::ip() const {
  return *ip_;
}
inline void cmdReturnEnterResult::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void cmdReturnEnterResult::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void cmdReturnEnterResult::set_ip(const char* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* cmdReturnEnterResult::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  return ip_;
}
inline ::std::string* cmdReturnEnterResult::release_ip() {
  clear_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = ip_;
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional uint32 port = 3;
inline bool cmdReturnEnterResult::has_port() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void cmdReturnEnterResult::set_has_port() {
  _has_bits_[0] |= 0x00000004u;
}
inline void cmdReturnEnterResult::clear_has_port() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void cmdReturnEnterResult::clear_port() {
  port_ = 0u;
  clear_has_port();
}
inline ::google::protobuf::uint32 cmdReturnEnterResult::port() const {
  return port_;
}
inline void cmdReturnEnterResult::set_port(::google::protobuf::uint32 value) {
  set_has_port();
  port_ = value;
}

// -------------------------------------------------------------------

// cmdNotifyLeaveRoom

// required uint32 id = 1;
inline bool cmdNotifyLeaveRoom::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdNotifyLeaveRoom::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdNotifyLeaveRoom::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdNotifyLeaveRoom::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 cmdNotifyLeaveRoom::id() const {
  return id_;
}
inline void cmdNotifyLeaveRoom::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Cmd

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_game_2eproto__INCLUDED
