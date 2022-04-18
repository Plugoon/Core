// Fill out your copyright notice in the Description page of Project Settings.


#include "O_Exception.h"

FS_Exception UO_Exception::ConvertToStruct() const{
    FS_Exception Result;
    Result.bHasError = true;
    Result.ExceptionType = ExceptionType;
    Result.Message = Message;
    return Result;
}


