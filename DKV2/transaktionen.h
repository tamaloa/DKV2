#ifndef TRANSAKTIONEN_H
#define TRANSAKTIONEN_H

#include "vertrag.h"

bool vertragBeenden(int vid);
bool VertragsEnde_LaufzeitEnde( Vertrag& v);
bool VertragsEnde_MitKFrist( Vertrag& v);
bool VertragsEnde_PassiverV( Vertrag& v);

#endif // TRANSAKTIONEN_H