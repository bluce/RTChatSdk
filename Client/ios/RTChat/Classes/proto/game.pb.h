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
class cmdRequestLogin;
class cmdNotifyLoginResult;
class cmdNotifyCreateResult;
class cmdRequestEnterRoom;
class cmdNotifyEnterResult;

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

class cmdRequestLogin : public ::google::protobuf::Message {
 public:
  cmdRequestLogin();
  virtual ~cmdRequestLogin();
  
  cmdRequestLogin(const cmdRequestLogin& from);
  
  inline cmdRequestLogin& operator=(const cmdRequestLogin& from) {
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
  static const cmdRequestLogin& default_instance();
  
  void Swap(cmdRequestLogin* other);
  
  // implements Message ----------------------------------------------
  
  cmdRequestLogin* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdRequestLogin& from);
  void MergeFrom(const cmdRequestLogin& from);
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
  
  // required string uniqueid = 1;
  inline bool has_uniqueid() const;
  inline void clear_uniqueid();
  static const int kUniqueidFieldNumber = 1;
  inline const ::std::string& uniqueid() const;
  inline void set_uniqueid(const ::std::string& value);
  inline void set_uniqueid(const char* value);
  inline void set_uniqueid(const char* value, size_t size);
  inline ::std::string* mutable_uniqueid();
  inline ::std::string* release_uniqueid();
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdRequestLogin)
 private:
  inline void set_has_uniqueid();
  inline void clear_has_uniqueid();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* uniqueid_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_game_2eproto();
  friend void protobuf_AssignDesc_game_2eproto();
  friend void protobuf_ShutdownFile_game_2eproto();
  
  void InitAsDefaultInstance();
  static cmdRequestLogin* default_instance_;
};
// -------------------------------------------------------------------

class cmdNotifyLoginResult : public ::google::protobuf::Message {
 public:
  cmdNotifyLoginResult();
  virtual ~cmdNotifyLoginResult();
  
  cmdNotifyLoginResult(const cmdNotifyLoginResult& from);
  
  inline cmdNotifyLoginResult& operator=(const cmdNotifyLoginResult& from) {
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
  static const cmdNotifyLoginResult& default_instance();
  
  void Swap(cmdNotifyLoginResult* other);
  
  // implements Message ----------------------------------------------
  
  cmdNotifyLoginResult* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdNotifyLoginResult& from);
  void MergeFrom(const cmdNotifyLoginResult& from);
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
  
  // required uint64 tempid = 1;
  inline bool has_tempid() const;
  inline void clear_tempid();
  static const int kTempidFieldNumber = 1;
  inline ::google::protobuf::uint64 tempid() const;
  inline void set_tempid(::google::protobuf::uint64 value);
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdNotifyLoginResult)
 private:
  inline void set_has_tempid();
  inline void clear_has_tempid();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint64 tempid_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_game_2eproto();
  friend void protobuf_AssignDesc_game_2eproto();
  friend void protobuf_ShutdownFile_game_2eproto();
  
  void InitAsDefaultInstance();
  static cmdNotifyLoginResult* default_instance_;
};
// -------------------------------------------------------------------

class cmdNotifyCreateResult : public ::google::protobuf::Message {
 public:
  cmdNotifyCreateResult();
  virtual ~cmdNotifyCreateResult();
  
  cmdNotifyCreateResult(const cmdNotifyCreateResult& from);
  
  inline cmdNotifyCreateResult& operator=(const cmdNotifyCreateResult& from) {
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
  static const cmdNotifyCreateResult& default_instance();
  
  void Swap(cmdNotifyCreateResult* other);
  
  // implements Message ----------------------------------------------
  
  cmdNotifyCreateResult* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdNotifyCreateResult& from);
  void MergeFrom(const cmdNotifyCreateResult& from);
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
  
  // optional uint64 roomid = 2;
  inline bool has_roomid() const;
  inline void clear_roomid();
  static const int kRoomidFieldNumber = 2;
  inline ::google::protobuf::uint64 roomid() const;
  inline void set_roomid(::google::protobuf::uint64 value);
  
