import QtQuick 2.14
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.4
//import QtCharts 2.3


Window {
    id: window
    width: 800
    height: 350
    visible: true
    color: "#161616"
    title: qsTr("Alarm Clock")

    property color commonTextColor: "#9bb851"
    property color commonButtonColor: "#DCDCDC"

    Timer {
                id:middleBlinkingTimer;
                interval: 500; running: false; repeat: false;

                //hide colon in the middle for 500ms
                onTriggered: {
                    middleText.visible = false;

                }
            }


    Timer {
                id:hoursBlinkingTimer;
                interval: 500; running: false; repeat: false;
                onTriggered: {

                    hours.visible = !hours.visible;

                }
            }


    Timer {
                id:alarm1BlinkingTimer;
                interval: 500; running: false; repeat: false;
                onTriggered: {

                    alarm1Icon.visible = !alarm1Icon.visible;

                }
            }


    Timer {
                id:alarm2BlinkingTimer;
                interval: 500; running: false; repeat: false;
                onTriggered: {

                    alarm2Icon.visible = !alarm2Icon.visible;

                }
            }




    Timer {
                id:minutesBlinkingTimer;
                interval: 500; running: false; repeat: false;
                onTriggered: {

                    minutes.visible = !minutes.visible;

                }
            }


    Timer {
                id:setClockTimer;
                interval: 3000; running: false; repeat: false;
                onTriggered: {
                    if (timer.screenState === 0) {
                        timer.screenState++;
                        setButton.palette.button = "#f89009"
                        downButton.palette.button = "#9bb851"
                        upButton.palette.button = "#9bb851"
                        alarmSetButton.enabled = false;
                        sleepButton.enabled = false;
                        snoozeButton.enabled = false;
                        volumeButton.enabled = false;

                        timer.startButtonReading();
                        idlingTimer.start();
                    }

                }
            }


    Timer {
                id:idlingTimer;
                interval: 20000; running: false; repeat: false;
                onTriggered: {
                    if (timer.screenState !== 0 || timer.alarmState !== 0 || timer.volumeState) {
                        timer.screenState = 0;
                        timer.alarmState = 0;
                        setButton.palette.button = "#DCDCDC"
                        upButton.palette.button = "#DCDCDC"
                        downButton.palette.button = "#DCDCDC"
                        alarmSetButton.palette.button = "#DCDCDC";
                        volumeButton.palette.button = "#DCDCDC";

                        setButton.enabled = true;
                        snoozeButton.enabled = true;
                        sleepButton.enabled = true;
                        alarmSetButton.enabled = true;
                        volumeButton.enabled = true;
                        timer.stopButtonReading();


                        vol1.visible = false;
                        vol2.visible = false;
                        vol3.visible = false;
                        vol4.visible = false;
                        vol5.visible = false;

                        timer.volumeState = false;

                    }

                }
            }





    RoundButton {
        id: downButton
        x: 576
        y: 19
        width: 85
        height: 40

        Image {
            width: 50
            height: 50
            source: "icons/down.png"
            layer.mipmap: false
            layer.textureMirroring: ShaderEffectSource.MirrorVertically
            focus: false
                antialiasing: true
                sourceSize.height: 50
                sourceSize.width: 50
                fillMode: Image.Stretch
                // adjust the size of the image to fit the button
                anchors.centerIn: parent // center the image inside the button
            }

        text: ""
        font.styleName: "Semibold"


        onPressed: {



            //if the user is setting alarm
            if (timer.alarmState > 0)
            {
                    console.debug(" Alarm setting mode ");
                    timer.handleAlarmSettingDown();
            }

            else if (!timer.volumeState) //if the user is setting clock/24hrmode/snooze
            {


                    if (timer.screenState === 1) {

                        //first parameter true-hours, false-minutes; second true-increase, false-decrease
                        timer.counter_hrs-=1;
                        timer.speedUpClockSetting(true, false);
                        //console.debug(" Decrease hours ");

                    }
                    else  if (timer.screenState === 2) {
                        timer.counter_min-=1;
                        timer.speedUpClockSetting(false, false);
                        //longPress.start();
                    }
                    else if (timer.screenState === 3) {
                        timer.mode24hrs = !timer.mode24hrs;
                        //clockModeText.text = timer.mode24hrs ? "" : ((timer.counter_hrs < 13) ? "AM" : "PM");
                    }
                    else if (timer.screenState === 4) {
                        timer.counter_snooze-=1;
                    }
            }

            //if user is setting Volume
            else {
                timer.decreaseVolume();
            }


        }

        onReleased: {
            //longPress.stop();
            timer.longPressDisabled();

            if (timer.screenState !==0 || timer.alarmState !== 0 || timer.volumeState) {
                idlingTimer.restart();
            }

        }

    }


    RoundButton {
        id: upButton
        x: 683
        y: 19
        width: 85
        height: 40
        text: ""

        Image {
            width: 50
            height: 50
            source: "icons/up.png"
            layer.mipmap: false
            layer.textureMirroring: ShaderEffectSource.MirrorVertically
            focus: false
                antialiasing: true
                sourceSize.height: 50
                sourceSize.width: 50
                fillMode: Image.Stretch
                // adjust the size of the image to fit the button
                anchors.centerIn: parent // center the image inside the button
            }



        onPressed: {

            //if the user is setting alarm
            if (timer.alarmState > 0)
            {

                    timer.handleAlarmSettingUp();
            }

            else if (!timer.volumeState)//if the user is setting clock/24hrmode/snooze
            {

                if (timer.screenState === 1) {
                    //console.debug(" Pressed to increase ");
                    timer.counter_hrs+=1;
                    timer.speedUpClockSetting(true, true); //1st par - hours, 2nd - increase
                }
                else  if (timer.screenState === 2) {
                    timer.counter_min+=1;
                    timer.speedUpClockSetting(false, true);
                }
                else if (timer.screenState === 3) {
                    timer.mode24hrs = !timer.mode24hrs;
                    //clockModeText.text = timer.mode24hrs ? "" : ((timer.counter_hrs < 13) ? "AM" : "PM");
                }
                else if (timer.screenState === 4) {
                    timer.counter_snooze+=1;
                }
            }
            //if user is setting Volume
            else {
                timer.increaseVolume();
            }

        }

        onReleased: {
            timer.longPressDisabled();

            //if the device is not in default mode, but setting in SET mode
            if (timer.screenState !==0 || timer.alarmState !== 0 || timer.volumeState) {
                idlingTimer.restart();
            }
        }


    }



    RoundButton {
        id: volumeButton
        x: 24
        y: 19
        width: 85
        height: 40
        text: ""


        Image {
            width: 30
            height: 30
            source: "icons/volume.png"
            layer.mipmap: false
            layer.textureMirroring: ShaderEffectSource.MirrorVertically
            focus: false
                antialiasing: true
                sourceSize.height: 50
                sourceSize.width: 50
                fillMode: Image.Stretch
                // adjust the size of the image to fit the button
                anchors.centerIn: parent // center the image inside the button
            }

        onReleased: {
            if (timer.screenState !==0 || timer.alarmState !== 0) {
                idlingTimer.restart();
            }
            else {
                //if it's not occupied in another mode
                    //console.debug(" timer started");

                if (!timer.volumeState)
                {
                    downButton.palette.button = "#9bb851"
                    upButton.palette.button = "#9bb851"
                    alarmSetButton.enabled = false;
                    setButton.enabled = false;
                    snoozeButton.enabled = false;
                    sleepButton.enabled = false;

                    volumeButton.palette.button = "#f89009";

                    timer.handleVolumeButton();
                    idlingTimer.restart();

                }
                else
                {

                    downButton.palette.button = "#DCDCDC"
                    upButton.palette.button = "#DCDCDC"
                    volumeButton.palette.button = "#DCDCDC";
                    alarmSetButton.enabled = true;
                    sleepButton.enabled = true;
                    snoozeButton.enabled = true;
                    setButton.enabled = true;


                    vol1.visible = false;
                    vol2.visible = false;
                    vol3.visible = false;
                    vol4.visible = false;
                    vol5.visible = false;

                    timer.handleVolumeButton();

                }



            }


        }
    }



    RoundButton {
        id: sleepButton
        x: 131
        y: 19
        width: 85
        height: 40
        text: ""

        Image {
            width: 30
            height: 30
            source: "icons/sleep.png"
            layer.mipmap: false
            layer.textureMirroring: ShaderEffectSource.MirrorVertically
            focus: false
                antialiasing: true
                sourceSize.height: 50
                sourceSize.width: 50
                fillMode: Image.Stretch
                // adjust the size of the image to fit the button
                anchors.centerIn: parent // center the image inside the button
            }


        onPressed:
        {
            timer.handleSleepButton();
        }

        onReleased: {
            if (timer.screenState !==0 || timer.alarmState !== 0) {
                idlingTimer.restart();
            }
        }

    }

    RoundButton {
        id: snoozeButton
        x: 239
        y: 19
        width: 85
        height: 40
        text: ""

        Image {
            width: 30
            height: 30
            source: "icons/snooze.png"
            layer.mipmap: false
            layer.textureMirroring: ShaderEffectSource.MirrorVertically
            focus: false
                antialiasing: true
                sourceSize.height: 50
                sourceSize.width: 50
                fillMode: Image.Stretch
                // adjust the size of the image to fit the button
                anchors.centerIn: parent // center the image inside the button
            }

        onPressed:
        {
            timer.handleSnoozeButton();
        }

        onReleased: {
            if (timer.screenState !==0 || timer.alarmState !== 0) {
                idlingTimer.restart();
            }
        }

    }

    RoundButton {
        id: setButton
        x: 349
        y: 19
        width: 85
        height: 40
        text: ""


        Image {
            width: 33
            height: 30
            source: "icons/setup.png"
            layer.mipmap: false
            layer.textureMirroring: ShaderEffectSource.MirrorVertically
            focus: false
                antialiasing: true
                sourceSize.height: 50
                sourceSize.width: 50
                fillMode: Image.Stretch
                // adjust the size of the image to fit the button
                anchors.centerIn: parent // center the image inside the button
            }


        onPressed: {
                        if (timer.screenState === 0 && timer.alarmState === 0)
                        {
                            //console.debug(" timer started");
                            setClockTimer.start();
                        }
                        else {

                           timer.handleSetButtonPress();
                        }

                }

        onReleased: {
                    setClockTimer.stop();

                    if (timer.screenState !==0 || timer.alarmState !== 0) {
                        idlingTimer.restart();
                    }


                }

    }

    RoundButton {
        id: alarmSetButton
        x: 463
        y: 19
        width: 85
        height: 40
        text: ""

        Image {
            width: 35
            height: 35
            verticalAlignment: Image.AlignVCenter
            source: "icons/alarm.png"
            anchors.verticalCenterOffset: -1
            anchors.horizontalCenterOffset: 0
            layer.mipmap: false
            layer.textureMirroring: ShaderEffectSource.MirrorVertically
            focus: false
                antialiasing: true
                sourceSize.height: 40
                sourceSize.width: 40
                fillMode: Image.Stretch
                // adjust the size of the image to fit the button
                anchors.centerIn: parent // center the image inside the button
            }


        onPressed: {
            if (timer.screenState === 0)
            {
                if (timer.alarmState === 0)
                {

                    downButton.palette.button = "#9bb851"
                    upButton.palette.button = "#9bb851"
                    alarmSetButton.palette.button = "#f89009";
                    setButton.enabled = false;
                    sleepButton.enabled = false;
                    snoozeButton.enabled = false;
                    volumeButton.enabled = false;
                }
                else if (timer.alarmState === 6)
                {

                    upButton.palette.button = "#DCDCDC";
                    downButton.palette.button = "#DCDCDC";
                    alarmSetButton.palette.button = "#DCDCDC";
                    setButton.enabled = true;
                    volumeButton.enabled = true;
                    sleepButton.enabled = true;
                    snoozeButton.enabled = true;
                }

                timer.enterSetAlarmMode();

            }
        }

        onReleased: {
            //console.debug("Alarm state: " + timer.alarmState);
            if (timer.screenState !==0 || timer.alarmState !== 0) {
                //console.debug("Restarting timer ");
                idlingTimer.restart();
            }
        }


    }


    FontLoader {
            id: segmentsFont
            source: "fonts/digital7.ttf"
        }

    Text {
        id: hours
        x: 83
        y: 73
        width: 290
        height: 220
        text: "25"
        font.letterSpacing: 5
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        //font: segmentsFont
        font.family: segmentsFont.name
        font.weight: Font.Normal
        fontSizeMode: Text.VerticalFit
        minimumPointSize: 20
        minimumPixelSize: 20
        //font.family: "Tahoma"
        font.bold: false
        font.pointSize: 220
        visible: true;
        color: commonTextColor;
    }



     Image {
         id: alarm1Icon
         x: 220
         y: 303
         width: 35
         height: 35
         visible: false
         source: "icons/alarm1g.png"
     }


     Image {
         id: alarm2Icon
         x: 503
         y: 303
         width: 35
         height: 35
         visible: false
         source: "icons/alarm2g.png"
     }


     Text {
         id: alarm2Text
         x: 387
         y: 251
         visible: false
         color: "#f74343"
         text: qsTr("A2")
         font.pixelSize: 12

     }






    Text {
        id: minutes
        x: 384
        y: 73
        width: 290
        height: 220
        color: commonTextColor
        text: "25"
        font.letterSpacing: 5
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        lineHeight: 1
        font.family: segmentsFont.name
        font.weight: Font.Medium
        font.pointSize: 220
        fontSizeMode: Text.VerticalFit
        minimumPixelSize: 20
        font.bold: false
        minimumPointSize: 20

    }

    Text {
        id: middleText
        x: 277
        y: 76
        width: 199
        height: 199
        color: commonTextColor
        text: ":"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.weight: Font.Normal
        font.pointSize: 370
        fontSizeMode: Text.VerticalFit
        minimumPixelSize: 20
        font.family: "Arial"
        font.bold: false
        minimumPointSize: 20


    }




    Rectangle {
        id: vol5
        x: 738
        y: 120
        width: 6
        height: 80
        color: "#f89009"
        visible: false
    }

    Rectangle {
        id: vol4
        x: 725
        y: 136
        width: 6
        height: 64
        color: "#f89009"
        visible: false
    }

    Rectangle {
        id: vol3
        x: 712
        y: 152
        width: 6
        height: 48
        color: "#f89009"
        visible: false
    }

    Rectangle {
        id: vol2
        x: 700
        y: 168
        width: 6
        height: 32
        color: "#f89009"
        visible: false
    }

    Rectangle {
        id: vol1
        x: 688
        y: 184
        width: 6
        height: 16
        color: "#f89009"
        visible: false
    }






    Text {
        id: seconds
        x: 663
        y: 207
        width: 105
        height: 105
        text: "00"
        color: "#55642e"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.bold: false
        font.pointSize: 60
        font.weight: Font.Normal
        minimumPixelSize: 20
        fontSizeMode: Text.VerticalFit
        font.family: segmentsFont.name
        //font.family: "Tahoma"
        minimumPointSize: 20
    }


    Text {
        id: clockModeText
        x: 36
        y: 139
        width: 30
        height: 30
        text: qsTr("")
        color: commonTextColor
        font.pixelSize: 18
        font.bold: true
        font.family: "Tahoma"
    }


    Connections
    {

        target:timer

        function onScreenStateChanged()
        {
            //console.debug("Should change color");
            if (timer.screenState === 0 && timer.alarmState === 0) {
                setButton.palette.button = commonButtonColor
                upButton.palette.button = "#DCDCDC"
                downButton.palette.button = "#DCDCDC"
                alarmSetButton.palette.button = "#DCDCDC";
                volumeButton.palette.button = "#DCDCDC";

                setButton.enabled = true;
                snoozeButton.enabled = true;
                sleepButton.enabled = true;
                alarmSetButton.enabled = true;
                volumeButton.enabled = true;
            }
        }


        //this is implemented every second
        function onSendSec(counter_sec, counter_min, counter_hrs, counter_snooze)
        {
            //setButton.palette.button = "#9bb851"
            seconds.text = counter_sec;
            minutes.text = counter_min;
            hours.text = counter_hrs;

            // show colon in the middle for 500ms
            middleText.visible = true;
            hours.visible = true;
            minutes.visible = true;
            middleBlinkingTimer.start();
            clockModeText.text = timer.mode24hrs ? "" : ((timer.counter_hrs < 13) ? "AM" : "PM");

            //blinking if Set Hours mode
            if (timer.screenState === 1) {
                hours.visible = true;
                hoursBlinkingTimer.start();
            }

            //blinking if Set Minutes mode
            else if (timer.screenState === 2) {
                    hours.visible = true;
                    minutes.visible = true;
                    minutesBlinkingTimer.start();
                }

            else if (timer.screenState === 3) {

                    hours.text = timer.mode24hrs ? "24" : "12"
                    minutes.text = "Hr";

                    hours.visible = true;
                    minutes.visible = true;
                    middleText.visible = false;
                    seconds.visible = false;
                    hoursBlinkingTimer.start();
                    minutesBlinkingTimer.start();

                }
            else if (timer.screenState === 4) {
                    hours.text = "";
                    minutes.text = counter_snooze;

                    hours.visible = true;
                    minutes.visible = true;
                    middleText.visible = false;
                    hoursBlinkingTimer.start();
                    minutesBlinkingTimer.start();

                }
            else {

                seconds.visible = true;
            }


        }




        function onSendMin(counter_str)
        {
            minutes.text = counter_str;
            minutes.visible = true;
            if (timer.screenState !==0 || timer.alarmState !== 0 || timer.volumeState) {
                idlingTimer.restart();
            }
        }

        function onSendHrs(counter_str)
        {
            hours.visible = true;
            hours.text = counter_str;
        }




        function alarm1HoursChanged(counter_str)
        {
            hours.visible = true;
            hours.text = timer.counter_hrs;
        }


        function alarm2HoursChanged(counter_str)
        {
            hours.visible = true;
            hours.text = timer.counter_hrs;
        }



        function onSendSnooze(counter_str)
        {
            minutes.visible = true;
            minutes.text = counter_str;
        }


        function onMode24hrsChanged(mode_string)
        {
            //console.debug(mode_string);
            hours.text = mode_string;
            clockModeText.text = timer.mode24hrs ? "" : ((timer.counter_hrs < 13) ? "AM" : "PM");
        }



        function onSendAlarm1Hours(alarmHours, alarmMinutes) {

            seconds.text = "";
            minutes.text = alarmMinutes;
            hours.text = alarmHours;
            middleText.visible = true;

            minutes.visible = true;
            hours.visible = true;
            hoursBlinkingTimer.start();


            if (timer.alarmState < 3)
            {
                alarm1Icon.visible = true;
                alarm1BlinkingTimer.start();
            }
            else
            {
                alarm2Icon.visible = true;
                alarm2BlinkingTimer.start();
            }


        }



        function onSendAlarm1Minutes(alarmHours, alarmMinutes) {

            seconds.text = "";
            minutes.text = alarmMinutes;
            hours.text = alarmHours;
            middleText.visible = true;

            hoursBlinkingTimer.stop();
            hours.visible = true;
            minutes.visible = true;
            minutesBlinkingTimer.start();


            if (timer.alarmState < 3)
            {
                alarm1Icon.visible = true;
                alarm1BlinkingTimer.start();
            }
            else
            {
                alarm2Icon.visible = true;
                alarm2BlinkingTimer.start();
            }

        }



        function onSendAlarm1State(alarmPart1, alarmPart2) {
            seconds.text = "";

            hours.text = alarmPart1;
            minutes.text = alarmPart2;
            middleText.visible = false;

            hoursBlinkingTimer.stop();
            minutesBlinkingTimer.stop();
            hoursBlinkingTimer.start();
            minutesBlinkingTimer.start();
            hours.visible = true;
            minutes.visible = true;

            if (timer.alarm1Enabled)
            {
                alarm1Icon.visible = true;
            }

        }


        function onSendAlarm2State(alarmPart1, alarmPart2) {
            seconds.text = "";

            hours.text = alarmPart1;
            minutes.text = alarmPart2;
            middleText.visible = false;

            hoursBlinkingTimer.stop();
            minutesBlinkingTimer.stop();
            hoursBlinkingTimer.start();
            minutesBlinkingTimer.start();
            hours.visible = true;
            minutes.visible = true;

            if (timer.alarm2Enabled)
            {
                alarm2Icon.visible = true;
            }

        }



        function onAlarm1EnabledChanged()
        {
            alarm1Icon.visible = timer.alarm1Enabled;
        }



        function onAlarm2EnabledChanged()
        {
            alarm2Icon.visible = timer.alarm2Enabled;
        }



        function onSendVolumeLevel(volume)
        {
            vol1.visible = true;
            vol2.visible = false;
            vol3.visible = false;
            vol4.visible = false;
            vol5.visible = false;

            if (volume > 1) {
                vol2.visible = true;

                if (volume > 2) {
                    vol3.visible = true;

                    if (volume > 3) {
                        vol4.visible = true;

                        if (volume > 4) {
                            vol5.visible = true;
                        }

                    }

                }
            }


        }


        function onColorBackChanged()
        {
            console.debug(timer.colorFont);
            window.color = timer.colorBack;
            commonTextColor = timer.colorFont;
            seconds.color = timer.colorSeconds;
            commonButtonColor = timer.colorButtons;
            idlingTimer.restart();
        }



    }




}





/*##^##
Designer {
    D{i:0;formeditorZoom:1.1}
}
##^##*/
