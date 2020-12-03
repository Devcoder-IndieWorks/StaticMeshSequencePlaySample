#include "ViveStaticMeshSequencePlay.h"
#include "ViveStaticMeshSequenceLoader.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "VLog.h"

AViveStaticMeshSequencePlay::AViveStaticMeshSequencePlay( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "Root" ) );

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StaticMeshComp" ) );
    MeshComponent->SetupAttachment( RootComponent );

    AutoPlay = false;
    RepeatPlay = false;

    IntervalTime = 0.033f; // 30 Frames Per Second
    FormerTime = 0.0f;
    ElapsedTime = 0.0f;

    Meshes[0] = nullptr;
    Meshes[1] = nullptr;

    BufferIndex = 0;
    CurrentFrame = 0;
    Playing = false;
    SetNewMesh = false;
}

void AViveStaticMeshSequencePlay::BeginPlay()
{
    Super::BeginPlay();

    if ( ensure( StaticMeshSequenceLoaderClass.Get() != nullptr ) )
        StaticMeshSequenceLoader = NewObject<UViveStaticMeshSequenceLoader>( this, StaticMeshSequenceLoaderClass );

    Playing = AutoPlay;
    ResetMemberVariables();

    if ( ensure( StaticMeshSequenceLoader != nullptr ) ) {
        StaticMeshSequenceLoader->SetupMeshAssetList();
        RequestAsyncLoad();
    }

    if ( ensure( FirstFrameMesh != nullptr ) )
        MeshComponent->SetStaticMesh( FirstFrameMesh );
}

void AViveStaticMeshSequencePlay::Tick( float InDeltaSecond )
{
    if ( Playing ) {
        if ( StaticMeshSequenceLoader != nullptr ) {
            ElapsedTime += InDeltaSecond;

            bool next = (ElapsedTime - FormerTime) >= IntervalTime;
            if ( next && SetNewMesh ) {
                RequestAsyncLoad();
                FormerTime = ElapsedTime;
                SetNewMesh = false;

                ApplyToStaticMeshComponent();
            }

            if ( RepeatPlay && next && StaticMeshSequenceLoader->IsFinished() )
                Play( true );
            else if ( next && StaticMeshSequenceLoader->IsFinished() )
                Stop();
        }
    }

    Super::Tick( InDeltaSecond );
}

void AViveStaticMeshSequencePlay::Play( bool InFirst )
{
    if ( InFirst )
        Stop();

    Playing = true;
    VLOG( Log, TEXT( "#### Start sequence play. ####" ) );
}

void AViveStaticMeshSequencePlay::RequestAsyncLoad()
{
    TWeakObjectPtr<AViveStaticMeshSequencePlay> weakThis( this );
    StaticMeshSequenceLoader->RequestAsyncLoad( [weakThis]( UStaticMesh* mesh ){
        if ( weakThis.IsValid() ) {
            weakThis->SetStaticMeshIntoBuffer( mesh );
            weakThis->ChangeBufferIndex();
        }
    } );
}

void AViveStaticMeshSequencePlay::Seek( int32 InFrameIndex )
{
    Pause();

    if ( StaticMeshSequenceLoader != nullptr ) {
        if ( StaticMeshSequenceLoader->SetFrameSeek( InFrameIndex ) ) {
            RequestSyncLoad();
            ApplyToStaticMeshComponent();
            CurrentFrame = InFrameIndex;

            RequestAsyncLoad();
            SetNewMesh = false;
            FormerTime = ElapsedTime;

            VLOG( Log, TEXT( "#### Set frame seek. Frame Index: [%d] ####" ), InFrameIndex );
        }
    }
}

void AViveStaticMeshSequencePlay::Pause()
{
    Playing = false;
    GEngine->ForceGarbageCollection( true );
    VLOG( Log, TEXT( "#### Pause sequence play. ####" ) );
}

void AViveStaticMeshSequencePlay::RequestSyncLoad()
{
    TWeakObjectPtr<AViveStaticMeshSequencePlay> weakThis( this );
    StaticMeshSequenceLoader->RequestSyncLoad( [weakThis]( UStaticMesh* mesh ){
        if ( weakThis.IsValid() ) {
            weakThis->SetStaticMeshIntoBuffer( mesh );
            weakThis->ChangeBufferIndex();
        }
    } );
}

void AViveStaticMeshSequencePlay::SetStaticMeshIntoBuffer( UStaticMesh* InStaticMesh )
{
    if ( ensure( BufferIndex < 2 ) ) {
        Meshes[ BufferIndex ] = InStaticMesh;
        SetNewMesh = true;
    }
}

int32 AViveStaticMeshSequencePlay::ChangeBufferIndex()
{
    ++BufferIndex;
    BufferIndex = (BufferIndex % 2);
    return BufferIndex;
}

void AViveStaticMeshSequencePlay::Stop()
{
    ResetMemberVariables();

    if ( ensure( StaticMeshSequenceLoader != nullptr ) ) {
        StaticMeshSequenceLoader->SetupMeshAssetList();
        RequestAsyncLoad();
    }

    if ( ensure( FirstFrameMesh != nullptr ) ) {
        MeshComponent->SetStaticMesh( nullptr );
        MeshComponent->SetStaticMesh( FirstFrameMesh );
    }

    Playing = false;
    GEngine->ForceGarbageCollection( true );
    VLOG( Log, TEXT( "#### Stop sequence play. ####" ) );
}

void AViveStaticMeshSequencePlay::ResetMemberVariables()
{
    BufferIndex = 0;
    CurrentFrame = 0;
    SetNewMesh = false;
    FormerTime = 0.0f;
    ElapsedTime = 0.0f;
    Meshes[0] = nullptr;
    Meshes[1] = nullptr;
}

void AViveStaticMeshSequencePlay::ApplyToStaticMeshComponent()
{
    if ( MeshComponent != nullptr ) {
        int32 index = BufferIndex == 0 ? 1 : 0;
        auto mesh = Meshes[ index ];
        if ( ensure( mesh != nullptr ) ) {
            MeshComponent->SetStaticMesh( nullptr );
            MeshComponent->SetStaticMesh( mesh );

            Meshes[ index ] = nullptr;
            ++CurrentFrame;
        }
    }
}

int32 AViveStaticMeshSequencePlay::GetCurrentFrame() const
{
    return CurrentFrame;
}
