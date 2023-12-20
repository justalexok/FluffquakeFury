// Copyright Alex Goulder

using UnrealBuildTool;
using System.Collections.Generic;

public class FluffquakeFuryEditorTarget : TargetRules
{
	public FluffquakeFuryEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "FluffquakeFury" } );
	}
}
