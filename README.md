# Makeblock Library v3.0

Arduino library for Makeblock electronic modules

This library is version 3.0, we do a lot of modifications in the code structure, It has a very big difference with the 2.0 version. we recommend all Makeblock customers to use this one.

How to use:

1. Download the source from the git https://github.com/Makeblock-official/Makeblock-Libraries

2. copy the makeblock folder to your arduino default library, Your Arduino library folder should now look like this 
   (on Windows): [arduino installation directory]\libraries\makeblock\src

3. Open the Arduino Application. If already open it, you need to restart to see changes.

4. Click "File->Examples". Here are some test program in MakeBlockDrive->example

5. Depending on the type of motherboard you're using, You should modify the header file.
   For example, you are using a mCore, you should change "#include <MeOrion.h>" to "#include <MeMCore.h>"
   Correspondence between the board and the header file is:

 Orion --------  MeOrion.h

 BaseBoard ----  MeBaseBoard.h
 
 mCore --------  MeMCore.h
 
 Shield -------  MeShield.h

