#include <Arduino.h>

#include "localization.h"

const Localization localization[ELanguage::eNLanguages] =
{
  {"Shopping List", "New item", "Add", "Shopping List", "Delete selected"},
  {"Einkaufszettel", "Neuer Eintrag", "Hinzuf&uuml;gen", "Einkaufszettel", "Ausgew&auml;hlte l&ouml;schen"},
};

