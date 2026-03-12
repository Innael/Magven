// Magvenures Game by Bolshakov Sergey.  All RightsReserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MagventuresEditorTarget : TargetRules
{
	public MagventuresEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Magventures" } );
	}
}
