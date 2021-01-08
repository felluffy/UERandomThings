// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicManager.h"

// Sets default values
AMusicManager::AMusicManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Player = CreateDefaultSubobject<UAudioComponent>("Player");
	Player->Activate();
}

// Called when the game starts or when spawned
void AMusicManager::BeginPlay()
{
	Super::BeginPlay();
	if(Player)
		Player->OnAudioFinished.AddDynamic(this, &AMusicManager::OnAudioPlaybackFinish);
	
}

// Called every frame
void AMusicManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMusicManager::OnAudioPlaybackFinish()
{
	bShuffle == true ? PlayNextShuffle() : PlayNext();
}

void AMusicManager::LoadAudio()
{
}

void AMusicManager::LoadWAVFile()
{
}

void AMusicManager::ListAllFromFile()
{
}

void AMusicManager::PlayNext()
{
	
	Player->SetSound(MusicList[LastIndex++]);
	LastIndex = LastIndex % MusicList.Num();
	Player->Play();
}

void AMusicManager::PlayNextShuffle()
{
	if(MusicList.Num() < 1)
		return;
	TArray<int32> MapForUnplayed;
	for(int32 i = 0, curr_ind = 0; i != ShufflePlayed.Num(); i++)
	{
		if(!ShufflePlayed[i])
		{
			MapForUnplayed.Add(i);
		}
	}
	if(MapForUnplayed.Num() == 0)
	{
		ShufflePlayed.Empty();
		ShufflePlayed.Init(false, MusicList.Num());
		LastIndex = 0;
		PlayNextShuffle();
	}
	else
	{
		FRandomStream RS;
		int32 ToPlay = RS.RandRange(0, MapForUnplayed.Num());
		ShufflePlayed[MapForUnplayed[ToPlay]] = true;
		Player->SetSound(MusicList[MapForUnplayed[ToPlay]]);
		Player->Play();
		MapForUnplayed.Empty();
	}
}

void AMusicManager::PauseAudio()
{
	if(Player)
		Player->SetPaused(false);
}

void AMusicManager::ResumeAudio()
{
	if(Player)
		Player->SetPaused(false);
}

void AMusicManager::SetProperties(bool Mute, float NewMaxVol, float NewVolumeMultiplier, float NewPitchMultiplier,
	bool ShouldShuffle)
{
	if(ShouldShuffle == true)
	{
		bShuffle = true;
		ShufflePlayed.Empty();
		ShufflePlayed.Init(false, MusicList.Num());
		LastIndex = 0;
		Player->Stop();
	}
	else
		bShuffle = false;
}

