#include "ViveStaticMeshSequenceLoader.h"
#include "Engine/StaticMesh.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/ObjectLibrary.h"

UViveStaticMeshSequenceLoader::UViveStaticMeshSequenceLoader( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
}

void UViveStaticMeshSequenceLoader::SetupMeshAssetList()
{
    CancelStreaming();

    FrameIndex = 1;
    if ( MeshAssetsList.Num() > 0 )
        return;

    if ( !ensure( !MeshAssetsPath.Path.IsEmpty() ) )
        return;

    auto objectLibrary = UObjectLibrary::CreateLibrary( UStaticMesh::StaticClass(), false, GIsEditor );
    if ( !ensure( objectLibrary != nullptr ) )
        return;

    FString content = TEXT( "Content" );
    int32 index = MeshAssetsPath.Path.Find( *content );
    auto meshFolder = MeshAssetsPath.Path.RightChop( index + content.Len() + 1 );
    objectLibrary->LoadAssetDataFromPath( FString::Printf( TEXT( "/Game/%s" ), *meshFolder ) );

    TArray<FAssetData> assetDatas;
    objectLibrary->GetAssetDataList( assetDatas );
    for ( auto& asset : assetDatas )
        MeshAssetsList.Add( asset.ToSoftObjectPath() );

    objectLibrary = nullptr;
}

bool UViveStaticMeshSequenceLoader::IsFinished() const
{
    return (FrameIndex == MeshAssetsList.Num());
}

bool UViveStaticMeshSequenceLoader::SetFrameSeek( int32 InFrameIndex )
{
    if ( InFrameIndex < MeshAssetsList.Num() ) {
        FrameIndex = InFrameIndex;
        return true;
    }

    return false;
}

void UViveStaticMeshSequenceLoader::RequestAsyncLoad( TFunction<void( UStaticMesh* )>&& InCallback )
{
    RequestAsyncLoad( FViveStaticMeshSequenceDelegate::CreateLambda( MoveTemp( InCallback ) ) );
}

void UViveStaticMeshSequenceLoader::RequestAsyncLoad( FViveStaticMeshSequenceDelegate InDelegateToCallback )
{
    CancelStreaming();

    if ( FrameIndex < MeshAssetsList.Num() ) {
        StreamingMeshPath = MeshAssetsList[ FrameIndex ];
        TWeakObjectPtr<UViveStaticMeshSequenceLoader> weakThis( this );
        StreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad( StreamingMeshPath, 
            [weakThis, InDelegateToCallback]{
                if ( weakThis.IsValid() ) {
                    if ( !weakThis->StreamingHandle.IsValid() )
                        return;

                    auto mesh = Cast<UStaticMesh>( weakThis->StreamingHandle->GetLoadedAsset() );
                    if ( mesh != nullptr )
                        InDelegateToCallback.ExecuteIfBound( mesh );
                }
            } );
        ++FrameIndex;
    }
}

void UViveStaticMeshSequenceLoader::RequestSyncLoad( TFunction<void( UStaticMesh* )>&& InCallback )
{
    RequestSyncLoad( FViveStaticMeshSequenceDelegate::CreateLambda( MoveTemp( InCallback ) ) );
}

void UViveStaticMeshSequenceLoader::RequestSyncLoad( FViveStaticMeshSequenceDelegate InDelegateToCallback )
{
    CancelStreaming();

    if ( FrameIndex < MeshAssetsList.Num() ) {
        StreamingMeshPath = MeshAssetsList[ FrameIndex ];
        StreamingHandle = UAssetManager::GetStreamableManager().RequestSyncLoad( StreamingMeshPath );
        if ( StreamingHandle.IsValid() ) {
            auto mesh = Cast<UStaticMesh>( StreamingHandle->GetLoadedAsset() );
            if ( mesh != nullptr )
                InDelegateToCallback.ExecuteIfBound( mesh );
        }
        ++FrameIndex;
    }
}

void UViveStaticMeshSequenceLoader::CancelStreaming()
{
    if ( StreamingHandle.IsValid() ) {
        StreamingHandle->CancelHandle();
        StreamingHandle.Reset();
    }

    if ( StreamingMeshPath.IsValid() )
        StreamingMeshPath.Reset();
}
