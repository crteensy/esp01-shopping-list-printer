#ifndef LOCALIZATION_H
#define LOCALIZATION_H

enum ELanguage
{
  eEnglish,
  eGerman,
  eNLanguages
};

struct Localization
{
  const char* htmlPageTitle;
  const char* htmlAddItemLabel;
  const char* htmlAddBtnLabel;
  const char* htmlListTitle;
  const char* htmlDeleteBtnLabel;
};

extern const Localization localization[ELanguage::eNLanguages];

#endif // LOCALIZATION_H
