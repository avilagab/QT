#ifndef ELSCLIENT_H
#define ELSCLIENT_H

//#include "edk.h"

#ifndef EDK_STATIC_LIB
#ifdef ELSCLIENT_EXPORTS
#ifdef WIN32
#define ELSCLIENT_API __declspec(dllexport)
#else
#define ELSCLIENT_API
#endif
#else
#ifdef WIN32
#define ELSCLIENT_API __declspec(dllimport)
#else
#define ELSCLIENT_API
#endif
#endif
#else
#define ELSCLIENT_API extern
#endif
extern "C" {

enum profileFileType
{
TRAINING,
EMOKEY
};
//! Initializes the connection to ElsCloud server
/*!
\return bool
- true if connect successfully
*/
ELSCLIENT_API bool ELS_Connect();

//! Login on ElsCloud with exist account
/*!
\param email - email of account
\param password - password of account
\param userID	- receives number of user
\return bool
- true if login successfully and return id of user
*/
ELSCLIENT_API bool ELS_Login(const char*email, const char*password, int* userID);

//! Create new infomation of headset on ElsCloud
/*!
\param userID
\return bool
- true if create headset successfully
*/
ELSCLIENT_API bool ELS_CreateHeadset(int userID);

//! Create new protocol on ElsCloud
/*!
\param name - name of protocol
\param protocolID - receives number of protocol
\return bool
- true if create protocol successfully and return id of protocol
*/
ELSCLIENT_API bool ELS_CreateProtocol(const char *name, int *protocolID);

//! Create subject in a protocol on ElsCloud
/*!
\param userID - id of user
\param protocolID	- id of protocol
\return bool
- true if add subject successfully
*/
ELSCLIENT_API bool ELS_AddSubjectToProtocol(int userID, int protocolID);

//! Set protocol on ElsCloud
/*!
\param protocolID	- protocolID of protocol
\return bool
- true if set protocol successfully
*/
ELSCLIENT_API bool ELS_SetProtocol(int protocolID);

//! Set experiment on ElsCloud
/*!
\param experimentID	- experimentID of experiment
\return bool
- true if set protocol successfully
*/
ELSCLIENT_API bool ELS_SetExperiment(int experimentID);
//! Create new experiment on ElsCloud
/*!
\param name - name of experiment
\param description - description of experiment
\param experimentID - receives number of experiment

\return bool
- true if create experiment successfully and return id of experiment
*/
ELSCLIENT_API bool ELS_CreateExperiment(const char *name, const char *description, int *experimentID);

//! Create new session on ElsCloud
/*!
\param sessionID - receives id of session
\return cont
- true if create session successfully and return id of session
*/
ELSCLIENT_API const char * ELS_CreateRecordingSession();

//! Start record data from headset and upload to ElsCloud
/*!
\return char
- return null if create sessionID successfully
*/
ELSCLIENT_API bool ELS_StartRecordData();
//! Start record data from headset and upload to ElsCloud
/*!
\param oceiFilePath
\param timeLimit (h)
\param overtime
\return char
- return null if create sessionID successfully
*/
ELSCLIENT_API bool ELS_StartRecordDataWithOCEFile(const char* oceiFilePath, bool& overtime, int timeLimit = 24);

//! Stop record data from headset
/*!
\return bool
- true if end successfully
*/
ELSCLIENT_API bool ELS_StopRecordData();

//! Request get report on ElsCloud
/*!
\return void
\param int*

*/
ELSCLIENT_API void ELS_GetReportOnline(int * engagementScore, int* excitementScore, int* stressScore,int* relaxScore,int* interestScore);

//! Request get offline report on ElsCloud
/*!
\param bool
- true if report is generated on ElsCloud successfully
*/
ELSCLIENT_API void ELS_GetOfflineReport(int * engagementScore, int* excitementScore, int* affinityScore);

//! Start marker of "eyes open" state and upload on ElsCloud
/*!
\return bool
- true if marker successfully
*/
ELSCLIENT_API bool ELS_Marker_EyeOpenStart();

//! End marker of "eyes open" state and upload on ElsCloud
/*!
\return bool
- true if marker successfully
*/
ELSCLIENT_API bool ELS_Marker_EyeOpenEnd();

//! Start marker of "eyes closed" state and upload on ElsCloud
/*!
\return bool
- true if marker successfully
*/
ELSCLIENT_API bool ELS_Marker_EyeClosedStart();

//! End marker of "eyes closed" state and upload on ElsCloud
/*!
\return bool
- true if marker successfully
*/
ELSCLIENT_API bool ELS_Marker_EyeClosedEnd();

//! Start marker of "recording" state and upload on ElsCloud
/*!
\return bool
- true if marker successfully
*/
ELSCLIENT_API bool ELS_Marker_RecordingStart();

//! Update note for session
/*!
\param note - note of session

\return bool
- true if update successfully
*/
ELSCLIENT_API bool ELS_UpdateNote(const char* note);

//! Update tag for session
/*!
\param tag - tag of session
\return bool
- true if update successfully
*/
ELSCLIENT_API bool ELS_UpdateTag(const char * tag[]);

//! Update photo for session
/*!
\param filePath - path of image file which want upload

\return bool
- true if upload successfully
*/
ELSCLIENT_API bool ELS_UploadPhoto(const char* filePath);

//! Terminate the connection to ElsCloud server and EmoEngine
/*!

*/
ELSCLIENT_API void ELS_Disconnect();

//! Update new EmoState from Engine
/*!
\param eState - new emoStateHandle
*/
ELSCLIENT_API const char* ELS_UploadEdfFile(const char * emostateFilePath, const char * edfFilePath);

//! Upload file profile of user
/*!
\param profileName -
\param filePath
\param ptype

\return bool
- 1 if succes, 0 if failed
*/
ELSCLIENT_API bool ELS_UploadProfileFile(const char* profileName,const char* filePath, profileFileType ptype);
//! Update exist profile of user
/*!
\param profileId -
\param filePath

\return bool
- 1 if succes, 0 if failed
*/
ELSCLIENT_API bool ELS_UpdateProfileFile(int profileId,const char* filePath);
//! Delete exist profile of user
/*!
\param profileId -

\return bool
- 1 if succes, 0 if failed
*/
ELSCLIENT_API bool ELS_DeleteProfileFile(int profileId);

//! Get info Profile of user
/*!
\param profile name
\ return int
- profile id of profile name
- 0 if request failed, -1 if not existe profile name
*/
ELSCLIENT_API int ELS_GetProfileId(const char* profileName);

//! Donwload file Profile
/*!
\param profile id
\param destPath
\return bool
- 1 if succes, 0 if failed
*/
ELSCLIENT_API bool ELS_DownloadFileProfile(int profileId,const char* destPath);

//! Get All Profile Name
/*!
\return int
- Number of Profile Name
*/
ELSCLIENT_API int ELS_GetAllProfileName();

//! Get ProfileIDNameAtIndex
/*!
\param index
\return const char*
- Profile Name at index
*/

ELSCLIENT_API int ELS_ProfileIDAtIndex(int index);
//! Get ProfileNameAtIndex
/*!
\param index
\return const char*
- Profile Name at index
*/

ELSCLIENT_API const char* ELS_ProfileNameAtIndex(int index);
//! Get last modified time of Profile
/*!
\param index
\return const char*
- last modified time of Profile at index
*/

ELSCLIENT_API const char* ELS_ProfileLastModifiedAtIndex(int index);
//! Get type of Profile
/*!
\param index
\return const char*
- type of Profile at index
*/

ELSCLIENT_API profileFileType ELS_ProfileTypeAtIndex(int index);

//! Save Open Close Eye Info to file
/*!
\param fileName
\return bool
- 1 if succes, 0 if failed
*/
ELSCLIENT_API bool ELS_SaveOpenCloseEyeInfo(const char* fileName);
//! Reset pasword
/*!
\param userName
\return bool
- 1 if succes, 0 if failed
*/
ELSCLIENT_API bool ELS_ResetPassword(const char* userName);
//! Logout
/*
\return bool
- 1 if succes, 0 if failed
*/
ELSCLIENT_API bool ELS_Logout();
//! Reupload offline data
/*
\param filename - name of file which contains offline info
\return bool
- 1 if succes, 0 if failed
*/
ELSCLIENT_API bool ELS_ReuploadOfflineSession(const char* filename);
//! save offline session info to file
/*
\param filename - the name of the file used to store offline session information for later upload
\return bool
- 1 if succes, 0 if failed
*/
ELSCLIENT_API void ELS_SaveOfflineSessionInfo(const char* fileName);
}

#endif // ELSCLIENT_H
