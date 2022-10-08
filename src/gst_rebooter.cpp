#include "common/cmd.pb.h"
#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include <ecal/msg/protobuf/subscriber.h>
#include <iostream>
#include <loguru.hpp>

void log_init(int argc, char **argv) {
  loguru::init(argc, argv);
  time_t curtime;
  time(&curtime);
  tm *nowtime = localtime(&curtime);
  int year = 1900 + nowtime->tm_year;
  int month = 1 + nowtime->tm_mon;
  int day = nowtime->tm_mday;
  std::string str =
      std::to_string(year) + std::to_string(month) + std::to_string(day);
  std::string log_path = "../log/" + str + ".log";
  loguru::add_file(log_path.c_str(), loguru::Append, loguru::Verbosity_INFO);
}

void callback(const char *topic_name_, const proto_messages::CMD &msg,
              long long time_, long long clock_, long long id_) {
  if (msg.camera0_on()) {
    LOG_F(INFO, "Camera 0 is %d", msg.camera0_on());
    int s0 = system("/home/nvidia/workspace/gst-rebooter/scripts/cam0.sh");
    LOG_F(INFO, "cam0 return %d", s0);
  }

  if (msg.camera1_on()) {
    int s1 = system("/home/nvidia/workspace/gst-rebooter/scripts/cam1.sh");
    LOG_F(INFO, "cam1 return %d", s1);
  }

  if (msg.camera2_on()) {
    int s2 = system("/home/nvidia/workspace/gst-rebooter/scripts/cam2.sh");
    LOG_F(INFO, "cam2 return %d", s2);
  }

  if (msg.camera3_on()) {
    int s3 = system("/home/nvidia/workspace/gst-rebooter/scripts/cam3.sh");
    LOG_F(INFO, "cam3 return %d", s3);
  }

  if (msg.camera4_on()) {
    int s4 = system("/home/nvidia/workspace/gst-rebooter/scripts/cam4.sh");
    LOG_F(INFO, "cam4 return %d", s4);
  }

  if (msg.camera5_on()) {
    int s5 = system("/home/nvidia/workspace/gst-rebooter/scripts/cam5.sh");
    LOG_F(INFO, "cam5 return %d", s5);
  }

  if (msg.camera6_on()) {
    int s6 = system("/home/nvidia/workspace/gst-rebooter/scripts/cam6.sh");
    LOG_F(INFO, "cam6 return %d", s6);
  }

  if (msg.camera7_on()) {
    int s7 = system("/home/nvidia/workspace/gst-rebooter/scripts/cam7.sh");
    LOG_F(INFO, "cam7 return %d", s7);
  }
}

int main(int argc, char *argv[]) {
  log_init(argc, argv);

  eCAL::Initialize(argc, argv, "gst_rebooter");
  eCAL::Util::EnableLoopback(true);
  eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1,
                          "gst_rebooter is running");

  eCAL::protobuf::CSubscriber<proto_messages::CMD> sub("cmd");
  

  auto lambda = [](const char *topic_name_, const proto_messages::CMD& cmd,
                      long long time_, long long clock_,
                      long long id_) { callback("cmd", cmd, 0, 0, 0); };
  sub.AddReceiveCallback(lambda);

  while (eCAL::Ok()) {
  }

  return 0;
}
