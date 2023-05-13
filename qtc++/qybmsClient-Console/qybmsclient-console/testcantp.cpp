#include "testcantp.h"
#include <iostream>
#include <QDebug>
#include <assert.h>
#include <string>


testCanTp::testCanTp(CANThread *canThr, CAN_TP *canTp)
{
    qDebug() << __FUNCTION__;

    m_canTpLink = canTp;
    m_canthread = canThr;
    assert(m_canTpLink != NULL);
    assert(m_canthread != NULL);

    m_is_started = false;
    m_state = e_tpIdle;
    m_canTpLink->set_callbackApp(this);

    m_timerX = new HRTimer_X();

    m_smt_option = 0;
    m_smt_txNum = 0;
    m_smt_isRunning = false;
    m_smt_txIdx = 0;
    m_stm_timerCBRunning = false;
}

testCanTp::~testCanTp()
{
    qDebug() << __FUNCTION__;
    m_canTpLink = NULL;
    m_canthread = NULL;

    delete m_timerX;
}

int testCanTp::receive_cantp_payload(uint8_t payload[], uint16_t outSize)
{
    m_state = e_tpReplied;

    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "-P";

    //debug only
//    qDebug("##### ISO-TP received a full can-tp payload[%d]:", outSize);
//     for(int i=0; i < outSize; i++)
//     {
//         if(outSize - i >=8)
//         {
//             qDebug("[%02x %02x %02x %02x %02x %02x %02x %02x]",
//                    payload[i], payload[i+1], payload[i+2], payload[i+3],
//                    payload[i+4], payload[i+5], payload[i+6], payload[i+7]);
//             i += 7;
//         }
//         else
//         {
//             qDebug("[%02x]", payload[i]);
//         }
//     }

    return 0;
}

void testCanTp::start_run()
{
    if(!m_is_started)
    {
        m_is_started = true;
        start();
    }

}
void testCanTp::stop_run()
{
    m_is_started = false;

    if(m_timerX != nullptr)
    {
        m_timerX->stop();
        m_timerX = nullptr;
    }
}

QString testCanTp::delta_hhmmss(QTime beginTS)
{
    int delta_ms = QTime::currentTime().msecsSinceStartOfDay() - beginTS.msecsSinceStartOfDay();
    if(delta_ms < 0) delta_ms = 0;
    int delta_s = delta_ms / 1000;
    uint32_t delta_h = delta_s / 60 / 60;
    uint32_t delta_m = (delta_s /60) % 60;
    delta_s = delta_s % 60;
    delta_ms = delta_ms % 1000;
    return QString("%1:%2:%3 %4").arg(delta_h, 2, 10, QLatin1Char('0'))
                              .arg(delta_m, 2, 10, QLatin1Char('0'))
                              .arg(delta_s, 2, 10, QLatin1Char('0'))
                              .arg(delta_ms, 3, 10, QLatin1Char('0'));
}


//qDebug("testCanTp::run() Started!");
uint8_t messages1[8]={0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77}; // req 8, replied 11
uint8_t messages2[11]={0x01, 0x0, 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00}; // req 11, replied 37
uint8_t messages3[11]={0x04, 0xF0, 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x0};// req 11, replied 251

uint8_t messages4[16]={0x02, 0x22, 0x0, 0x22, 0x22, 0x33, 0x44, 0x55,
                       0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD};// req 16, replied 512
uint8_t messages5[512]={0x03, 0x33, 0x0, 0x33, 0x44, 0x55, 0x66, 0x77,};// req 512, replied 16

void testCanTp::timer_CB()
{
    if(!m_stm_timerCBRunning)
    {
        sendMsgTask();
    }
}

void testCanTp::start_sendMsgTask(int msgIdx, int txNum)
{
    m_smt_option = msgIdx;
    m_smt_txNum = txNum;
    m_smt_txIdx = 0;
    m_smt_isRunning = true;

    m_testBegin_ts = QTime::currentTime();
    m_timerX->start(this, 1);
    //qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "sendMsgTask Start !";
}

