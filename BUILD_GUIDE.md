This is a short document outlining how the Gradle Build System interacts with ToastC++ and external libraries like WPILib.
===

## Building for Native System
Building for the system you're running on is simple. Run:
```
gradlew build
```

## Building for RoboRIO
### Building Dependencies
Before building for the RoboRIO, you have to build WPILib. To do this, run
```
gradlew :wpi:wpilib:build -Parm -Proborio
```
### Building Toast
```
gradlew build -Parm -Proborio
```
### Deploying
You can deploy ToastC++ to a RoboRIO using
```
gradlew deploy -Parm -Proborio
```

This will deploy over USB, you can deploy over any other interface or IP Address using
```
gradlew deploy -Parm -Proborio -Pip=<ip address of RoboRIO>
```
