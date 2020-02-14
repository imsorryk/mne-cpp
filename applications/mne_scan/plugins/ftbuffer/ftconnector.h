//=============================================================================================================
/**
 * @file     ftconnector.h
 * @author   Gabriel Motta <gbmotta@mgh.harvard.edu>
 * @version  dev
 * @date     February, 2020
 *
 * @section  LICENSE
 *
 * Copyright (C) 2020, Gabriel Motta. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 * the following conditions are met:
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 *       following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 *       the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @brief    Contains the declaration of the Natus class.
 *
 */


#ifndef FTCONNECTOR_H
#define FTCONNECTOR_H

//*************************************************************************************************************
//=============================================================================================================
// QT Includes
//=============================================================================================================

#include <QCoreApplication>
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QtCore/QtPlugin>
#include <QBuffer>
#include <QThread>

//*************************************************************************************************************
//=============================================================================================================
// EIGEN INCLUDES
//=============================================================================================================

#include <Eigen/Core>

//*************************************************************************************************************
//=============================================================================================================
// OTHER INCLUDES
//=============================================================================================================

#include <cstring>

//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE FtBufferToolboxPlugin
//=============================================================================================================

namespace FTBUFFERPLUGIN
{

//*************************************************************************************************************
//=============================================================================================================
// DEFINITIONS
//=============================================================================================================

#define VERSION    (qint16)0x0001

#define PUT_HDR    (qint16)0x0101 /* decimal 257 */
#define PUT_DAT    (qint16)0x0102 /* decimal 258 */
#define PUT_EVT    (qint16)0x0103 /* decimal 259 */
#define PUT_OK     (qint16)0x0104 /* decimal 260 */
#define PUT_ERR    (qint16)0x0105 /* decimal 261 */

#define GET_HDR    (qint16)0x0201 /* decimal 513 */
#define GET_DAT    (qint16)0x0202 /* decimal 514 */
#define GET_EVT    (qint16)0x0203 /* decimal 515 */
#define GET_OK     (qint16)0x0204 /* decimal 516 */
#define GET_ERR    (qint16)0x0205 /* decimal 517 */

#define FLUSH_HDR  (qint16)0x0301 /* decimal 769 */
#define FLUSH_DAT  (qint16)0x0302 /* decimal 770 */
#define FLUSH_EVT  (qint16)0x0303 /* decimal 771 */
#define FLUSH_OK   (qint16)0x0304 /* decimal 772 */
#define FLUSH_ERR  (qint16)0x0305 /* decimal 773 */

#define WAIT_DAT   (qint16)0x0402 /* decimal 1026 */
#define WAIT_OK    (qint16)0x0404 /* decimal 1027 */
#define WAIT_ERR   (qint16)0x0405 /* decimal 1028 */

#define PUT_HDR_NORESPONSE (qint16)0x0501 /* decimal 1281 */
#define PUT_DAT_NORESPONSE (qint16)0x0502 /* decimal 1282 */
#define PUT_EVT_NORESPONSE (qint16)0x0503 /* decimal 1283 */

/* these are used in the data_t and event_t structure */
#define DATATYPE_CHAR    (qint32)0
#define DATATYPE_UINT8   (qint32)1
#define DATATYPE_UINT16  (qint32)2
#define DATATYPE_UINT32  (qint32)3
#define DATATYPE_UINT64  (qint32)4
#define DATATYPE_INT8    (qint32)5
#define DATATYPE_INT16   (qint32)6
#define DATATYPE_INT32   (qint32)7
#define DATATYPE_INT64   (qint32)8
#define DATATYPE_FLOAT32 (qint32)9
#define DATATYPE_FLOAT64 (qint32)10

//*************************************************************************************************************
//=============================================================================================================
// FIELDTRIP MESSAGE STRUCTS
//=============================================================================================================

typedef struct {
    qint32 nchans;
    qint32 nsamples;
    qint32 data_type;
    qint32 bufsize;     /* size of the buffer in bytes */
} datadef_t;

typedef struct {
    qint32  nchans;
    qint32  nsamples;
    qint32  nevents;
    float   fsample;
    qint32  data_type;
    qint32  bufsize;     /* size of the buffer in bytes */
} headerdef_t;

typedef struct {
    qint16 version;   /* see VERSION */
    qint16 command;   /* see PUT_xxx, GET_xxx and FLUSH_xxx */
    qint32 bufsize;   /* size of the buffer in bytes */
} messagedef_t;

typedef struct {
    messagedef_t *def;
    void         *buf;
} message_t;

typedef struct {
    qint32 begsample; /* indexing starts with 0, should be >=0 */
    qint32 endsample; /* indexing starts with 0, should be <header.nsamples */
} datasel_t;

typedef struct {
    qint32 nsamples;
    qint32 nevents;
} samples_events_t;

//*************************************************************************************************************

class FtConnector : public QObject
{
    Q_OBJECT

    friend class FtBufferSetupWidget;

public:

    /**
     * FtConnector - constructor of the FtConnector class. Only initializes variables to zero.
     */
    FtConnector();

    //=========================================================================================================
    /**
     * ~FtConnector - desctructor of the FtConnector class. Disconnects and deletes m_pSocket.
     */
    ~FtConnector();

    //=========================================================================================================
    /**
     * @brief connect - connects to buffer at address m_sAddress and port m_iPort
     * @return true if successful, false if unsuccessful
     */
    bool connect();

    //=========================================================================================================
    /**
     * @brief disconnect - disconnects m_pSocket
     * @return true if successful, false if unsuccessful
     */
    bool disconnect();

