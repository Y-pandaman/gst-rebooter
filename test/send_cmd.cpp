#include "common/cmd.pb.h"
#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include <iostream>
#include <loguru.hpp>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  eCAL::Initialize(argc, argv, "send_cmd");
  eCAL::Util::EnableLoopback(true);
  eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1,
                          "send_cmd is running");

  eCAL::protobuf::CPublisher<proto_messages::CMD> pub("cmd");
  proto_messages::CMD cmd;
  cmd.set_camera0_on(false);
  cmd.set_camera1_on(false);
  cmd.set_camera2_on(false);
  cmd.set_camera3_on(false);
  cmd.set_camera4_on(false);
  cmd.set_camera5_on(false);
  cmd.set_camera6_on(false);
  cmd.set_camera7_on(false);

  while (eCAL::Ok()) {

    cmd.set_camera0_on(true);
    pub.Send(cmd);
    LOG_F(INFO, "Camera 0 is %d", cmd.camera0_on());
    eCAL::Process::SleepMS(1000);
    cmd.set_camera0_on(false);
    pub.Send(cmd);
    LOG_F(INFO, "Camera 0 is %d", cmd.camera0_on());
    eCAL::Process::SleepMS(1000);

  }
  return 0;
}
