// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class StaticMeshSequencePlaySampleEditorTarget : TargetRules
{
	public StaticMeshSequencePlaySampleEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "StaticMeshSequencePlaySample" } );
	}
}