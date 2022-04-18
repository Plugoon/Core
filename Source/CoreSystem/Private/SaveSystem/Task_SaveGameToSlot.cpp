#include "SaveSystem/Task_SaveGameToSlot.h"
#include "Kismet/GameplayStatics.h"
#include "FL_Logger.h"

Task_SaveGameToSlot::Task_SaveGameToSlot(USaveGame* _saveGame, FString _saveGameName, FThreadSafeCounter *_RunningTasksCounter) {
    RunningTasksCounter = _RunningTasksCounter;
    saveGame = _saveGame;
    saveGameName = _saveGameName;
}

void Task_SaveGameToSlot::DoWork() const {
    UGameplayStatics::SaveGameToSlot(saveGame, saveGameName, 0);
    RunningTasksCounter->Decrement();
}