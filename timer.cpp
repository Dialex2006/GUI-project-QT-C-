#include "timer.h"
#include<unistd.h>
#include "wirpi.h"

#include <QDebug>
#include <cmath>


timer::timer(QObject *parent) : QObject(parent)
{

    QObject::connect(timer2, &QTimer::timeout, this, &timer::timer_tick);
    QObject::connect(timerToSpeedUpClockSetting, &QTimer::timeout, this, &timer::speeding);
    QObject::connect(timerToSpeedUpAlarmSettingUp, &QTimer::timeout, this, &timer::speedAlarmUp);
    QObject::connect(timerToSpeedUpAlarmSettingDown, &QTimer::timeout, this, &timer::speedAlarmDown);
    QObject::connect(timerForSnooze, &QTimer::timeout, this, &timer::repeatAlarmAfterSnooze);
    QObject::connect(timerToSetVolume, &QTimer::timeout, this, &timer::stopSound);
    QObject::connect(timerToSetColors, &QTimer::timeout, this, &timer::checkButtonReading);



    timerToSetVolume->setSingleShot(true);
    alarm1sound.setSource(QUrl::fromLocalFile("/home/pi/Desktop/QT/projectV2copy_v1/sound/alarm1.wav"));
    //effect.setLoopCount(QSoundEffect::Infinite);
    alarm1sound.setLoopCount(-2);
    alarm1sound.setVolume(1.25f);
    //alarm1sound.play();

    alarm2sound.setSource(QUrl::fromLocalFile("/home/pi/Desktop/QT/projectV2copy_v1/sound/alarm1.wav"));
    //effect.setLoopCount(QSoundEffect::Infinite);
    alarm2sound.setLoopCount(-2);
    alarm2sound.setVolume(1.25f);

}


void timer::startTimer() {

    timer2->start(1000);
}

void timer::stopTimer() {


    timer2->stop();
}




//main function triggered every second
void timer::timer_tick(void)
{


        m_counter_sec++;
        if (m_counter_sec == 60) {
            m_counter_sec = 0;
            m_counter_min++;
            if (m_counter_min == 60 ) {
                m_counter_min = 0;
                m_counter_hrs++;
            }

        }


        //checking if alarm needs to be fired up or not
        if (m_alarm1Enabled)
        {
            //qDebug() << " Alarm1 is playing: " << alarm1sound.isPlaying() << " + " << alarm2sound.isPlaying();
            if (m_counter_hrs == m_alarm1Hours && m_counter_min == m_alarm1Minutes && m_counter_sec == 0)
            {
                qDebug() << " Start Playing Alarm1 NOW ";
                alarm1sound.stop();
                alarm1sound.play();
            }
        }

        if (m_alarm2Enabled)
        {
            qDebug() << " Alarm is playing: " << alarm1sound.isPlaying() << " + " << alarm2sound.isPlaying();
            if (m_counter_hrs == m_alarm2Hours && m_counter_min == m_alarm2Minutes && m_counter_sec == 0)
            {
                qDebug() << " Start Playing Alarm2 NOW ";
                alarm2sound.play();
            }
        }


        //sending the content for SetAlarmMode
        if (m_alarmState == 1) {
            //qDebug() << " About to send HOURS command, mode: " << m_alarmState;
            emit sendAlarm1Hours(QString("%1").arg(m_alarm1Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm1Minutes, 2, 10, QChar('0')));
        }
        else if (m_alarmState == 2) {
            emit sendAlarm1Minutes(QString("%1").arg(m_alarm1Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm1Minutes, 2, 10, QChar('0')));
        }
        else if (m_alarmState == 3) {
            convertAlarmStateIntoStrings(1);
        }
        else if (m_alarmState == 4) {
            emit sendAlarm1Hours(QString("%1").arg(m_alarm2Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm2Minutes, 2, 10, QChar('0')));
        }
        else if (m_alarmState == 5) {
            emit sendAlarm1Minutes(QString("%1").arg(m_alarm2Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm2Minutes, 2, 10, QChar('0')));
        }
        else if (m_alarmState == 6) {
            convertAlarmStateIntoStrings(2);
        }

        else {

            if (m_mode24hrs || m_counter_hrs < 13) {
                emit sendSec(QString("%1").arg(m_counter_sec, 2, 10, QChar('0')), QString("%1").arg(m_counter_min, 2, 10, QChar('0')), QString("%1").arg(m_counter_hrs, 2, 10, QChar('0')), QString("%1").arg(m_snooze, 2, 10, QChar('0')));
            }
            else {
                emit sendSec(QString("%1").arg(m_counter_sec, 2, 10, QChar('0')), QString("%1").arg(m_counter_min, 2, 10, QChar('0')), QString("%1").arg(m_counter_hrs-12, 2, 10, QChar('0')), QString("%1").arg(m_snooze, 2, 10, QChar('0')));
            }

        }

        //qDebug() << "Pressed: " << button_val;
        emit sendVoltage(gaug_val1, gaug_val2);


}






