#pragma once

#include "Async/AsyncWork.h"
#include "GameFramework/SaveGame.h"

class CORESYSTEM_API Task_SaveGameToSlot: public FNonAbandonableTask {
    //friend  class FAsyncTask<Task_SaveGameToSlot>;
    friend class FAutoDeleteAsyncTask<Task_SaveGameToSlot>;

public:
    /**
    * \brief Constructor
    * \param _saveGame SaveGame Object
    * \param _saveGameName Path where to save the SaveGame Object
    * \param RunningTasksCounter Task Counter which will be decremented after saving is done
    */
    Task_SaveGameToSlot(USaveGame* _saveGame, FString _saveGameName, FThreadSafeCounter *RunningTasksCounter);
    
    // this function is executed when we tell our task to execute
    void DoWork() const;

    /* This function is needed from the API of the engine. 
    My guess is that it provides necessary information
    about the thread that we occupy and the progress of our task */
    FORCEINLINE TStatId GetStatId() const {
        RETURN_QUICK_DECLARE_CYCLE_STAT(AsyncCalcPrime, STATGROUP_ThreadPoolAsyncTasks);
    }

private:
    USaveGame* saveGame;
    FString saveGameName;
    FThreadSafeCounter *RunningTasksCounter;
};