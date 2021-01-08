// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "MusicManager.generated.h"

UCLASS()
class TESTING_API AMusicManager : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, Category = Audio, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* Player;
public:	
	// Sets default values for this actor's properties
	AMusicManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category=Audio)
	void OnAudioPlaybackFinish();

	UFUNCTION(BlueprintCallable, Category=Audio)
    void LoadAudio();

	UFUNCTION(BlueprintCallable, Category=Audio)
    void LoadWAVFile();

	UFUNCTION(BlueprintCallable, Category=Audio)
    void ListAllFromFile();

	UFUNCTION(BlueprintCallable, Category=Audio)
	void PlayNext();

	UFUNCTION(BlueprintCallable, Category=Audio)
    void PlayNextShuffle();

	UFUNCTION(BlueprintCallable, Category=Audio)
    void PauseAudio();

	UFUNCTION(BlueprintCallable, Category=Audio)
    void ResumeAudio();

	UFUNCTION(BlueprintCallable, Category=Audio)
    void SetProperties(bool Mute = false, float NewMaxVol = -1, float NewVolumeMultiplier = -1, float NewPitchMultiplier = -1, bool ShouldShuffle = false);


public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	float MaxVol;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	float VolumeMultiplier;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	float PitchMultiplier;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	bool bShuffle;

private:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	TArray<USoundBase*> MusicList;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ShufflePlayed;
	
	int32 LastIndex = 0;
};
