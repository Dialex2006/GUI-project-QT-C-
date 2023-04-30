#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>
#include <QSoundEffect>

#include "wirpi.h"

class timer : public QObject
{
    Q_OBJECT
    bool setClockFlag = false;
    const int BUTTON_PIN = 27;
    float gaug_val1=0;
    float gaug_val2=0;
    QTimer *timer2 = new QTimer(this);
    QTimer *timerToSpeedUpClockSetting = new QTimer(this);
    QTimer *timerToSpeedUpAlarmSettingUp = new QTimer(this);
    QTimer *timerToSpeedUpAlarmSettingDown = new QTimer(this);
    QTimer *timerForSnooze = new QTimer(this);
    QTimer *timerToSetVolume = new QTimer(this);
    QTimer *timerToSetColors = new QTimer(this);
    // screenState determines what to show on the screen (0- clock by default, 1- set hours, 2- set minutes, 3- 12h/24h selection, 4- set snooze
    Q_PROPERTY(int screenState READ screenState WRITE setScreenState NOTIFY screenStateChanged)
    Q_PROPERTY(int counter_hrs READ counter_hrs WRITE setCounter_hrs NOTIFY counter_hrsChanged)
    Q_PROPERTY(int counter_min READ counter_min WRITE setCounter_min NOTIFY counter_minChanged)
    Q_PROPERTY(int counter_snooze READ counter_snooze WRITE setCounter_snooze NOTIFY counter_snoozeChanged)
    Q_PROPERTY(bool mode24hrs READ mode24hrs WRITE setMode24hrs NOTIFY mode24hrsChanged)
    Q_PROPERTY(int alarm1Hours READ alarm1Hours WRITE setAlarm1Hours NOTIFY alarm1HoursChanged)
    Q_PROPERTY(int alarm1Minutes READ alarm1Minutes WRITE setAlarm1Minutes NOTIFY alarm1MinutesChanged)
    Q_PROPERTY(int alarm2Hours READ alarm2Hours WRITE setAlarm2Hours NOTIFY alarm2HoursChanged)
    Q_PROPERTY(int alarm2Minutes READ alarm2Minutes WRITE setAlarm2Minutes NOTIFY alarm2MinutesChanged)
    Q_PROPERTY(int alarmState READ alarmState WRITE setAlarmState NOTIFY alarmStateChanged)
    Q_PROPERTY(bool volumeState READ volumeState WRITE setVolumeState NOTIFY volumeStateChanged)
    Q_PROPERTY(bool alarm1Enabled READ alarm1Enabled WRITE setAlarm1Enabled NOTIFY alarm1EnabledChanged)
    Q_PROPERTY(bool alarm2Enabled READ alarm2Enabled WRITE setAlarm2Enabled NOTIFY alarm2EnabledChanged)
    Q_PROPERTY(QString colorBack READ colorBack WRITE setColorBack NOTIFY colorBackChanged)
    Q_PROPERTY(QString colorFont READ colorFont WRITE setColorFont NOTIFY colorFontChanged)
    Q_PROPERTY(QString colorSeconds READ colorSeconds WRITE setColorSeconds NOTIFY colorSecondsChanged)
    Q_PROPERTY(QString colorButtons READ colorButtons WRITE setColorButtons NOTIFY colorButtonsChanged)