void timer::handleSetButtonPress() {

    if (m_screenState > 0 && m_screenState < 4) {
        setScreenState(m_screenState+1);
        //qDebug() << " SET mode changed ";
    }
    else {
        timerToSetColors->stop();
        setScreenState(0);
        //qDebug() << " SET mode back to default ";

    }

}






void timer::handleVolumeButton() {

    setVolumeState(!m_volumeState);

}





void timer::speedUpClockSetting(bool hoursType, bool increaseType)  {



    this->hoursType = hoursType;
    this->increaseType = increaseType;
    qDebug() << " Timer for long press ";
    timerToSpeedUpClockSetting->start(150);

}


//handle UP button when in Alarm Setting Mode
void timer::handleAlarmSettingUp()
{

    //enabling/disabling alarm1 and alarm2
    if (m_alarmState == 3)
    {
        setAlarm1Enabled(!m_alarm1Enabled);
    }
    else if (m_alarmState == 6)
    {
        setAlarm2Enabled(!m_alarm2Enabled);
    }

    else
    {

        if (m_alarmState == 1)
        {
            setAlarm1Hours(m_alarm1Hours+1);

        }
        else if (m_alarmState == 2)
        {
            setAlarm1Minutes(m_alarm1Minutes+1);
        }
        else if (m_alarmState == 4)
        {
            setAlarm2Hours(m_alarm2Hours+1);

        }
        else if (m_alarmState == 5)
        {
            setAlarm2Minutes(m_alarm2Minutes+1);
        }


        timerToSpeedUpAlarmSettingUp->start(100);

    }

}



//handle DOWN button when in Alarm Setting Mode
void timer::handleAlarmSettingDown()
{

    //enabling/disabling alarm1 and alarm2
    if (m_alarmState == 3)
    {
        setAlarm1Enabled(!m_alarm1Enabled);
    }
    else if (m_alarmState == 6)
    {
        setAlarm2Enabled(!m_alarm2Enabled);
    }

    else
    {
        if (m_alarmState == 1)
        {
            setAlarm1Hours(m_alarm1Hours-1);
        }
        else if (m_alarmState == 2)
        {
            setAlarm1Minutes(m_alarm1Minutes-1);
        }
        else if (m_alarmState == 4)
        {
            setAlarm2Hours(m_alarm2Hours-1);
        }
        else if (m_alarmState == 5)
        {
            setAlarm2Minutes(m_alarm2Minutes-1);
        }


        timerToSpeedUpAlarmSettingDown->start(100);
    }

}



