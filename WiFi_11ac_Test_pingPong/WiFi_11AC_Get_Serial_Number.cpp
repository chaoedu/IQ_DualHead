#include "stdafx.h"
#include "WiFi_11AC_Test.h"
#include "IQmeasure.h"

// This variable is declared in WiFi_Test_Internal.cpp
extern vDUT_ID      g_WiFi_11ac_Dut;
extern TM_ID        g_WiFi_11ac_Test_ID;

using namespace std;

// Input Parameter Container
map<string, WIFI_SETTING_STRUCT> l_11ACgetSerialNumberParamMap;

// Return Value Container 
map<string, WIFI_SETTING_STRUCT> l_11ACgetSerialNumberReturnMap;

struct tagParam
{
} l_11ACgetSerialNumberParam;

struct tagReturn
{
    char SERIAL_NUMBER[MAX_BUFFER_SIZE];         /*!< A string contains serial number generated by DUT control.  */
    char ERROR_MESSAGE[MAX_BUFFER_SIZE];         /*!< A string for error message. */
} l_11ACgetSerialNumberReturn;

#ifndef WIN32
int init11ACGetSerialNumberContainers = Initialize11ACGetSerialNumberContainers();
#endif

//! WiFi_Get_Serial_Number
/*!
 * Input Parameters
 *
 *  - None 
 *
 * Return Values
 *      -# A string contains the generated serial number
 *      -# A string for error message
 *
 * \return 0 No error occurred
 * \return -1 Failed
 */
WIFI_11AC_TEST_API int WIFI_11AC_Get_Serial_Number(void)
{
    int  err = ERR_OK;
    int  dummyValue = 0;
	char vErrorMsg[MAX_BUFFER_SIZE]  = {'\0'};
	char logMessage[MAX_BUFFER_SIZE] = {'\0'};

    /*---------------------------------------*
     * Clear Return Parameters and Container *
     *---------------------------------------*/
	ClearReturnParameters(l_11ACgetSerialNumberReturnMap);

    /*------------------------*
     * Respond to QUERY_INPUT *
     *------------------------*/
    err = TM_GetIntegerParameter(g_WiFi_11ac_Test_ID, "QUERY_INPUT", &dummyValue);
    if( ERR_OK==err )
    {
        RespondToQueryInput(l_11ACgetSerialNumberParamMap);
        return err;
    }
	else
	{
		// do nothing
	}

    /*-------------------------*
     * Respond to QUERY_RETURN *
     *-------------------------*/
    err = TM_GetIntegerParameter(g_WiFi_11ac_Test_ID, "QUERY_RETURN", &dummyValue);
    if( ERR_OK==err )
    {
        RespondToQueryReturn(l_11ACgetSerialNumberReturnMap);
        return err;
    }
	else
	{
		// do nothing
	}

	try
	{
	   /*-----------------------------------------------------------*
		* Both g_WiFi_11ac_Test_ID and g_WiFi_11ac_Dut need to be valid (>=0) *
		*-----------------------------------------------------------*/
		if( g_WiFi_11ac_Test_ID<0 || g_WiFi_11ac_Dut<0 )  
		{
			err = -1;
			LogReturnMessage(logMessage, MAX_BUFFER_SIZE, LOGGER_ERROR, "[WiFi_11AC] WiFi_Test_ID or WiFi_Dut not valid.\n");
			throw logMessage;
		}
		else
		{
			LogReturnMessage(logMessage, MAX_BUFFER_SIZE, LOGGER_INFORMATION, "[WiFi_11AC] WiFi_Test_ID = %d and WiFi_Dut = %d.\n", g_WiFi_11ac_Test_ID, g_WiFi_11ac_Dut);
		}

		TM_ClearReturns(g_WiFi_11ac_Test_ID);

		/*----------------------*
		 * Get input parameters *
		 *----------------------*/
		err = GetInputParameters(l_11ACgetSerialNumberParamMap);
		if ( ERR_OK!=err )
		{
			LogReturnMessage(logMessage, MAX_BUFFER_SIZE, LOGGER_ERROR, "[WiFi_11AC] Input parameters are not complete.\n");
			throw logMessage;
		}
		else
		{
			LogReturnMessage(logMessage, MAX_BUFFER_SIZE, LOGGER_INFORMATION, "[WiFi_11AC] Get input parameters return OK.\n");
		}

		// And clear vDut parameters at beginning.
		vDUT_ClearParameters(g_WiFi_11ac_Dut);
	    
		err = vDUT_Run(g_WiFi_11ac_Dut, "GET_SERIAL_NUMBER");		
		if ( ERR_OK!=err )
		{	// Check vDut return "ERROR_MESSAGE" or not, if "Yes", must handle it.
			err = ::vDUT_GetStringReturn(g_WiFi_11ac_Dut, "ERROR_MESSAGE", vErrorMsg, MAX_BUFFER_SIZE);
			if ( ERR_OK==err )	// Get "ERROR_MESSAGE" from vDut
			{
				err = -1;	// set err to -1, means "Error".
				LogReturnMessage(logMessage, MAX_BUFFER_SIZE, LOGGER_ERROR, vErrorMsg);
				throw logMessage;
			}
			else	// Just return normal error message in this case
			{
				LogReturnMessage(logMessage, MAX_BUFFER_SIZE, LOGGER_ERROR, "[WiFi_11AC] vDUT_Run(GET_SERIAL_NUMBER) return error.\n");
				throw logMessage;
			}
		}
		else
		{
			LogReturnMessage(logMessage, MAX_BUFFER_SIZE, LOGGER_INFORMATION, "[WiFi_11AC] vDUT_Run(GET_SERIAL_NUMBER) return OK.\n");
		}

		// TODO: Example, get Return parameters here
		err = ::vDUT_GetStringReturn(g_WiFi_11ac_Dut, "SERIAL_NUMBER", l_11ACgetSerialNumberReturn.SERIAL_NUMBER, MAX_BUFFER_SIZE);		
		if ( ERR_OK!=err )
		{
			err = ERR_OK;	// This is an optional return parameter, thus always return OK
			sprintf_s(l_11ACgetSerialNumberReturn.SERIAL_NUMBER, MAX_BUFFER_SIZE, "");
		}
		else
		{
			LogReturnMessage(logMessage, MAX_BUFFER_SIZE, LOGGER_INFORMATION, "[WiFi_11AC] vDUT_GetStringReturn(SERIAL_NUMBER) return OK.\n");
		}

		/*-----------------------*
		 *  Return Test Results  *
		 *-----------------------*/
		if (ERR_OK==err)
		{
			sprintf_s(l_11ACgetSerialNumberReturn.ERROR_MESSAGE, MAX_BUFFER_SIZE, "[Info] Function completed.\n");
			ReturnTestResults(l_11ACgetSerialNumberReturnMap);
		}
		else
		{
			// do nothing
		}
	}
	catch(char *msg)
    {
        ReturnErrorMessage(l_11ACgetSerialNumberReturn.ERROR_MESSAGE, msg);
    }
    catch(...)
    {
		ReturnErrorMessage(l_11ACgetSerialNumberReturn.ERROR_MESSAGE, "[WiFi_11AC] Unknown Error!\n");
		err = -1;
    }

    return err;
}

