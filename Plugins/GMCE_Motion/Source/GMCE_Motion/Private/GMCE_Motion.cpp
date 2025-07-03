#include "GMCE_Motion.h"

#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FGMCE_MotionModule"

void FGMCE_MotionModule::StartupModule()
{
	UGameplayTagsManager::Get().AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("GMCE_Motion/Config/Tags"));
}

void FGMCE_MotionModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGMCE_MotionModule, GMCE_Motion)