//accelerated change UP of ALARM settings: hours and minutes
void timer::speedAlarmUp()  {


    if (m_alarmState == 1)
    {
        setAlarm1Hours(m_alarm1Hours+1);
    }
    else if (m_alarmState == 2)
    {
        setAlarm1Minutes(m_alarm1Minutes+1);
    }
    else if (m_alarmState == 4)
    {
        setAlarm2Hours(m_alarm2Hours+1);
    }
    else if (m_alarmState == 5)
    {
        setAlarm2Minutes(m_alarm2Minutes+1);
    }


}




//accelerated change DOWN of ALARM settings: hours and minutes
void timer::speedAlarmDown()  {


    if (m_alarmState == 1)
    {
        setAlarm1Hours(m_alarm1Hours-1);
    }
    else if (m_alarmState == 2)
    {
        setAlarm1Minutes(m_alarm1Minutes-1);
    }
    else if (m_alarmState == 4)
    {
        setAlarm2Hours(m_alarm2Hours-1);
    }
    else if (m_alarmState == 5)
    {
        setAlarm2Minutes(m_alarm2Minutes-1);
    }


}






//accelerated change of CLOCK settings: hours and minutes

void timer::speeding()  {

    if (hoursType && increaseType) {
        setCounter_hrs(m_counter_hrs+1);
    }
    else if (!hoursType && increaseType) {
        setCounter_min(m_counter_min+1);

    }
    else if (hoursType && !increaseType) {
        setCounter_hrs(m_counter_hrs-1);

    }
    else if (!hoursType && !increaseType) {
        setCounter_min(m_counter_min-1);

    }

}




void timer::longPressDisabled()  {

    if (m_screenState !=0)
        timerToSpeedUpClockSetting->stop();
    else if (alarm1SettingModeEnabled)
    {
        timerToSpeedUpAlarmSettingUp->stop();
        timerToSpeedUpAlarmSettingDown->stop();
    }

}



int timer::screenState()
{
    return m_screenState;
}


QString timer::colorBack()
{
    return curr_colorBack;
}

QString timer::colorFont()
{
    return curr_colorFont;
}

QString timer::colorSeconds()
{
    return curr_colorSeconds;
}

QString timer::colorButtons()
{
    return curr_colorButtons;
}



int timer::counter_hrs()
{
    return m_counter_hrs;
}



int timer::counter_min()
{
    return m_counter_min;
}



int timer::counter_snooze()
{
    return m_snooze;
}


int timer::alarm1Hours()
{
    return m_alarm1Hours;
}


int timer::alarm1Minutes()
{
    return m_alarm1Minutes;
}


int timer::alarm2Hours()
{
    return m_alarm2Hours;
}


int timer::alarm2Minutes()
{
    return m_alarm2Minutes;
}



bool timer::mode24hrs()
{
    return m_mode24hrs;
}



int timer::alarmState()
{
    return m_alarmState;
}


bool timer::volumeState()
{
    return m_volumeState;
}




void timer::setVolumeState(bool volume)
{

    m_volumeState = volume;
    qDebug() << " Volume MODE: " << m_volumeState;
}


void timer::setScreenState(int stateNumber)
{
    m_screenState = stateNumber;
    //qDebug() << " Alarm mode: " << stateNumber;
    emit screenStateChanged();
}





void timer::setMode24hrs(bool mode)
{
    m_mode24hrs = mode;
    if (mode) {hoursModeString = "24";}
    else {hoursModeString = "12";}

    emit mode24hrsChanged(hoursModeString);
}



void timer::enterSetAlarmMode()
{
    alarm1SettingModeEnabled = true;
    if (m_alarmState < 6) {m_alarmState++;}
    else {m_alarmState=0;}



    qDebug() << " Alarm mode: " << m_alarmState;
    if (m_alarmState == 1)
    {
        emit sendAlarm1Hours(QString("%1").arg(m_alarm1Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm1Minutes, 2, 10, QChar('0')));
    }
    else if (m_alarmState == 2)
    {
        emit sendAlarm1Minutes(QString("%1").arg(m_alarm1Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm1Minutes, 2, 10, QChar('0')));
    }
    else if (m_alarmState == 3) {
        convertAlarmStateIntoStrings(1);
    }
    else if (m_alarmState == 4)
    {
        emit sendAlarm2Hours(QString("%1").arg(m_alarm2Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm2Minutes, 2, 10, QChar('0')));
    }
    else if (m_alarmState == 5)
    {
        emit sendAlarm2Minutes(QString("%1").arg(m_alarm2Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm2Minutes, 2, 10, QChar('0')));
    }
    else if (m_alarmState == 6) {
        convertAlarmStateIntoStrings(2);
    }





}



