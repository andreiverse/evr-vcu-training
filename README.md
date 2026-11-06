
### setup
```bash
# setup the build database
arduino-cli compile --fqbn arduino:avr:uno --only-compilation-database --build-path build .
```

### compile and upload
```bash
arduino-cli compile . -p /dev/ttyACM0 -v
arduino-cli upload . -p /dev/ttyACM0 -v
```