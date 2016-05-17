# Makeblock Library v3.23

Revision of history:
Author           Time           Version          Descr
Mark Yan         2015/07/24     3.0.0            Rebuild the old lib.
Rafael Lee       2015/09/02     3.1.0            Added some comments and macros.
Lawrence         2015/09/09     3.2.0            Include some Arduino's official headfiles which path specified.
Mark Yan         2015/11/02     3.2.1            fix bug on MACOS.
Mark Yan         2016/01/21     3.2.2            fix some library bugs.
Mark Yan         2016/05/17     3.2.3            add support for MegaPi and Auriga Board.

Arduino Library for Makeblock Electronic Modules

This library is v3.23, we've made a lot of modifications in the code structure. Their is now a very big difference
from v2.0. We strongly recommend that all Makeblock customers use the new version.

How to use:

1. Download the source from the git https://github.com/Makeblock-official/Makeblock-Libraries

2. copy the makeblock folder to your arduino default library. Your Arduino library folder should now look like this 
   (on Windows): [arduino installation directory]\libraries\makeblock\src
   (On MACOS): [arduino Package Contents]\contents\Jave\libraries\makeblock\src

3. Open the Arduino Application. (If it's already open, you will need to restart it to see changes.)

4. Click "File-> Examples". Here are some test programs in MakeBlockDrive-> example

5. Depending on the type of board you're using, you need to modify the header file to match.
   For example, if you're using a mCore. You should change "#include <MeOrion.h>" to "#include <MeMCore.h>"
   Corresponding boards and there header file are:

   Orion <-------->  MeOrion.h

   BaseBoard <---->  MeBaseBoard.h

   mCore <-------->  MeMCore.h

   Shield <------->  MeShield.h

   Auriga <------->  MeAuriga.h

   MegaPi <------->  MeMegaPi.h
