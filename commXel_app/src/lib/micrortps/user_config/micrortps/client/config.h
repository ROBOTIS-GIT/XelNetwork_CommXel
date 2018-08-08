//    Copyright 2017 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#ifndef _MICRORTPS_CLIENT_CONFIG_H_
#define _MICRORTPS_CLIENT_CONFIG_H_

#define MICRORTPS_CLIENT_VERSION_MAJOR 1
#define MICRORTPS_CLIENT_VERSION_MINOR 0
#define MICRORTPS_CLIENT_VERSION_MICRO 0
#define MICRORTPS_CLIENT_VERSION_STR "1.0.0"

#define PROFILE_CREATE_ENTITIES_REF
#define PROFILE_CREATE_ENTITIES_XML
#define PROFILE_READ_ACCESS

#if defined(__OPENCR__)
#define PLATFORM_NAME_OPENCR
#define PROFILE_SERIAL_TRANSPORT
#elif defined(__COMMXEL__)
#define PLATFORM_NAME_COMMXEL
#define PROFILE_UDP_TRANSPORT
#define PROFILE_SERIAL_TRANSPORT
//#define PROFILE_TCP_TRANSPORT
#endif

#define MR_CONFIG_MAX_OUTPUT_BEST_EFFORT_STREAMS     4
#define MR_CONFIG_MAX_OUTPUT_RELIABLE_STREAMS        20
#define MR_CONFIG_MAX_INPUT_BEST_EFFORT_STREAMS      4
#define MR_CONFIG_MAX_INPUT_RELIABLE_STREAMS         20

#define MR_CONFIG_MAX_SESSION_CONNECTION_ATTEMPTS    4
#define MR_CONFIG_MIN_SESSION_CONNECTION_INTERVAL    1000
#define MR_CONFIG_MIN_HEARTBEAT_TIME_INTERVAL        10

#define MR_CONFIG_UDP_TRANSPORT_MTU                  2048
#define MR_CONFIG_TCP_TRANSPORT_MTU                  2048
#define MR_CONFIG_SERIAL_TRANSPORT_MTU               2048

#endif // _MICRORTPS_CLIENT_CONFIG_H_
