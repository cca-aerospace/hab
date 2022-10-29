#ifdef DEBUG_PRINT
#define Print(s)                                                               \
    {}
#define Println(s)                                                             \
    {}
#else
#define Print(s)                                                               \
    { Serial.print(s); }
#define Println(s)                                                             \
    { Serial.println(s); }
#endif