void timer::setCounter_hrs(int hours)
{
    if (hours == 24) {
        hours = 0;
    }
    else if (hours == -1) {
        hours = 23;
    }
    m_counter_hrs = hours;


    if (m_mode24hrs || m_counter_hrs < 13) { emit sendHrs(QString("%1").arg(m_counter_hrs, 2, 10, QChar('0')));}
    else { emit sendHrs(QString("%1").arg(m_counter_hrs-12, 2, 10, QChar('0'))); }
}



void timer::setCounter_min(int minutes)
{

    if (minutes == 60) {
        minutes = 0;
    }
    else if (minutes == -1) {
        minutes = 59;
    }
    m_counter_min = minutes;

    emit sendMin(QString("%1").arg(m_counter_min, 2, 10, QChar('0')));
}




void timer::setCounter_snooze(int snooze)
{

    if (snooze == 61) {
        snooze = 5;
    }
    else if (snooze == 4) {
        snooze = 60;
    }
    m_snooze = snooze;

    emit sendSnooze(QString("%1").arg(m_snooze, 2, 10, QChar('0')));
}




void timer::setAlarm1Hours(int hours)
{

    if (hours == 24) {
        hours = 0;
    }
    else if (hours == -1) {
        hours = 23;
    }
    m_alarm1Hours = hours;

    emit sendAlarm1Hours(QString("%1").arg(m_alarm1Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm1Minutes, 2, 10, QChar('0')));

}



void timer::setAlarm1Minutes(int minutes)
{

    if (minutes == 60) {
        minutes = 0;
    }
    else if (minutes == -1) {
        minutes = 59;
    }
    m_alarm1Minutes = minutes;

    emit sendAlarm1Minutes(QString("%1").arg(m_alarm1Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm1Minutes, 2, 10, QChar('0')));

}



void timer::setAlarm2Hours(int hours)
{

    if (hours == 24) {
        hours = 0;
    }
    else if (hours == -1) {
        hours = 23;
    }
    m_alarm2Hours = hours;

    emit sendAlarm1Hours(QString("%1").arg(m_alarm2Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm2Minutes, 2, 10, QChar('0')));

}



void timer::setAlarm2Minutes(int minutes)
{

    if (minutes == 60) {
        minutes = 0;
    }
    else if (minutes == -1) {
        minutes = 59;
    }
    m_alarm2Minutes = minutes;

    emit sendAlarm1Minutes(QString("%1").arg(m_alarm2Hours, 2, 10, QChar('0')), QString("%1").arg(m_alarm2Minutes, 2, 10, QChar('0')));

}







void timer::setAlarmState(int alarmNumber)
{

    m_alarmState = alarmNumber;

}



bool timer::alarm1Enabled()
{
    return m_alarm1Enabled;
}


bool timer::alarm2Enabled()
{
    return m_alarm2Enabled;
}


void timer::setAlarm1Enabled(bool enabled)
{
    m_alarm1Enabled = enabled;
    convertAlarmStateIntoStrings(1);

    emit alarm1EnabledChanged();
}


void timer::setAlarm2Enabled(bool enabled)
{
    m_alarm2Enabled = enabled;
    qDebug() << " Alarm 2 state: " << m_alarm2Enabled;
    convertAlarmStateIntoStrings(2);
    emit alarm2EnabledChanged();
}



