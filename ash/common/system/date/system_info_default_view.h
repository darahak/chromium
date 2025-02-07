// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_COMMON_SYSTEM_DATE_SYSTEM_INFO_DEFAULT_VIEW_H_
#define ASH_COMMON_SYSTEM_DATE_SYSTEM_INFO_DEFAULT_VIEW_H_

#include "ash/ash_export.h"
#include "ash/common/login_status.h"
#include "base/macros.h"
#include "ui/views/view.h"

namespace ash {
#if defined(OS_CHROMEOS)
class PowerStatusView;
#endif  // defined(OS_CHROMEOS)
class SystemTrayItem;

namespace tray {
class DateView;
}  // namespace tray

// The default view for the system info row in the system menu. Contains the
// current date and, if a battery is present, a string showing the current
// power status.
class ASH_EXPORT SystemInfoDefaultView : public views::View {
 public:
  SystemInfoDefaultView(SystemTrayItem* owner, LoginStatus login);

  ~SystemInfoDefaultView() override;

  tray::DateView* GetDateView();
  const tray::DateView* GetDateView() const;

 private:
  tray::DateView* date_view_;

#if defined(OS_CHROMEOS)
  PowerStatusView* power_status_view_ = nullptr;
#endif  // defined(OS_CHROMEOS)

  DISALLOW_COPY_AND_ASSIGN(SystemInfoDefaultView);
};

}  // namespace ash

#endif  // ASH_COMMON_SYSTEM_DATE_SYSTEM_INFO_DEFAULT_VIEW_H_
