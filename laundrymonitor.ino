#define MICROPHONE_PIN  A5
#define LED_PIN_RED     D3
#define LED_PIN_COM     D4
#define LED_PIN_GRN     D5
#define LED_PIN_BLU     D6

const int SILENCE_PERIOD = 30;
const int NOISE_FACTOR   = 10;

int sample;

////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
    pinMode(MICROPHONE_PIN, INPUT);
    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_COM, OUTPUT);
    pinMode(LED_PIN_GRN, OUTPUT);
    pinMode(LED_PIN_BLU, OUTPUT);
    Serial.begin(9600);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
    
    // Blink blue twice to indicate start
    digitalWrite(LED_PIN_BLU, HIGH);
    delay(500);
    digitalWrite(LED_PIN_BLU, LOW);
    delay(500);
    digitalWrite(LED_PIN_BLU, HIGH);
    delay(500);
    digitalWrite(LED_PIN_BLU, LOW);
    delay(500);
    
    // Take sound samples every second for 10s, to establish a baseline
    // Glow green while taking samples
    digitalWrite(LED_PIN_GRN, HIGH);
    int sampleBaseline = 0;
    
    for (int secondCount = 0; secondCount < 10; secondCount++)
    {
        sampleBaseline += analogRead(MICROPHONE_PIN);
        delay(1000);
    }
    
    sampleBaseline = sampleBaseline / 10;
    
    Serial.println(sampleBaseline);
    Serial.println(-1);
    digitalWrite(LED_PIN_GRN, LOW);
    
    // Monitor for when the sound level is less than that baseline for 30s straight
    // Red LED = sound detected; purple = counting silence
    int elapsedSeconds = 0;
    
    while (elapsedSeconds <= SILENCE_PERIOD)
    {
        sample = analogRead(MICROPHONE_PIN);
        Serial.println(sample);
        
        if (sample < (sampleBaseline * NOISE_FACTOR) && sample < 1000)
        {
            digitalWrite(LED_PIN_RED, HIGH);
            digitalWrite(LED_PIN_BLU, LOW);
            elapsedSeconds = 0;
        }
        else
        {
            digitalWrite(LED_PIN_RED, HIGH);
            digitalWrite(LED_PIN_BLU, HIGH);
            elapsedSeconds += 1;
        }
        
        delay(1000);
        Serial.println(elapsedSeconds);
    }
    
    // Silence has lasted long enough; alert that laundry is done
    Particle.publish("laundryStatus", "done", 60, PRIVATE);
    digitalWrite(LED_PIN_BLU, LOW);
    
    // Enter a holding pattern until user retrieves device
    while (true)
    {
        digitalWrite(LED_PIN_RED, HIGH);
        delay(500);
        digitalWrite(LED_PIN_RED, LOW);
        delay(500);
    }
    
}