void timer::convertAlarmStateIntoStrings(int num)
{

    //num = 1 for Alarm1; num = 2 for Alarm2
    if (num == 1)
    {
        if (m_alarm1Enabled) {
            emit sendAlarm1State("", "ON");
        }
        else {
            emit sendAlarm1State(" O", "FF");

        }
    }
    else
    {
            if (m_alarm2Enabled) {
                emit sendAlarm2State("", "ON");
            }
            else {
                emit sendAlarm2State(" O", "FF");
            }

    }

}



void timer::handleSleepButton()
{
    if (alarm1sound.isPlaying())
    {
        alarm1sound.stop();
    }
    else if (alarm2sound.isPlaying())
    {
        alarm2sound.stop();
    }
}


void timer::handleSnoozeButton()
{
    if (alarm1sound.isPlaying())
    {
        alarm1sound.stop();
        timerForSnooze->start(m_snooze*60000);
        alarmNumSnooze = 1;
    }
    else if (alarm2sound.isPlaying())
    {
        alarm2sound.stop();
        timerForSnooze->start(m_snooze*60000);
        alarmNumSnooze = 2;
    }
}


void timer::repeatAlarmAfterSnooze()
{
    if (alarmNumSnooze == 1)
    {
        alarm1sound.play();
    }
    else if (alarmNumSnooze == 2)
    {
        alarm2sound.play();
    }

}



void timer::decreaseVolume()
{
    if (volumeLevel > 1) volumeLevel--;
    emit sendVolumeLevel(volumeLevel);

    alarm1sound.setVolume(float(volumeLevel)/float(5));
    alarm2sound.setVolume(float(volumeLevel)/float(5));
    alarm1sound.play();
    timerToSetVolume->start(600);

}




void timer::increaseVolume()
{
    qDebug() << " Increase: " << volumeLevel;
    if (volumeLevel < 5) volumeLevel++;

    //alarm1sound.setLoopCount(1);
    alarm1sound.setVolume(float(volumeLevel)/float(5));
    alarm2sound.setVolume(float(volumeLevel)/float(5));
    alarm1sound.play();
    timerToSetVolume->start(600);

    emit sendVolumeLevel(volumeLevel);

}





void timer::stopSound()
{

    qDebug() << " Stopping Alarm1 ";
    alarm1sound.stop();

}




void timer::startButtonReading()
{
    timerToSetColors->start(200);
    qDebug() << "Button timer started ";
}


void timer::stopButtonReading()
{
    timerToSetColors->stop();
    qDebug() << "Button timer stopped ";
}


void timer::checkButtonReading()
{

    gaug_val1=wirpi.Read_sensor0()*100.0/800000/3.3;
    gaug_val1=wirpi.Read_sensor0()/8000.00/3.3;
    qDebug() << "Button press value: " << gaug_val1;
    if (gaug_val1 > 0.85)
    {
        timerToSetColors->stop();
        m_colorSchemeNum++;
        if (m_colorSchemeNum > 4) m_colorSchemeNum = 0;
        //curr_colorBack = m_colorBack[m_colorSchemeNum];
        setColorBack(m_colorBack[m_colorSchemeNum]);
        setColorFont(m_colorFont[m_colorSchemeNum]);
        setColorSeconds(m_colorSeconds[m_colorSchemeNum]);
        setColorButtons(m_colorButtons[m_colorSchemeNum]);
        qDebug() << "Font color: " << curr_colorFont;
        emit colorBackChanged();
        timerToSetColors->start(3000);
        //timerToSetColors->setSingleShot(true);
    }
    else timerToSetColors->start(200);

    gaug_val1 = 0;

}


void timer::setColorBack(QString num)
{
    curr_colorBack = num;

}

void timer::setColorFont(QString num)
{
    curr_colorFont = num;

}


void timer::setColorSeconds(QString num)
{
    curr_colorSeconds = num;

}

void timer::setColorButtons(QString num)
{
    curr_colorButtons = num;

}
