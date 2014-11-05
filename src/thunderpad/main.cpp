//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "app/app.h"
#include <qsharedmemory.h>

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 *
 * Creates a new instance of the application and checks
 * for other running instances.
 *
 * If so, the application passes the arguments to the
 * existing application and exits.
 *
 * If that step fails, the application will notify
 * the user of the error and will ask him/her to quit the
 * application or create a new instance of the application.
 */

int main (int argc, char *argv[])
{
    Application app (argc, argv);
    QSharedMemory mem ("thunderpad");

    if (mem.create (1) && mem.error() != QSharedMemory::AlreadyExists)
        return app.exec();

    return mem.attach() ? app.exec() : app.showInitError();
}
