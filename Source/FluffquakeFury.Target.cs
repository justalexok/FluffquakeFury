// Copyright Alex Goulder

using UnrealBuildTool;
using System.Collections.Generic;

public class FluffquakeFuryTarget : TargetRules
{
	public FluffquakeFuryTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "FluffquakeFury" } );
	}
}
