//
// Created by aren on 28/10/21.
//

#include "GasMenu.h"

void SwitchGasCallback(MenuItem &item) {
    for (int i = 0; i < GasList->size(); i++) {
        Gas gas = (*GasList)[i];
        char *gasStr = gas.toString();

        MenuItem(gasStr, GasMenuCallback)

        GasMenu.Items[i] = &GasItemsActual[i];
    }
    CurrUIState.Menu = &GasMenu;
    CurrUIState.ClearNeeded = true;
}

void GasMenuCallback(MenuItem &item) {
    double FrO2, FrHe;

    sscanf(item.GetText(), "%lf/%lf", &FrO2, &FrHe);

    for (int i = 0; i < DecoActual.Gases.size(); i++) {
        if (DecoActual.Gases[i].FrO2 * 100 == FrO2 && DecoActual.Gases[i].FrHe == FrHe) {
            DecoActual.SwitchGas(i);
        }
    }

    for (int i = 0; i < GasMenu.ItemsSize; i++) {
        free((char *) GasMenu.Items[i]->MenuText);
    }
    RestoreMenu();
}