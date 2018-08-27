// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_TEST_CHROMEDRIVER_LOG_REPLAY_REPLAY_HTTP_CLIENT_H_
#define CHROME_TEST_CHROMEDRIVER_LOG_REPLAY_REPLAY_HTTP_CLIENT_H_

#include <memory>
#include <set>
#include <string>

#include "base/files/file_path.h"
#include "chrome/test/chromedriver/chrome/browser_info.h"
#include "chrome/test/chromedriver/chrome/devtools_http_client.h"
#include "chrome/test/chromedriver/log_replay/devtools_log_reader.h"
#include "chrome/test/chromedriver/net/sync_websocket_factory.h"

// Subclass of DevToolsHttpClient that redirects communication
// that would happen with Chrome to a DevToolsLogReader (i.e. a ChromeDriver
// log file). This enables log replay of DevTools HTTP communication with
// DevTools.
class ReplayHttpClient : public DevToolsHttpClient {
 public:
  // Initializes a DevToolsLogReader with the given log file.
  ReplayHttpClient(const NetAddress& address,
                   scoped_refptr<URLRequestContextGetter> context_getter,
                   const SyncWebSocketFactory& socket_factory,
                   std::unique_ptr<DeviceMetrics> device_metrics,
                   std::unique_ptr<std::set<WebViewInfo::Type>> window_types,
                   std::string page_load_strategy,
                   const base::FilePath& log_file);
  ~ReplayHttpClient() override;

 private:
  // DevToolsLogReader that we read the responses from
  DevToolsLogReader log_reader_;

  // This is the only function that we override from DevToolsHttpClient;
  // instead of actually sending an HTTP request it looks for the
  // corresponding request in the log file and returns the response accordingly.
  bool FetchUrlAndLog(const std::string& url,
                      URLRequestContextGetter* getter,
                      std::string* response) override;
};

#endif  // CHROME_TEST_CHROMEDRIVER_LOG_REPLAY_REPLAY_HTTP_CLIENT_H_
