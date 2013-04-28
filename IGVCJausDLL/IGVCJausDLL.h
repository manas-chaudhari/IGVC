#define EXPORT extern "C" __declspec(dllexport) 

EXPORT int init();
EXPORT int loop(int input);
EXPORT int simulate_message_received();
EXPORT void stop();
EXPORT void updateLocalPose(double X_m, double Y_m, double Yaw_rad);
EXPORT void readMessage(char* messageString);