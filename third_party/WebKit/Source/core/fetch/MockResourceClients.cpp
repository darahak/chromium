// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/fetch/MockResourceClients.h"

#include "core/fetch/ImageResource.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace blink {

MockResourceClient::MockResourceClient(Resource* resource)
    : m_resource(resource),
      m_notifyFinishedCalled(false),
      m_encodedSizeOnNotifyFinished(0) {
  ThreadState::current()->registerPreFinalizer(this);
  m_resource->addClient(this);
}

MockResourceClient::~MockResourceClient() {}

void MockResourceClient::notifyFinished(Resource* resource) {
  ASSERT_FALSE(m_notifyFinishedCalled);
  m_notifyFinishedCalled = true;
  m_encodedSizeOnNotifyFinished = resource->encodedSize();
}

void MockResourceClient::removeAsClient() {
  m_resource->removeClient(this);
  m_resource = nullptr;
}

void MockResourceClient::dispose() {
  if (m_resource) {
    m_resource->removeClient(this);
    m_resource = nullptr;
  }
}

DEFINE_TRACE(MockResourceClient) {
  visitor->trace(m_resource);
  ResourceClient::trace(visitor);
}

MockImageResourceClient::MockImageResourceClient(ImageResource* resource)
    : MockResourceClient(resource),
      m_imageChangedCount(0),
      m_encodedSizeOnLastImageChanged(0),
      m_imageNotifyFinishedCount(0),
      m_encodedSizeOnImageNotifyFinished(0) {
  toImageResource(m_resource.get())->addObserver(this);
}

MockImageResourceClient::~MockImageResourceClient() {}

void MockImageResourceClient::removeAsClient() {
  toImageResource(m_resource.get())->removeObserver(this);
  MockResourceClient::removeAsClient();
}

void MockImageResourceClient::dispose() {
  if (m_resource)
    toImageResource(m_resource.get())->removeObserver(this);
  MockResourceClient::dispose();
}

void MockImageResourceClient::imageChanged(ImageResource* image,
                                           const IntRect*) {
  m_imageChangedCount++;
  m_encodedSizeOnLastImageChanged = image->encodedSize();
}

void MockImageResourceClient::imageNotifyFinished(ImageResource* image) {
  ASSERT_EQ(0, m_imageNotifyFinishedCount);
  m_imageNotifyFinishedCount++;
  m_encodedSizeOnImageNotifyFinished = image->encodedSize();
}

bool MockImageResourceClient::notifyFinishedCalled() const {
  EXPECT_EQ(m_notifyFinishedCalled ? 1 : 0, m_imageNotifyFinishedCount);

  return m_notifyFinishedCalled;
}

MockFontResourceClient::MockFontResourceClient(Resource* resource)
    : m_resource(resource),
      m_fontLoadShortLimitExceededCalled(false),
      m_fontLoadLongLimitExceededCalled(false) {
  ThreadState::current()->registerPreFinalizer(this);
  m_resource->addClient(this);
}

MockFontResourceClient::~MockFontResourceClient() {}

void MockFontResourceClient::fontLoadShortLimitExceeded(FontResource*) {
  ASSERT_FALSE(m_fontLoadShortLimitExceededCalled);
  ASSERT_FALSE(m_fontLoadLongLimitExceededCalled);
  m_fontLoadShortLimitExceededCalled = true;
}

void MockFontResourceClient::fontLoadLongLimitExceeded(FontResource*) {
  ASSERT_TRUE(m_fontLoadShortLimitExceededCalled);
  ASSERT_FALSE(m_fontLoadLongLimitExceededCalled);
  m_fontLoadLongLimitExceededCalled = true;
}

void MockFontResourceClient::dispose() {
  if (m_resource) {
    m_resource->removeClient(this);
    m_resource = nullptr;
  }
}

}  // namespace blink
