// Copyright Lemy. All Rights Reserved.

#pragma once
#include "NativeGameplayTags.h"

namespace  LabGameplayTags{	
	LYRALAB_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	LYRALAB_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	LYRALAB_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	LYRALAB_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	LYRALAB_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
	LYRALAB_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);
	LYRALAB_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	
}