  // optional string ip = 3;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 3;
  inline const ::std::string& ip() const;
  inline void set_ip(const ::std::string& value);
  inline void set_ip(const char* value);
  inline void set_ip(const char* value, size_t size);
  inline ::std::string* mutable_ip();
  inline ::std::string* release_ip();
  
  // optional uint32 port = 4;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 4;
  inline ::google::protobuf::uint32 port() const;
  inline void set_port(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdNotifyCreateResult)
 private:
  inline void set_has_isok();
  inline void clear_has_isok();
  inline void set_has_roomid();
  inline void clear_has_roomid();
  inline void set_has_ip();
  inline void clear_has_ip();
  inline void set_has_port();
  inline void clear_has_port();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint64 roomid_;
  bool isok_;
  ::google::protobuf::uint32 port_;
  ::std::string* ip_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_game_2eproto();
  friend void protobuf_AssignDesc_game_2eproto();
  friend void protobuf_ShutdownFile_game_2eproto();
  
  void InitAsDefaultInstance();
  static cmdNotifyCreateResult* default_instance_;
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

class cmdNotifyEnterResult : public ::google::protobuf::Message {
 public:
  cmdNotifyEnterResult();
  virtual ~cmdNotifyEnterResult();
  
  cmdNotifyEnterResult(const cmdNotifyEnterResult& from);
  
  inline cmdNotifyEnterResult& operator=(const cmdNotifyEnterResult& from) {
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
  static const cmdNotifyEnterResult& default_instance();
  
  void Swap(cmdNotifyEnterResult* other);
  
  // implements Message ----------------------------------------------
  
  cmdNotifyEnterResult* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const cmdNotifyEnterResult& from);
  void MergeFrom(const cmdNotifyEnterResult& from);
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
  
