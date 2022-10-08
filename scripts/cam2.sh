#!/bin/bash

#!!! open record video in windows !!!

time=$(date "+%Y-%m-%d-%H:%M:%S")

DEVICE=${DEVICE:-/dev/video2}
IP=${IP:-192.168.89.231}
PORT=${PORT:-5000}
WIDTH=${WIDTH:-1920}
HEIGHT=${HEIGHT:-1080}
FPS=${FPS:-30}
BITRATE=${BITRATE:-3500}

# single port send, h264enc
#gst-launch-1.0 -v v4l2src device=${DEVICE} ! video/x-raw,width=${WIDTH},height=${HEIGHT},framerate=${FPS}/1 ! videoscale ! videoconvert ! x264enc tune=zerolatency bitrate=${BITRATE} speed-preset=superfast ! rtph264pay ! udpsink host=${IP} port=${PORT} async=false sync=false

# save origin video
#gst-launch-1.0 -v v4l2src device=${DEVICE} ! video/x-raw,width=${WIDTH},height=${HEIGHT},framerate=${FPS}/1 ! videoscale ! videoconvert ! tee name=t t. ! queue leaky=1 ! x264enc tune=zerolatency bitrate=${BITRATE} speed-preset=superfast ! rtph264pay ! udpsink host=${IP} port=${PORT} async=false sync=false t. ! queue ! avimux ! filesink location=$time.avi

# send h264 video and save
gst-launch-1.0 -v v4l2src device=${DEVICE} ! video/x-raw,width=${WIDTH},height=${HEIGHT},framerate=${FPS}/1 ! videoscale ! videoconvert ! x264enc tune=zerolatency bitrate=${BITRATE} speed-preset=superfast ! tee name=t t. ! queue leaky=1 ! rtph264pay ! udpsink host=${IP} port=${PORT} async=false sync=false t. ! queue ! avimux ! filesink location=$time.avi

# single port send, omxh264enc
#gst-launch-1.0 -v v4l2src device=${DEVICE} ! video/x-raw,width=${WIDTH},height=${HEIGHT},framerate=${FPS}/1 ! videoscale ! videoconvert ! 'video/x-raw' ! nvvidconv ! 'video/x-raw(memory:NVMM),format=I420,width=1920,height=1080' ! omxh264enc insert-sps-pps=true bitrate=16000000 ! h264parse ! 'video/x-h264,stream-format=byte-stream' ! rtph264pay name=pay0 pt=96 ! udpsink host=${IP} port=${PORT} async=false sync=false

# multi port send
#gst-launch-1.0 -v v4l2src device=${DEVICE} ! video/x-raw,width=${WIDTH},height=${HEIGHT},framerate=${FPS}/1 ! videoscale ! videoconvert ! 'video/x-raw' ! nvvidconv ! 'video/x-raw(memory:NVMM),format=I420,width=1280,height=720' ! omxh264enc  ! h264parse ! 'video/x-h264,stream-format=byte-stream' ! rtph264pay name=pay0 pt=96 ! multiudpsink clients="${IP}:5000,${IP}:5001,${IP}:5002,${IP}:5003,${IP}:5004,${IP}:5005,${IP}:5006,${IP}:5007" async=false sync=false

# send omxh264enc video and save
#gst-launch-1.0 -v v4l2src device=${DEVICE} ! video/x-raw,width=${WIDTH},height=${HEIGHT},framerate=${FPS}/1 ! videoscale ! videoconvert ! 'video/x-raw' ! nvvidconv ! 'video/x-raw(memory:NVMM),format=I420,width=1920,height=1080' ! omxh264enc bitrate=8000000 ! h264parse ! 'video/x-h264,stream-format=byte-stream' ! tee name=t t. ! queue leaky=1 ! rtph264pay name=pay0 pt=96 ! udpsink host=${IP} port=${PORT} async=false sync=false t. ! queue ! avimux ! filesink location=$time.avi
