#include "Thread.hpp"
#include "Configuration.hpp"
#include "Tools.hpp"
#include "Log.hpp"

#include "CrossCorrelation.hpp"
#include "MicrophoneInterface.hpp"
#include "NodeJSInterface.hpp"

static pthread_cond_t  sov_launchSynchroCond 	= PTHREAD_COND_INITIALIZER;
static pthread_mutex_t sov_launchSynchroMutex	= PTHREAD_MUTEX_INITIALIZER;

class MediaThread : public Thread{
private:
	std::string ov_fileName;
	MicInterface ov_micInterface;
	CrossCorrelation ov_crossCorrelation;
protected:
	virtual void run(){
		ov_crossCorrelation(ov_fileName);
		ov_crossCorrelation.init();
		ov_micInterface.init();
		pthread_cond_wait(&sov_launchSynchroCond, &sov_launchSynchroMutex);
		while(1){
			ov_micInterface.getSamples();
			ov_crossCorrelation.process();
		}
	}
public:
	int setFileName(std::string av_fileName){
		ov_fileName = av_fileName;
		return 0;
	}
	int getTimeStamp(long &av_timeStamp){
		return CrossCorrelation.getTimeStamp(av_timeStamp);
	}
};

int main(int argc, char* argv[]){
	NodeJSInterface 			lv_nodeJSInterface;
	MediaThread 				lv_mediaThread;
	std::vector<std::string> 	lv_reqParsing;
	std::string 				lv_readString;
	bool 						lv_isDeviceRegistered;
	enum State 					lv_state;

    lv_state = SYNC_ENDED;
	lv_isDeviceRegistered = verifyDevice();

	while(1){
		lv_nodeJSInterface.listen();
		lv_nodeJSInterface.read(lv_readString);
		parseRequest(lv_readString,lv_reqParsing):
		if(!lv_isDeviceRegistered){
            lv_nodeJSInterface.write("ERROR_DEVICE_NOT_REGISTERED");
            LOG_ERROR << "Device is not registered";
        }
        else{
            switch(lv_state){
                case SYNC_ENDED:
                	switch(lv_reqParsing[0]){
                		LOG_INFO << "Current state: SYNC_ENDED  Current event: "<<lv_reqParsing[0];
                		case "LOAD_FILE":
                			LOG_INFO << "File name : " << lv_reqParsing[1];
                			if(isFileExist(lv_reqParsing[1])){
                				MediaThread.setFileName(lv_reqParsing[1]);
                				lv_nodeJSInterface.write("OK FILE_FOUND_AND_LOADED");
                				//returnValue=pthread_create( &synchronizationThread, NULL, synchronizationThreadHandle, (void*) (buffer+10));
                				//if(returnValue){
                					//logProcessP("MAIN THREAD  - LOG ERROR - main: pthread_create() return code: ",returnValue);
                					//exit(EXIT_FAILURE);
                				//}
                				LOG_INFO << "File : "<<lv_reqParsing[1]<< " is found and loaded";
                				lv_state = FILE_CHOSEN;
                			}
                			else{
                				lv_nodeJSInterface.write("ERROR FILE_NOT_FOUND");
                				LOG_ERROR << "File : "<<lv_reqParsing[1]<< " isn't found";
                			}
                		break;
                		default:
                			lv_nodeJSInterface.write("ERROR FILE_NOT_CHOSEN_YET");
                			LOG_ERROR << "File isn't chosen yet";
                		break;
                	}
                break;
				case FILE_CHOSEN:
                    switch(lv_reqParsing[0]){
                    	LOG_INFO << "Current state: FILE_CHOSEN  Current event: "<<lv_reqParsing[0];
                		case "LOAD_FILE":
                			LOG_INFO << "File name : " << lv_reqParsing[1];
                			if(isFileExist(lv_reqParsing[1])){
                				//pthread_cancel(synchronizationThread);
                            	//logProcess("MAIN THREAD  - LOG WARN - main: ynchronization thread is killed");
                				MediaThread.setFileName(lv_reqParsing[1]);
                				lv_nodeJSInterface.write("OK FILE_FOUND_AND_LOADED");
                				//returnValue=pthread_create( &synchronizationThread, NULL, synchronizationThreadHandle, (void*) (buffer+10));
                				//if(returnValue){
                					//logProcessP("MAIN THREAD  - LOG ERROR - main: pthread_create() return code: ",returnValue);
                					//exit(EXIT_FAILURE);
                				//}
                				LOG_INFO << "File : "<<lv_reqParsing[1]<< " is found and loaded";
                				lv_state = FILE_CHOSEN;
                			}
                			else{
                				lv_nodeJSInterface.write("ERROR FILE_NOT_FOUND");
                				LOG_ERROR << "File : "<<lv_reqParsing[1]<< " isn't found";
                			}
                		break;
                		case "PLAY_FILE":
                			lv_nodeJSInterface.write("OK SYNCHRONIZATION_LAUNCHED");
                        	pthread_cond_signal(&sov_launchSynchroCond);
                        	LOG_INFO << "Starting synchronization ...";
                        	state = SYNC_STARTED;
                		break;
                		case "GET_TIMST":
                			lv_nodeJSInterface.write("OK 0");
                			LOG_INFO << "Handling timestamp request TS = 0";
                		break;
                		default:
                		break;
                	}
                break;
                case SYNC_STARTED:
                	LOG_INFO << "Current state: SYNC_STARTED";
                	switch(lv_reqParsing[0]){
                		case "LOAD_FILE":
                		break;
                		case "PLAY_FILE":
                		break;
                		case "GET_TIMST":
                		break;
                		default:
                		break;
                	}
                break;
                default:
                	LOG_INFO << "Current state: UNKNOWN_STATE";
                	lv_nodeJSInterface.write("ERROR UNKNOWN_STATE");
                break;
            }
        }
        lv_nodeJSInterface.closeClient();
	}
	lv_nodeJSInterface.closeServer();
	return 0;
}