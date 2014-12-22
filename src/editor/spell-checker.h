//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include <QStringList>
#include <QTextStream>

#include "hunspell.h"
#include "hunspell.hxx"

class SpellChecker {

public:
  SpellChecker(const QString &dictionaryPath);
  ~SpellChecker(void);

  bool spell(const QString &word);
  QStringList suggest(const QString &word);
  void ignoreWord(const QString &word);

private:
  void put_word(const QString &word);
  Hunspell *_hunspell;
  QString _encoding;
  QTextCodec *_codec;
};
