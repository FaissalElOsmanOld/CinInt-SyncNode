#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#define LOG_LEVEL DEBUG	

#define SYNCNODE_CPP_PROCESS_PORT_NUM 51712
#define CLOCKS_PER_MSEC CLOCKS_PER_SEC*1000

#define NB_OF_SECONS_REQUIRED_FOR_VALIDATION 5
#define	NB_OF_MINUTE_HANDLED_IN_A_THREAD 5
#define	IDENTIFICATION_THRESHOLD 0.8
#define NB_OF_THREADS 3
#define	SAMPLING_RATE 8000
#ifndef __arm__
#define DEVICE_MAC_ADDRESS "f4:5c:89:c5:5e:af\0"
#else
#define DEVICE_MAC_ADDRESS "b8:27:eb:45:90:ad\0"
#endif

#define FOUR_SEC_BUFF_TIME_IN_MICRO_SEC 4000000
#define PCM_NAME "plughw:CARD=duo,DEV=0"
#define CHANNELS_NUM 1
#endif