  // @@protoc_insertion_point(class_scope:Cmd.cmdNotifyEnterResult)
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
  static cmdNotifyEnterResult* default_instance_;
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

// cmdRequestLogin

// required string uniqueid = 1;
inline bool cmdRequestLogin::has_uniqueid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdRequestLogin::set_has_uniqueid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdRequestLogin::clear_has_uniqueid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdRequestLogin::clear_uniqueid() {
  if (uniqueid_ != &::google::protobuf::internal::kEmptyString) {
    uniqueid_->clear();
  }
  clear_has_uniqueid();
}
inline const ::std::string& cmdRequestLogin::uniqueid() const {
  return *uniqueid_;
}
inline void cmdRequestLogin::set_uniqueid(const ::std::string& value) {
  set_has_uniqueid();
  if (uniqueid_ == &::google::protobuf::internal::kEmptyString) {
    uniqueid_ = new ::std::string;
  }
  uniqueid_->assign(value);
}
inline void cmdRequestLogin::set_uniqueid(const char* value) {
  set_has_uniqueid();
  if (uniqueid_ == &::google::protobuf::internal::kEmptyString) {
    uniqueid_ = new ::std::string;
  }
  uniqueid_->assign(value);
}
inline void cmdRequestLogin::set_uniqueid(const char* value, size_t size) {
  set_has_uniqueid();
  if (uniqueid_ == &::google::protobuf::internal::kEmptyString) {
    uniqueid_ = new ::std::string;
  }
  uniqueid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* cmdRequestLogin::mutable_uniqueid() {
  set_has_uniqueid();
  if (uniqueid_ == &::google::protobuf::internal::kEmptyString) {
    uniqueid_ = new ::std::string;
  }
  return uniqueid_;
}
inline ::std::string* cmdRequestLogin::release_uniqueid() {
  clear_has_uniqueid();
  if (uniqueid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = uniqueid_;
    uniqueid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// cmdNotifyLoginResult

// required uint64 tempid = 1;
inline bool cmdNotifyLoginResult::has_tempid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdNotifyLoginResult::set_has_tempid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdNotifyLoginResult::clear_has_tempid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdNotifyLoginResult::clear_tempid() {
  tempid_ = GOOGLE_ULONGLONG(0);
  clear_has_tempid();
}
inline ::google::protobuf::uint64 cmdNotifyLoginResult::tempid() const {
  return tempid_;
}
inline void cmdNotifyLoginResult::set_tempid(::google::protobuf::uint64 value) {
  set_has_tempid();
  tempid_ = value;
}

// -------------------------------------------------------------------

// cmdNotifyCreateResult

// required bool isok = 1;
inline bool cmdNotifyCreateResult::has_isok() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdNotifyCreateResult::set_has_isok() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdNotifyCreateResult::clear_has_isok() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdNotifyCreateResult::clear_isok() {
  isok_ = false;
  clear_has_isok();
}
inline bool cmdNotifyCreateResult::isok() const {
  return isok_;
}
inline void cmdNotifyCreateResult::set_isok(bool value) {
  set_has_isok();
  isok_ = value;
}

// optional uint64 roomid = 2;
inline bool cmdNotifyCreateResult::has_roomid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void cmdNotifyCreateResult::set_has_roomid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void cmdNotifyCreateResult::clear_has_roomid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void cmdNotifyCreateResult::clear_roomid() {
  roomid_ = GOOGLE_ULONGLONG(0);
  clear_has_roomid();
}
inline ::google::protobuf::uint64 cmdNotifyCreateResult::roomid() const {
  return roomid_;
}
inline void cmdNotifyCreateResult::set_roomid(::google::protobuf::uint64 value) {
  set_has_roomid();
  roomid_ = value;
}

// optional string ip = 3;
inline bool cmdNotifyCreateResult::has_ip() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void cmdNotifyCreateResult::set_has_ip() {
  _has_bits_[0] |= 0x00000004u;
}
inline void cmdNotifyCreateResult::clear_has_ip() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void cmdNotifyCreateResult::clear_ip() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& cmdNotifyCreateResult::ip() const {
  return *ip_;
}
inline void cmdNotifyCreateResult::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void cmdNotifyCreateResult::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void cmdNotifyCreateResult::set_ip(const char* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* cmdNotifyCreateResult::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  return ip_;
}
inline ::std::string* cmdNotifyCreateResult::release_ip() {
  clear_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = ip_;
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional uint32 port = 4;
inline bool cmdNotifyCreateResult::has_port() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void cmdNotifyCreateResult::set_has_port() {
  _has_bits_[0] |= 0x00000008u;
}
inline void cmdNotifyCreateResult::clear_has_port() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void cmdNotifyCreateResult::clear_port() {
  port_ = 0u;
  clear_has_port();
}
inline ::google::protobuf::uint32 cmdNotifyCreateResult::port() const {
  return port_;
}
inline void cmdNotifyCreateResult::set_port(::google::protobuf::uint32 value) {
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

// cmdNotifyEnterResult

// required bool isok = 1;
inline bool cmdNotifyEnterResult::has_isok() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void cmdNotifyEnterResult::set_has_isok() {
  _has_bits_[0] |= 0x00000001u;
}
inline void cmdNotifyEnterResult::clear_has_isok() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void cmdNotifyEnterResult::clear_isok() {
  isok_ = false;
  clear_has_isok();
}
inline bool cmdNotifyEnterResult::isok() const {
  return isok_;
}
inline void cmdNotifyEnterResult::set_isok(bool value) {
  set_has_isok();
  isok_ = value;
}

// optional string ip = 2;
inline bool cmdNotifyEnterResult::has_ip() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void cmdNotifyEnterResult::set_has_ip() {
  _has_bits_[0] |= 0x00000002u;
}
inline void cmdNotifyEnterResult::clear_has_ip() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void cmdNotifyEnterResult::clear_ip() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& cmdNotifyEnterResult::ip() const {
  return *ip_;
}
inline void cmdNotifyEnterResult::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void cmdNotifyEnterResult::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void cmdNotifyEnterResult::set_ip(const char* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* cmdNotifyEnterResult::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  return ip_;
}
inline ::std::string* cmdNotifyEnterResult::release_ip() {
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
inline bool cmdNotifyEnterResult::has_port() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void cmdNotifyEnterResult::set_has_port() {
  _has_bits_[0] |= 0x00000004u;
}
inline void cmdNotifyEnterResult::clear_has_port() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void cmdNotifyEnterResult::clear_port() {
  port_ = 0u;
  clear_has_port();
}
inline ::google::protobuf::uint32 cmdNotifyEnterResult::port() const {
  return port_;
}
inline void cmdNotifyEnterResult::set_port(::google::protobuf::uint32 value) {
  set_has_port();
  port_ = value;
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