void testCanTp::sendMsgTask()
{
    m_stm_timerCBRunning = true;
    if(m_smt_option > 0 && m_smt_txNum > 0 && m_smt_isRunning)
    {
        //qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "sendMsgTask";

        if(m_canTpLink->tx_status() == ISOTP_SEND_STATUS_INPROGRESS ||
           m_canTpLink->rx_status() == ISOTP_RECEIVE_STATUS_INPROGRESS ||
           m_state == e_tpReq)
        {
            //waiting ...for the state change
            //qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "-N";
            m_stm_timerCBRunning = false;
            return;
        }

        int ret = ISOTP_RET_OK;
        if(m_smt_txNum > m_smt_txIdx)
        {
            //Try to send ...
            switch(m_smt_option)
            {
            case 1:
                messages1[3] = m_smt_txIdx;
                ret = m_canTpLink->send_a_message(messages1, sizeof(messages1));
                break;
            case 2:
                messages2[3] = m_smt_txIdx;
                ret = m_canTpLink->send_a_message(messages2, sizeof(messages2));
                break;
            case 3:
                messages3[3] = m_smt_txIdx;
                ret = m_canTpLink->send_a_message(messages3, sizeof(messages3));
                break;
            case 4:
                messages4[4] = m_smt_txIdx;
                ret = m_canTpLink->send_a_message(messages4, sizeof(messages4));
                break;
            case 5:
                messages5[3] = m_smt_txIdx;
                for(uint byte=8; byte< sizeof(messages5); byte++)
                {
                    messages5[byte] = (byte & 0xFF);
                }
                ret = m_canTpLink->send_a_message(messages5, sizeof(messages5));
                break;
            default:
                ret = ISOTP_RET_OK;
                break;
            }

            if(ret == ISOTP_RET_OK)
            {
                m_smt_txIdx ++;
                m_state = e_tpReq;
                qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
                         <<": i = " << m_smt_txIdx;
            }
            else
            {
                qDebug("----------- Tx i =  %d NOK ret=%d-----------", m_smt_txIdx, ret);
            }
        }
        else
        {
            //m_timerX->stop();

            //qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "sendMsgTask stop !";
            qDebug("\n####  <<<< It takes %s for sending message %d for %d times >>>>",
                    delta_hhmmss(m_testBegin_ts).toStdString().c_str(),
                    m_smt_option,
                    m_smt_txNum);

            m_smt_option = 0;
            m_smt_txNum = 0;
            m_smt_isRunning = false;
        }
    }

    m_stm_timerCBRunning = false;
}



void testCanTp::run()
{
    std::string optionStr ;
    int option = 0;
    int txNum = 0;

    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "testCanTp::run" ;

    do
    {
        std::cout << std::endl << std::endl;
        std::cout <<"#### Select option index 1|2|3|4|5|100(dump)|101(resetSts)|200(exit):"<<std::endl;
        std::cin >> optionStr;

        option = QString(optionStr.c_str()).toInt();
        if(option == 100)
        {
            m_canTpLink->dump();
        }
        else if(option == 101)
        {
            m_canTpLink->resetSts();
            m_canTpLink->dump();
        }
        else if(option == 200)
        {
            std::cout << "Exit the program ..." << std::endl;
            sleep(3);
            stop_run();
            break;
        }
        else if(option >=1 && option <= 5)// send out test message
        {
            qDebug("#### Selected option to send test message#%d", option);

            std::cout <<"#### Enter Tx Num:"<<std::endl;
            std::cin >> txNum;
            std::cout << "#### Tx Number = "<< txNum <<std::endl;

            if(txNum == 0)
            {
                qDebug("No message is sent as txNum = 0 !");
                continue;
            }

           //start a task to send msg
            start_sendMsgTask(option, txNum);

        }
        else
        {
            qDebug("#### Selected option [%s] is Invalid !", optionStr.c_str());
        }
    }while(m_is_started);


    m_is_started = false;
    //qDebug("testCanTp::run() Exit!");
}

