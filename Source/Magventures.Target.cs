// Magvenures Game by Bolshakov Sergey.  All RightsReserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MagventuresTarget : TargetRules
{
	public MagventuresTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Magventures" } );
	}
}
