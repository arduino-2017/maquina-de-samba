/*
 * Maquina de Samba
 * Crear - Arte y Tecnologia
 * http://elarteylatecnologia.com.ar
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <AFMotor.h>
#include <MIDI.h>

/*
 * This is the time in milliseconds we have the actuator on either state
 * before leaving it floating.
 *
 * The ones we use draw quite a bit of current and this value is a good
 * compromise between speed, force and dissipation.
 */
#define HOLD_TIME 75

MIDI_CREATE_DEFAULT_INSTANCE();

AF_DCMotor motor0(1);
AF_DCMotor motor1(2);
AF_DCMotor motor2(3);
AF_DCMotor motor3(4);

AF_DCMotor motores[] = {
    motor0,
    motor1,
    motor2,
    motor3
};


/*
 * A table of times to know when to move back the actuator to it's resting
 * position and to unpower it.
 *
 * If nonzero that motor is active. Otherwise we do nothing.
 */
unsigned long upTimes[] = {0, 0, 0, 0};
unsigned long releaseTimes[] = {0, 0, 0, 0};


void setup() {
    for(uint8_t idx=0; idx < 4; idx++) {
        AF_DCMotor mot = motores[idx];

        mot.setSpeed(255);
        mot.run(RELEASE);
    }

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);

    // Initiate MIDI communications, listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);
    Serial.begin(115200); // use a standard bitrate for computer serial ports.
}


void loop() {
    unsigned long now = millis();

    MIDI.read();

    for(uint8_t idx=0; idx < 4; idx++) {
        if (upTimes[idx] && abs(upTimes[idx] - now) > HOLD_TIME) {
            upTimes[idx] = 0;
            releaseTimes[idx] = now + HOLD_TIME;

            if (releaseTimes[idx] == 0) {
                releaseTimes[idx] += 1;
            }

            AF_DCMotor mot = motores[idx];
            mot.run(BACKWARD);
        }

        if (releaseTimes[idx] && abs(releaseTimes[idx] - now) > HOLD_TIME) {
            releaseTimes[idx] = 0;

            AF_DCMotor mot = motores[idx];
            mot.run(RELEASE);
        }
    }
}


/*
 * This is called when a new NOTE ON event arrives.
 * Maps the notes 57 59 60 62 to the four channels (z x c v on vkeybd)
 *
 * From the docs:
 * Try to keep your callbacks short (no delays ect)
 * otherwise it would slow down the loop() and have a bad impact
 * on real-time performance.
 */
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    unsigned long now = millis();
    byte idx = 255;

    switch(pitch) {
    case 57:
        idx = 0;
        break;
    case 59:
        idx = 1;
        break;
    case 60:
        idx = 2;
        break;
    case 62:
        idx = 3;
        break;
    default:
        break;
    }

    if (idx != 255) {
        releaseTimes[idx] = 0;
        upTimes[idx] = now + HOLD_TIME;

        if (upTimes[idx] == 0) {
            upTimes[idx] += 1;
        }

        AF_DCMotor mot = motores[idx];
        mot.run(FORWARD);
    }
}


void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
}

