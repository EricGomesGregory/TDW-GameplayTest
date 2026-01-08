// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWInputComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWInputComponent)

UTDWInputComponent::UTDWInputComponent()
{
}

void UTDWInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}



