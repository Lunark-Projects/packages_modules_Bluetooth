/*
 * Copyright 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "hal/snoop_logger.h"
#include "hci/address.h"
#include "module.h"

namespace bluetooth {
namespace activity_attribution {

enum class Activity : uint8_t { UNKNOWN = 0, ACL, ADVERTISE, CONNECT, CONTROL, HFP, ISO, SCAN, VENDOR };

using CreationTime = std::chrono::time_point<std::chrono::system_clock>;

struct BtaaAggregationEntry {
  hci::Address address;
  Activity activity;
  uint16_t wakeup_count;
  uint32_t byte_count;
  uint32_t wakelock_duration_ms;
  CreationTime creation_time;
};

class ActivityAttribution : public bluetooth::Module {
 public:
  ActivityAttribution() = default;
  ActivityAttribution(const ActivityAttribution&) = delete;
  ActivityAttribution& operator=(const ActivityAttribution&) = delete;

  ~ActivityAttribution() = default;

  void Capture(const hal::HciPacket& packet, hal::SnoopLogger::PacketType type);
  void OnWakelockAcquired();
  void OnWakelockReleased();
  void OnWakeup();
  void NotifyActivityAttributionInfo(int uid, const std::string& package_name, const std::string& device_address);

  static const ModuleFactory Factory;

 protected:
  std::string ToString() const override;
  void ListDependencies(ModuleList* list) const override;
  void Start() override;
  void Stop() override;
  DumpsysDataFinisher GetDumpsysData(flatbuffers::FlatBufferBuilder* builder) const override;  // Module

 private:
  struct impl;
  std::unique_ptr<impl> pimpl_;
};

}  // namespace activity_attribution
}  // namespace bluetooth
