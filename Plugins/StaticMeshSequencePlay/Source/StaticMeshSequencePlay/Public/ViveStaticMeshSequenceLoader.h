// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "Core.h"
#include "UObject/Object.h"
#include "Engine/StreamableManager.h"
#include "ViveStaticMeshSequenceLoader.generated.h"

DECLARE_DELEGATE_OneParam( FViveStaticMeshSequenceDelegate, class UStaticMesh* );

UCLASS( Blueprintable, BlueprintType ) 
class STATICMESHSEQUENCEPLAY_API UViveStaticMeshSequenceLoader : public UObject
{
    GENERATED_UCLASS_BODY()
public:
    void SetupMeshAssetList();
    bool IsFinished() const;
    bool SetFrameSeek( int32 InFrameIndex );

    //-------------------------------------------------------------------------

    void RequestAsyncLoad( TFunction<void( class UStaticMesh* )>&& InCallback );
    void RequestAsyncLoad( FViveStaticMeshSequenceDelegate InDelegateToCallback );

    //-------------------------------------------------------------------------

    void RequestSyncLoad( TFunction<void( class UStaticMesh* )>&& InCallback );
    void RequestSyncLoad( FViveStaticMeshSequenceDelegate InDelegateToCallback );

private:
    void CancelStreaming();

private:
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="StaticMeshSequencePlay|Properties", meta=(AllowPrivateAccess=true, RelativePath) )
    FDirectoryPath MeshAssetsPath;

private:
    int32 FrameIndex;
    TArray<FSoftObjectPath> MeshAssetsList;

    //-------------------------------------------------------------------------

    FSoftObjectPath StreamingMeshPath;
    TSharedPtr<FStreamableHandle> StreamingHandle;
};
