#include "FL_Exception.h"

void UFL_Exception::CreateException(TSubclassOf<UO_Exception> Type, const FString& Message, UObject* Owner, FS_Exception& Exception) {
    UO_Exception* Ex = CreateException(Type, Message, Owner);
    if (Ex) {
        Exception = Ex->ConvertToStruct();
    } else {
        Exception.bHasError = false;
        Exception.ExceptionType = "NoException";
        Exception.Message = "Invalid Exception";
    }
}


UO_Exception* UFL_Exception::CreateException(TSubclassOf<UO_Exception> Type, const FString& Message, UObject* Owner) {
    if (!Type) {
        return nullptr;
    }
    UO_Exception* Result = NewObject<UO_Exception>(Owner, Type);
    Result->Message = Message;
    return Result;
}


