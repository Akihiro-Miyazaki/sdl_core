/*

 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/add_sub_menu_command.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"


namespace application_manager {

namespace commands {

AddSubMenuCommand::AddSubMenuCommand(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

AddSubMenuCommand::~AddSubMenuCommand() {
}

void AddSubMenuCommand::Run() {
  ApplicationImpl* application =
      static_cast<ApplicationImpl*>(ApplicationManagerImpl::instance()->
      application((*message_)[strings::params][strings::connection_key]));

  if (!application) {
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  NsSmartDeviceLink::NsSmartObjects::CSmartObject* menu =
      application->FindSubMenu(
          (*message_)[strings::msg_params][strings::menu_id].asInt());

  if (menu) {
    if ((*menu)[strings::menu_name].asString() ==
        (*message_)[strings::msg_params][strings::menu_name].asString()) {
      SendResponse(false,
                   NsSmartDeviceLinkRPC::V2::Result::DUPLICATE_NAME);
    }

    SendResponse(false, NsSmartDeviceLinkRPC::V2::Result::INVALID_ID);

    return;
  }

  const int corellation_id =
        (*message_)[strings::params][strings::correlation_id];
  const int connection_key =
        (*message_)[strings::params][strings::connection_key];
  // TODO(VS): HMI Request Id
  const int hmi_request_id = 200;

  ApplicationManagerImpl::instance()->AddMessageChain(NULL,
        connection_key, corellation_id, hmi_request_id, &(*message_));

  ApplicationManagerImpl::instance()->SendMessageToHMI(message_);
}

}  // namespace commands

}  // namespace application_manager
