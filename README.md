# Makeblock Library v3.22

Arduino Library for Makeblock Electronic Modules

This library is v3.22, we've made a lot of modifications in the code structure. Their is now a very big difference
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
