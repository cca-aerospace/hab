#ifdef MODE_DEBUG
#define Print(s)                                                               \
    { Serial.print(s); }
#define Println(s)                                                             \
    { Serial.println(s); }
#define Show(prompt, value, units)                                             \
    {                                                                          \
        Serial.print(prompt);                                                  \
        Serial.print(F(": "));                                                 \
        Serial.print(value);                                                   \
        Serial.println(units);                                                 \
    }
#else
#define Print(s)                                                               \
    {}
#define Println(s)                                                             \
    {}
#endif