    QSoundEffect alarm1sound, alarm2sound;


public:
    WirPi wirpi;
    explicit timer(QObject *parent = 0);
    int screenState();
    bool mode24hrs();
    int counter_hrs();
    int counter_min();
    int counter_snooze();
    int alarm1Hours();
    int alarm1Minutes();
    int alarm2Hours();
    int alarm2Minutes();
    int alarmState();
    bool volumeState();
    bool alarm1Enabled();
    bool alarm2Enabled();
    QString colorBack();
    QString colorFont();
    QString colorSeconds();
    QString colorButtons();
    void setScreenState(int stateNumber);
    void setMode24hrs(bool mode);
    void setCounter_hrs(int hours);
    void setCounter_min(int minutes);
    void setCounter_snooze(int snooze);
    void setAlarm1Hours(int hours);
    void setAlarm1Minutes(int minutes);
    void setAlarm2Hours(int hours);
    void setAlarm2Minutes(int minutes);
    void setAlarmState(int alarmNumber);
    void setVolumeState(bool volume);
    void speeding();
    void speedAlarmUp();
    void speedAlarmDown();
    void setAlarm1Enabled(bool enabled);
    void setAlarm2Enabled(bool enabled);
    void repeatAlarmAfterSnooze();
    void stopSound();
    void setColorBack(QString num);
    void setColorFont(QString num);
    void setColorSeconds(QString num);
    void setColorButtons(QString num);
    void checkButtonReading();


signals:
    void sendSec(const QString &counter_sec, const QString &counter_min, const QString &counter_hrs, const QString &counter_snooze);
    void sendMin(const QString &counter_min);
    void sendHrs(const QString &counter_hrs);
    void sendSnooze(const QString &m_snooze);
    void sendAlarm1Hours(const QString &m_alarm1Hours, const QString &m_alarm1Minutes);
    void sendAlarm1Minutes(const QString &m_alarm1Hours, const QString &m_alarm1Minutes);
    void sendAlarm2Hours(const QString &m_alarm2Hours, const QString &m_alarm2Minutes);
    void sendAlarm2Minutes(const QString &m_alarm2Hours, const QString &m_alarm2Minutes);
    void sendAlarm1State(const QString &state1, const QString &state2);
    void sendAlarm2State(const QString &state1, const QString &state2);
    //void setHoursMode();
    void screenStateChanged();
    void alarmStateChanged();
    void volumeStateChanged();
    void mode24hrsChanged(const QString hoursModeString);
    void counter_hrsChanged(const QString &counter_hrs);
    void counter_minChanged(const QString &counter_min);
    void counter_snoozeChanged(const QString &counter_snooze);
    void alarm1HoursChanged(const QString &m_alarm1Hours);
    void alarm1MinutesChanged(const QString &m_alarm1Minutes);
    void alarm2HoursChanged(const QString &m_alarm2Hours);
    void alarm2MinutesChanged(const QString &m_alarm2Minutes);
    void alarm1EnabledChanged();
    void alarm2EnabledChanged();
    void sendVolumeLevel(int volume);
    void sendVoltage(float volt1, float volt2);
    void colorBackChanged();
    void colorFontChanged();
    void colorSecondsChanged();
    void colorButtonsChanged();



public slots:
    void startTimer();
    void stopTimer();
    void timer_tick(void);
    void handleSetButtonPress();
    void speedUpClockSetting(bool hoursType, bool increaseType);
    void longPressDisabled();
    void enterSetAlarmMode();
    void handleAlarmSettingUp();
    void handleAlarmSettingDown();
    void handleSleepButton();
    void handleSnoozeButton();
    void handleVolumeButton();
    void decreaseVolume();
    void increaseVolume();
    void startButtonReading();
    void stopButtonReading();


private:
    int m_counter_sec = 0;
    int m_counter_min = 0;
    int m_counter_hrs = 0;
    int m_screenState = 0;
    int m_snooze = 5;
    int m_alarm1Hours = 0;
    int m_alarm1Minutes = 1;
    bool m_mode24hrs = true;
    int m_alarm2Hours = 0;
    int m_alarm2Minutes = 2;
    QString hoursModeString = "24";
    bool hoursType = false;
    bool increaseType = false;
    bool alarm1SettingModeEnabled = false;
    int m_alarmState = 0;
    bool m_volumeState = false;
    bool m_alarm1Enabled = false;
    bool m_alarm2Enabled = false;
    void convertAlarmStateIntoStrings(int num);
    int alarmNumSnooze;
    int volumeLevel = 2;
    QString curr_colorBack = "#000000"; //black
    QString curr_colorFont = "#9bb851"; //green
    QString curr_colorSeconds = "#55642E"; //dark green
    QString curr_colorButtons = "#DCDCDC"; //light grey
    QString m_colorBack[5] = {"#000000", "#000E45", "#192F23", "#210F00", "#F4F6F7"};
    QString m_colorFont[5] = {"#9bb851", "#C8DFEE", "#D2FFB8", "#FFB89B", "#000000"};
    QString m_colorSeconds[5] = {"#55642E", "#5D6870", "#708763", "#705144", "#8B8B8B"};
    QString m_colorButtons[5] = {"#DCDCDC", "#DCDCDC", "#DCDCDC", "#DCDCDC", "#DCDCDC"};
    int m_colorSchemeNum = 0;


};


#endif // TIMER_H
