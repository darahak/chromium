// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/chromeos/arc/notification/arc_boot_error_notification.h"

#include "base/macros.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "chrome/grit/generated_resources.h"
#include "components/user_manager/user_manager.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/chromeos/resources/grit/ui_chromeos_resources.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/notification.h"
#include "ui/message_center/notification_types.h"
#include "ui/message_center/notifier_settings.h"

namespace arc {

namespace {

const char kLowDiskSpaceId[] = "arc_low_disk";
const char kNotifierId[] = "arc_boot_error";
const char kDisplaySource[] = "arc_boot_error_source";
const char kStoragePage[] = "storage";

class LowDiskSpaceErrorNotificationDelegate
    : public message_center::NotificationDelegate {
 public:
  LowDiskSpaceErrorNotificationDelegate() = default;

  // message_center::NotificationDelegate
  void ButtonClick(int button_index) override {
    chrome::ShowSettingsSubPageForProfile(
        ProfileManager::GetActiveUserProfile(), kStoragePage);
  }

 private:
  ~LowDiskSpaceErrorNotificationDelegate() override = default;

  DISALLOW_COPY_AND_ASSIGN(LowDiskSpaceErrorNotificationDelegate);
};

void ShowLowDiskSpaceErrorNotification() {
  message_center::ButtonInfo storage_settings(
      l10n_util::GetStringUTF16(IDS_LOW_DISK_NOTIFICATION_BUTTON));
  storage_settings.icon = ui::ResourceBundle::GetSharedInstance().GetImageNamed(
      IDR_STORAGE_MANAGER_BUTTON);
  message_center::RichNotificationData optional_fields;
  optional_fields.buttons.push_back(storage_settings);

  message_center::NotifierId notifier_id(
      message_center::NotifierId::SYSTEM_COMPONENT, kNotifierId);
  const AccountId& account_id =
      user_manager::UserManager::Get()->GetPrimaryUser()->GetAccountId();
  notifier_id.profile_id = account_id.GetUserEmail();

  message_center::MessageCenter::Get()->AddNotification(
      base::MakeUnique<message_center::Notification>(
          message_center::NOTIFICATION_TYPE_SIMPLE, kLowDiskSpaceId,
          l10n_util::GetStringUTF16(
              IDS_ARC_CRITICALLY_LOW_DISK_NOTIFICATION_TITLE),
          l10n_util::GetStringUTF16(
              IDS_ARC_CRITICALLY_LOW_DISK_NOTIFICATION_MESSAGE),
          gfx::Image(ui::ResourceBundle::GetSharedInstance().GetImageNamed(
              IDR_DISK_SPACE_NOTIFICATION_CRITICAL)),
          base::UTF8ToUTF16(kDisplaySource), GURL(), notifier_id,
          optional_fields, new LowDiskSpaceErrorNotificationDelegate()));
}

}  // namespace

ArcBootErrorNotification::ArcBootErrorNotification(
    ArcBridgeService* bridge_service)
    : ArcService(bridge_service) {
  arc_bridge_service()->AddObserver(this);
}

ArcBootErrorNotification::~ArcBootErrorNotification() {
  arc_bridge_service()->RemoveObserver(this);
}

void ArcBootErrorNotification::OnBridgeStopped(
    ArcBridgeService::StopReason reason) {
  if (reason == ArcBridgeService::StopReason::LOW_DISK_SPACE) {
    ShowLowDiskSpaceErrorNotification();
  }
}

}  // namespace arc
