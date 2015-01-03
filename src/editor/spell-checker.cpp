//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex_spataru@outlook.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301
//  USA
//

#include "spell-checker.h"

SpellChecker::SpellChecker(const QString &dictionaryPath) {
    QString dictFile = dictionaryPath + ".dic";
    QString affixFile = dictionaryPath + ".aff";
    QByteArray dictFilePathBA = dictFile.toLocal8Bit();
    QByteArray affixFilePathBA = affixFile.toLocal8Bit();
    _hunspell = new Hunspell(affixFilePathBA.constData(), dictFilePathBA.constData());

    // Detect encoding analyzing the SET option in the affix file
    _encoding = "ISO8859-1";
    QFile _affixFile(affixFile);

    if (_affixFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&_affixFile);
        QRegExp enc_detector("^\\s*SET\\s+([A-Z0-9\\-]+)\\s*", Qt::CaseInsensitive);

        for (QString line = stream.readLine(); !line.isEmpty();
             line = stream.readLine()) {
            if (enc_detector.indexIn(line) > -1) {
                _encoding = enc_detector.cap(1);
                break;
            }
        }

        _affixFile.close();
    }

    _codec = QTextCodec::codecForName(this->_encoding.toLatin1().constData());


}

SpellChecker::~SpellChecker(void) {
    delete _hunspell;
}

bool SpellChecker::spell(const QString &word) {
    return _hunspell->spell(_codec->fromUnicode(word).constData()) != 0;
}

QStringList SpellChecker::suggest(const QString &word) {
    char **suggestWordList;

    // Encode from Unicode to the encoding used by current dictionary
    int numSuggestions = _hunspell->suggest(
                &suggestWordList, _codec->fromUnicode(word).constData());
    QStringList suggestions;

    for (int i = 0; i < numSuggestions; ++i) {
        suggestions << _codec->toUnicode(suggestWordList[i]);
        free(suggestWordList[i]);
    }

    return suggestions;
}

void SpellChecker::ignoreWord(const QString &word) {
    put_word(word);
}

void SpellChecker::put_word(const QString &word) {
    _hunspell->add(_codec->fromUnicode(word).constData());
}