    //=========================================================================================================
    /**
     * @brief getHeader - requests and receives header data from buffer, saves relevant parameters internally.
     * @return true if successful, false if unsuccessful
     */
    bool getHeader();

    //=========================================================================================================
    /**
     * @brief getData - requests and receives data from buffer, parses it, and stores it in m_pMatEmit
     * @return true if successful, false if unsuccessful
     */
    bool getData();

    //=========================================================================================================
    /**
     * @brief getAddr - gets address currently stored in m_sAddress
     * @return returns m_sAddress
     */
    QString getAddr();

    //=========================================================================================================
    /**
     * @brief setAddr - sets m_sAddress to a new address
     * @param sNewAddress - a QString with an address (not checked to se if valid)
     * @return true if successful, false if unsuccessful
     */
    bool setAddr(const QString &sNewAddress);

    //=========================================================================================================
    /**
     * @brief getPort - gets port numbr currently stored in m_iPort
     * @return returns m_iPort
     */
    int getPort();

    //=========================================================================================================
    /**
     * @brief setPort - sets m_iPort to a new port number
     * @param iPort - an int with a new desired port number
     * @return true if successful, false if unsuccessful
     */
    bool setPort(const int& iPort);

    //=========================================================================================================
    /**
     * @brief echoStatus - prints relevant class data to terminal. Useful for debugging.
     */
    void echoStatus();

    //=========================================================================================================
    /**
     * @brief getMatrix - returns m_pMatEmit, newest buffer data formatted as an Eigen MatrixXd
     * @return returns m_pMatEmit
     */
    Eigen::MatrixXd getMatrix();

    //=========================================================================================================
    /**
     * @brief newData - returns m_bNewData whether or not new data has been read from buffer
     * @return returns m_bNewData
     */
    bool newData();

    //=========================================================================================================
    /**
     * @brief resetEmitData - sets m_bNewData to false, deletes m_pMatEmit
     */
    void resetEmitData();

private:

    /**
     * @brief sendRequest - sends a formated message to the buffer. command and bufsize must be set before calling.
     * @param messagedef - request structure with the appropriate command and bufszie paramters set.
     */
    void sendRequest(messagedef_t &messagedef);

    //=========================================================================================================
    /**
     * @brief sendDataSel - sends a formated datasel message, for defining the first and last sample we are requesting from the buffer
     * @param datasel - formattd first and last sample index we are requesting from the buffer
     */
    void sendDataSel(datasel_t &datasel);

    //=========================================================================================================
    /**
     * @brief sendSampleEvents - sensds a formated sampleevents message, used for receving updated sample an event numbers from buffer
     * @param threshold - buffer will respond once sample/event numbers reach the thresholds
     */
    void sendSampleEvents(samples_events_t &threshold);

    //=========================================================================================================
    /**
     * @brief parseHeaderDef - parses headerdef message and saves parameters(channels, frequency, datatype, newsamples)
     * @param readBuffer - QBuffer with return headerdef_t data from buffer
     * @return true if successful, false if unsuccessful
     */
    bool parseHeaderDef(QBuffer &readBuffer);

    //=========================================================================================================
    /**
     * @brief parseMessageDef - parses messadef and returns bufsize
     * @param readBuffer - QBuffer with return messagedef_t data from buffer
     * @return returns messagedef_t.bufsize
     */
    int parseMessageDef(QBuffer &readBuffer);

    //=========================================================================================================
    /**
     * @brief parseDataDef - parses datadef and returns bufsize
     * @param dataBuffer - QBuffer with return datadef_t data from buffer
     * @return returns datadef_t.bufsize
     */
    int parseDataDef(QBuffer &dataBuffer);

    //=========================================================================================================
    /**
     * @brief parseData - parses sample data received from buffer, formates it and saves it to m_pMatEmit;
     * @param datasampBuffer - QBuffer with return data from buffer
     * @param bufsize - bufsize of sample data
     * @return true if successful, false if unsuccessful
     */
    bool parseData(QBuffer &datasampBuffer, int bufsize);

    //=========================================================================================================
    /**
     * @brief prepBuffer - Opens Buffer, reads nuBytes from socket and sets index to zero
     * @param buffer - QBuffer to which daa will be written
     * @param numBytes - how many bytes to read from socket
     */
    void prepBuffer(QBuffer &buffer, int numBytes);

    //=========================================================================================================
    /**
     * @brief totalBuffSamples - returns total amount of samples written to buffer
     * @return returns total amount of samples written to buffer
     */
    int totalBuffSamples();

//*************************************************************************************************************

    QTcpSocket*         m_pSocket;                              /**< Socket that manages the connection to the ft buffer */

    QString             m_sAddress          = "127.0.0.1";      /**< Address where the ft buffer is found */
    int                 m_iPort             = 1972;             /**< Port where the ft bufferis found */

    int                 m_iNumChannels;                         /**< Number of channels in the buffer data */
    float               m_fSampleFreq;                          /**< Sampling frequency of data in the buffer */
    int                 m_iDataType;                            /**< Type of data in the buffer */

    int                 m_iNumSamples;                          /**< Number of samples we've read from the buffer */
    int                 m_iNumNewSamples;                       /**< Number of total samples (read and unread) in the buffer */
    int                 m_iMsgSamples;                          /**< Number of samples in the latest buffer transmission receied */

    Eigen::MatrixXd*    m_pMatEmit;                             /**< Container to format data to tansmit to FtBuffProducer */
    bool                m_bNewData;                             /**< Indicate whether we've received new data */
};

}//namespace

#endif // FTCONNECTOR_H
