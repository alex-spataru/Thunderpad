//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "app/app.h"

int main(int argc, char *argv[]) {
    Application app(argc, argv);
    QSharedMemory mem("thunderpad");

    if (mem.create(1) && mem.error() != QSharedMemory::AlreadyExists)
        return app.exec();

    // TODO: Find a way to communicate the app with running instance
    return app.showInitError();
}
