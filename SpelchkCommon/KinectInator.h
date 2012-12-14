/****************************************************************************
*                                                                           *
*  OpenNI 1.x Alpha                                                         *
*  Copyright (C) 2011 PrimeSense Ltd.                                       *
*                                                                           *
*  This file is part of OpenNI.                                             *
*                                                                           *
*  OpenNI is free software: you can redistribute it and/or modify           *
*  it under the terms of the GNU Lesser General Public License as published *
*  by the Free Software Foundation, either version 3 of the License, or     *
*  (at your option) any later version.                                      *
*                                                                           *
*  OpenNI is distributed in the hope that it will be useful,                *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
*  GNU Lesser General Public License for more details.                      *
*                                                                           *
*  You should have received a copy of the GNU Lesser General Public License *
*  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.           *
*                                                                           *
****************************************************************************/

#ifndef __OPENNIFTW
/**************************************************
*  ERIC MCCANN WAS HERE AND TURNED THIS INSIDE OUT.
*  IT WAS ORIGINALY NiSimpleSkeleton.cpp FROM PRIMESENSE SAMPLES
*  now it's abstracted in a class, and runs in parallel with the opengl mainloop
**************************************************/
#define __OPENNIFTW
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <XnCppWrapper.h>

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define SAMPLE_XML_PATH "OpenNIConfig.xml"
#define SAMPLE_XML_PATH_LOCAL "OpenNIConfig.xml"
#define MAX_NUM_USERS 15

namespace TiemSpelchk
{

class Lurn2SpielNub
{
    private:
        //---------------------------------------------------------------------------
        // Globals
        //---------------------------------------------------------------------------
        xn::Context g_Context;
        xn::ScriptNode g_scriptNode;
        xn::DepthGenerator g_DepthGenerator;
        xn::UserGenerator g_UserGenerator;

        XnBool g_bNeedPose = FALSE;
        XnChar g_strPose[20] = "";
        
        void future_thread_func();
        bool needsToSeppuku;
    public
        void Start();
        void Shutdown();
        Lurn2SpielNub();
        ~Lurn2SpielNub();
}
#endif
