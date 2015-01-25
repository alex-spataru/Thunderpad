//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2015 Alex Spataru <alex_spataru@outlook.com>
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

#include "app/app.h"

/*!
 * \internal
 * Creates the \c {Application} and decides whenever
 * to create a new instance of Thunderpad or send data
 * to the existing instance.
 */

int main (int argc, char *argv[])
{
    Application app (argc, argv);
    QString arguments;

    if (argc != 1)
    {
        for (int i = 0; i < argc; i++)
            arguments = argv[i];
    }

    if (app.isRunning() && app.sendMessage (arguments))
        return 0;

    return app.start (arguments);
}
