/*******************************************************************************
* \file main.h
* \date 03.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief The main entry point of this application.
*******************************************************************************/

#if !defined(__MAIN_H)
#define __MAIN_H

#define WELCOMEMSG_LINE_0 "   ___                     _                       _                         _ "
#define WELCOMEMSG_LINE_1 "  / __\\ ___ _ __ _ __   __| | ___   /\\ /\\___ _   _| |__   ___   __ _ _ __ __| |"
#define WELCOMEMSG_LINE_2 " /__\\/// _ \\ '__| '_ \\ / _` |/ _ \\ / //_/ _ \\ | | | '_ \\ / _ \\ / _` | '__/ _` |"
#define WELCOMEMSG_LINE_3 "/ \\/  \\  __/ |  | | | | (_| | (_) / __ \\  __/ |_| | |_) | (_) | (_| | | | (_| |"
#define WELCOMEMSG_LINE_4 "\\_____/\\___|_|  |_| |_|\\__,_|\\___/\\/  \\/\\___|\\__, |_.__/ \\___/ \\__,_|_|  \\__,_|"
#define WELCOMEMSG_LINE_5 "                                             |___/                             "

/**
 * \brief Main function entry point of this application. This function is called
 *        by the startup assembly file startup_stm32f103xe.s.
 * \return Status code.
 */
int main(void);

#endif // __MAIN_H