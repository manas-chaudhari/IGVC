#define EXPORT extern "C" __declspec(dllexport) 

EXPORT int init(int subsystem, int node, int component);
EXPORT int loop(int input);
EXPORT void stop();
EXPORT void updateLocalPose(double X_m, double Y_m, double Yaw_rad);
EXPORT void updateVelocityState(double VelocityX_mps, double YawRate_rps);
EXPORT void readMessage(char* messageString);