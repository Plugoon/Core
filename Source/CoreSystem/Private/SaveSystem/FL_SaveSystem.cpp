#include "SaveSystem/FL_SaveSystem.h"

#include "AC_Core.h"
#include "SaveSystem/AC_SaveSystem.h"
#include "A_Core.h"
#include "C_Core.h"
#include "EX_InvalidArgument.h"
#include "EX_NotValid.h"
#include "EX_NotUnique.h"
#include "FL_Exception.h"
#include "GM_Core.h"
#include "O_Exception.h"
#include "PC_Core.h"
#include "GI_Core.h"

bool UFL_SaveSystem::IsCoreClass(UObject* source) {
    return (
        Cast<AA_Core>(source) ||
        Cast<AGM_Core>(source) ||
        Cast<APC_Core>(source) ||
        Cast<AC_Core>(source) ||
        Cast<UAC_Core>(source) ||
        Cast<UGI_Core>(source)
        );
}

TScriptInterface<II_SaveSystem> UFL_SaveSystem::GetSaveSystem() {
    AGM_Core* gm = Cast<AGM_Core>(GWorld->GetAuthGameMode());

    if (gm) {
        
        TArray<UAC_SaveSystem*> saveSystems;
        gm->GetComponents<UAC_SaveSystem>(saveSystems);

        if (saveSystems.Num() == 0) {
            throw UFL_Exception::CreateException(UEX_NotValid::StaticClass(), TEXT("No Save System found."), gm);
        } else if (saveSystems.Num() > 1) {
            throw UFL_Exception::CreateException(UEX_NotUnique::StaticClass(), TEXT("Too many Save System found."), gm);
        } else {
            return saveSystems[0];
        }
    } else {
        throw UFL_Exception::CreateException(UEX_InvalidArgument::StaticClass(), TEXT("Game Mode is not of type GM_Core."), GWorld->GetAuthGameMode());
    }
}