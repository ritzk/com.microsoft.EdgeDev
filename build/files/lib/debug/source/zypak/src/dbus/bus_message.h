// Copyright 2020 Endless Mobile, Inc.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <dbus/dbus.h>

#include <memory>

#include "base/base.h"
#include "base/unique_fd.h"
#include "dbus/internal/bus_types.h"

namespace zypak::dbus {

using TypeCode = internal::TypeCode;
using TypeCodeKind = internal::TypeCodeKind;

template <TypeCode Code>
using BusTypeTraits = internal::BusTypeTraits<Code>;

// A reference to a particular interface, object, and service.
class FloatingRef {
 public:
  constexpr FloatingRef(std::string_view service, std::string_view object,
                        std::string_view interface)
      : service_(std::move(service)), object_(std::move(object)), interface_(std::move(interface)) {
  }

  std::string_view service() const { return service_; }
  std::string_view object() const { return object_; }
  std::string_view interface() const { return interface_; }

 private:
  std::string_view service_;
  std::string_view object_;
  std::string_view interface_;
};

// A D-Bus message.
class Message {
 public:
  Message(const Message& other) : Message(other.message()) {}
  virtual ~Message() {}

  Message(Message&& other) = default;

  DBusMessage* message() const { return message_.get(); }

 protected:
  Message(DBusMessage* message) : message_(message) { dbus_message_ref(message_.get()); }

 private:
  struct DBusMessageDeleter {
    void operator()(DBusMessage* message) { dbus_message_unref(message); }
  };

  std::unique_ptr<DBusMessage, DBusMessageDeleter> message_;

  friend class BusThread;
};

}  // namespace zypak::dbus
