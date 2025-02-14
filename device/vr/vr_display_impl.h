// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_VR_VR_DISPLAY_IMPL_H
#define DEVICE_VR_VR_DISPLAY_IMPL_H

#include <memory>

#include "base/macros.h"

#include "device/vr/vr_device.h"
#include "device/vr/vr_export.h"
#include "device/vr/vr_service.mojom.h"
#include "mojo/public/cpp/bindings/binding.h"

namespace device {

class VRDisplayImpl : public mojom::VRDisplay {
 public:
  VRDisplayImpl(device::VRDevice* device, VRServiceImpl* service);
  ~VRDisplayImpl() override;

  mojom::VRDisplayClient* client() { return client_.get(); }

 private:
  friend class VRServiceImpl;

  void GetPose(const GetPoseCallback& callback) override;
  void ResetPose() override;

  void RequestPresent(bool secureOrigin,
                      const RequestPresentCallback& callback) override;
  void ExitPresent() override;
  void SubmitFrame(mojom::VRPosePtr pose) override;

  void UpdateLayerBounds(mojom::VRLayerBoundsPtr left_bounds,
                         mojom::VRLayerBoundsPtr right_bounds) override;

  mojo::Binding<mojom::VRDisplay> binding_;
  mojom::VRDisplayClientPtr client_;
  device::VRDevice* device_;
  VRServiceImpl* service_;
};

}  // namespace device

#endif  //  DEVICE_VR_VR_DISPLAY_IMPL_H
