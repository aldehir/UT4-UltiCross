#pragma once

#include "Core.h"
#include "Object.h"

template<typename T>
T* FindProperty(UObject* Top, const FString& PropertyPath, void** Out)
{
  TArray<FString> PathSegments;
  PropertyPath.ParseIntoArray(PathSegments, TEXT("."));

  void* Current = (void*)Top;
  UStruct* CurrentMeta = StaticCast<UStruct*>(Top->GetClass());
  UProperty* Property = nullptr;

  for (const FString& Segment : PathSegments)
  {
    FName AsName = FName(*Segment);
    Property = CurrentMeta->FindPropertyByName(AsName);

    // Return nullptr if we could not find the property
    if (Property == nullptr)
    {
      return nullptr;
    }

    UStructProperty* StructProperty = Cast<UStructProperty>(Property);
    if (StructProperty != nullptr)
    {
      Current = Property->ContainerPtrToValuePtr<void>(Current);
      CurrentMeta = StaticCast<UStruct*>(StructProperty->Struct);
    }
  }

  // Could not find property
  if (Property == nullptr || Current == nullptr)
  {
    return nullptr;
  }

  // Set out pointer to the value pointer
  *Out = Property->ContainerPtrToValuePtr<void>(Current);
  return Cast<T>(Property);
}

template<typename T>
T* FindPropertyChecked(UObject* Top, const FString& PropertyPath, void** Out)
{
  T* Property = FindProperty<T>(Top, PropertyPath, Out);
  check(Property != nullptr);
  return Property;
}
