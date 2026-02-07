// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueprintOnlineHelperUtils.h"
#include "ThirdParty/Steamworks/Steamv157/sdk/public/steam/steam_api.h"

#define LOCTEXT_NAMESPACE "FBlueprintOnlineHelperUtilsModule"

void FBlueprintOnlineHelperUtilsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	SteamAPI_Init();
}

void FBlueprintOnlineHelperUtilsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	SteamAPI_Shutdown();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlueprintOnlineHelperUtilsModule, BlueprintOnlineHelperUtils)