int Initialize11ACGetSerialNumberContainers(void)
{
    /*------------------*
     * Input Parameters: *
     *------------------*/
    l_11ACgetSerialNumberParamMap.clear();

    WIFI_SETTING_STRUCT setting;
    
    /*----------------*
     * Return Values: *
     * ERROR_MESSAGE  *
     *----------------*/
    l_11ACgetSerialNumberReturnMap.clear();

    l_11ACgetSerialNumberReturn.SERIAL_NUMBER[0] = '\0';
    setting.type = WIFI_SETTING_TYPE_STRING;
    if (MAX_BUFFER_SIZE==sizeof(l_11ACgetSerialNumberReturn.SERIAL_NUMBER))    // Type_Checking
    {
        setting.value       = (void*)l_11ACgetSerialNumberReturn.SERIAL_NUMBER;
        setting.unit        = "";
        setting.helpText    = "Serial number generated by DUT.";
        l_11ACgetSerialNumberReturnMap.insert( pair<string,WIFI_SETTING_STRUCT>("SERIAL_NUMBER", setting) );
    }
    else    
    {
        printf("Parameter Type Error!\n");
        exit(1);
    }

    l_11ACgetSerialNumberReturn.ERROR_MESSAGE[0] = '\0';
    setting.type = WIFI_SETTING_TYPE_STRING;
    if (MAX_BUFFER_SIZE==sizeof(l_11ACgetSerialNumberReturn.ERROR_MESSAGE))    // Type_Checking
    {
        setting.value       = (void*)l_11ACgetSerialNumberReturn.ERROR_MESSAGE;
        setting.unit        = "";
        setting.helpText    = "Error message occurred";
        l_11ACgetSerialNumberReturnMap.insert( pair<string,WIFI_SETTING_STRUCT>("ERROR_MESSAGE", setting) );
    }
    else    
    {
        printf("Parameter Type Error!\n");
        exit(1);
    }

    return 0;
}

