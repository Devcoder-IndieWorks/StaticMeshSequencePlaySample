// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "ViveStaticMeshSequencePlay.generated.h"

UCLASS( Abstract ) 
class STATICMESHSEQUENCEPLAY_API AViveStaticMeshSequencePlay : public AActor
{
    GENERATED_UCLASS_BODY()
public:
    void BeginPlay() override;
    void Tick( float InDeltaSecond ) override;

    //-------------------------------------------------------------------------

    int32 ChangeBufferIndex();
    void SetStaticMeshIntoBuffer( class UStaticMesh* InStaticMesh );

public:
    UFUNCTION( BlueprintCallable, Category="StaticMeshSequencePlay|Functions" )
    void Play( bool InFirst = false );
    UFUNCTION( BlueprintCallable, Category="StaticMeshSequencePlay|Functions" )
    void Stop();
    UFUNCTION( BlueprintCallable, Category="StaticMeshSequencePlay|Functions" )
    void Pause();
    UFUNCTION( BlueprintCallable, Category="StaticMeshSequencePlay|Functions" )
    void Seek( int32 InFrameIndex );
    UFUNCTION( BlueprintCallable, Category="StaticMeshSequencePlay|Functions" )
    int32 GetCurrentFrame() const;

private:
    void ResetMemberVariables();

    //-------------------------------------------------------------------------

    void RequestAsyncLoad();
    void RequestSyncLoad();

    //-------------------------------------------------------------------------

    void ApplyToStaticMeshComponent();

private:
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="StaticMeshSequencePlay|Properties", meta=(AllowPrivateAccess=true) )
    TSubclassOf<class UViveStaticMeshSequenceLoader> StaticMeshSequenceLoaderClass;
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="StaticMeshSequencePlay|Properties", meta=(AllowPrivateAccess=true) )
    float IntervalTime;
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="StaticMeshSequencePlay|Properties", meta=(AllowPrivateAccess=true) )
    class UStaticMesh* FirstFrameMesh;
    UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category="StaticMeshSequencePlay|Properties", meta=(AllowPrivateAccess=true) )
    class UStaticMeshComponent* MeshComponent;
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="StaticMeshSequencePlay|Properties", meta=(AllowPrivateAccess=true) )
    bool AutoPlay;
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="StaticMeshSequencePlay|Properties", meta=(AllowPrivateAccess=true) )
    bool RepeatPlay;

    UPROPERTY( Transient )
    class UViveStaticMeshSequenceLoader* StaticMeshSequenceLoader;
    UPROPERTY( Transient )
    class UStaticMesh* Meshes[2];

private:
    int32 BufferIndex;
    int32 CurrentFrame;
    bool Playing;
    bool SetNewMesh;
    float FormerTime;
    float ElapsedTime;
};
