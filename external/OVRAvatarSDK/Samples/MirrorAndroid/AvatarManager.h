/************************************************************************************

Filename    :   AvatarManager.h
Content     :   Sample usage of the AvatarSDK.
Created     :
Authors     :

Copyright   :   Copyright 2014 Oculus VR, LLC. All Rights reserved.

************************************************************************************/

#ifndef AVATARMANAGER_H
#define AVATARMANAGER_H

#include "OVR_Input.h"

namespace OVR
{

class AvatarManager
{
public:
			AvatarManager();
			~AvatarManager();

	void	Initialize( const char* appId, jobject activity, JNIEnv* jni );
	void	Destroy();

	void	Frame( const ovrFrameInput & vrFrame, const Matrix4f viewMatrix );
	void	AppendSurfaceList( Array< ovrDrawSurface > * surfaceList );
};

} // namespace OVR

#endif // AVATARMANAGER_H
